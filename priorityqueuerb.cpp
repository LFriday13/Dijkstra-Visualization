#include "priorityqueuerb.h"

PriorityQueueRB::~PriorityQueueRB() {
    while(!is_empty()){
        remove(root->vertex);
    }
}

void PriorityQueueRB::push(Vertex* to_insert) {
    if(!root) {
        root = new Node(to_insert);
        set_color(root, false);
        return;
    }

    Node* current = root;

    while(true) {
        if(*to_insert < *current->vertex){
            if(!current->left) {
                current->left = new Node(to_insert);
                current->left->parent = current;
                adjust_insertion(current->left);
                return;
            }
            current = current->left;
        }

        else if (*to_insert > *current->vertex){
            if(!current->right) {
                current->right = new Node(to_insert);
                current->right->parent = current;
                adjust_insertion(current->right);
                return;
            }
            current = current->right;
        }

        else { return; }
    }
}

void PriorityQueueRB::rotate_left(Node* current) {
    if(current && current->right){
        Node* old_right = current->right;
        old_right->parent = current->parent;

        if(old_right->parent) {
            if(current == left_of(parent_of(current))) {
                current->parent->left = old_right;
            }
            else { current->parent->right = old_right; }
        }
        else { root = old_right; }

        current->right = old_right->left;
        if(current->right){current->right->parent = current;}
        old_right->left = current;
        current->parent = old_right;
    }
}

void PriorityQueueRB::rotate_right(Node* current) {
    if(current && current->left){
        Node* old_left = current->left;
        old_left->parent = current->parent;

        if(old_left->parent) {
            if(current == left_of(parent_of(current))) {
                current->parent->left = old_left;
            }
            else { current->parent->right = old_left; }
        }
        else { root = old_left; }

        current->left = old_left->right;
        if(current->left){current->left->parent = current;}
        old_left->right = current;
        current->parent = old_left;
    }
}


void PriorityQueueRB::adjust_insertion(Node *current) {
    if(current && current->parent && current->parent->is_red) {
        if(is_red_of(sibling_of(parent_of(current)))) {
            set_color(parent_of(current), false);
            set_color(sibling_of(parent_of(current)), false);
            set_color(grandparent_of(current), true);
            adjust_insertion(grandparent_of(current));
        }

        else if (parent_of(current) == left_of(grandparent_of(current))) {
            if (current == current->parent->right) {
                rotate_left(current = current->parent);
            }
            set_color(parent_of(current), false);
            set_color(grandparent_of(current), true);
            rotate_right(grandparent_of(current));
        }

        else if (parent_of(current) == right_of(grandparent_of(current))) {
            if (current == current->parent->left) {
                rotate_right(current = current->parent);
            }
            set_color(parent_of(current), false);
            set_color(grandparent_of(current), true);
            rotate_left(grandparent_of(current));
        }
    }

    set_color(root, false);
}

void PriorityQueueRB::adjust_removal(Node *current) {
    while (current != root && !current->is_red) {
        if (current == left_of(parent_of(current))) {
            Node* sibling = current->parent->right;

            if(is_red_of(sibling)) {
                set_color(sibling, false);
                set_color(parent_of(current), true);
                rotate_left(parent_of(current));
                sibling = right_of(parent_of(current));
            }

            if(!is_red_of(left_of(sibling)) && !is_red_of(right_of(sibling))) {
                set_color(sibling, true);
                current = parent_of(current);
            } else {
                if(!is_red_of(right_of(sibling))) {
                    set_color(left_of(sibling), false);
                    set_color(sibling, true);
                    rotate_right(sibling);
                    sibling = right_of(parent_of(current));
                }

                set_color(sibling, is_red_of(parent_of(current)));
                set_color(parent_of(current), false);
                set_color(right_of(sibling), false);
                rotate_left(parent_of(current));
                current = root;
            }

        } else {
            Node* sibling = current->parent->left;

            if(is_red_of(sibling)) {
                set_color(sibling, false);
                set_color(parent_of(current), true);
                rotate_right(parent_of(current));
                sibling = left_of(parent_of(current));
            }

            if(!is_red_of(left_of(sibling)) && !is_red_of(right_of(sibling))) {
                set_color(sibling, true);
                current = parent_of(current);
            } else {
                if(!is_red_of(left_of(sibling))) {
                    set_color(right_of(sibling), false);
                    set_color(sibling, true);
                    rotate_left(sibling);
                    sibling = left_of(parent_of(current));
                }

                set_color(sibling, is_red_of(parent_of(current)));
                set_color(parent_of(current), false);
                set_color(left_of(sibling), false);
                rotate_right(parent_of(current));
                current = root;
            }

        }
    }

    set_color(current, false);
}

void PriorityQueueRB::remove(Vertex* const & to_remove) {
    Node* holding = root;
    while(holding) {
        if(*to_remove == *holding->vertex){
            break;
        }
        else if(*to_remove < *holding->vertex) {
            holding = holding->left;
        }
        else if (*to_remove > *holding->vertex) {
            holding = holding->right;
        }
    }
    if(!holding){ return; }

    if(holding->right && holding->left){
        Node* predecessor = holding->left;
        while(predecessor->right){
            predecessor = predecessor->right;
        }
        holding->vertex = predecessor->vertex;
        holding = predecessor;
    }

    Node* pull = (!holding->left) ? holding->right : holding->left;
    if(pull){
        if(holding == root) {root = pull;}
        else if (holding->parent->left == holding){
            holding->parent->left = pull;
            pull->parent = holding->parent;
        }
        else {
            holding->parent->right = pull;
            pull->parent = holding->parent;
        }
        if(!holding->is_red) {adjust_removal(pull);}
    }
    else if (holding == root) { root = nullptr; }
    else {
         if(!holding->is_red) {adjust_removal(holding);}
         if(holding->parent->left == holding){
             holding->parent->left = nullptr;
         }
         else{ holding->parent->right = nullptr; }
    }

    delete holding;
}

bool PriorityQueueRB::recursive_contains(Node* current, Vertex* to_check) const {
    if(!current) {return false;}
    if(*current->vertex == *to_check) {return true;}
    else if(*to_check < *current->vertex) {
        return recursive_contains(current->left, to_check);
    }
    else {
        return recursive_contains(current->right, to_check);
    }
}

bool PriorityQueueRB::is_empty() const {
    return root == nullptr;
}

bool PriorityQueueRB::contains(Vertex* const & to_check) const {
    return recursive_contains(root, to_check);
}

Vertex* const& PriorityQueueRB::pop() const {
    Node* current = root;
    while(current->left != nullptr) {
        current = current->left;
    }
    return current->vertex;
}

void PriorityQueueRB::set_color(Node* current, bool _is_red) {
    if(current) { current->is_red = _is_red; }
}

bool PriorityQueueRB::is_red_of(Node* current) const {
    if(current){return current->is_red;}
    return false;
}

Node* PriorityQueueRB::parent_of(Node* current) const {
    if(current){ return current->parent; }
    return nullptr;
}

Node* PriorityQueueRB::grandparent_of(Node* current) const {
    if(current && current->parent) {return current->parent->parent;}
    return nullptr;
}

Node* PriorityQueueRB::sibling_of(Node* current) const {
    if(current && current->parent) {
        if(current->parent->left) {
            if(*current->parent->left->vertex == *current->vertex) {
                return current->parent->right;
            }
            return current->parent->left;
        }
    }
    return nullptr;
}

Node* PriorityQueueRB::left_of(Node* current) const {
    if(current) { return current->left; }
    return nullptr;
}

Node* PriorityQueueRB::right_of(Node* current) const {
    if(current) { return current->right; }
    return nullptr;
}
