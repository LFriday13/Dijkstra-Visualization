#ifndef UNVISITEDVERTICESSET_H
#define UNVISITEDVERTICESSET_H

#include "graph.h"
#include <vector>
#include <algorithm>
using namespace std;

class UnvisitedVerticesSet
{
public:
    UnvisitedVerticesSet(Graph *graph);
    ~UnvisitedVerticesSet();
    unsigned int hash(Vertex *hashing);
    void insert(Vertex *to_insert);
    void remove(Vertex *to_remove);
    bool visited(Vertex *to_access);

private:
    Vertex** array;
    const unsigned int n_vertices;
};

#endif // UNVISITEDVERTICESSET_H
