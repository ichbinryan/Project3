//
// Created by Ryan Read on 11/27/16.
//

#include "simulator.h"
#include <limits.h>

int graph[MAX][MAX];  //to build adjacency matrix.

//easier to deal with in dijkstra

int generate_network_node_array(struct router * node, char * filename){
    int size = 150;
    int next_int;

    FILE *f = fopen(filename, "r");
    rewind(f);

    while(fscanf(f, "%d", &next_int) == 1){
        printf("test this: %d\n", next_int);
    }
    fclose(f);

}

int dijkstras(struct router * nodes, struct router source){

    int distances[MAX];
    int nod[MAX];
    int unvisited[MAX];



    /*

    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAX; j++){
            graph[i][nodes[i].edges[j]] = nodes[i].edge_propagation_delay[j];
        }
    }

    for(int i = 0; i < MAX; i++){
        printf("%d: ", i);
        for(int j = 0; j < MAX; j++){
           printf("%d ", graph[i][j]);
        }
        printf("\n");
    }

    int big = INT_MAX;

    int dist[MAX] = {big};
    int prev[MAX] = {-1};

    dist[source.id] = 0;*/

    //int * queue = makeQueue(source);

}

void makeQueue(struct router s){
    //use a copy of s
    struct router v = s;
    printf("S and Vs num edges - %d %d\n", s.num_edges, v.num_edges);
    //v.id = 1000;
    //printf("The id is %d\n", s.id);


    int node[MAX] = {-1};
    int length[MAX] = {-1};
    int indices[MAX] = {-1};
    int placed  = 0;
    for(int i  = 0; i<=10; i++){
        for(int j = 0; j < v.num_edges;j++) {
            while (v.edge_propagation_delay[j] == i) {
                length[placed] = i;
                node[placed] = v.edges[j];
                indices[placed] = j;
                placed++;
                j++;
            }
        }
    }
    for(int i = 0; i <10; i++){
        printf("lenght: %d \n", length[i]);
        printf("edges: %d\n", node[i]);
        printf("indices: %d\n", indices[i]);
    }

    /*
    for(int i = 0; i<10; i++){
        for(int j = 0; j<v.num_edges; j++) {
            while (v.edge_propagation_delay[j] == i) {
                v.edges[placed] = v.edges[j];
                v.edge_propagation_delay[placed] = i;
                placed++;
            }
        }
    }*/


}



int minDistance(int distance[], int set[]){
    int min = 15;
    int index = -1;
    for(int i = 0; i<MAX; i++){
        if(set[i] == 0 && distance[i]<min && distance[i] != -1){
            min = distance[i];
            index = i;
        }
    }
    return index;
}

int generate_trip(struct source_destination pairs){
    for(int i = 0; i < 20; i++){
        pairs.source[i]=-1;
        pairs.destination[i]=-1;
    }

    int s;
    int d;

    for(int i = 0; i < 20; i++){
        s = ran(MAX);

        pairs.source[i] = s;
        for(int j = 0; j<i; j++){ //fill sources
            if(pairs.source[i] == pairs.source[j]){
                i--; //this is to generate unique nodes
            }
        }

    }

    for(int i = 0; i < 20; i++){
        d = ran(MAX);

        pairs.destination[i] = d;
        for(int j = 0; j<i; j++){
            if(pairs.destination[i] == pairs.destination[j]){
                i--;
            }
        }

    }

    for(int i = 0; i < 20; i++){
        if(pairs.source[i] == pairs.destination[i]){
            pairs.destination[i] = ran(MAX); //make sure that we are not
            // sending a packet from a node to itself
        }
    }

    printf("s:");

    for(int i = 0; i<20; i++){
        printf("%d ", pairs.source[i]);
    }

    printf("\n");
    printf("d:");

    for(int i = 0; i<20; i++){
        printf("%d ", pairs.destination[i]);
    }
    printf("\n");

} //end generate trip

//this will go up to 1000000 for our mbps to bps conversion
int standard_uniform(int max){ //for
    float x = (float) rand()/RAND_MAX;
    int y = (int) (max * x);
    return y;
} //end standard_uniform

int standard_uniform_interval(int a, int b){
    float u = (float) rand()/RAND_MAX;
    int y = (int) (a + u * (b-a));
    return y;
} //end standard_uniform_interval


int generate_bandwidth(struct network net, int edges){

    for(int i = 0; i < edges; i++){
        net.bandwidth[i] = standard_uniform_interval(1, 10);
    }

} //end generate_bandwidth

void test(int size, struct network net){
    for(int i = 0; i < 1000; i++){
        net.big[i] = -1;
    }
} //end test this can be deleted

void print_sample_node(struct router rout){
    int i = 0;
    printf("Node ID: %d\n", rout.id);
    printf("Node edges: ");
    while(rout.num_edges>i){ //change to for loop
        printf("%d ", rout.edges[i]);
        i++;
    }
    printf("\n");

    printf("Node delay: ");
    for(int j = 0; j<rout.num_edges; j++){
        printf("%d ", rout.edge_propagation_delay[j]);
    }
    printf("\n");

}

/**
 * generates random bandwidth and prop delay;
 * @param nodes
 * @return
 */
int generate_delay(struct node * nodes){

//TODO:Modularize some of main, it is getting big.

}

int main(int argc, char * argv[]){
    struct generated_graph grph;

    if(argc<3){
        printf("USAGE:  ./simulator.out <String: input graph filename> <int: seed for number generator>");
        exit(-1);
    }

    //struct packet packets_on_wire[100000];
    //int packet_delay[1000000] = {-1};

    char * filename = argv[1];
    int seed = atoi(argv[1]);

    //seed = 2;

    struct source_destination sd;

    srand(seed);
    int num_edges = 600;
    int try = generate_graph(filename, MAX, num_edges);
    generate_trip(sd);

    //edges information
    //this is my network layer
    int big[num_edges];
    int little[num_edges];

    //read file into arrays
    struct router * nodes = malloc(150 * sizeof(struct router));

    FILE *file = fopen(filename, "r");
    int num1, num2 = 0;

    fscanf(file, "%d", &num1);
    fscanf(file, "%d", &num2);

    for(int i = 0; i<150; i++){
        nodes[i].num_edges = 0;
        printf("num edge test %d\n", nodes[i].num_edges);
    }

    int del = 0;

    //we are setting properties of nodes and edges here.

    for(int i = 0; i<num_edges; i++){
        fscanf(file, "%d %d", &little[i], &big[i]);
        //fscanf(file, "%d", &big[i]);

        printf("Little an big: %d %d\n", little[i], big[i]);

        nodes[little[i]].edges[nodes[little[i]].num_edges] = big[i];
        nodes[big[i]].edges[nodes[big[i]].num_edges] = little[i];
        printf("set test: %d\n", nodes[little[i]].edges[nodes[little[i]].num_edges]);
        //inserting bandwidth and propagation delay
        //propagation delay first
        del = standard_uniform_interval(1, 10);
        printf("delay generated %d\n", del);
        nodes[little[i]].edge_propagation_delay[nodes[little[i]].num_edges] = del; //in seconds
        nodes[big[i]].edge_propagation_delay[nodes[big[i]].num_edges] = del; //in seconds
        nodes[little[i]].id = little[i];
        nodes[big[i]].id = big[i];

        //now bandwidth


        nodes[little[i]].num_edges++;
        nodes[big[i]].num_edges++; //need to remember to do both in all cases.
        //our graph of nodes has now been generated using node struct.
        //can add bandwidth/propagation delay here.

    }
    fclose(file);

    //run dijkstras on nodes

    //build routing tables, each node only need the next router to go to

    //begin scheduling of packets

    //begin simulation
    print_sample_node(nodes[149]);
    //dijkstras(nodes, nodes[0]);

    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAX; j++){
            graph[i][nodes[i].edges[j]] = nodes[i].edge_propagation_delay[j];
        }
    }

    for(int i = 0; i < MAX; i++){
        printf("%d: ", i);
        for(int j = 0; j < MAX; j++){
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }


} //end main

