#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <algorithm>
#include "edge.h"

using namespace std;

class Graph
{
public:
    Graph();
    ~Graph();

    void add_vertex(Vertex *vertex);
    void remove_vertex(Vertex *vertex);
    void add_edge(Edge *edge);
    void remove_edge(Edge *edge);
    void update_edges(Vertex *vertex);
    vector<list<Vertex*>>& access_graph();
    unsigned int number_of_vertices();
    Edge* get_edge(Vertex *source, Vertex *target);
private:
    vector<list<Vertex*>> vertices;
    vector<Edge*> edges;
};

#endif // GRAPH_H
