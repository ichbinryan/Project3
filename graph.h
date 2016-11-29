//
// Created by Ryan Read on 11/21/16.
//

#ifndef PROJECT3_GRAPH_H
#define PROJECT3_GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 150

int num_nodes;
int num_edges;

int ran(int k);
int permute(int * array, int nodes);
//struct graph generate_graph();
int generate_graph(char * file_name, int nodes, int edges);
//int write_to_file(int * graph[]);
//int connected();
int permute(int * gra, int size);


struct graph{
    int nodes[150][100];
};

#endif //PROJECT3_GRAPH_H
