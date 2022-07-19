#include "graph.h"

Graph::Graph() {}

Graph::~Graph() {
    while(!vertices.empty()) {
        delete vertices.back().front();
        vertices.pop_back();
    }

    while(!edges.empty()) {
        delete edges.back();
        edges.pop_back();
    }
}

void Graph::add_vertex(Vertex *vertex) {
    list<Vertex*> to_add = {vertex};
    vertices.push_back(to_add);
}

void Graph::remove_vertex(Vertex *vertex) {
    for(auto it = edges.rbegin(); it != edges.rend();){
        if((*it)->source == vertex || (*it)->target == vertex){
            this->remove_edge(*(it++));
        }
        else{
            ++it;
        }
    }

    unsigned int position = vertex->get_label();
    vertices.erase(vertices.begin() + position);

    delete vertex;

    int i = 0;
    for(auto it = vertices.begin(); it != vertices.end(); ++it, ++i){
        (*it).front()->update_label(i);
    }
}

void Graph::add_edge(Edge *edge){
    unsigned int position = edge->source->get_label();
    vertices[position].push_back(edge->target);

    position = edge->target->get_label();
    vertices[position].push_back(edge->source);

    edges.push_back(edge);
}

void Graph::remove_edge(Edge *edge) {
    for(auto it = vertices.begin(); it != vertices.end(); ++it){
        if((*it).front() == edge->source ){
            (*it).remove(edge->target);
        }
        if((*it).front() == edge->target){
            (*it).remove(edge->source);
        }
    }

    for(auto it = edges.begin(); it != edges.end(); ++it){
        if((*it) == edge){
            edges.erase(it);
            break;
        }
    }

    delete edge;
}

void Graph::update_edges(Vertex *vertex){
    for(auto it = edges.begin(); it != edges.end(); ++it){
        if((*it)->source == vertex || (*it)->target == vertex){
            (*it)->adjust_edge();
        }
    }
}

unsigned int Graph::number_of_vertices() {
    return vertices.size();
}

vector<list<Vertex*>>& Graph::access_graph(){
    return vertices;
}

Edge* Graph::get_edge(Vertex *source, Vertex *target){
    Edge* edge = nullptr;

    for(auto it = edges.begin(); it != edges.end(); ++it){
        if ( ((*it)->source == source || (*it)->target == source)
             && ((*it)->source == target || (*it)->target == target) )
        {
            edge = (*it);
        }
    }

    return edge;
}
