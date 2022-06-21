#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHARACTHER_PER_LINE 1000
#define TRUE 1
#define FALSE 0

typedef struct list_node {
    Node_data data;

    struct list_node *prev;
    struct list_node *next;
} List_node, *List_node_ptr;

typedef struct list {
    List_node_ptr top;
    List_node_ptr bottom;
} List;


List l_merge(List, List);


List l_read_csv(FILE *in) {
    List_node_ptr curr, prev, top, l_get_node(char *);;
    List csv;

    char line[MAX_CHARACTHER_PER_LINE];
   
    fgets(line, MAX_CHARACTHER_PER_LINE, in);
    prev = NULL;
    curr = l_get_node(line);
    csv.top = curr;

    while(fgets(line, MAX_CHARACTHER_PER_LINE, in) != NULL) {
        prev = curr;
        curr = l_get_node(line);
        prev->next = curr;
        curr->prev = prev;
    }   
    csv.bottom = curr;
    return csv;
}

List_node_ptr l_get_node(char *l) {
    int length = 0;
    int i=0;
    int is_string_mode = FALSE;

    List_node_ptr rp = (List_node_ptr) malloc(sizeof(List_node));
    while(*l != '\0') {
        if ((is_string_mode == FALSE && *l == ',') || *l == '\n') {
            char column[length+1];            
            int j=0;
            while (j<length) {
                column[j] = l[-length+j];
                j++;
            }
            if (i==0) rp->data.id = atoi(column);
            else if (i==1) {
                column[length] = '\0';
                rp->data.name = strdup(column);
            } 
            else if (i==2) rp->data.places_count = atoi(column);
            length = 0;
            i++;
        } else if (*l == '"'){
            is_string_mode  = (is_string_mode == TRUE ? FALSE : TRUE);
            length++;
        } else {
            length++;
        }
        l++;
    }

    rp->next = NULL;
    rp->prev = NULL;

    return rp;
}

List l_sort_list(List l, List (*sort)(List)) {
    clock_t begin = clock();
    l = sort(l);
    clock_t end = clock();
    double time_spent = (double)(end - begin)/CLOCKS_PER_SEC;
    printf("took %f seconds for sorting..\n", time_spent);
    return l;
}

List l_merge_sort(List l) {
    List_node_ptr l_get_mid(List);
    List left, right;
    List_node_ptr mid = l_get_mid(l);
    
    left.top = l.top;
    left.bottom = (mid != left.top ? mid->prev : left.top); 
    left.bottom->next = NULL;
    if (left.top != left.bottom) left = l_merge_sort(left);
    
    right.top = mid;
    right.bottom = (l.bottom != right.top ? l.bottom : right.top);
    right.bottom->next = NULL;
    if (right.top != right.bottom) right = l_merge_sort(right);

    return l_merge(left, right);
}

List_node_ptr l_get_mid(List l) {
    List_node_ptr fast, slow, mid;
    fast = l.top;
    slow = l.top;
    while (fast != l.bottom) {
        if (fast->next != NULL || fast->next != l.bottom) {
            if (fast->next->next != NULL) fast = fast->next->next;
            else fast = fast->next;
        } 
        slow = slow->next;
    }

    return slow;
} 

List l_merge(List l, List r) {
    // void push_to_result(List_node_ptr, List_node_ptr);

    List result;
    List_node_ptr left_curr = l.top;
    List_node_ptr right_curr = r.top;
    List_node_ptr result_curr = NULL;

    if (left_curr->data.places_count > right_curr->data.places_count) {
        result.top = left_curr;
        left_curr = left_curr->next;
    } else {
        result.top = right_curr;
        right_curr = right_curr->next;
    }
    result_curr = result.top;
    
    while(left_curr != NULL && right_curr != NULL) {
        if (left_curr->data.places_count > right_curr->data.places_count) {
            result_curr->next = left_curr;
            left_curr = left_curr->next;
            result_curr->next->prev = result_curr;
            result_curr = result_curr->next;
            // push_to_result(result_curr, left_curr);
        } else {
            result_curr->next = right_curr;
            right_curr = right_curr->next;
            result_curr->next->prev = result_curr;
            result_curr = result_curr->next;
            // push_to_result(result_curr, right_curr);
        }
    }
    while (left_curr == NULL && right_curr != NULL) {
        result_curr->next = right_curr;
        result_curr->next->prev = result_curr;
        result_curr = result_curr->next;
        right_curr = right_curr->next;
        // push_to_result(result_curr, right_curr);
    }
    while (right_curr == NULL && left_curr != NULL) {
        result_curr->next = left_curr;
        result_curr->next->prev = result_curr;
        result_curr = result_curr->next;
        left_curr = left_curr->next;
        // push_to_result(result_curr, left_curr);
    }
   
    result.bottom = result_curr;
    return result;
}

//인자로 넘어온 포인터를 수정한게 반영이 안되는듯.. 어떻게 처리?
// void push_to_result(List_node_ptr r, List_node_ptr i) {
//     printf("r:%i, i:%i\n", r->id, i->id);
//     r->next = i;
//     r->next->prev = r;
//     r = r->next;
//     i = i->next;
// }

List l_bubble_sort(List l) {
    void l_swap(List_node_ptr, List_node_ptr);
    int is_sorted = FALSE;
    List_node_ptr rp = l.top;
    while(is_sorted == FALSE) {
        rp = l.top;
        is_sorted=TRUE;

        if (rp->data.places_count < rp->next->data.places_count) {
            l.top = rp->next;
            l_swap(rp, rp->next);
            is_sorted = FALSE;
        }
        rp = rp->next;
 
        while(rp->next != NULL) {
            if (rp->data.places_count < rp->next->data.places_count) {
                l_swap(rp, rp->next);
                is_sorted = FALSE;
            }
            if (rp->next != NULL) rp = rp->next;
        }
    }
    return l;
}

void l_swap(List_node_ptr a, List_node_ptr b) {
    List_node_ptr next, prev;
    next = b->next;
    prev = a->prev;
    if (a->prev != NULL) a->prev->next = b;
    if (b->next != NULL) b->next->prev = a;
    
    b->next = a;
    b->prev = prev;

    a->next = next;
    a->prev = b;
}

void l_write_csv(List l, FILE *out) {
    List_node_ptr curr = l.top;
    while(curr->next != NULL) {
        fprintf(out, "%i, ", curr->data.id);
        fprintf(out, "%s, ", curr->data.name);
        fprintf(out, "%i\n", curr->data.places_count);
        curr = curr->next;
    }
    fprintf(out, "%i, ", curr->data.id);
    fprintf(out, "%s, ", curr->data.name);
    fprintf(out, "%i\n", curr->data.places_count);
}

List_node_ptr l_find_or_insert (List l, Node_data d) {
    List_node_ptr new_list_node (Node_data);
    List_node_ptr curr = l.top;

    if (l.top == NULL) {
        l.top = new_list_node(d);
        return l.top;
    }

    while (curr->data.places_count >= d.places_count && curr != NULL) {
        if (d.id == curr->data.id) {
            return curr;
        }
        if (curr->next == NULL) {
            List_node_ptr inserted = new_list_node(d);
            curr->next = inserted;
            inserted->prev = curr;
            return inserted;
        }
        curr = curr->next;
    }
    List_node_ptr inserted = new_list_node(d);
    List_node_ptr next = curr->next;
    curr->next = inserted;
    inserted->prev = curr;
    inserted->next = next;
    next->prev = curr;
    return inserted;
}

List_node_ptr new_list_node (Node_data d) {
    List_node_ptr n = (List_node_ptr) malloc(sizeof(List_node));
    n->data = d;
    return n;
}