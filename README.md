This is a simulator in C for set-associative caches under the LRU replacement strategy. The inputs to the simulator will be the line length, lines per set, and the cache size. The number of sets will be computed from these inputs.

Each line in the trace consists of two fields. The first field is a number from 0 to 4, and the second is the address of the trace in hex.

The first field specifies the access type of the reference:

0 = read data 
1 = write data 
2 = instruction fetch 
3 = unknown access type 
4 = causes a flushing of the cache, i.e., all the lines in the cache are invalidated. If the cache is writeback, everything is written back. 
The address space is 32 bits long (or 8 hex positions). If fewer than 8 positions are specified, the leading positions are 0. Thus, for example, an address of abcd is to be read as the hex address 0x0000abcd.

C stands for the total cache size in bytes, L for the line size in bytes, and K for the set associativity (number of lines per set). FA means fully associative (i.e., the entire cache is one set). To begin with, assume there is just a single cache that holds both instructions and data.

[a] C = 1 KByte, K = 1 line per set, L = 4, 8, 16, 32, 64, 128 and 256 bytes.

[b] K = 4 lines per set, L = 16 bytes, C = 64, 128, 256, 512, 1024 bytes; 2K, 4K bytes.

[c] C = 256 Bytes, L = 16 bytes, K = 1, 2, 4, 8, and 16 lines per set.

[d] Now, assume you have separate instruction and data caches. These are each of size 1 kByte. Determine the instruction and data miss rates, assuming that K=1 line per set and L=16 bytes. (In simulating these caches, filter out accesses of unknown access type).

[e] Use any uniform random number generator (RNG) for this part. For example, you can use the RNG rand() that is part of the stdlib standard utilities library. There are many alternatives readily available for download. (Warning: rand() is NOT a very good generator: it is convenient and good enough for our purposes, but it is not as good as others that are available in terms of the statistical properties of the random number stream that it generates. So, don't ever use it in "real life": there are many better ones available). Use this RNG to generate a trace of 10,000 read accesses randomly distributed over the entire address space. Run this trace on the cache configurations in part (b) above. Compare the miss rate you get with a random trace against that originally obtained in part (b).

[f] What is the miss rate you would get in each of the configurations in part (c) if the trace were purely sequential, i.e., if each access was to a word whose byte address was exactly 4 greater than that of the previous access? Construct such a trace, starting with an access to location 0 and 1,000 entries long. Use this as a check of the correctness of your simulator.

