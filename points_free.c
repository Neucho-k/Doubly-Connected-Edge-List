#include<stdio.h>
#include<stdlib.h>
#include"topo_strt.h"

void 
points_free(int* num, point_ptr **location) {
    int i;

    for (i = 0; i < *num; i++) {
        free((*location)[i]);
    }

    free(*location);
    printf("Free points done!\n"); 
}