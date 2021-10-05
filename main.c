/* COMP20003 Assignment_1*/

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"watchtowers.h"
#include"topo_strt.h"
#include"allo_output.h"


int 
main(int argc, char** argv){
    int num_towers = 0, num_vertices = 0, num_splits = 0;
    int num_dcel = 0, num_faces = 0;
    tower_ptr* towers = NULL;
    point_ptr* vertics = NULL; 
    split_ptr* splits = NULL;
    edge_ptr* dcel = NULL;
    edge_ptr* faces = NULL;


    // Check if there are enough arguments
    if (argc < 2) {
        printf("Argc is less than 2!\n");
        return 0;
    }

    towers_read(argv[1], &num_towers, &towers);
    polygon_read(argv[2], &num_vertices, &vertics);
    dcel_create(&num_vertices, &vertics, &num_dcel, &dcel);
    faces_create(&dcel, &num_faces, &faces);
    splits_read(&num_splits, &splits);
    split_polygon(&num_faces, &faces, &num_vertices, &vertics, &num_dcel, &dcel, &num_splits, &splits);
    
    /*// Printing the vertices
    for (int j = 0; j < num_vertices; j++) {
        printf("%dth: x = %f and y = %f\n", j, vertics[j]->x, vertics[j]->y);
    }*/

    //Printing DCEL
    for (int i = 0; i < num_dcel; i ++) {
        printf("%dth address = %p, idx_strt = %d, idx_end = %d, prev_edge = %p, next_edge = %p, otr_edge = %p, corr_face = %d, idx_edge = %d\n", 
                i, dcel[i], dcel[i]->idx_strt, dcel[i]->idx_end, dcel[i]->prev_edge, dcel[i]->next_edge, dcel[i]->otr_edge, dcel[i]->corr_face, dcel[i]->idx_edge);
    }

    /*// printing Face
    for (int i =0; i < num_faces; i++) {
        printf("%dth face points to %p\n", i, faces[i]);
    }*/

    output(argv[3], &num_towers, &towers, &num_dcel, &dcel, &num_faces, &faces, &num_vertices, &vertics);
    

    towers_free(&num_towers, &towers);
    points_free(&num_vertices, &vertics);
    splits_free(&num_splits, &splits);
    dcel_free(&num_dcel, &dcel);
    faces_free(&num_faces, &faces);
    return 0;
}
