//
// Created by Ryan Read on 11/27/16.
//

#ifndef PROJECT3_SIMULATOR_H
#define PROJECT3_SIMULATOR_H

#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 150

struct router routers[MAX];
struct link * links;
int generated_packets;
int arrived_packets;

struct source_destination{  //randomly generated source destination pairs
    int source[20];
    int destination[20];
};

struct link{
    int small;  //smaller numbrered node
    int big;    //larger numbered node
    int bandwidth; //bandwidth in bps, not MBPS!!!
};

struct source{  //redistribute into router?
    float poisson;  //distribution to generate
    int prop_delay;
};

struct packet{
    int source;
    int destination;
    int size;  //size is in bytes, can be up to 1,000,000 bytes 1mb
};

struct router{
    struct packet input[30];
    struct packet output[30];
    int routing_table[MAX][MAX];
};

int generate_trip(struct source_destination pairs);

#endif //PROJECT3_SIMULATOR_H
