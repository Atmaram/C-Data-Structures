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

int main(int argc, char *argv[]) {
    printf("Test result\n");
    printf("---------------\n");
    test_hashmap();
    printf("Hurray! All tests passed successfully!\n");
    printf("---------------\n");
    return 0;
}
