#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0


int main (int argc, char *argv[]) {
    void bubble_sort(int*, int);

    if (argc == 1) {
        printf("Please type numbers to sort\n");
        exit(1);

    } else {
        int input_size = argc - 1;
        int *input_numbers = malloc((input_size)*sizeof *input_numbers);
        int i = 0;
        
        while(--argc > 0) input_numbers[i++] = atoi(*++argv);
        bubble_sort(input_numbers, input_size);
        for (i=0; i < input_size; i++) printf("%i ", input_numbers[i]);
        printf("\n");
    }
    return 0;
}

void bubble_sort(int *arr, int size) {
    void swap(int*, int*);

    int is_sorted = FALSE;
    int i=0;
        while(is_sorted == FALSE) {
            is_sorted=TRUE;
            i=0;
            while(i < size-1) {
                if (arr[i] > arr[i+1]) {
                    swap(&arr[i], &arr[i+1]);
                    is_sorted = FALSE;
                }
                i++;
            }
        }
}

void swap(int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}