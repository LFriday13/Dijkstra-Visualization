#include "priorityqueue.h"

PriorityQueue& PriorityQueue::right_subtree() { return root->right; }
PriorityQueue& PriorityQueue::left_subtree() { return root->left; }

const PriorityQueue& PriorityQueue::right_subtree() const { return root->right; }
const PriorityQueue& PriorityQueue::left_subtree() const { return root->left; }

PriorityQueue::~PriorityQueue() { delete root; }

int PriorityQueue::height() const {
    return is_empty() ?
                -1 : root->height;
}

int PriorityQueue::bfactor() const {
    return is_empty() ?
                0 : right_subtree().height() - left_subtree().height();
}

void PriorityQueue::fix_height() const {
    if (is_empty()){ return; }

    int left_h = left_subtree().height();
    int right_h = right_subtree().height();
    root->height = 1 + max(left_h, right_h);
}

void PriorityQueue::rotate_left() {
    Node* r = right_subtree().root;
    right_subtree() = r->left;
    r->left = *this;
    fix_height();
    this->root = r;
    fix_height();
}

void PriorityQueue::rotate_right() {
    Node* l = left_subtree().root;
    left_subtree() = l->right;
    l->right = *this;
    fix_height();
    this->root = l;
    fix_height();
}

void PriorityQueue::balance() {
    if (is_empty()) { return; }

   fix_height();
   int balance_factor = bfactor();

   if (balance_factor == 2) {
       if (right_subtree().bfactor() < 0) { right_subtree().rotate_right(); }
       return rotate_left();
   }

   if (balance_factor == -2) {
       if (left_subtree().bfactor() > 0) { left_subtree().rotate_left(); }
       return rotate_right();
   }
}

bool PriorityQueue::contains(Vertex* const & vertex) const {
    if (is_empty()) { return false; }
    else if (*vertex == *root->vertex) { return true; }
    else if (*vertex < *root->vertex) { return left_subtree().contains(vertex); }
    else { return right_subtree().contains(vertex); }
}

Vertex* const& PriorityQueue::pop() const {
    const PriorityQueue& left_t = left_subtree();

    if (left_t.is_empty()) {
        Vertex** vertex = &root->vertex;
        return *vertex;
    }

    return left_t.pop();
}

void PriorityQueue::push(Vertex* const & to_insert) {
    if (is_empty()) { root = new Node(to_insert); }
    else if (*to_insert < *root->vertex) { left_subtree().push(to_insert); }
    else if (*to_insert > *root->vertex) { right_subtree().push(to_insert); }
    balance();
}

void PriorityQueue::remove(Vertex* const & to_remove) {
    if (is_empty()) { return; }
    if (*to_remove < *root->vertex) { left_subtree().remove(to_remove); }
    else if (*to_remove > *root->vertex) { right_subtree().remove(to_remove); }
    else {
        PriorityQueue& left_t = left_subtree();
        PriorityQueue& right_t = right_subtree();

        if (!left_t.is_empty() && !right_t.is_empty()) {
            root->vertex = right_t.pop();
            right_t.remove(root->vertex);
        }

        else {
            Node* node_to_remove = root;
            *this = left_t.is_empty() ? right_t : left_t;
            right_t.root = left_t.root = nullptr;
            delete node_to_remove;
        }
    }

    balance();
}
