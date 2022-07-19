#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QGraphicsView>
#include "graph.h"
#include "unvisitedverticesset.h"
// #include "priorityqueue.h" // AVL Tree Implementation
#include "priorityqueuerb.h" // Red-Black Tree Implementation

class CustomView: public QGraphicsView
{
public:
    CustomView(QWidget *parent = nullptr);
    ~CustomView();
    void set_drawing_vertices(bool _drawing_vertices);
    void set_drawing_edges(bool _drawing_edges);
    void set_started(bool _started);
    void set_showing(bool _showing);
    void generate();
    void step();
    void clear_paths();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Graph *graph;
    int next_label {0};

    bool drawing_vertices {false};
    bool drawing_edges {false};
    bool started {false};
    bool generating_paths {false};
    bool finished {false};
    bool showing_paths {false};

    double offset_x {0}, offset_y {0};
    Vertex *selected {nullptr};
    Vertex *source {nullptr}, *target {nullptr};

    Vertex *path_source {nullptr}, *current{nullptr}, *path_target {nullptr};
    Edge *current_edge {nullptr};
    std::list<Vertex*>::iterator it;

    vector<Edge*> edges_of_path;
    vector<Vertex*> vertices_of_path;

    UnvisitedVerticesSet* unvisited {nullptr};
    // PriorityQueue* priority_queue {nullptr}; // AVL Tree Implementation
    PriorityQueueRB* priority_queue {nullptr}; // Red-Black Tree Implementation

    void start_algorithm(Vertex *vertex);
    void set_current(Vertex* vertex);
    void evaluate(Vertex* vertex);
    void show_path();
    void clear_current_path();
};

#endif // CUSTOMVIEW_H
