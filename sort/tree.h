#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHARACTHER_PER_LINE 1000
#define TRUE 1
#define FALSE 0

typedef struct tree_node {
    Node_data data;
    struct tree_node *left, *right, *parent;
} Tree_node, *Tree_node_ptr;

typedef struct {
    Tree_node_ptr root;
} Binary_tree;


Binary_tree t_read_csv (FILE *in) {
    Tree_node_ptr curr, t_get_node(Binary_tree, char*);
    Binary_tree csv;
    csv.root = NULL;

    clock_t begin = clock();
    char line[MAX_CHARACTHER_PER_LINE];
    fgets(line, MAX_CHARACTHER_PER_LINE, in);
    csv.root = t_get_node(csv, line);

    while(fgets(line, MAX_CHARACTHER_PER_LINE, in) != NULL) {
        t_get_node(csv, line);
    }   
    clock_t end = clock();
    double time_spent = (double)(end - begin)/CLOCKS_PER_SEC;
    printf("took %f seconds for building tree..\n", time_spent);

    return csv;
}

Tree_node_ptr t_get_node(Binary_tree t, char *l) {
    Tree_node_ptr t_find_or_insert(Binary_tree, Node_data);
    int length = 0;
    int i=0;
    int is_string_mode = FALSE;

    Tree_node_ptr tp = (Tree_node_ptr) malloc(sizeof(Tree_node));
    Node_data data;
    
    while(*l != '\0') {
        if ((is_string_mode == FALSE && *l == ',') || *l == '\n') {
            char column[length+1];            
            int j=0;
            while (j<length) {
                column[j] = l[-length+j];
                j++;
            }
            if (i==0) data.id = atoi(column);
            else if (i==1) {
                column[length] = '\0';
                data.name = strdup(column);
            } 
            else if (i==2) data.places_count = atoi(column);
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

    tp = t_find_or_insert(t, data);

    return tp;
}

Tree_node_ptr t_find_or_insert (Binary_tree t, Node_data d){
    Tree_node_ptr new_tree_node(Node_data);
    Tree_node_ptr curr = t.root;

    if (t.root == NULL) {
        t.root = new_tree_node(d);
        t.root->left = NULL;
        t.root->right = NULL;
        t.root->parent = NULL;
        return t.root;
    }

    while(curr->data.id != d.id) {
        if (curr->data.places_count < d.places_count) {
            if (curr->left == NULL) {
                curr->left = new_tree_node(d);
                curr->left->parent = curr;
                return curr->left;
            } else {
                curr = curr->left;
            }
        } else {
            if (curr->right == NULL) {
                curr->right = new_tree_node(d);
                curr->right->parent = curr;
                return curr->right;
            } else {
                curr = curr->right;
            }
        }
    }
    return curr;
}

Tree_node_ptr new_tree_node(Node_data d) {
    Tree_node_ptr n = (Tree_node_ptr) malloc(sizeof(Tree_node));
    n->data = d;
    n->left = NULL;
    n->right = NULL;
    return n;
}


void t_write_csv(Binary_tree tree, FILE *out) {
    Tree_node_ptr node = tree.root;
    if (node == NULL) return;
    Tree_node_ptr inorder_successor(Tree_node_ptr);

    if (node == NULL) return;
    while (node->left != NULL) node = node->left;
    while (node != NULL) {
        fprintf(out, "%i:%s:%i\n", node->data.places_count, node->data.name, node->data.id);
        node = inorder_successor(node);
    }
}

Tree_node_ptr inorder_successor(Tree_node_ptr node) {
    if (node->right != NULL) {
        node = node->right;
        while(node->left != NULL) node = node->left;
        return node;
    }
    Tree_node_ptr parent = node->parent;
    while(parent!=NULL && parent->right==node) {
        node=parent;
        parent=node->parent;
    }
    return parent;
}