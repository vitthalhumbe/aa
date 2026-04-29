#include <iostream>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    Node *left, *right;
    int height;
    Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

class AVL {
    Node* root = nullptr;

    int height(Node* n) { return n ? n->height : 0; }

    int bf(Node* n) { return n ? height(n->left) - height(n->right) : 0; }

    void updateHeight(Node* n) {
        n->height = 1 + max(height(n->left), height(n->right));
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* insert(Node* node, int key) {
        if (!node) return new Node(key);
        if      (key < node->key) node->left  = insert(node->left,  key);
        else if (key > node->key) node->right = insert(node->right, key);
        else return node;  // duplicate, ignore

        updateHeight(node);
        int b = bf(node);

        if (b >  1 && key < node->left->key)  return rightRotate(node);         // LL
        if (b < -1 && key > node->right->key) return leftRotate(node);          // RR
        if (b >  1 && key > node->left->key)  { node->left  = leftRotate(node->left);  return rightRotate(node); } // LR
        if (b < -1 && key < node->right->key) { node->right = rightRotate(node->right); return leftRotate(node); } // RL

        return node;
    }

    bool search(Node* node, int key) {
        if (!node) return false;
        if (key == node->key) return true;
        return key < node->key ? search(node->left, key) : search(node->right, key);
    }

    void inorder(Node* node) {
        if (!node) return;
        inorder(node->left);
        cout << node->key << " ";
        inorder(node->right);
    }

public:
    void insert(int key) { root = insert(root, key); }

    void search(int key) {
        cout << (search(root, key) ? "Found\n" : "Not found\n");
    }

    void display() { inorder(root); cout << "\n"; }
};

int main() {
    AVL avl;
    int ch, key;
    do {
        cout << "\n1.Insert 2.Search 3.Display 4.Exit\n>> ";
        cin >> ch;
        switch (ch) {
            case 1: cout << "Key: "; cin >> key; avl.insert(key); break;
            case 2: cout << "Key: "; cin >> key; avl.search(key); break;
            case 3: avl.display(); break;
            case 4: break;
            default: cout << "Invalid\n";
        }
    } while (ch != 4);
}
