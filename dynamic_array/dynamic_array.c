#include <stdio.h>
#include <stdlib.h>

#define INITAL_ARRAY_SIZE 10

typedef struct {
    int max_size;
    int last_index;
    int *arr_ptr;
} Dynamic_array, *Dynamic_array_ptr;

Dynamic_array_ptr new_dynamic_array(int);
Dynamic_array_ptr d_push(Dynamic_array_ptr, int);
int d_pop(Dynamic_array_ptr);
int* alloc_array(int*, int);

int main () {
    Dynamic_array_ptr test = new_dynamic_array(5);

    printf("max size:%i, last index:%i, inited array:", test->max_size, test->last_index);
    for (int i=0; i<=test->last_index; i++) printf("%i, ", test->arr_ptr[i]);
    printf("\n");

    for (int i=0; i<10; i++) { d_push(test, i); }
    printf("max size:%i, last index:%i, pushed array:", test->max_size, test->last_index);
    for (int i=0; i<=test->last_index; i++) printf("%i, ", test->arr_ptr[i]);
    printf("\n");

    int pop = d_pop(test);
    printf("%i pop\n", pop);
}

Dynamic_array_ptr new_dynamic_array (int arr_size) {
    Dynamic_array_ptr d = (Dynamic_array_ptr) malloc(sizeof(Dynamic_array));
    d->max_size = (arr_size > INITAL_ARRAY_SIZE ? arr_size : INITAL_ARRAY_SIZE); // should be optimized
    d->arr_ptr = malloc(sizeof(int)*arr_size);
    d->last_index = -1;
    
    return d;
}

Dynamic_array_ptr d_push (Dynamic_array_ptr d, int item) {
    if (d->last_index < d->max_size - 1) {
        d->arr_ptr[d->last_index + 1] = item;
    } else {
        int *temp = alloc_array(d->arr_ptr, d->max_size * 2); // should be optimized
        free(d->arr_ptr);
        d->arr_ptr = temp;
        d->arr_ptr[d->last_index + 1] = item;
        d->max_size = d->max_size * 2;
    }
    d->last_index += 1;

    return d;
}

int d_pop (Dynamic_array_ptr d) {
    int item = d->arr_ptr[d->last_index];
    d->arr_ptr[d->last_index] = NULL;
    d->last_index -= 1;

    return item;
}

int* alloc_array (int* arr, int size) {
    int *arr_new = malloc(sizeof(int)*size);
    for (int i=0; i<size; i++) {
        arr_new[i] = arr[i];
    }
    
    return arr_new;
}