/*
 *  hash.h
 *  c4
 *
 *
 */

/* murmur hash 2
 will use 64 bit version on 64 bit platforms 
 
 more info: http://sites.google.com/site/murmurhash/
 
 this code is MIT licensed
 
 */

extern unsigned int murmur_hash_2(const void *key, int len, unsigned int seed);