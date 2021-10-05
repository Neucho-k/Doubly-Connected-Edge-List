#include<stdio.h>
#include<stdlib.h>
#include"topo_strt.h"
#include"watchtowers.h"

#define TRUE 1
#define FALSE 0

int tower_alloc_chk(edge_ptr* face, tower_ptr* tower, point_ptr** vertcs);
int yr_test_above(double x, double y, double m, double c);
int yr_test_below(double x, double y, double m, double c);
double gradient(double x1, double x2, double y1, double y2);

void 
output(char* filename, int* num_towers, tower_ptr **towers, int* num_dcel, 
        edge_ptr **dcel, int* num_faces, edge_ptr** faces, int* num_vertcs,
        point_ptr ** vertcs){
    int i, j;
    FILE *file = fopen(filename, "w");
    int pop_in_face[*num_faces];

    // Face 0
    for (i = 0; i < *num_faces; i++){
        fprintf(file, "%d\n", i);
        pop_in_face[i] = 0;

        for (j = 0; j < *num_towers; j++){
            if (tower_alloc_chk(&(*faces)[i], &(*towers)[j], vertcs) == TRUE) {
                pop_in_face[i]+=(*towers)[j]->pop;
                fprintf(file, "Watchtower ID: %s, Postcode: %s, "
                        "Population Served: %d, "
                        "Watchtower Point of Contact Name: %s, x: %f, y: %f\n", 
                        (*towers)[j]->ID, (*towers)[j]->postcode, 
                        (*towers)[j]->pop, (*towers)[j]->mang, 
                        (*towers)[j]->x, (*towers)[j]->y);
            }
        }

    }

    for (i = 0; i < *num_faces; i++) {
        fprintf(file, "Face %d population served: %d\n", i, pop_in_face[i]);
    }

    fclose(file);
    printf("Putput Done!\n");
}

int 
tower_alloc_chk(edge_ptr* face, tower_ptr* tower, point_ptr** vertcs) {
    // Returning True means insides the edge, Flase means outside the edge
    edge_ptr fst_edge = *face;
    int t = 0;
    double m, c;

    while (fst_edge != *face || t == 0) {
        double strt_x, strt_y, end_x, end_y;
        strt_x = (*vertcs)[fst_edge->idx_strt]->x;
        strt_y = (*vertcs)[fst_edge->idx_strt]->y;
        end_x = (*vertcs)[fst_edge->idx_end]->x;
        end_y = (*vertcs)[fst_edge->idx_end]->y;

        if ( strt_x == end_x ){
            // what if strt_x == end_strt_x
            if (strt_y <= strt_y) {
                // if str_y < end_y, line from bottom to top vertically
                if ( (*tower)->x >= strt_x ) {
                    t++;
                }
            }
            else {
                // if str_y > end_y, from top to bottom vertically
                if ( (*tower)->x <= strt_x ) {
                    t++;
                }
            }
        }
        else if ( strt_x > end_x ) {
            // what if strt_x > end_strt_x
            m = gradient(strt_x, end_x, strt_y, end_y);
            c = end_y - m * end_x;
            if (yr_test_above((*tower)->x, (*tower)->y, m, c) == TRUE) { // Condition true if above
                t++;
            }
            else {
                return FALSE;
            }

        }
        else if ( strt_x < end_x ){
            // what if strt_x < end_strt_x
            m = gradient(strt_x, end_x, strt_y, end_y);
            c = end_y - m * end_x;
            if (yr_test_below((*tower)->x, (*tower)->y, m, c) == TRUE) { // Condition true if below
                t++;
            }
            else {
                return FALSE;
            }
        }

        fst_edge = fst_edge->next_edge;
    }

    return TRUE;
}

double 
gradient(double x1, double x2, double y1, double y2){
    return ((y2-y1)/(x2-x1));
}

int 
yr_test_above(double x, double y, double m, double c) {
    int result = FALSE; 
    return result+=((y - (m * x + c)) >= 0);
}

int 
yr_test_below(double x, double y, double m, double c) {
    int result = FALSE; 
    return result+=((y - (m * x + c)) <= 0);
}