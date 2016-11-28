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

//struct router routers[MAX];

int generated_packets; //also packet id
int arrived_packets;

struct source_destination{  //randomly generated source destination pairs
    int source[20];
    int destination[20];
    double total_delay[20];
    int packets_sent[20];
    //array for poisson distribution for sources
};

struct network{
    int * small;  //smaller numbrered node
    int * big;    //larger numbered node
    int * bandwidth; //bandwidth in bps, not MBPS!!!
    int * prop_delay; //prop_delay in seconds
};

struct source{  //redistribute into router?
    float poisson;  //distribution to generate
    int prop_delay;
};

struct packet{
    int packet_id;
    int source;
    int destination;
    int size;  //size is in bytes, can be up to 1,000,000 bytes 1mb
    int location; //-1 for on the wire, or integer for in router queue
};

struct router{
    struct packet input[30];
    struct packet output[30];
    int routing_table[MAX][MAX];
};

struct generated_graph{
    int nodes[150][100];
};

int generate_trip(struct source_destination pairs);
int standard_uniform(int max);  //return an int from one to one thousand
int generate_bandwidth(struct network net, int edges);


#endif //PROJECT3_SIMULATOR_H
