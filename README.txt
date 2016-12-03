Project 3
Ryan Read

This program simulates a network of 150 nodes.  these nodes are connected by a random number of edges seeded
by the the integer provided to graph.out.

The graph program will produce the network topology.  Because of how this graph was generated, it is guaranteed
to be connected.

The simulator will begin randomly generating packets and sending thim from a set of 20 sources to 20 destinations.
Each node has a input and output queue of 20 packets.  If these are filled, the router will drop any other incoming
packet.  Any packet that has been generated exists in a nodes queue or "on the wire" (the wire is a struct packet array).


