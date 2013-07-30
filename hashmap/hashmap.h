#ifndef _HASHMAP_H
#define _HASHMAP_H

struct hashmap;

/* They key type information is needed for an actual comparison 
   of the key when traversing through the bucket. We cannot have
   a generic comparison function for different types. Equality
   for a string is different from an equality for an integer. */
enum key_type {
    STRING_KEY,
    INTEGER_KEY,
    CHARACTER_KEY,
    FLOAT_KEY,
    POINTER_KEY
};

/* Bounded hashmap will use atmost max_size buckets. 
   This is memory efficent when you know in advance the maximum size of your
   hashtable */
struct hashmap *create_bounded_hashmap(int max_size, enum key_type,
        void (*data_free_func)(void *data));
/* Unbounded hashmap uses a fixed default number of buckets. If your 
   data is magnitudes of order more than this number, there might be lots
   of collisions and hence, performance degradation. On the other hand,
   if your data is magnitudes of order less than this number, a lot of
   space might simply be wasted. Use it if you don't know the approximate
   size in advance */
struct hashmap *create_unbounded_hashmap(enum key_type,
        void (*data_free_func)(void *data));
void delete_hashmap(struct hashmap *hm);

#define set_key_value(hm, key, value) _set_key_value(hm, (void *)key, \
        (void *)value)
#define get_value_by_key(hm, key) _get_value_by_key(hm, (void *)key)
#define pop_key(hm, key) _pop_key(hm, (void *)key)

#endif
