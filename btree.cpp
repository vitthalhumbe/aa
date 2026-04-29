#include <iostream>
using namespace std;

const int T = 2; // min degree, max keys per node = 2T-1 = 3

struct BTreeNode {
    int keys[2*T - 1];
    BTreeNode* children[2*T];
    int n;        // current key count
    bool leaf;

    BTreeNode(bool isLeaf) : n(0), leaf(isLeaf) {
        for (int i = 0; i < 2*T; i++) children[i] = nullptr;
    }
};

class BTree {
    BTreeNode* root;

    // --- split full child into two, push middle key up to parent ---
    void splitChild(BTreeNode* parent, int i, BTreeNode* child) {
        BTreeNode* newNode = new BTreeNode(child->leaf);
        newNode->n = T - 1;

        for (int j = 0; j < T-1; j++)             // copy right half to newNode
            newNode->keys[j] = child->keys[j+T];

        if (!child->leaf)
            for (int j = 0; j < T; j++)            // copy right children too
                newNode->children[j] = child->children[j+T];

        child->n = T - 1;                          // left child shrinks

        for (int j = parent->n; j >= i+1; j--)    // make room in parent children
            parent->children[j+1] = parent->children[j];
        parent->children[i+1] = newNode;

        for (int j = parent->n-1; j >= i; j--)    // make room in parent keys
            parent->keys[j+1] = parent->keys[j];
        parent->keys[i] = child->keys[T-1];        // push median up
        parent->n++;
    }

    // --- insert into a node guaranteed to be non-full ---
    void insertNonFull(BTreeNode* node, int key) {
        int i = node->n - 1;
        if (node->leaf) {
            while (i >= 0 && key < node->keys[i]) {  // shift keys right
                node->keys[i+1] = node->keys[i];
                i--;
            }
            node->keys[i+1] = key;
            node->n++;
        } else {
            while (i >= 0 && key < node->keys[i]) i--;
            i++;
            if (node->children[i]->n == 2*T-1) {     // child is full, split it
                splitChild(node, i, node->children[i]);
                if (key > node->keys[i]) i++;
            }
            insertNonFull(node->children[i], key);
        }
    }

    // --- search: returns node containing key, nullptr if not found ---
    BTreeNode* search(BTreeNode* node, int key) {
        int i = 0;
        while (i < node->n && key > node->keys[i]) i++;
        if (i < node->n && key == node->keys[i]) return node;
        if (node->leaf) return nullptr;
        return search(node->children[i], key);
    }

    // --- deletion helpers ---

    int getPredecessor(BTreeNode* node, int idx) {
        BTreeNode* cur = node->children[idx];
        while (!cur->leaf) cur = cur->children[cur->n];  // go rightmost
        return cur->keys[cur->n - 1];
    }

    int getSuccessor(BTreeNode* node, int idx) {
        BTreeNode* cur = node->children[idx+1];
        while (!cur->leaf) cur = cur->children[0];       // go leftmost
        return cur->keys[0];
    }

    void merge(BTreeNode* node, int idx) {
        BTreeNode* left  = node->children[idx];
        BTreeNode* right = node->children[idx+1];

        left->keys[T-1] = node->keys[idx];               // pull median down
        for (int i = 0; i < right->n; i++)
            left->keys[T+i] = right->keys[i];
        if (!left->leaf)
            for (int i = 0; i <= right->n; i++)
                left->children[T+i] = right->children[i];

        for (int i = idx+1; i < node->n; i++)            // close gap in parent
            node->keys[i-1] = node->keys[i];
        for (int i = idx+2; i <= node->n; i++)
            node->children[i-1] = node->children[i];

        left->n += right->n + 1;
        node->n--;
        delete right;
    }

    void borrowFromPrev(BTreeNode* node, int idx) {
        BTreeNode* child   = node->children[idx];
        BTreeNode* sibling = node->children[idx-1];

        for (int i = child->n-1; i >= 0; i--)            // shift child right
            child->keys[i+1] = child->keys[i];
        if (!child->leaf)
            for (int i = child->n; i >= 0; i--)
                child->children[i+1] = child->children[i];

        child->keys[0]    = node->keys[idx-1];            // parent key down to child
        if (!child->leaf)
            child->children[0] = sibling->children[sibling->n];
        node->keys[idx-1] = sibling->keys[sibling->n-1]; // sibling key up to parent
        child->n++;
        sibling->n--;
    }

    void borrowFromNext(BTreeNode* node, int idx) {
        BTreeNode* child   = node->children[idx];
        BTreeNode* sibling = node->children[idx+1];

        child->keys[child->n] = node->keys[idx];          // parent key down to child
        if (!child->leaf)
            child->children[child->n+1] = sibling->children[0];
        node->keys[idx] = sibling->keys[0];               // sibling key up to parent

        for (int i = 1; i < sibling->n; i++)              // shift sibling left
            sibling->keys[i-1] = sibling->keys[i];
        if (!sibling->leaf)
            for (int i = 1; i <= sibling->n; i++)
                sibling->children[i-1] = sibling->children[i];
        child->n++;
        sibling->n--;
    }

    void fill(BTreeNode* node, int idx) {
        if (idx != 0 && node->children[idx-1]->n >= T)
            borrowFromPrev(node, idx);
        else if (idx != node->n && node->children[idx+1]->n >= T)
            borrowFromNext(node, idx);
        else {
            if (idx != node->n) merge(node, idx);
            else                merge(node, idx-1);
        }
    }

    void deleteKey(BTreeNode* node, int key) {
        int idx = 0;
        while (idx < node->n && node->keys[idx] < key) idx++;

        if (idx < node->n && node->keys[idx] == key) {
            if (node->leaf) {
                for (int i = idx+1; i < node->n; i++)     // shift left, overwrite
                    node->keys[i-1] = node->keys[i];
                node->n--;
            } else {
                if (node->children[idx]->n >= T) {
                    int pred = getPredecessor(node, idx);  // replace with predecessor
                    node->keys[idx] = pred;
                    deleteKey(node->children[idx], pred);
                } else if (node->children[idx+1]->n >= T) {
                    int succ = getSuccessor(node, idx);    // replace with successor
                    node->keys[idx] = succ;
                    deleteKey(node->children[idx+1], succ);
                } else {
                    merge(node, idx);                      // merge then delete
                    deleteKey(node->children[idx], key);
                }
            }
        } else {
            if (node->leaf) { cout << key << " not found\n"; return; }
            bool last = (idx == node->n);
            if (node->children[idx]->n < T) fill(node, idx);
            if (last && idx > node->n) deleteKey(node->children[idx-1], key);
            else                       deleteKey(node->children[idx],   key);
        }
    }

    void inorder(BTreeNode* node) {
        if (!node) return;
        for (int i = 0; i < node->n; i++) {
            if (!node->leaf) inorder(node->children[i]);
            cout << node->keys[i] << " ";
        }
        if (!node->leaf) inorder(node->children[node->n]);
    }

public:
    BTree() { root = new BTreeNode(true); }

    void insert(int key) {
        BTreeNode* r = root;
        if (r->n == 2*T-1) {                              // root full, grow tree up
            BTreeNode* s = new BTreeNode(false);
            s->children[0] = r;
            splitChild(s, 0, r);
            int i = (key > s->keys[0]) ? 1 : 0;
            insertNonFull(s->children[i], key);
            root = s;
        } else {
            insertNonFull(r, key);
        }
    }

    void remove(int key) {
        deleteKey(root, key);
        if (root->n == 0 && !root->leaf) {                // root emptied, shrink tree
            BTreeNode* old = root;
            root = root->children[0];
            delete old;
        }
    }

    void search(int key) {
        cout << (search(root, key) ? "Found\n" : "Not found\n");
    }

    void display() { inorder(root); cout << "\n"; }
};

int main() {
    BTree bt;
    int ch, key;
    do {
        cout << "\n1.Insert 2.Delete 3.Search 4.Display 5.Exit\n>> ";
        cin >> ch;
        switch (ch) {
            case 1: cout << "Key: "; cin >> key; bt.insert(key); break;
            case 2: cout << "Key: "; cin >> key; bt.remove(key);  break;
            case 3: cout << "Key: "; cin >> key; bt.search(key);  break;
            case 4: bt.display(); break;
            case 5: break;
        }
    } while (ch != 5);
}
