#include "edge.h"

Edge::Edge(Vertex* source, Vertex* target, unsigned int weight, QGraphicsScene *scene)
    :QGraphicsLineItem(scene->addLine(1,1,1,1,QPen(Qt::black))),
      source(source), target(target), weight(weight)
{
    QPointF pf = source->sceneBoundingRect().center();
    QPointF pt = target->sceneBoundingRect().center();
    this->setLine(pt.x(), pt.y(), pf.x(), pf.y());

    parentItem()->setZValue(-100);
    setZValue(-100);

    textItem = scene->addSimpleText(QString::number(weight));
    textItem->setPos((pt.x() + pf.x())/2,(pt.y() + pf.y())/2 - 10);
    textItem->setParentItem(this);
}

void Edge::adjust_edge() {
    QPointF pf = source->sceneBoundingRect().center();
    QPointF pt = target->sceneBoundingRect().center();
    this->setLine(pt.x(), pt.y(), pf.x(), pf.y());
    textItem->setPos((pt.x() + pf.x())/2,(pt.y() + pf.y())/2 - 10);
}

unsigned int Edge::get_weight(){
    return weight;
}
