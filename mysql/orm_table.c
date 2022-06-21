#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./hash.c"

struct table {
    char *name;
    int rows_length;
    int columns_length;
    Hash_table **rows;
} table;


struct table *orm_new_table(char *name);
void orm_free_table(struct table *table);
void orm_print_table_row(char *key, void *value, void *callback);

struct table *orm_new_table(char *name) {
    struct table *table = malloc(sizeof (struct table));
    table->name = "places";

    return table;
}

void orm_free_table(struct table *table) {
    for (int i=0; i < table->rows_length; i++) {
        H_free(table->rows[i]); //free(p->value)를 hash.c 안에 넣었는데 좋은 선택이 아닌 것같음.. 
    }
    free(table->rows);
    free(table);
}

void orm_print_table_row(char *key, void *value, void *callback) {
    printf("key:%s, value:%s\n", key, value);
}