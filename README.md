C-Data-Structures
=================

Some data structures that can be used in C programs. Most of the times, I need to use them in programming contests.

Right now, it uses the FNV hashing function. The performance starts taking a hit after I have 100k keys. Performance tests indicate 35ms to store or lookup per 10k keys. But the number changes to 8 seconds for 100k keys. Will need a better hash function.
