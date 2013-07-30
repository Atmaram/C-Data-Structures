#include "../hashmap.h"

enum bool {
    false,
    true
};

struct hashmap {
    struct node **bucket;
    int table_size;
    enum key_type type;
    int (*hash)(void *key);
    enum bool (*equals)(void *key, void *existing_key);
    void (*data_free_func)(void *data);
    void *(*key_copy_func)(void *key);
};

struct node {
    void *key;
    void *value;
    struct node *next;
};

void _set_key_value(struct hashmap *hm, void *key, void *data);
void *_get_value_by_key(struct hashmap *hm, void *key);
void *_pop_key(struct hashmap *hm, void *key);
