#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"

class Edge : public QGraphicsLineItem
{
public:
    friend class Graph;
    Edge(Vertex* source, Vertex* target, unsigned int weight, QGraphicsScene *scene);
    void adjust_edge();
    unsigned int get_weight();

private:
    QGraphicsSimpleTextItem *textItem;
    Vertex *source, *target;
    unsigned int weight;
};

#endif // EDGE_H
