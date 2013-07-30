#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "hashmap.h"

static int TABLE_SIZE = 1048576;

uint64_t fnv1_hash (void *key, int n_bytes)
{
    unsigned char *p = key;
    uint64_t h = 14695981039346656037;
    int i;
    for (i = 0; i < n_bytes; i++) {
        h = (h * 1099511628211) ^ p[i];
    }
    return h % TABLE_SIZE;
}

int hash_string(void *key) {
    return fnv1_hash(key, sizeof(char)*strlen((char *)key));
}

int hash_integer(void *key) {
    return fnv1_hash(&key, sizeof(int));
}

int hash_character(void *key) {
    return fnv1_hash(&key, sizeof(char));
}

int hash_float(void *key) {
    return fnv1_hash(&key, sizeof(float));
}

int hash_pointer(void *key) {
    return fnv1_hash(key, sizeof(void *));
}

enum bool string_equals(void *key, void *existing_key) {
    return strcmp((char *)key, (char *)existing_key) == 0;
}

enum bool generic_equals(void *key, void *existing_key) {
    return key == existing_key;
}

void *string_copy_func(void *key) {
    char *s = (char *)key;
    char *copy = (char *)malloc(strlen(s)*sizeof(char));
    strcpy(copy, s);
    return copy;
}

void *no_copy_func(void *key) {
    return key;
}

struct hashmap *create_bounded_hashmap(int max_size,
        enum key_type type, void (*data_free_func)(void *data)) {
    int i;
    TABLE_SIZE = max_size;
    struct hashmap *hm = (struct hashmap *)malloc(
            sizeof(struct hashmap));
    hm->bucket = (struct node **)malloc(
            max_size * sizeof(struct node *));
    for (i = 0; i < max_size; i++) {
        hm->bucket[i] = 0;
    }
    hm->table_size = max_size;
    hm->type = type;
    hm->data_free_func = data_free_func;
    switch (type)
    {
        case STRING_KEY:
            hm->hash = hash_string;
            hm->equals = string_equals;
            hm->key_copy_func = string_copy_func;
            break;
        case INTEGER_KEY:
            hm->hash = hash_integer;
            hm->equals = generic_equals;
            hm->key_copy_func = no_copy_func;
            break;
        case CHARACTER_KEY:
            hm->hash = hash_character;
            hm->equals = generic_equals;
            hm->key_copy_func = no_copy_func;
            break;
        case FLOAT_KEY:
            hm->hash = hash_float;
            hm->equals = generic_equals;
            hm->key_copy_func = no_copy_func;
            break;
        default:
            hm->hash = hash_pointer;
            hm->equals = generic_equals;
            hm->key_copy_func = no_copy_func;
    }
    return hm;
}

struct hashmap *create_unbounded_hashmap(enum key_type type,
        void (*data_free_func)(void *data)) {
    return create_bounded_hashmap(TABLE_SIZE, type, data_free_func);
}

void delete_bucket(struct node *head,
        void (*data_free_func)(void *), enum bool deallocate_key) {
    while(head) {
        struct node *temp = head;
        head = head->next;
        if (data_free_func)
            data_free_func(temp->value);
        if (deallocate_key)
            free(temp->key);
        free(temp);
    }
}

void delete_hashmap(struct hashmap *hm) {
    int i;
    enum bool deallocate_key = (
            hm->type == STRING_KEY ? true : false);
    for (i = 0; i < hm->table_size; i++) {
        delete_bucket(hm->bucket[i], hm->data_free_func,
                deallocate_key);
    }
    free(hm);
}

void _set_key_value(struct hashmap *hm, void *key, void *data) {
    int idx = hm->hash(key);
    struct node *head = hm->bucket[idx];
    while (head) {
        if (hm->equals(key, head->key))
        {
            void *key_copy = hm->key_copy_func(key);
            if (hm->data_free_func)
                hm->data_free_func(head->value);
            head->key = key_copy;
            head->value = data;
            return;
        }
        head = head->next;
    }
    struct node *new_node = (struct node *)malloc(
            sizeof(struct node));
    void *key_copy = hm->key_copy_func(key);
    new_node->next = hm->bucket[idx];
    hm->bucket[idx] = new_node;
    new_node->key = key_copy;
    new_node->value = data;
}

void *_get_value_by_key(struct hashmap *hm, void *key) {
    int idx = hm->hash(key);
    struct node *head = hm->bucket[idx];
    while (head) {
        if (hm->equals(key, head->key))
            return head->value;
        head = head->next;
    }
    return 0;
}

void *_pop_key(struct hashmap *hm, void *key) {
    int idx = hm->hash(key);
    struct node *head = hm->bucket[idx];
    if (head) {
        while (head->next) {
            if (hm->equals(key, head->next->key)) {
                struct node *temp = head->next;
                void *data = temp->value;
                head->next = temp->next;
                free(temp);
                return data;
            }
            head = head->next;
        }
    }
    return 0;
}
