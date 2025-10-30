package bpe

type BPETokenizer struct {
	// maps the token_id to token_string 
	vocabs map[int]string
	// maps the token_string to token_id
	inverse_vocabs map[string]int

	//dictionary of bpe merges
	bpe_merges map[[2]int]int

	// (string_A,string_B) -> rank
	bpe_ranks map[[2]string]int 
}

//constructor
func NewBPETokenizer() *BPETokenizer {
	return &BPETokenizer{
		vocabs: make(map[int]string),
		inverse_vocabs: make(map[string]int),
		bpe_merges: make(map[[2]int]int),
		bpe_ranks: make(map[[2]string]int),
	}
}


