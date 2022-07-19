#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include "graph.h"

struct Node;

class PriorityQueue
{
private:
    Node* root {nullptr};

    PriorityQueue& right_subtree();
    PriorityQueue& left_subtree();
    const PriorityQueue& right_subtree() const;
    const PriorityQueue& left_subtree() const;

    int height() const;
    int bfactor() const;
    void fix_height() const;
    void rotate_left();
    void rotate_right();
    void balance();

public:
    PriorityQueue() = default;
    ~PriorityQueue();

    bool is_empty() const { return root == nullptr; }
    bool contains(Vertex* const & vertex) const;
    Vertex* const& pop() const;
    void push(Vertex* const & to_insert);
    void remove(Vertex* const & to_remove);
};

struct Node {
    Vertex* vertex;
    int height {0};
    PriorityQueue left;
    PriorityQueue right;
    Node(Vertex* _vertex): vertex(_vertex) {}
};

#endif // PRIORITYQUEUE_H
