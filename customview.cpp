#include "customview.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <cstdlib>
#include <ctime>

CustomView::CustomView(QWidget *parent): QGraphicsView(parent)
{
    graph = new Graph;
}

CustomView::~CustomView(){
    if(unvisited) {
        delete unvisited;
        unvisited = nullptr;
    }

    if(priority_queue) {
        delete priority_queue;
        priority_queue = nullptr;
    }

    delete graph;
    scene()->clear();
    delete scene();
}

void CustomView::set_drawing_vertices(bool _drawing_vertices) {
    if(generating_paths){ return; }

    drawing_vertices = _drawing_vertices;
    selected = nullptr;
}

void CustomView::set_drawing_edges(bool _drawing_edges) {
    if(generating_paths){ return; }

    drawing_edges = _drawing_edges;
    source = nullptr;
    target = nullptr;
}

void CustomView::set_showing(bool _showing) {
    if(finished) {
        showing_paths = _showing;
        clear_current_path();
    }
}

void CustomView::generate(){
    if(generating_paths){ return; }

    delete graph;
    graph = new Graph;
    next_label = 0;

    bool ok1, ok2, ok3;
    int n_vertices = QInputDialog::getInt(this, tr("Insert Number of Vertices"), tr("Vertices:"), 0, 0, 100, 1, &ok1);
    double density = QInputDialog::getDouble(this, tr("Insert Density of Edges"), tr("Density:"), 0, 0, 1, 3, &ok2);
    int max_weight = QInputDialog::getInt(this, tr("Insert Maximum Edge Weight"), tr("Weight:"), 1, 1, 100, 1, &ok3);
    int n_edges = static_cast<int>(density * n_vertices * (n_vertices - 1) / 2);
    if(ok1 && ok2 && ok3) {

        srand(static_cast<unsigned int>(time(nullptr)));
        int k = n_vertices;

        // Adding random vertices
        while(k > 0){
            double random_x = std::rand() % 720;
            double random_y = std::rand() % 470;
            graph->add_vertex(new Vertex(next_label++,-170+random_x,-50+random_y,scene()));
            --k;
        }

        // Adding random edges
        k = n_edges;
        vector<list<Vertex*>> *vertices = &graph->access_graph();
        while(k > 0) {
            unsigned int random_source = std::rand() % n_vertices;
            unsigned int random_target = std::rand() % n_vertices;
            if(random_target == random_source){ continue; }
            Vertex* source = (*vertices)[random_source].front();
            Vertex* target = (*vertices)[random_target].front();
            if(graph->get_edge(source,target) != nullptr){continue;}
            unsigned int weight = 1 + std::rand() % max_weight;
            graph->add_edge(new Edge(source, target, weight,scene()));
            --k;
        }

    }
}

void CustomView::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        if(drawing_vertices){
            QPointF scenePoint = mapToScene(event->pos());
            graph->add_vertex(new Vertex(next_label++,scenePoint.x(), scenePoint.y(),scene()));
            return;

        } else {
            QGraphicsItem* item = QGraphicsView::itemAt(event->pos().x(), event->pos().y());
            Vertex* vertex = qgraphicsitem_cast<Vertex*>(item);
            QGraphicsSimpleTextItem* text = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(item);

            if(text) {
                vertex = qgraphicsitem_cast<Vertex*>(text->parentItem());
            }

            if(vertex){
                if(drawing_edges && source == nullptr){
                    source = vertex;
                    source->setPen(QPen(Qt::magenta));
                    return;

                } else if(drawing_edges && source != nullptr && vertex != source) {
                    target = vertex;

                    bool ok;
                    int i = QInputDialog::getInt(this, tr("Insert Weight"), tr("Weight:"), 1, 1, 100, 1, &ok);
                    if(ok && i > 0) {
                        unsigned int weight = static_cast<unsigned int>(i);
                        source->setPen(QPen(Qt::black));
                        graph->add_edge(new Edge(source, target, weight, scene()));
                        source = nullptr;
                        target = nullptr;
                    }
                    return;
                } else {
                    selected = vertex;
                    if (started) {
                        start_algorithm(selected);
                    }
                    if (showing_paths) {
                        path_target = selected;
                        show_path();
                    }
                    offset_x = event->pos().x();
                    offset_y = event->pos().y();
                    return;
                }
            }

        }
    }

    if(event->button() == Qt::RightButton && !drawing_edges && !drawing_vertices && !generating_paths){
        QGraphicsItem* item = QGraphicsView::itemAt(event->pos().x(), event->pos().y());
        Vertex* vertex = qgraphicsitem_cast<Vertex*>(item);
        Edge* edge = qgraphicsitem_cast<Edge*>(item);

        if(vertex) {
            graph->remove_vertex(vertex);
            next_label = static_cast<int>(graph->number_of_vertices());
            return;
        }
        if(edge) {
            graph->remove_edge(edge);
            return;
        }
    }
}

void CustomView::mouseMoveEvent(QMouseEvent *event){
    if(selected){
        double dx = event->pos().x() - offset_x;
        double dy = event->pos().y() - offset_y;
        offset_x += dx;
        offset_y += dy;
        selected->moveBy(dx,dy);
        graph->update_edges(selected);
    }
}

void CustomView::mouseReleaseEvent(QMouseEvent *event){
    selected = nullptr;
}

void CustomView::set_started(bool _started) {
    if(!generating_paths){ started = _started; }
}

void CustomView::start_algorithm(Vertex *vertex) {
    generating_paths = true;

    unvisited = new UnvisitedVerticesSet(graph);
    // priority_queue = new PriorityQueue; // AVL Tree Implementation
    priority_queue = new PriorityQueueRB; // Red-Black Tree Implementation

    path_source = vertex;
    path_source->update_distance(0);

    priority_queue->push(path_source);
    Vertex* next_current = priority_queue->pop();
    priority_queue->remove(next_current);
    set_current(next_current);

    scene()->selectionChanged();

    started = false;
}

void CustomView::set_current(Vertex* vertex) {
    if(current){ current->setPen(QPen(Qt::gray)); }
    if(path_target){
        priority_queue->contains(path_target) ?
                path_target->setPen(QPen(Qt::cyan)) : path_target->setPen(QPen(Qt::gray));
        current_edge->setPen(QPen(Qt::black));
        path_target = nullptr;
        current_edge = nullptr;
    }

    current = vertex;
    unvisited->remove(current);
    vector<list<Vertex*>>* vertices = &graph->access_graph();
    list<Vertex*>*adjacency = &((*vertices)[current->get_label()]);
    it = adjacency->begin();
    current->setPen(QPen(Qt::blue));
}

void CustomView::evaluate(Vertex* vertex) {
    if(path_target){
        priority_queue->contains(path_target) ?
                path_target->setPen(QPen(Qt::cyan)) : path_target->setPen(QPen(Qt::gray));
        if(current_edge){ current_edge->setPen(QPen(Qt::black)); }
    }

    path_target = vertex;
    current_edge = graph->get_edge(current, path_target);
    current_edge->setPen(QPen(Qt::blue));
    int tentative_distance = current->get_distance() + current_edge->get_weight();

    if(path_target->get_distance() == -1 || tentative_distance < path_target->get_distance()){
       priority_queue->remove(path_target);
       path_target->update_distance(tentative_distance);
       path_target->set_previous(current);
       path_target->setPen(QPen(Qt::green));
       priority_queue->push(path_target);

    } else {
       path_target->setPen(QPen(Qt::red));

    }
}

void CustomView::step() {
    if(finished){
        QMessageBox msg;
        msg.warning(this,QString("Finished"),QString("Algorithm has finished."));
        return;
    }
    if(current == nullptr) { return; }

    vector<list<Vertex*>>* vertices = &graph->access_graph();
    list<Vertex*>*adjacency = &((*vertices)[current->get_label()]);

    for(; it != adjacency->end(); ++it){
        if(!unvisited->visited(*it)) {
            evaluate(*(it++));
            return;
        }
    }

    while(!priority_queue->is_empty()){
        Vertex* next_current = priority_queue->pop();
        priority_queue->remove(next_current);
        if(!unvisited->visited(next_current)) {
            set_current(next_current);
            return;
        }
    }

    finished = true;
    current->setPen(QPen(Qt::gray));
    current = nullptr;

    delete unvisited;
    unvisited = nullptr;
    delete priority_queue;
    priority_queue = nullptr;
}

void CustomView::clear_paths() {
    if(!vertices_of_path.empty()){
        for(auto it = edges_of_path.begin(); it != edges_of_path.end(); ++it){
            (*it)->setPen(QPen(Qt::black));
        }
        vertices_of_path.clear();
        edges_of_path.clear();
    }

    finished = generating_paths = false;
    path_source = current = path_target = nullptr;

    if(current_edge != nullptr){
        current_edge->setPen(QPen(Qt::black));
        current_edge = nullptr;
    }

    vector<list<Vertex*>>* vertices = &graph->access_graph();
    for(auto it = vertices->begin(); it != vertices->end(); ++it){
        (*it).front()->setPen(QPen(Qt::black));
        (*it).front()->set_previous(nullptr);
        (*it).front()->update_distance(-1);
    }

    if(unvisited) {
        delete unvisited;
        unvisited = nullptr;
    }

    if(priority_queue) {
        delete priority_queue;
        priority_queue = nullptr;
    }
}

void CustomView::show_path() {
    if(!vertices_of_path.empty()){
        for(auto it = vertices_of_path.begin(); it != vertices_of_path.end(); ++it){
            (*it)->setPen(QPen(Qt::gray));
        }

        for(auto it = edges_of_path.begin(); it != edges_of_path.end(); ++it){
            (*it)->setPen(QPen(Qt::black));
        }
        vertices_of_path.clear();
        edges_of_path.clear();
    }

    if(path_target->get_distance() == -1){
        QMessageBox msg;
        msg.warning(this,QString("Invalid"),QString("A path does not exists."));
        return;
    }

    // Calculate The Path
    if(path_target == path_source) {
        vertices_of_path.push_back(path_source);
    } else {
        vertices_of_path.push_back(path_target);
        Vertex* previous = path_target->get_previous();
        Edge* segment = graph->get_edge(path_target, previous);
        edges_of_path.push_back(segment);
        while(previous != path_source){
            path_target = previous;
            previous = previous->get_previous();
            segment = graph->get_edge(path_target, previous);
            vertices_of_path.push_back(path_target);
            edges_of_path.push_back(segment);
        }
        vertices_of_path.push_back(previous);
    }

    for(auto it = vertices_of_path.begin(); it != vertices_of_path.end(); ++it){
        (*it)->setPen(QPen(Qt::blue));
    }

    for(auto it = edges_of_path.begin(); it != edges_of_path.end(); ++it){
        (*it)->setPen(QPen(Qt::blue));
    }
}

void CustomView::clear_current_path() {
    if(!vertices_of_path.empty()){
        for(auto it = vertices_of_path.begin(); it != vertices_of_path.end(); ++it){
            (*it)->setPen(QPen(Qt::gray));
        }

        for(auto it = edges_of_path.begin(); it != edges_of_path.end(); ++it){
            (*it)->setPen(QPen(Qt::black));
        }
        vertices_of_path.clear();
        edges_of_path.clear();
    }
}
