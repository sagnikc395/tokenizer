str_inp = "hello in korean 🙁😙"
#print([ord(x) for x in str_inp])

#in raw bytes 
tokens = list(str_inp.encode("utf-8"))
# utf-16 is much wasteful encoding 
#print(list(str_inp.encode("utf-16")))
#utf-32 same is much more wasteful 
#print(list(str_inp.encode("utf-32")))


# iterate and find the pair of bytes that occur
# frequenlty and merge them 

def get_stats(ids):
    '''
    Given a list of integers, return a dict of counts of consecutive pairs 
    '''
    counts = {} 
    for pair in zip(ids,ids[1:]): 
        # iterate through consecutive elements 
        counts[pair] = counts.get(pair,0) + 1
    return counts 

stats = get_stats(tokens)
# value,key list with the items and given reverse item 
print(sorted(((v,k) for k,v in stats.items()),reverse=True))

# get the max of the dictionary stats to rank keys 
top_pair = max(stats,key=stats.get)
print(top_pair)