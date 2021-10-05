voronoi1: main.o towers_read.o towers_free.o polygon_read.o points_free.o \
			splits_read.o splits_free.o dcel_create.o dcel_free.o \
			split_polygon.o faces_create.o faces_free.o output.o
	gcc -Wall -o voronoi1 main.o towers_read.o towers_free.o polygon_read.o \
	    points_free.o splits_read.o splits_free.o dcel_create.o dcel_free.o \
		split_polygon.o faces_create.o faces_free.o output.o

main.o: main.c watchtowers.h topo_strt.h allo_output.h
	gcc -Wall -o main.o main.c -c

towers_read.o: towers_read.c watchtowers.h
	gcc -Wall -o towers_read.o towers_read.c -c

towers_free.o: towers_free.c towers_read.o watchtowers.h
	gcc -Wall -o towers_free.o towers_free.c -c

polygon_read.o: polygon_read.c topo_strt.h
	gcc -Wall -o polygon_read.o polygon_read.c -c

points_free.o: points_free.c polygon_read.o topo_strt.h
	gcc -Wall -o points_free.o points_free.c -c

splits_read.o: splits_read.c topo_strt.h
	gcc -Wall -o splits_read.o splits_read.c -c

splits_free.o: splits_free.c splits_read.o topo_strt.h
	gcc -Wall -o splits_free.o splits_free.c -c

dcel_create.o: dcel_create.c polygon_read.o topo_strt.h
	gcc -Wall -o dcel_create.o dcel_create.c -c

dcel_free.o: dcel_free.c dcel_create.o topo_strt.h
	gcc -Wall -o dcel_free.o dcel_free.c -c

split_polygon.o: split_polygon.c polygon_read.o splits_read.o \
                    dcel_create.o topo_strt.h
	gcc -Wall -o split_polygon.o split_polygon.c -c

faces_create.o: faces_create.c dcel_create.o topo_strt.h
	gcc -Wall -o faces_create.o faces_create.c -c

faces_free.o: faces_free.c faces_create.o topo_strt.h
	gcc -Wall -o faces_free.o faces_free.c -c

output.o: output.c topo_strt.h allo_output.h
	gcc -Wall -o output.o output.c -c
