//
// Created by Ryan Read on 11/27/16.
//

#include "simulator.h"
#include <limits.h>

int graph[MAX][MAX];  //to build adjacency matrix.
int band_graph[MAX][MAX];
//easier to deal with...
struct router * nodes;
struct source_destination sd;
struct packet wire_arr[1000];
struct packet dummy;
struct packet place_holder;
int totes_generated_packets; //also packet id
int totes_arrived_packets;
int totes_dropped_packets;

void print_sample_node(struct router rout);

int packet_to_wire(int node_id){ //Need to set next hop and arrival
    printf("packet %d put on wire!\n", place_holder.packet_id);
    int next = nodes[node_id].routing_table[place_holder.destination];
    place_holder.next_hop = next;
    place_holder.next_arrival_time = graph[place_holder.location][place_holder.next_hop];
    printf("This packet has %d wait time on wire going to %d\n", place_holder.next_arrival_time, place_holder.next_hop);
    int i = 0;
    while(1) {
        if (wire_arr[i].packet_id == -1) {
            printf("wire index %d\n", i);
            wire_arr[i] = place_holder;
            break;
        }
        i++;
    }
    nodes[node_id].output_queue[nodes[node_id].output_start] = dummy;
    nodes[node_id].output_start = (nodes[node_id].output_start+1)%20;
    nodes[node_id].output_elements--;
}

void destroy_packet(int node_id){
    nodes[node_id].input_queue[nodes[node_id].input_start] = dummy;
    nodes[node_id].input_start++;
    nodes[node_id].input_start = nodes[node_id].input_start%20;
    nodes[node_id].input_elements--;
}

struct packet generate_packet(int source, int destination){  //make a packet
    struct packet pack;
    //printf("Packet generated from %d to %d\n", source, destination);

    nodes[source].my_generated_packets++;
    //pack = malloc(1 * sizeof( struct packet ) );
    pack.packet_id = totes_generated_packets;
    //printf("pack id %d\n", totes_generated_packets);
    pack.source = source;
    pack.destination = destination;
    pack.next_arrival_time = nodes[source].edge_propagation_delay[nodes[source].routing_table[destination]];
    pack.size = ran(1000000);
    int band = nodes[source].routing_table[destination];

    pack.queue_wait_time = (pack.size/band_graph[source][nodes[source].routing_table[destination]])+1;
    //printf("bandwidth test %d\n", pack.queue_wait_time );

    totes_generated_packets++;
    //try to insert into queue now at return location.
    //in_queue(nodes[source], pack);
    return pack;

}

int in_queue(int id, struct packet p){ //add packet to input router.

    printf("entered in queue");

    if(nodes[id].input_elements>20){
        //print_sample_node(nodes[id]);
        printf("%d dropped packet %d\n", nodes[id].id, p.packet_id);
        nodes[id].my_dropped_packets++;
        totes_dropped_packets++;
        //set packet location to null
        //free(pack);
    }
    else{

        printf("with wait time %d\n", place_holder.queue_wait_time);
        p.location = id;
        nodes[id].input_queue[nodes[id].input_end] = p;
        nodes[id].input_end = nodes[id].input_end+1;
        nodes[id].input_end = nodes[id].input_end%20;
        nodes[id].input_elements++;

        printf("elements in input queue %d\n", nodes[id].input_elements);
    }

}

int in_to_out(int id){ //transfer from input to output queue


    printf("transferring packet %d at node %d\n", nodes[id].input_queue[nodes[id].input_start].packet_id
            , nodes[id].id);

    place_holder = nodes[id].input_queue[nodes[id].input_start];
    nodes[id].input_queue[nodes[id].input_start] = dummy;
    nodes[id].input_start = (nodes[id].input_start+1)%20;
    nodes[id].input_elements--;

    if(nodes[id].output_elements>=20){

        printf("%d dropped packet %d\n", nodes[id].id, place_holder.packet_id);
        //if(node.input_start<=node.input_end)
        //free(p);
    }
    else{
        nodes[id].output_queue[nodes[id].output_end] = place_holder;
        nodes[id].output_end++;
        nodes[id].output_end= nodes[id].output_end%20;
        nodes[id].output_elements++;

        printf("output elements %d\n", nodes[id].output_elements);

    }
}

int minDistance(int dist[], int visited[])
{
    int min_dist = INT_MAX;
    int min_location;

    for (int v = 0; v < MAX; v++)
        if (visited[v] == 0 && dist[v] <= min_dist)
            min_dist = dist[v], min_location = v;

    return min_location;
}


//takes global adjacency list, calculates shortest path
//this function will set the outgoing node for given node to source
//a little backward than one might expect.
//
void dijkstra_previous(int source)
{
    int previous[MAX];  //previous[i] will be the outgoing node for i to node source.
    int dist[MAX];     // Holds distance values.  We do not use these explicitly, but
    //we need to track for dijkstras

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

    dist[source.id] = 0;

    //int * queue = makeQueue(source);*/

    int visited[MAX];  // 0 for true 1 for false, have we visited this node?
    for (int i = 0; i < MAX; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }

    // we don't care about source to itself
    dist[source] = 0;

    // Find shortest path for all vertices
    //previous may be updated many times here.
    for (int loc = 0; loc < MAX-1; loc++)
    {

        int min = minDistance(dist, visited); //pick the smallest element

        // Mark the picked vertex as processed
        visited[min] = 1;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (int z = 0; z < MAX; z++)
            
            if (!visited[z] && graph[min][z] && dist[min] != INT_MAX) {
                if(dist[min]+graph[min][z] < dist[z]) { //if dist(u, v) currently more than the new sum, update.
                    dist[z] = dist[min] + graph[min][z];
                    previous[z] = min;  //set previous every time an update occurs.
                    //previous must be hit at least once to update from
                    //INT_MAX
                    //printf("previous for %d set to %d\n", z, previous[z]);
                }
            }
    }
    //we need to use the previous array to set the routing tables for everything except the source
    //above.
    //need to test this for zero.
    for(int i = 0; i < MAX; i++){
        if(i!=source)
            nodes[i].routing_table[source] = previous[i];
        else
            nodes[i].routing_table[source] = i;
    }
}

int generate_network_node_array(struct router * node, char * filename){ //this is no longer used.
    int size = 150;
    int next_int;

    FILE *f = fopen(filename, "r");
    rewind(f);

    while(fscanf(f, "%d", &next_int) == 1){
        printf("test this: %d\n", next_int);
    }
    fclose(f);

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
        printf("length: %d \n", length[i]);
        printf("edges: %d\n", node[i]);
        printf("indices: %d\n", indices[i]);
    }
}


int generate_trip(){
    for(int i = 0; i < 20; i++){
        sd.source[i]=-1;
        sd.destination[i]=-1;
    }

    int s;
    int d;

    for(int i = 0; i < 20; i++){
        s = ran(MAX);

        sd.source[i] = s;
        for(int j = 0; j<i; j++){ //fill sources
            if(sd.source[i] == sd.source[j]){
                i--; //this is to generate unique nodes
            }
        }

    }

    for(int i = 0; i < 20; i++){
        d = ran(MAX);
        sd.destination[i] = d;
        for(int j = 0; j<i; j++){
            if(sd.destination[i] == sd.destination[j]){
                i--;
            }
        }
    }

    for(int i = 0; i < 20; i++){
        if(sd.source[i] == sd.destination[i]){
            sd.destination[i] = ran(MAX); //make sure that we are not
            // sending a packet from a node to itself
        }
    }

    printf("s:");

    for(int i = 0; i<20; i++){
        printf("%d ", sd.source[i]);
    }

    printf("\n");
    printf("d:");

    for(int i = 0; i<20; i++){
        printf("%d ", sd.destination[i]);
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

void print_sample_packet(struct packet pack){
    printf("Packet id %d\n", pack.packet_id);

    printf("Source: %d  Destination: %d\n", pack.source, pack.destination);
}

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

    printf("Node bandwidth: ");
    for(int j = 0; j<rout.num_edges; j++){
        printf("%d ", rout.edge_bandwidth[j]);
    }
    printf("\n");

    printf("Routing table for %d:\n", rout.id);
    for(int i = 0; i < MAX; i++){
        printf("%d %d \n", i, rout.routing_table[i]);
    }

    printf("Input queue\n");
    printf("Input start %d and input end %d and total elements %d\n", rout.input_start, rout.input_end, rout.input_elements);
    for(int i = 0; i<20; i++){

        printf("-> id: %d, wait %d, source: %d, destination: %d\n",
               rout.input_queue[i].packet_id, rout.input_queue[i].queue_wait_time,
                rout.input_queue[i].source, rout.input_queue[i].destination);

    }

    printf("Output queue\n");
    printf("Output start %d and Output end %d and total elements %d\n", rout.output_start, rout.output_end, rout.output_elements);
    for(int i = 0; i<20; i++){
        printf("-> id: %d, wait %d, source: %d, destination: %d\n",
               rout.output_queue[i].packet_id, rout.output_queue[i].queue_wait_time,
               rout.output_queue[i].source, rout.output_queue[i].destination);

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
    struct packet p;
    totes_generated_packets = 1;

    dummy.packet_id = -1;

    if(argc<3){
        printf("USAGE:  ./simulator.out <String: input graph filename> <int: seed for number generator>");
        exit(-1);
    }

    //struct packet packets_on_wire[100000];
    //int packet_delay[1000000] = {-1};

    char * filename = argv[1];
    int seed = atoi(argv[1]);

    //seed = 2;





    srand(seed);
    int num_edges = 200;
    int try = generate_graph(filename, MAX, num_edges);
    generate_trip(sd);

    //edges information
    //this is my network layer
    int big[num_edges];
    int little[num_edges];

    //read file into arrays
    nodes = malloc(150 * sizeof(struct router));
    //wire  = malloc(200 * sizeof(struct packets));
    //struct packet wire_arr[1000];
    for(int i = 0; i<1000; i++){
        wire_arr[i]=dummy;
    }

    //initialize all router
    for(int i = 0; i < MAX; i++){
        nodes[i].input_start = 0;
        nodes[i].input_end = 0;
        nodes[i].output_start = 0;
        nodes[i].output_end = 0;
        nodes[i].input_start = 0;
        nodes[i].input_end = 0;
    }

    FILE *file = fopen(filename, "r");
    int num1, num2 = 0;

    fscanf(file, "%d", &num1);
    fscanf(file, "%d", &num2);

    for(int i = 0; i<MAX; i++){
        nodes[i].num_edges = 0;
        printf("num edge test %d\n", nodes[i].num_edges);
    }

    int del = 0;
    int bandwidth = 0;


    //building nodes
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
        //now bandwidth
        del = standard_uniform_interval(1, 10); //TODO check this after updating resolution
        bandwidth = standard_uniform_interval(1, 1000000);

        printf("delay generated %d\n", del);

        nodes[little[i]].edge_propagation_delay[nodes[little[i]].num_edges] = del; //in seconds
        nodes[little[i]].edge_bandwidth[nodes[little[i]].num_edges] = bandwidth;
        nodes[big[i]].edge_propagation_delay[nodes[big[i]].num_edges] = del; //in seconds
        nodes[big[i]].edge_bandwidth[nodes[big[i]].num_edges] = bandwidth;
        nodes[little[i]].id = little[i];
        nodes[big[i]].id = big[i];
        nodes[little[i]].input_delay = ran(3);
        nodes[big[i]].input_delay = ran(3); //TODO re evaluate wait distribution

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
    //print_sample_node(nodes[149]);
    //dijkstras(nodes, nodes[0]);

    for(int i = 0; i < MAX; i++){
        printf("TEST: \n");
        for(int j = 0; j < MAX; j++){
            graph[i][nodes[i].edges[j]] = nodes[i].edge_propagation_delay[j];
            band_graph[i][nodes[i].edges[j]] = nodes[i].edge_bandwidth[j];
        }
    }

    //HACK!!!  to fix zero issue
    for(int i = 0; i<MAX; i++){
        graph[i][0] = graph[0][i];
        band_graph[i][0] = band_graph[0][i];
        nodes[i].edge_propagation_delay[0] = graph[i][0];
    }


    for(int i = 0; i < MAX; i++){
        printf("%d: ", i);
        for(int j = 0; j < MAX; j++){
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }

    //print_sample_node(nodes[0]);
    for(int i = 0; i < MAX; i++) {
        dijkstra_previous(i);
    }
    //print_sample_node(nodes[0]);

    //struct packet pack = generate_packet(0, 10);
    //in_queue(nodes[0], pack);

    int rand;
    //moved = 0;

    //begin simulation
    for(int time = 0; time<10000; time++){


        for(int i  = 0; i<40; i++){
            rand = ran(40);
            //printf("rand is %d\n", rand);
            if(rand<20){
                //printf("test s and d %d %d\n", sd.source[rand], sd.destination[rand]);
                place_holder = generate_packet(sd.source[rand], sd.destination[rand]);
                //printf("after packet generation %d %d %d\n", place_holder.packet_id, place_holder.source, place_holder.destination);
                //printf("with attributes wait time %d and size %d\n", place_holder.queue_wait_time, place_holder.size);
                in_queue(nodes[sd.source[rand]].id, place_holder);
                //print_sample_node(nodes[sd.source[rand]]);
            }
            else{
                //printf("next time\n");
            }
        }// maybe generate packets

        //for all packets, decrease packet wait time
        //if time hits zero or less, send to output queue

        for(int i = 0; i<MAX; i++){
            //printf("test wait time %d\n",nodes[i].input_queue[nodes[i].input_start].queue_wait_time);
            if(nodes[i].input_queue[0].queue_wait_time>0){

                nodes[i].input_queue[0].queue_wait_time--;

               // printf("Wait time for packet at node %d is %d\n", nodes[i].id,
                 //      nodes[i].input_queue[nodes[i].input_start].queue_wait_time);
            }
        }

        for(int i = 0; i < 1000; i++){
            if(wire_arr[i].next_arrival_time > 0){
                wire_arr[i].next_arrival_time--;
            }
            if(wire_arr[i].next_arrival_time == 0 && wire_arr[i].packet_id != -1){
                in_queue(wire_arr[i].next_hop, wire_arr[i]);
                wire_arr[i] = dummy;
            }
        }

        printf("current wire: ");
        for(int i = 0; i < 20; i++){
            printf("-- packet ID %d wait time %d and next hop %d\n", wire_arr[i].packet_id, wire_arr[i].next_arrival_time, wire_arr[i].next_hop);
        }
        //print_sample_node(nodes[132]);


        //if we are done, send to output queue
        for(int j = 0; j<MAX; j++){
            if(nodes[j].input_queue[nodes[j].input_start].queue_wait_time<=0 &&
                    nodes[j].input_queue[nodes[j].input_start].packet_id!=0){
                if(nodes[j].input_queue[nodes[j].input_start].destination == j){
                    totes_arrived_packets++;
                    destroy_packet(nodes[j].input_queue[nodes[j].input_start].destination); //destroy this packet.
                }
                else {

                    printf("packet at %d with id waiting time %d\n", j,
                           nodes[j].input_queue[nodes[j].input_start].queue_wait_time);
                    in_to_out(j);
                }
                //print_sample_node(nodes[j]);
            }
        }/**/

        //send to wire first?
        for(int i = 0; i<MAX; i++){
            //need to make sure every packet released is replaced with dummy;

            if(nodes[i].output_elements>0){
                //print_sample_node(nodes[i]);
                //printf("the output from %d is %d index.\n", i, nodes[i].output_start);
                place_holder = nodes[i].output_queue[nodes[i].output_start];
                //print_sample_node(nodes[i]);
                packet_to_wire(i);
                //nodes[i].output_start = (nodes[i].output_start+1)%20;
            }
        }/**/

        //check wire to see if arrived;
        for(int i = 0; i<1000; i++){
            //printf("are we there yet\n");
            if(wire_arr[i].next_arrival_time == 0 && wire_arr[i].packet_id != -1){
                printf("packet coming off wire\n");
                in_queue(wire_arr[i].next_hop, wire_arr[i]);
                int hop = wire_arr[i].next_hop;
                printf("Packet %d should have been in queueued at %d or dropped\n",  wire_arr[i].packet_id, hop);
                //print_sample_node(nodes[hop]);
            }
        }/**/
    }//end of simulation for

    printf("Total packets generated: %d\n", totes_generated_packets-1);
    printf("Total packets dropped: %d\n", totes_dropped_packets);
    printf("Total packets arrived: %d\n", totes_arrived_packets);

    //printf("Total moved %d\n", moved);*/
} //end main