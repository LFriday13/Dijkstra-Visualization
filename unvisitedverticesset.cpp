#include "unvisitedverticesset.h"

UnvisitedVerticesSet::UnvisitedVerticesSet(Graph *graph)
    : n_vertices(graph->number_of_vertices())
{
    array = new Vertex* [n_vertices];
    vector<list<Vertex*>> *vertices = &graph->access_graph();
    for(auto it = vertices->begin(); it != vertices->end(); ++it){
        insert((*it).front());
    }
}

UnvisitedVerticesSet::~UnvisitedVerticesSet() {
    delete [] array;
}

unsigned int UnvisitedVerticesSet::hash(Vertex *hashing) {
    return (static_cast<unsigned int>(hashing->get_label()) % n_vertices);
}

void UnvisitedVerticesSet::insert(Vertex *to_insert){
    unsigned int position = hash(to_insert);
    array[position] = to_insert;
}

bool UnvisitedVerticesSet::visited(Vertex *to_access){
    unsigned int position = hash(to_access);
    return (array[position] == nullptr);
}

void UnvisitedVerticesSet::remove(Vertex *to_remove){
    unsigned int position = hash(to_remove);
    array[position] = nullptr;
}
