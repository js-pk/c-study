#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct query {
    char *query_raw;
    char *query_with_values;
    char **values;
    int values_length;
} query;

struct query *orm_new_query(char *query_raw, char **values, int values_length);
void orm_free_query(struct query *query);

struct query *orm_new_query(char *query_raw, char **values, int values_length) {
    char *orm_get_query_with_values(const char *string, char **values, int values_length);

    if (!query_raw) {
        printf("Error while initing query: query_raw should not be NULL!\n");
        exit(1);
    }

    struct query *query = malloc(sizeof (struct query));
    query->query_raw = (char *) malloc(strlen(query_raw) + 1);
    strcpy(query->query_raw, query_raw);
    query->values = values;
    query->values_length = values_length;
    query->query_with_values = orm_get_query_with_values(query->query_raw, query->values, query->values_length);

    return query;
}

void orm_free_query(struct query *query) {
    free(query->query_raw);
    free(query->query_with_values);
    free(query);
}

char *orm_get_query_string(char *table_name, struct query *select, struct query *where, struct query *order, struct query *limit) {
    
    int string_length = 
        (select ? strlen(select->query_with_values) + 5 + 1 : 0) +
        (table_name ? strlen(table_name) + 4 + 2 : 0) +
        (where ? strlen(where->query_with_values) + 5 + 2 : 0) + 
        (order ? strlen(order->query_with_values) + 8 + 2 : 0) + 
        (limit ? strlen(limit->query_with_values) + 5 + 2 : 0);
    
    char *string = (char *) malloc(string_length + 1);

    if (select) {
        string = strcat(string, "SELECT ");
        string = strcat(string, select->query_with_values);
    } else {
        string = strcat(string, "SELECT *");
    }

    if (table_name) {
        string = strcat(string," FROM ");
        string = strcat(string, table_name); 
    } else {
        fprintf(stderr, "Table name should not be NULL\n");
        exit(1);
    }

    if (where) {
        string = strcat(string, " WHERE ");
        string = strcat(string, where->query_with_values);
    }

    if (order) {
        string = strcat(string, " ORDER BY ");
        string = strcat(string, order->query_with_values);
    }

    if (limit) {
        string = strcat(string, " LIMIT ");
        string = strcat(string, limit->query_with_values);
    }

    return string;
}

char *orm_get_query_with_values(const char *string, char **values, int values_length) {
    char *parameter = "?";

    size_t values_char_length = 0;
    size_t i;
    for (i=0; i<values_length; i++) {
        values_char_length += strlen(values[i]);
    }

    const char *p = strstr(string, parameter);
    size_t string_length = strlen(string) - 1;
    size_t replaced_string_length = string_length + values_char_length;

    char *result = (char *) malloc(replaced_string_length + 1);

    if (!values_char_length || !p) { //if no arguments to insert value, return copy of the original string
        return strcpy(result, string);    
    }

    char *dst = result;
    const char *start_substring = string;

    for (i=0; i<values_length; ++i) {
        const char *end_substring = strstr(start_substring, parameter);
        size_t substring_length = end_substring - start_substring;
        memcpy(dst, start_substring, substring_length);
        dst += substring_length;
        memcpy(dst, values[i], strlen(values[i]));
        dst += strlen(values[i]);
        start_substring = end_substring + 1;
    }

    size_t remains = string_length - (start_substring - string) + 1;
    memcpy(dst, start_substring, remains);

    return result;
}
