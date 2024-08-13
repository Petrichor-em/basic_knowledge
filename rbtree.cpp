#include <iostream>
#include <vector>

#define nil nullptr

class RB_Tree {

    enum Color {
        RED, BLACK,
    };

    struct Node {
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
        Color color = RED;
        int key = 0;
    };

private:
    Node *root = nullptr;
    Node *rotate_right(Node *n); // rotate the tree and return new root.
    Node *rotate_left(Node *n);
    bool is_left_child(Node *n);
    bool is_right_child(Node *n);
    bool is_leaf(Node *n);
    bool is_root(Node *n);
    Node *grandparent(Node *n);
    Node *uncle(Node *n);
    Node *sibling(Node *n);
    void insert_helper(Node *curr, Node *parent, Node *to_insert);
    void print_helper(std::ostream &os, Node *n);
    void insert_case1(Node *n);
    void insert_case2(Node *n);
    void insert_case3(Node *n);
    void insert_case4(Node *n);
    void insert_case5(Node *n);
    void insert_case6(Node *n);
    bool is_inner_child(Node *n);
    bool is_outer_child(Node *n);
    bool has_uncle(Node *n);

public:
    RB_Tree() = default;
    ~RB_Tree() = default;
    void insert(int key);
    void print(std::ostream &os);
    is_empty() { return root == nullptr; }
};

RB_Tree::Node *RB_Tree::rotate_right(Node *n) {
    std::cerr << "rotate_right execute.\n";
    Node *parent_of_root = n->parent; // the root here means n, subtree actually.
    Node *pivot = n->left;
    n->left = pivot->right;
    if (pivot->right != nil) {
        n->left->parent = n;
    }
    pivot->right = n;
    n->parent = pivot;    
    if (parent_of_root == nullptr) { // n is root.
        pivot->parent = nullptr;
        root = pivot;
    } else {
        pivot->parent = parent_of_root;
        if (n->key < parent_of_root->key)
            parent_of_root->left = pivot;
        else
            parent_of_root->right = pivot;
    }
    return pivot; // new root of subtree.
    std::cerr << "rotate_right end execution.\n";
}

RB_Tree::Node *RB_Tree::rotate_left(Node *n) {
    std::cerr << "rotate_left execute.\n";
    Node *parent_of_root = n->parent;
    Node *pivot = n->right;
    n->right = pivot->left;
    if (pivot->left != nil) {
        n->right->parent = n;
    }
    pivot->left = n;
    n->parent = pivot;    
    if (parent_of_root == nullptr) {
        pivot->parent = nullptr;
        root = pivot;
    } else {
        pivot->parent = parent_of_root;
        if (n->key < parent_of_root->key)
            parent_of_root->left = pivot;
        else
            parent_of_root->right = pivot;
    }
    return pivot;
    std::cerr << "rotate_left end execution.\n";
}

bool RB_Tree::is_leaf(Node *n) {
    return n == nil;
}

bool RB_Tree::is_root(Node *n) {
    return n->parent == nullptr;
}

bool RB_Tree::is_right_child(Node *n) {
    if (is_leaf(n)) return false;
    if (is_root(n)) return false;
    return n == n->parent->right;
}

bool RB_Tree::is_left_child(Node *n) {
    if (is_leaf(n)) return false;
    if (is_root(n)) return false;
    return n == n->parent->left;
}

bool RB_Tree::has_uncle(Node *n) {
    return uncle(n) != nil;
}

RB_Tree::Node *RB_Tree::grandparent(Node *n) {
    return n->parent->parent;
}

RB_Tree::Node *RB_Tree::sibling(Node *n) {
    if (is_left_child(n)) return n->parent->right;
    else return n->parent->left;
}

RB_Tree::Node *RB_Tree::uncle(Node *n) {
    if (is_left_child(n->parent)) return grandparent(n)->right;
    else return grandparent(n)->left;
}

/*
struct Node {
    Node *parent;
    Node *left;
    Node *right;
    Color color;
    int key;
};
*/

void RB_Tree::insert_helper(Node *curr, Node *parent, Node *n) {
    if (curr == nil) {
        if (n->key < parent->key) parent->left = n;
        else parent->right = n;
        n->parent = parent;
        insert_case3(n);
        return;
    }
    if (n->key == curr->key) {
        delete n;
        n = nullptr;
        return;
    }
    if (n->key < curr->key) insert_helper(curr->left, curr, n);
    else insert_helper(curr->right, curr, n);
}

void RB_Tree::insert(int key) {
    Node *n = new Node;
    n->key = key;
    if (root == nullptr) {
        root = n;
        return;
    } else insert_helper(root, nullptr, n);
    std::cerr << n->key << " inserted." << std::endl;
}

/*
 * The calling sequense of insert_case*() is not by number, but basically follows:
 *
 *      case3 -> case1 -> case4 -> case2 -> case5 -> case6
 *      ^                          |
 *      |__________________________|
 *
 */ 

void RB_Tree::insert_case1(Node *n) {
    std::cerr << "insert_case1 execute\n";
    if (n->parent->color == BLACK) return;
    insert_case4(n);
}

void RB_Tree::insert_case2(Node *n) {
    std::cerr << "insert_case2 execute\n";
    if (n->parent->color == RED && has_uncle(n) && uncle(n)->color == RED) {
        n->parent->color = uncle(n)->color = BLACK;
        grandparent(n)->color = RED;
        insert_case3(grandparent(n));
    }
    insert_case5(n);
}

void RB_Tree::insert_case3(Node *n) {
    std::cerr << "insert_case3 execute\n";
    if (n == root) return;
    insert_case1(n);    
}

void RB_Tree::insert_case4(Node *n) {
    std::cerr << "insert_case4 execute\n";
    if (n->parent->color == RED && n->parent == root) {
        n->parent->color = BLACK; 
        return;
    }
    insert_case2(n);    
}

void RB_Tree::insert_case5(Node *n) {
    std::cerr << "insert_case5 execute\n";
    if (n->parent->color == RED && (!has_uncle(n) || (has_uncle(n) && uncle(n)->color == BLACK))) {
        if (is_inner_child(n)) {
            Node *parent = n->parent;
            if (is_right_child(n)) {
                rotate_left(n->parent);
            } else rotate_right(n->parent);
            insert_case6(parent);
        } else insert_case6(n); // n is now outer child of its grandparent.
    } // Insertion must complete here.
}

void RB_Tree::insert_case6(Node *n) { // when n is outer child
    std::cerr << "insert_case6 execute\n";
    Node *parent = n->parent;
    Node *grandparent = this->grandparent(n);
    if (is_left_child(n)) {
       rotate_right(grandparent);
    } else rotate_left(grandparent);
    parent->color = BLACK;
    grandparent->color = RED;
}

bool RB_Tree::is_inner_child(Node *n) {
    if (n == nullptr || n->parent == nullptr || grandparent(n) == nullptr) return false;
    return ((is_left_child(n) && is_right_child(n->parent)) || (is_right_child(n) && is_left_child(n->parent)));
}

bool RB_Tree::is_outer_child(Node *n) {
    if (n == nullptr || n->parent == nullptr || grandparent(n) == nullptr) return false;
    return ((is_left_child(n) && is_right_child(n->parent)) || (is_right_child(n) && is_left_child(n->parent)));

}

void RB_Tree::print_helper(std::ostream &os, Node *n) {
    std::cerr << "->\n";
    if (n == nil) {
        std::cerr << "<-\n";
        return;
    }
    print_helper(os, n->left);
    os << n->key << "\t";
    if (n->color == RED) std::cerr << "RED";
    else if(n->color == BLACK) std::cerr << "BLACK";
    std::cerr << "\n";
    print_helper(os, n->right);
    std::cerr << "<-\n";
}

void RB_Tree::print(std::ostream &os) {
    print_helper(os, root);
}

int main() {
    RB_Tree tree;
    std::vector<int> arr{4, 2, 7, 10, 1, 9, 5, 17, 20, 39, 8, 24, 55, 39};
    for (auto elem : arr) {
        tree.insert(elem);
        std::cerr << "-----------------------\n";
        tree.print(std::cerr);
        std::cerr << "-----------------------\n";
    }
//    tree.print(std::cout);
    return 0;
}
