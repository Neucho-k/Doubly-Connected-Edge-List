typedef struct {
    double x, y;
}point_t;

typedef struct {
    int strt_edge, end_edge;
}split_t;

typedef struct edge{
    int idx_end, idx_strt, idx_edge;
    struct edge *next_edge, *prev_edge, *otr_edge;
    int corr_face;
}edge_t;

typedef point_t* point_ptr;
typedef split_t* split_ptr;
typedef edge_t* edge_ptr;

void 
polygon_read(char* filename, int*num, point_ptr **location);

void 
points_free(int* num, point_ptr **location);

void 
splits_read(int* num, split_ptr **location);

void 
splits_free(int* num, split_ptr **location);

void 
dcel_create(int* num_vertcs, point_ptr **vertics, int* num_dcel, 
                edge_ptr **location);

void 
dcel_free(int* num, edge_ptr **dcel);

void 
split_polygon(int* num_faces, edge_ptr** faces, int* num_vertics, 
                point_ptr **vertics, int* num_dcel, edge_ptr **dcel, 
                int* num_splits, split_ptr **splits);

void 
faces_create(edge_ptr **dcel, int* num_faces, edge_ptr** faces);

void 
faces_free(int* num_faces, edge_ptr** faces);