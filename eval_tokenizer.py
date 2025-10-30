#!/usr/bin/python
#
#evaluate and test on gpt-2

import tiktoken
import timeit

gpt2_tokenizer = tiktoken.get_encoding("gpt2")


for i in range(300):
    decoded = gpt2_tokenizer.decode([i])
    print(f"{i} : {decoded} \t")

    
