#include <iostream>
using namespace std;
#define SIZE 10

struct Node {
    int key, value;
    Node* next;
    Node(int k, int v) : key(k), value(v), next(nullptr) {}
};

class HashMap {
    Node* table[SIZE] = {};

    int hash(int key) { return abs(key) % SIZE; }

public:
    void insert(int key, int val) {
        int idx = hash(key);
        Node* cur = table[idx];
        while (cur) {
            if (cur->key == key) { cur->value = val; return; } // update if exists
            cur = cur->next;
        }
        Node* node = new Node(key, val);
        node->next = table[idx];
        table[idx] = node;  // insert at head
    }

    void search(int key) {
        Node* cur = table[hash(key)];
        while (cur) {
            if (cur->key == key) { cout << "Found: " << cur->value << "\n"; return; }
            cur = cur->next;
        }
        cout << "Not found\n";
    }

    void remove(int key) {
        int idx = hash(key);
        Node* cur = table[idx];
        Node* prev = nullptr;
        while (cur && cur->key != key) { prev = cur; cur = cur->next; }
        if (!cur) { cout << "Not found\n"; return; }
        if (!prev) table[idx] = cur->next;
        else prev->next = cur->next;
        delete cur;
        cout << "Deleted\n";
    }

    void display() {
        for (int i = 0; i < SIZE; i++) {
            cout << "[" << i << "] -> ";
            Node* cur = table[i];
            while (cur) { cout << "{" << cur->key << ":" << cur->value << "} -> "; cur = cur->next; }
            cout << "NULL\n";
        }
    }
};

int main() {
    HashMap hm;
    int ch, k, v;
    do {
        cout << "\n1.Insert 2.Search 3.Delete 4.Display 5.Exit\n>> ";
        cin >> ch;
        switch (ch) {
            case 1: cout << "Key Val: "; cin >> k >> v; hm.insert(k, v); break;
            case 2: cout << "Key: ";     cin >> k;      hm.search(k);    break;
            case 3: cout << "Key: ";     cin >> k;      hm.remove(k);    break;
            case 4: hm.display(); break;
            case 5: break;
        }
    } while (ch != 5);
}
