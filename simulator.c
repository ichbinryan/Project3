//
// Created by Ryan Read on 11/27/16.
//

#include "simulator.h"

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



}

int main(int argc, char * argv[]){

    if(argc<3){
        printf("USAGE:  ./simulator.out <String: input graph filename> <int: seed for number generator>");
        exit(-1);
    }
    char * filename = argv[1];
    int seed = atoi(argv[1]);

    //seed = 2;

    struct source_destination sd;

    srand(seed);
    rand();
    int try = generate_graph(filename, MAX, 300);
    generate_trip(sd);
}

