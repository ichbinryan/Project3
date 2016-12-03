/**
 * file for graph generation.
 * Generates graph, then tests for connectedness*
 * *edit, no longer tests.  the way graph is generated should ensure connectedness.
 */
#include "graph.h"

int ran(int k){ //rand is currently using seed from command line input from sim.c

    return rand() % k;

}

int permute(int * array, int nodes){
    int index = 0;
    int place;
    for(int i = 0; i < nodes; i++){
        index = ran(nodes);
        place = array[i];
        array[i] = array[index];
        array[index] = place;
    }
    for(int i = 0; i<nodes; i++){
        //check for equal;
        if(array[i] == i){
            return -1; //redo
        }
        printf("array: %d\n", array[i]);
    }
    return 1;
}

int generate_graph(char * file_name, int nodes, int edges){
    //srand(time(NULL));
    int permuted[MAX];
    int graph[MAX][MAX];

    for(int i = 0; i<MAX; i++){
        permuted[i]=i;
        for(int j = 0; j<MAX; j++) {
            graph[i][j] = -1;
        }
    }

    int perm = permute(permuted, MAX);

    while(perm<0){
        perm = permute(permuted, MAX);
    }

    //insert first value, then loop until we create a linear graph, ensuring connectivity

    int placed = 0;
    int current, next, low, hi, temp;
    int sec_index=0;

    while(placed<MAX-1){
        current = permuted[placed];
        next = permuted[placed+1];

        if(next < current){ //swap values
            temp = current;
            current = next;
            next = temp;
        }

        while(graph[current][sec_index]!=-1){
            sec_index++;
        }

        graph[current][sec_index] = next;
        placed ++;
        sec_index = 0;
    }

    for(int i = 0; i<MAX; i ++){
        printf("%d: ", i);
        for(int j = 0; j<MAX; j++){
            if(graph[i][j]!=-1) {
                printf("%d ", graph[i][j]);
            }
        }
        printf("\n");
    }
    printf("we have finished placing %d nodes.  Proceed to fill in edges.\n", placed+1);

    int from, to, ind;
    ind = 0;

    while(placed<edges){
        if(placed == edges){
            break;
        }
        from = ran(MAX);
        to = ran(MAX);
        while(to == from || to<from) {
            if (to == from) {
                while (to == from) {
                    to = ran(MAX);
                }
            }
            if (to < from) {
                temp = to;
                to = from;
                from = temp;
            }
        }

        while(graph[from][ind]!=-1){
            if(graph[from][ind] == to) break;
            else{
                ind ++;
            }

        }

        if(graph[from][ind] != to){
            graph[from][ind] = to;
            placed++;
            printf(".\n");

        }
        ind = 0;


    }//end inf while

    for(int i = 0; i<nodes; i ++){
        printf("%d: ", i);
        for(int j = 0; j<MAX; j++){
            if(graph[i][j]!=-1) {
                printf("%d ", graph[i][j]);
            }
        }
        printf("\n");
    }
    FILE *f = fopen(file_name, "w+");
    fprintf(f, "%d\t%d\n", nodes, edges);


    for(int i = 0; i<MAX; i++){
        for(int j = 0; j<MAX; j++){
            if(graph[i][j]!=-1){
                fprintf(f,"%d\t%d\n", i, graph[i][j]);
            }
        }
    }/**/
    fclose(f);
    //generate bandwidth.

}//end generate graph

int main(int argc, char * argv[]){
    if(argc<2 || atoi(argv[1])<0){
        printf("USAGE: ./graph.out <number to seed random number greater than 0>");
        exit(-1);
    }
    int seed = atoi(argv[1]);
    srand(seed);
    int edges = 150;
    printf("edges %d\n", edges);
    int extra = ran(500);
    printf("We are generating %d extra\n", extra);
    edges = edges+extra;
    generate_graph("file.txt", 150, edges);
}


