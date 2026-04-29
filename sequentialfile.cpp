#include <iostream>
#include <fstream>
using namespace std;

struct EmployeeRecord {
    int id;
    char name[50];
    char designation[50];
    float salary;
    bool isDeleted;
};

struct IndexRecord {
    int id;
    long pos;
};

class EmployeeManagement {
public:
    void add() {
        EmployeeRecord e;
        cout << "ID: ";         cin >> e.id;
        cin.ignore();
        cout << "Name: ";        cin.getline(e.name, 50);
        cout << "Designation: "; cin.getline(e.designation, 50);
        cout << "Salary: ";      cin >> e.salary;
        e.isDeleted = false;

        ofstream df("data.dat", ios::binary | ios::app);
        df.seekp(0, ios::end);
        long pos = df.tellp();
        df.write((char*)&e, sizeof(e));
        df.close();

        IndexRecord idx = { e.id, pos };
        ofstream inf("index.dat", ios::binary | ios::app);
        inf.write((char*)&idx, sizeof(idx));
        cout << "Added\n";
    }

    void search() {
        int key; cout << "ID: "; cin >> key;
        ifstream inf("index.dat", ios::binary);
        ifstream df("data.dat",  ios::binary);
        IndexRecord idx;
        while (inf.read((char*)&idx, sizeof(idx))) {
            if (idx.id == key) {
                EmployeeRecord e;
                df.seekg(idx.pos);
                df.read((char*)&e, sizeof(e));
                if (!e.isDeleted)
                    cout << "ID: " << e.id << "\nName: " << e.name
                         << "\nDesignation: " << e.designation
                         << "\nSalary: " << e.salary << "\n";
                else cout << "Record deleted\n";
                return;
            }
        }
        cout << "Not found\n";
    }

    void remove() {
        int key; cout << "ID: "; cin >> key;
        fstream  df("data.dat",   ios::binary | ios::in | ios::out);
        ifstream inf("index.dat", ios::binary);
        IndexRecord idx;
        while (inf.read((char*)&idx, sizeof(idx))) {
            if (idx.id == key) {
                EmployeeRecord e;
                df.seekg(idx.pos);
                df.read((char*)&e, sizeof(e));
                if (!e.isDeleted) {
                    e.isDeleted = true;
                    df.seekp(idx.pos);
                    df.write((char*)&e, sizeof(e));
                    cout << "Deleted\n";
                } else cout << "Already deleted\n";
                return;
            }
        }
        cout << "Not found\n";
    }

    void reset() {
        ofstream("data.dat",  ios::binary | ios::trunc);
        ofstream("index.dat", ios::binary | ios::trunc);
        cout << "Files cleared\n";
    }
};

int main() {
    EmployeeManagement em;
    int ch;
    do {
        cout << "\n1.Add 2.Search 3.Delete 4.Reset 5.Exit\n>> ";
        cin >> ch;
        switch (ch) {
            case 1: em.add();    break;
            case 2: em.search(); break;
            case 3: em.remove(); break;
            case 4: em.reset();  break;
            case 5: break;
            default: cout << "Invalid\n";
        }
    } while (ch != 5);
}
