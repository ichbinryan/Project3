//
// Created by Ryan Read on 11/27/16.
//

#include "simulator.h"

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
    printf("Node edges: ");
    while(rout.num_edges>i){
        printf("%d ", rout.edges[i]);
        i++;
    }

}

/**
 * generates random bandwidth and prop delay;
 * @param nodes
 * @return
 */
int generate_delay(struct node * nodes){



}

int main(int argc, char * argv[]){
    struct generated_graph grph;

    if(argc<3){
        printf("USAGE:  ./simulator.out <String: input graph filename> <int: seed for number generator>");
        exit(-1);
    }

    struct packet packets_on_wire[100000];
    int packet_delay[1000000] = {-1};

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
    for(int i = 0; i<num_edges; i++){
        fscanf(file, "%d %d", &little[i], &big[i]);
        //fscanf(file, "%d", &big[i]);

        printf("Little an big: %d %d\n", little[i], big[i]);

        nodes[little[i]].edges[nodes[little[i]].num_edges] = big[i];
        nodes[big[i]].edges[nodes[big[i]].num_edges] = little[i];
        printf("set test: %d\n", nodes[little[i]].edges[nodes[little[i]].num_edges]);
        nodes[little[i]].num_edges++;
        nodes[big[i]].num_edges++; //need to remember to do both in all cases.
        //our graph of nodes has now been generated using node struct.

    }
    fclose(file);

    print_sample_node(nodes[149]);


} //end main

