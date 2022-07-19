#ifndef VERTEX_H
#define VERTEX_H

#include <QGraphicsScene>
#include <QGraphicsItem>

class Vertex : public QGraphicsEllipseItem
{
public:
    friend class Graph;
    Vertex(int label, double x, double y, QGraphicsScene *scene);
    void update_label(int _label);
    void update_distance(int distance);
    int get_distance();
    unsigned int get_label();
    void set_previous(Vertex* vertex);
    Vertex* get_previous();
    friend bool operator< (Vertex const &lhs, Vertex const &rhs);
    friend bool operator> (Vertex const &lhs, Vertex const &rhs);
    friend bool operator== (Vertex const &lhs, Vertex const &rhs);

private:
    QGraphicsSimpleTextItem *textItem;
    int label;
    int distance {-1};
    Vertex* previous {nullptr};
};

#endif // VERTEX_H
