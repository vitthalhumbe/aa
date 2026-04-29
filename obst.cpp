#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// search
Node* search(Node* root, int val) {
    if (!root || root->data == val) return root;
    if (val < root->data) return search(root->left, val);
    return search(root->right, val);
}

// insert
Node* insert(Node* root, int val) {
    if (!root) return new Node(val);
    if (val < root->data) root->left  = insert(root->left,  val);
    else                  root->right = insert(root->right, val);
    return root;
}

// find min node (used in delete)
Node* minNode(Node* root) {
    while (root->left) root = root->left;
    return root;
}

// delete
Node* deleteNode(Node* root, int val) {
    if (!root) return nullptr;
    
    if (val < root->data) {
        root->left  = deleteNode(root->left,  val);
    } else if (val > root->data) {
        root->right = deleteNode(root->right, val);
    } else {
        // case 1 & 2: no child or one child
        if (!root->left)  { Node* tmp = root->right; delete root; return tmp; }
        if (!root->right) { Node* tmp = root->left;  delete root; return tmp; }
        
        // case 3: two children — replace with inorder successor
        Node* successor  = minNode(root->right);
        root->data       = successor->data;
        root->right      = deleteNode(root->right, successor->data);
    }
    return root;
}

// inorder to verify
void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

int main() {
    Node* root = nullptr;
    
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);

    cout << "Inorder: ";
    inorder(root);  // 20 30 40 50 70
    cout << endl;

    cout << "Search 40: " << (search(root, 40) ? "Found" : "Not found") << endl;
    cout << "Search 99: " << (search(root, 99) ? "Found" : "Not found") << endl;

    root = deleteNode(root, 30);
    cout << "After delete 30: ";
    inorder(root);  // 20 40 50 70
    cout << endl;

    return 0;
}