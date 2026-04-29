#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int d) : data(d), next(nullptr) {}
};

class HashTable {
    Node** table;
    int size;

    int hash(int key) { return key % size; }

public:
    HashTable(int s) : size(s) {
        table = new Node*[size];
        for (int i = 0; i < size; i++) table[i] = nullptr;
    }

    void insert(int key) {
        int idx = hash(key);
        Node* newNode = new Node(key);
        if (!table[idx]) { table[idx] = newNode; }
        else {
            Node* temp = table[idx];
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
        cout << key << " inserted at index " << idx << "\n";
    }

    void search(int key) {
        Node* temp = table[hash(key)];
        while (temp) {
            if (temp->data == key) { cout << key << " found\n"; return; }
            temp = temp->next;
        }
        cout << key << " not found\n";
    }

    void remove(int key) {
        int idx = hash(key);
        Node* temp = table[idx];
        Node* prev = nullptr;
        while (temp && temp->data != key) { prev = temp; temp = temp->next; }
        if (!temp) { cout << key << " not found\n"; return; }
        if (!prev) table[idx] = temp->next;
        else prev->next = temp->next;
        delete temp;
        cout << key << " deleted\n";
    }

    void display() {
        for (int i = 0; i < size; i++) {
            cout << "[" << i << "] -> ";
            Node* temp = table[i];
            while (temp) { cout << temp->data << " -> "; temp = temp->next; }
            cout << "NULL\n";
        }
    }
};

int main() {
    HashTable ht(7);
    int ch, key;
    while (true) {
        cout << "\n1.Insert 2.Search 3.Delete 4.Display 5.Exit\n>> ";
        cin >> ch;
        if (ch == 5) break;
        if (ch != 4) { cout << "Key: "; cin >> key; }
        if      (ch == 1) ht.insert(key);
        else if (ch == 2) ht.search(key);
        else if (ch == 3) ht.remove(key);
        else if (ch == 4) ht.display();
    }
}
