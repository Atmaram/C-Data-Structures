#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashmap.h"

void test_hashmap(void) {
    struct hashmap *hm = create_bounded_hashmap(20, INTEGER_KEY, NULL);
    set_key_value(hm, 25, 5);
    set_key_value(hm, 49, 7);
    assert((int)get_value_by_key(hm, 25) == 5);
    assert((int)get_value_by_key(hm, 49) == 7);
    delete_hashmap(hm);

    hm = create_unbounded_hashmap(STRING_KEY, NULL);
    set_key_value(hm, "foo", "bar");
    set_key_value(hm, "spam", "eggs");
    assert(strcmp(get_value_by_key(hm, "foo"), "bar") == 0);
    assert(strcmp(get_value_by_key(hm, "spam"), "eggs") == 0);
    assert(get_value_by_key(hm, "bar") == 0);
    delete_hashmap(hm);
    printf("Hashmap tests passed\n");
}

void test_performance_store(struct hashmap *hm) {
    struct timeval t0, t1;
    int i;
    int n = 100000;

    gettimeofday(&t0, NULL);
    for (i = 0; i < n; i++) {
        set_key_value(hm, i, i);
    }
    gettimeofday(&t1, NULL);
    printf("Stored %d keys in %.2g seconds\n", n,
            t1.tv_sec - t0.tv_sec + 1E-6 * (t1.tv_usec - t0.tv_usec));
}

void test_performance_lookup(struct hashmap *hm) {
    struct timeval t0, t1;
    int i;
    int n = 100000;

    gettimeofday(&t0, NULL);
    for (i = 0; i < n; i++) {
        get_value_by_key(hm, i);
    }
    gettimeofday(&t1, NULL);
    printf("Looked up %d keys in %.2g seconds\n", n,
            t1.tv_sec - t0.tv_sec + 1E-6 * (t1.tv_usec - t0.tv_usec));
}

void test_performance(void) {
    struct hashmap *hm = create_unbounded_hashmap(INTEGER_KEY, NULL);
    test_performance_store(hm);
    test_performance_lookup(hm);
    delete_hashmap(hm);
}

int main(int argc, char *argv[]) {
    printf("Test result\n");
    printf("---------------\n");
    test_hashmap();
    printf("Hurray! All tests passed successfully!\n");
    test_performance();
    printf("---------------\n");
    return 0;
}
