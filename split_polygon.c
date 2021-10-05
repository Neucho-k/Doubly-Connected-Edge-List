#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"topo_strt.h"

#define NUM_NEWPT 2
#define STRT_NEWPT 2
#define END_NEWPT 1
#define HALF 0.50
#define NEW_HEDGE_1 1
#define NEW_HEDGE_2 2
#define TWIN_EDGE 1
#define TRUE 1
#define FALSE 0

void midpoint(point_ptr midpt, point_ptr strt_pt, point_ptr end_pt);
void strt_n_end_edge(int* idx_strt_dcel, int* idx_end_dcel, int strt_edge, 
                        int end_edge, int* num_dcel, edge_ptr **dcel);

void 
split_polygon(int* num_faces, edge_ptr** faces, int* num_vertics, 
                point_ptr **vertics, int* num_dcel, edge_ptr **dcel, 
                int* num_splits, split_ptr **splits){

    int i, idx_strt_dcel, idx_end_dcel, nth_nface, nth_edge = *num_dcel;
    int strt_edge_twin = FALSE, end_edge_twin = FALSE;

    edge_ptr tmp_C1A2_edge, tmp_C2C1_edge;
    edge_ptr tmp_B1C2_edge, tmp_edge, new_edge_ptr;

    // Reading the split data
    for (i = 0; i < *num_splits; i++){
        printf("%dth split below: \n", i);
        strt_n_end_edge(&idx_strt_dcel, &idx_end_dcel, (*splits)[i]->strt_edge,
                             (*splits)[i]->end_edge, num_dcel, dcel);
        
        /************************vertics**************************************/
        // Allocating memory for two new mid-points
        *num_vertics+=NUM_NEWPT;
        *vertics=(point_ptr*)realloc(*vertics,sizeof(point_ptr)*(*num_vertics));
        assert(*vertics);
        (*vertics)[*num_vertics - STRT_NEWPT] = malloc(sizeof(point_t));
        assert((*vertics)[*num_vertics - STRT_NEWPT]);
        (*vertics)[*num_vertics - END_NEWPT] = malloc(sizeof(point_t));
        assert((*vertics)[*num_vertics - END_NEWPT]);

        // First mid-point at the strarted edge
        midpoint( (*vertics)[*num_vertics - STRT_NEWPT], 
                    (*vertics)[(*dcel)[idx_strt_dcel]->idx_strt], 
                    (*vertics)[(*dcel)[idx_strt_dcel]->idx_end]);
        
        // Second mid-point at the ended edge
        midpoint( (*vertics)[*num_vertics - END_NEWPT], 
                    (*vertics)[(*dcel)[idx_end_dcel]->idx_strt], 
                    (*vertics)[(*dcel)[idx_end_dcel]->idx_end]);

        /***************************dcel**************************************/
        // New edge(first-half, clockwise) created and numbered in DCEL
        *dcel=(edge_ptr*)realloc(*dcel,sizeof(edge_ptr)*
                                        (*num_dcel+NEW_HEDGE_2));
        assert(*dcel);
        (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
        assert((*dcel)[*num_dcel]);
        new_edge_ptr = (*dcel)[*num_dcel]; // New edge pointer saved for future used

        (*dcel)[*num_dcel]->idx_strt = (*num_vertics - STRT_NEWPT);
        (*dcel)[*num_dcel]->idx_end = *num_vertics - END_NEWPT;
        (*dcel)[*num_dcel]->next_edge = (*dcel)[idx_end_dcel];
        (*dcel)[*num_dcel]->prev_edge = (*dcel)[idx_strt_dcel];
        (*dcel)[*num_dcel]->corr_face = (*dcel)[idx_strt_dcel]->corr_face; // Old face
        (*dcel)[*num_dcel]->idx_edge = nth_edge; // clockweise half edge

        (*num_dcel)++; // Number of dcel update
        nth_edge++;

        // Number of faces update
        (*num_faces)++;
        nth_nface = *num_faces - 1;
        *faces = (edge_ptr*) realloc(*faces, sizeof(edge_ptr) * (*num_faces));
        assert(*faces);

        // New edge(second-half, anti-clockwise) created and numbered in DCEL
        (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
        assert((*dcel)[*num_dcel]);
        tmp_C2C1_edge = (*dcel)[*num_dcel]; // Saving the pointer of this edge for future update
        (*faces)[nth_nface] = (*dcel)[*num_dcel];

        (*dcel)[*num_dcel]->idx_strt = *num_vertics - END_NEWPT; 
        (*dcel)[*num_dcel]->idx_end = *num_vertics - STRT_NEWPT;
        (*dcel)[*num_dcel]->otr_edge = (*dcel)[*num_dcel-TWIN_EDGE];
        (*dcel)[*num_dcel]->corr_face = nth_nface; // New face
        (*dcel)[*num_dcel]->idx_edge = new_edge_ptr->idx_edge; // anticlockwise half edge
        
        (*dcel)[*num_dcel - 1]->otr_edge = (*dcel)[*num_dcel]; // Assigning clockwise new hedge->ort_edge

        (*num_dcel)++; // Number of dcel update

        // C1A2: New edge with strt_midpt to endpt of start_edge 
        *dcel = (edge_ptr*) realloc(*dcel, sizeof(edge_ptr) * 
                                        (*num_dcel + NEW_HEDGE_1));
        assert(*dcel);
        (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
        assert((*dcel)[*num_dcel]);

        tmp_C2C1_edge->next_edge = (*dcel)[*num_dcel]; // Upadge the next edge for previous edge
        tmp_C1A2_edge = (*dcel)[*num_dcel]; // Saving the pointer of this edge for future update

        (*dcel)[*num_dcel]->idx_strt = *num_vertics - STRT_NEWPT; // idex_startpt
        (*dcel)[*num_dcel]->idx_end = (*dcel)[idx_strt_dcel]->idx_end; // idex_endpt
        (*dcel)[*num_dcel]->prev_edge = (*dcel)[*num_dcel-1]; // Prev_edge
        (*dcel)[*num_dcel]->next_edge = (*dcel)[idx_strt_dcel]->next_edge; // Next_edge
        if ((*dcel)[idx_strt_dcel]->otr_edge == NULL) { // Means no twins for strt_pt
            (*dcel)[*num_dcel]->otr_edge = NULL;
            strt_edge_twin = FALSE;
        }
        else { // C1A2 has twin edge == strt_edge has twin
            strt_edge_twin = TRUE;
        }
        (*dcel)[*num_dcel]->idx_edge = nth_edge; //
        (*dcel)[*num_dcel]->corr_face = nth_nface; // New face
        (*num_dcel)++; // Number of dcel update
        nth_edge++;
       

        // B1C2: New edge with strtpt of A2B1 to end_mitpt
        *dcel = (edge_ptr*) realloc(*dcel, sizeof(edge_ptr) * 
                    (*num_dcel + NEW_HEDGE_1));
        assert(*dcel);
        (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
        assert((*dcel)[*num_dcel]);
        tmp_B1C2_edge = (*dcel)[*num_dcel];

        tmp_edge = tmp_C1A2_edge; // tmp = prev_edge(C1A2)
        while (tmp_edge->next_edge != (*dcel)[idx_end_dcel]) {
            tmp_edge->next_edge->prev_edge = tmp_edge;
            tmp_edge = tmp_edge->next_edge;
            tmp_edge->corr_face = nth_nface; // Update new index of faces
        }
        tmp_edge->next_edge = tmp_B1C2_edge; // Update the next_edge for the prev_edge of ended_edge
        
        (*dcel)[*num_dcel]->idx_strt = tmp_edge->idx_end;
        (*dcel)[*num_dcel]->idx_end = *num_vertics - END_NEWPT;
        (*dcel)[*num_dcel]->prev_edge = tmp_edge;
        (*dcel)[*num_dcel]->next_edge = tmp_C2C1_edge;
        if ((*dcel)[idx_end_dcel]->otr_edge == NULL) {
            (*dcel)[*num_dcel]->otr_edge = NULL;
            end_edge_twin = FALSE;
        }
        else { // B1C2 has twin edge == end_strt has twin
            end_edge_twin = TRUE;
        }
        (*dcel)[*num_dcel]->idx_edge = nth_edge;
        (*dcel)[*num_dcel]->corr_face = nth_nface;
        tmp_C2C1_edge->prev_edge = (*dcel)[*num_dcel]; // Update twin hedge -> prev_edge
        (*num_dcel)++;
        nth_edge++;

        // Processing the strated edge
        (*dcel)[idx_strt_dcel]->idx_end = tmp_C1A2_edge->idx_strt; // Change the idx of end_point of strt_edge
        (*dcel)[idx_strt_dcel]->next_edge = new_edge_ptr; 
        if (strt_edge_twin == TRUE) { // Means twins edge exited for strt_pt
             /* changing data for twin edge, maybe into a function*/
            edge_ptr strt_otr_edge = (*dcel)[idx_strt_dcel]->otr_edge;
            
            // First half idx_strt (starting point changed & prev_edge)
            strt_otr_edge->idx_strt = new_edge_ptr->idx_strt;

            // Second half (ending point changed)
            *dcel = (edge_ptr*) realloc(*dcel, sizeof(edge_ptr) 
                        * (*num_dcel + NEW_HEDGE_1));
            assert(*dcel);
            (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
            assert((*dcel)[*num_dcel]);
            
            (*dcel)[*num_dcel]->idx_strt = tmp_C1A2_edge->idx_end;
            (*dcel)[*num_dcel]->idx_end = tmp_C1A2_edge->idx_strt;
            (*dcel)[*num_dcel]->next_edge = strt_otr_edge;
            (*dcel)[*num_dcel]->prev_edge = strt_otr_edge->prev_edge;
            strt_otr_edge->prev_edge = (*dcel)[*num_dcel]; // Update first half->prev_edge
            (*dcel)[*num_dcel]->otr_edge = tmp_C1A2_edge;
            tmp_C1A2_edge->otr_edge = (*dcel)[*num_dcel];
            (*dcel)[*num_dcel]->idx_edge = tmp_C1A2_edge->idx_edge;
            (*dcel)[*num_dcel]->corr_face = strt_otr_edge->corr_face;

            (*num_dcel)++;

        }

        // Processing the ended edge
        (*dcel)[idx_end_dcel]->idx_strt = tmp_B1C2_edge->idx_end; // Change the idx of start_point of end_edge
        (*dcel)[idx_end_dcel]->prev_edge = new_edge_ptr; 

        if (end_edge_twin == TRUE) { // Means twins edge exited for end_pt
            /* changing data for twin edge*/
            edge_ptr end_otr_edge = (*dcel)[idx_end_dcel]->otr_edge;

            // First half idx_strt (starting point changed & prev_edge)
            end_otr_edge->idx_strt = new_edge_ptr->idx_end;

            // Second half (ending point changed)
            *dcel = (edge_ptr*) realloc(*dcel, sizeof(edge_ptr) 
                        * (*num_dcel + NEW_HEDGE_1));
            assert(*dcel);
            (*dcel)[*num_dcel] = (edge_ptr) malloc(sizeof(edge_t));
            assert((*dcel)[*num_dcel]);

            (*dcel)[*num_dcel]->idx_strt = tmp_B1C2_edge->idx_end;
            (*dcel)[*num_dcel]->idx_end = tmp_B1C2_edge->idx_strt;
            (*dcel)[*num_dcel]->prev_edge = end_otr_edge;
            (*dcel)[*num_dcel]->next_edge = end_otr_edge->prev_edge;
            end_otr_edge->prev_edge = (*dcel)[*num_dcel]; // Update first half->prev_edge
            (*dcel)[*num_dcel]->otr_edge = tmp_B1C2_edge;
            tmp_B1C2_edge->otr_edge = (*dcel)[*num_dcel];
            (*dcel)[*num_dcel]->idx_edge = tmp_B1C2_edge->idx_edge;
            (*dcel)[*num_dcel]->corr_face = end_otr_edge->corr_face;

            (*num_dcel)++;
        }

    }
    printf("Split_polyton done!\n");

}
/*****************************************************************************/
void 
midpoint(point_ptr midpt, point_ptr strt_pt, point_ptr end_pt) {

    midpt->x = (strt_pt->x + end_pt->x) * HALF;
    midpt->y = (strt_pt->y + end_pt->y) * HALF;
    
}

void 
strt_n_end_edge(int* idx_strt_dcel, int* idx_end_dcel, int strt_edge, 
                    int end_edge, int* num_dcel, edge_ptr **dcel) {
    int i, j;

    for (i = 0; i < *num_dcel; i++) {
        if ( (*dcel)[i]->idx_edge == strt_edge ) {

            for (j = 0; j < *num_dcel; j++) {
                if ( (*dcel)[j]->idx_edge == end_edge ) {

                    if ((*dcel)[i]->corr_face == (*dcel)[j]->corr_face) {
                        *idx_strt_dcel = i;
                        *idx_end_dcel = j;
                        printf("*idx_strt_dcel = %d and *idx_end_dcel = %d\n", *idx_strt_dcel, *idx_end_dcel);
                        return;
                    }
                }
            }
        }
    }

    printf("Splliting line is crossing some exciting line, Error!\n");
    exit(EXIT_FAILURE);
}