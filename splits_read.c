#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"topo_strt.h"

void 
splits_read(int* num, split_ptr **location){
    int strt, end;

    printf("Enter the splits, plz: \n");
    while(scanf("%d %d", &strt, &end) == 2) {
        *location = (split_ptr*) realloc(*location, sizeof(split_ptr) * (*num + 1));
        assert(*location);
        (*location)[*num] = (split_ptr) malloc(sizeof(split_t));
        assert((*location)[*num]);

        (*location)[*num]->strt_edge = strt;
        (*location)[*num]->end_edge = end;

        // Updating numbeb of points
        (*num)++; // Check if needs backets
    }

    printf("Splits Completed!\n");  
}