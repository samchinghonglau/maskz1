#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "list.c"

void test_map1 (void (*f) (element_t*, element_t), struct list* out_list, struct list* in_list) {
    for (int i = 0; i<list_len(in_list);i++){
        element_t *out = malloc(sizeof(element_t));
        f(out, list_get(in_list, i));
        list_append(out_list, *out);
        free(out);
    }
}
void test_foreach (void (*f) (element_t), struct list* list) {
    for (int i = 0; i < list->len; i++)
      f (list->data [i]);
}

void inc (element_t* rpv, element_t av) {
    intptr_t* rp = (intptr_t*) rpv;
    intptr_t a = (intptr_t) av;
    *rp = a + 1;
}

void print (element_t vv) {
    intptr_t v = (intptr_t) vv;
    printf("%ld\n", v);
}

int main() {
    intptr_t a[] = {1,2,3,4,5,6,7,8,9,10,11};
    struct list* l0 = list_create();
    struct list* l1 = list_create();
    list_append_array (l0, (element_t*) a, sizeof(a)/sizeof(a[0]));
    test_map1 (inc, l1, l0);
    test_foreach (print, l1);
}