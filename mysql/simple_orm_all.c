#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include </usr/local/mysql/include/mysql.h>

#include "./orm_table.c"
#include "./orm_query.c"

//for github uploading..

MYSQL *orm_connect(char *server, char*user, char*password, char*database);
struct table *orm_all(MYSQL *database, struct table *table, struct query *select, struct query *where, struct query *order, struct query *limit);

int main() {
    MYSQL *database = orm_connect(
        "server", 
        "user_name", 
        "password", 
        "db_name"
    );

    struct table *table = orm_new_table("table_name");

    struct query *select = orm_new_query(
        "*",
        NULL,
        0
    );

    struct query *where = orm_new_query(
        "id = ?",
        (char *[]){"1"},
        1
    );

    struct query *order = orm_new_query(
        "grade DESC",
        NULL,
        0
    );

    struct query *limit = orm_new_query(
        "10",
        NULL,
        0
    );

    table = orm_all(
        database, 
        table, 
        select,
        where,
        order,
        limit
    );

    for (int i=0; i < table->rows_length; i++) {
        H_map(table->rows[i], orm_print_table_row, NULL);
    }
 
    orm_free_table(table);
    orm_free_query(select);
    orm_free_query(where);
    orm_free_query(order);
    orm_free_query(limit);

    mysql_close(database);
    return(0);
} 

MYSQL *orm_connect(char *server, char*user, char*password, char*database){
     MYSQL *conn = mysql_init(NULL);
     if (mysql_real_connect(conn, server, user, password, database, 0, NULL,0)){
         return (conn);
     } else {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
     }
}

struct table *orm_all(MYSQL *database, struct table *table, struct query *select, struct query *where, struct query *order, struct query *limit) {
    char *orm_get_query_string(char *table_name, struct query *select, struct query *where, struct query *order, struct query *limit);
    struct table *orm_fetch_data(MYSQL_RES *result, struct table *table);
    
    MYSQL_RES *result;
    char *query_string = orm_get_query_string(table->name, select, where, order, limit);
    
    if (mysql_query(database, query_string)) {
        fprintf(stderr, "%s\n", mysql_error(database));
        exit(1);
    } else {
        result = mysql_store_result(database);
    }

    orm_fetch_data(result, table);

    free(query_string);
    mysql_free_result(result);
    return table;
}

struct table *orm_fetch_data(MYSQL_RES *result, struct table *table) {
    table->rows_length = mysql_num_rows(result);
    table->rows = (Hash_table **)malloc(sizeof (Hash_table) * table->rows_length); //just alloc few memory for start
    table->columns_length = mysql_num_fields(result);

    MYSQL_ROW row;
    
    int i=0;

    while ((row = mysql_fetch_row(result))) {
        MYSQL_FIELD *fields = mysql_fetch_fields(result);
        unsigned long *lengths = mysql_fetch_lengths(result);
        table->rows[i] = H_new(table->columns_length); //alloc each row

        for (int j = 0; j < table->columns_length; j++) {
            char *value = malloc(sizeof(char) * (lengths[j] + 1));
            H_put(table->rows[i], fields[j].name, strcpy(value, (row[j] ? row[j] : "NULL")));
        }
        
        i++;
    }
    return table;
}



