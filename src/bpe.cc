/**

Implementation of the core BPE algorithm from scratch.
*/

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class BPETokenizer {
public:
  // id -> token
  std::unordered_map<int, std::string> vocab;
  // token -> id
  std::unordered_map<std::string, int> vocab_inverse;
  // (id1,id2) -> new_id
  std::map<std::pair<int, int>, int> bpe_merges;

  BPETokenizer() = default;

  // training
  void train(const std::string &text, int vocab_size,
             const std::set<std::string> &allowed_special = {"<|endoftext|>"}) {
    std::string processed = preprocess(text);

    // init vocab with unique chars
    std::set<char> chars(processed.begin(), processed.end());

    int id = 0;
    for (char c : chars) {
      std::string s(1, c);
      vocab[id] = s;
      vocab_inverse[s] = id;
      id += 1;
    }

    // add the special tokens
    for (const auto &tok : allowed_special) {
      vocab[id] = tok;
      vocab_inverse[tok] = id;
      id += 1;
    }

    // tokenizer the text into character ids
    std::vector<int> token_ids;
    for (char c : processed) {
      token_ids.push_back(vocab_inverse[std::string(1, c)]);
    }

    // repeatedly merge the most frequent pairs
    while ((int)vocab.size() < vocab_size) {
      auto pair = find_freq_pair(token_ids);
      if (pair.first == -1)
        break;

      token_ids = replace_pair(token_ids, pair, id);
      std::string merged = vocab[pair.first] + vocab[pair.second];
      vocab[id] = merged;
      vocab_inverse[merged] = id;
      bpe_merges[pair] = id;
      id += 1;
    }
  }

  // ---- Encoding the string into a vector of int ----
  std::vector<int> encode(const std::string &text) {
    std::string processed = preprocess(text);
    std::vector<int> token_ids;

    for (char c : processed) {
      std::string s(1, c);
      if (vocab_inverse.count(s)) {
        token_ids.push_back(vocab_inverse[s]);
      }
    }
    return tokenize_with_bpe[token_ids];
  }

  // --- Decoding the vector of ints to a string ----
  std::string decode(const std::vector<int> &token_ids) {
    std::string out;
    for (int id : token_ids) {
      if (vocab.count(id)) {
        out += vocab[id];
      }
      // Replace Ġ with space to simulate GPT-2 spacing
      size_t pos = 0;
      while ((pos = out.find("Ġ", pos)) != std::string::npos) {
        out.replace(pos, 2, " ");
      }
      return out;
    }

  private:
    // preprocess
    std::string preprocess(const std::string &text) {
      std::string result;
      for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == ' ' && i != 0) {
          result += "Ġ";
        } else if (text[i] != ' ') {
          result += text[i];
        }
      }
      return result;
    }

    // --- finding the most frequent pair
    std::pair<int, int> find_freq_pair(const std::vector<int> &token_ids) {
      std::map<std::pair<int, int>, int> freq;
      for (size_t i = 0; i + 1 < token_ids.size(); ++i) {
        freq[{token_ids[i], token_ids[i + 1]}] += 1;
      }
      if (freq.empty()) {
        return {-1, -1};
      }
      return std::max_element(freq.begin(), freq.end(),
                              [](const auto &a, const auto &b) {
                                return a.second < b.second;
                              })
          ->first;
    }

    // --- replace pair ---
    std::vector<int> replace_pair(const std::vector<int> &ids,
                                  std::pair<int, int> pair, int new_id) {
      std::vector<int> out;
      size_t i = 0;
      while (i < ids.size()) {
        if (i < ids.size() - 1 && ids[i] == pair.first &&
            ids[i + 1] == pair.second) {
          out.push_back(new_id);
          i += 2;
        } else {
          out.push_back(ids[i]);
          i += 1;
        }
      }
      return out;
    }
    // --- APPLY BPE MERGES ---
    std::vector<int> tokenize_with_bpe(std::vector<int> token_ids) {
      bool merged = true;
      while (merged && token_ids.size() > 1) {
        merged = false;
        std::vector<int> new_tokens;
        for (size_t i = 0; i < token_ids.size();) {
          if (i < token_ids.size() - 1) {
            auto pair = std::make_pair(token_ids[i], token_ids[i + 1]);
            if (bpe_merges.count(pair)) {
              new_tokens.push_back(bpe_merges[pair]);
              i += 2;
              merged = true;
              continue;
            }
          }
          new_tokens.push_back(token_ids[i]);
          i++;
        }
        token_ids = new_tokens;
      }
      return token_ids;
    }
  };
