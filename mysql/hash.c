#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct item{
    char *key;
    void *value;
    struct item *next;
} Item;

typedef struct {
    int size;
    int length;
    Item **buckets;
} Hash_table;

Hash_table *H_new(int);
unsigned H_hash(Hash_table *, char *);
void *H_put(Hash_table *, char *, void *);
void *H_get(Hash_table *, char *);
void *H_remove(Hash_table *, char *);
void H_free(Hash_table *);
void H_map(Hash_table *, void (char *, void *, void *), void *);

Hash_table *H_new(int hint) {
    static int primes[] = {53, 97, 211, 509, 1021, 2053, 4093, 8191, 16381, 32771, 65521};
    Hash_table *H;
    int i = 0;
    
    if (hint < primes[7]) 
        while(primes[i] < hint) i++;
    else 
        i=7;

    H = (Hash_table *) malloc(sizeof(Hash_table) + primes[i]*sizeof(H->buckets[0]));
    H->size = primes[i];
    H->buckets = (Item **)(H + 1);
    for (i=0; i<H->size; i++)
        H->buckets[i] = NULL;
    H->length = 0;

    return H;
}

unsigned H_hash (Hash_table *H, char *key) {
    unsigned h;
    for (h = 0; *key != '\0'; key++) {
        h = *key + 31 * h;
    }
    return h % H->size;
}

void *H_put(Hash_table *H, char *key, void *value) {
    Item *p;
    unsigned i = H_hash(H, key);
    for (p = H->buckets[i]; p; p = p->next) {
        if (strcmp(p->key, key) == 0) {
            break;
        }
    }
    if (p == NULL) {
        p = (Item *)malloc(sizeof(Item));
        p->key = key;
        p->value = value;
        p->next = H->buckets[i];
        H->buckets[i] = p;
        H->length++;

        return NULL;
    } else {
        void *old = p->value;
        p->value = value;
        return old;
    }
}

void *H_get(Hash_table *H, char *key) {
    Item *p;
    unsigned i = H_hash(H, key);
    for (p = H->buckets[i]; p; p = p->next) {
        if (strcmp(p->key, key) == 0) {
            return p->value;
        }
    }
    return NULL;
}

void *H_remove(Hash_table *H, char *key) {
    Item **pp;
    unsigned i = H_hash(H, key);
    for (pp = &(H->buckets[i]); *pp; pp = &((*pp)->next)) {
        if (strcmp((*pp)->key, key) == 0) {
            Item *copied = *pp;
            void *value = copied->value;
            *pp = copied->next; 
            free(copied);
            H->length--;
            return value;
        }
    }
    return NULL;
}

void H_map(Hash_table *H, void apply(char *key, void *value, void *callback), void *callback) {
    int i;
    Item *p;
    for (i=0; i < H->size; i++) {
        for (p = H->buckets[i]; p; p = p->next) {
            apply(p->key, p->value, callback);
        }
    }
}

void H_free(Hash_table *H) {
    int i;
    Item *p, *q;
    if (H->length > 0) {
        for (i=0; i < H->size; i++) {
            for (p = H->buckets[i]; p; p = q) {
                q = p->next;
                free(p->value);
                free(p);
            }
        }
    }
    free(H);
}

