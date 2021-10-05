#include<stdio.h>
#include<stdlib.h>
#include"topo_strt.h"

void 
splits_free(int* num, split_ptr **location) {
    int i;

    for (i = 0; i < *num; i++) {
        free((*location)[i]);
    }

    free(*location);
    printf("Free splits done!\n"); 
}