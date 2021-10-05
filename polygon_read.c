#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"topo_strt.h"

void 
polygon_read(char* filename, int*num, point_ptr **location){
    double x, y;
    FILE *file = fopen(filename, "r");

    while(fscanf(file, "%lf %lf", &x, &y) != EOF) {
        *location = (point_ptr*) realloc(*location,sizeof(point_ptr)*(*num+1));
        assert(*location);
        (*location)[*num] = (point_ptr) malloc(sizeof(point_t));
        assert((*location)[*num]);

        (*location)[*num]->x = x;
        (*location)[*num]->y = y;

        // Updating numbeb of points
        (*num)++; // Check if needs backets
    }

    fclose(file);   
    printf("Polygen Completed!\n");
}