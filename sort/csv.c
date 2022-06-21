#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHARACTHER_PER_LINE 1000
#define TRUE 1
#define FALSE 0

typedef struct {
    int id;
    char *name;
    int places_count;
} Node_data;

#include "linked_list.h"
#include "tree.h"

int main() {
    FILE *in = fopen("spaces.csv", "r");
    FILE *out = fopen("ordered_spaces.csv", "w");

    // List l_csv = l_read_csv(in);
    // l_csv = l_sort_list(l_csv, l_bubble_sort); // took 152.784402 seconds for sorting..
    // l_csv = l_sort_list(l_csv, l_merge_sort); // took 0.024062 seconds for sorting..
    

    Binary_tree t_csv = t_read_csv(in);

    clock_t begin = clock();
    Node_data to_insert;
    to_insert.id = 9999999;
    to_insert.name = "테스트";
    to_insert.places_count = 0;
    // l_find_or_insert(l_csv, to_insert);
    t_find_or_insert(t_csv, to_insert);
    clock_t end = clock();

    double time_spent = (double)(end - begin)/CLOCKS_PER_SEC;
    printf("took %f seconds for inserting..\n", time_spent);
    // linked_list: min 0.000002 second, max 0.025253 second
    // tree: min 0.000003 second, max 0.000268 second 

    // l_write_csv(l_csv, out);
    t_write_csv(t_csv, out);

    fclose(in);
    fclose(out);
}

