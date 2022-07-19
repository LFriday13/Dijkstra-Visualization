#ifndef PRIORITYQUEUERB_H
#define PRIORITYQUEUERB_H

#include "vertex.h"

struct Node {
    Node(Vertex* new_vertex): vertex(new_vertex) {}
    Vertex* vertex;
    Node* parent {nullptr};
    Node* left {nullptr};
    Node* right {nullptr};
    bool is_red {true};
};

class PriorityQueueRB {
private:
    Node* root {nullptr};

    void rotate_right(Node* current);
    void rotate_left(Node* current);
    bool recursive_contains(Node* current, Vertex* to_check) const;    
    void adjust_insertion(Node* current);
    void adjust_removal(Node* current);
    void set_color(Node* current, bool _is_red);

    bool is_red_of(Node* current) const;
    Node* parent_of(Node* current) const;
    Node* grandparent_of(Node* current) const;
    Node* sibling_of(Node* current) const;
    Node* left_of(Node* current) const;
    Node* right_of(Node* current) const;

public:
    PriorityQueueRB(): root(nullptr) {}
    ~PriorityQueueRB();
    bool is_empty() const;
    bool contains(Vertex* const & vertex) const;
    Vertex* const& pop() const;
    void push(Vertex* to_insert);
    void remove(Vertex* const & to_remove);
};


#endif // PRIORITYQUEUERB_H
