#include "vertex.h"

Vertex::Vertex(int label, double x, double y, QGraphicsScene *scene)
    : QGraphicsEllipseItem(scene->addEllipse(0,0,0,0,QPen(Qt::black),QBrush(Qt::white))),
      label(label)
{
    setRect(x,y,20,20);

    parentItem()->setZValue(100);
    setZValue(100);
    setBrush(QBrush(Qt::white));

    textItem = scene->addSimpleText(QString(""));
    textItem->setPos(x+4,y+4);
    textItem->setParentItem(this);
}

void Vertex::update_label(int _label) {
    label = _label;
}

void Vertex::update_distance(int _distance){
    distance = _distance;
    if(distance == -1){
        textItem->setText(QString(""));
    } else {
        textItem->setText(QString::number(distance));
    }
}

unsigned int Vertex::get_label(){
    return static_cast<unsigned int>(label);
}

int Vertex::get_distance(){
    return distance;
}

void Vertex::set_previous(Vertex* vertex){
    previous = vertex;
}

Vertex* Vertex::get_previous(){
    return previous;
}

bool operator< (Vertex const &lhs, Vertex const &rhs){
    return (lhs.distance == rhs.distance) ? (lhs.label < rhs.label) : (lhs.distance < rhs.distance);
}

bool operator> (Vertex const &lhs, Vertex const &rhs){
    return (lhs.distance == rhs.distance) ? (lhs.label > rhs.label) : (lhs.distance > rhs.distance);
}

bool operator== (Vertex const &lhs, Vertex const &rhs){
    return (lhs.label == rhs.label);
}
