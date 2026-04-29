#include <iostream>
#define MAX 20
using namespace std;

class graph {
    int vertices, edges;
    int adj[MAX][MAX];
    
    public:
    
    void accept() {
        cout<< "Enter number of vertices : ";
        cin >> vertices;
        cout<< "Enter number of Edges : ";
        cin >> edges;
        
        for (int i = 0; i< vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                adj[i][j] = 0;
            }
        }
        int u, v, w;
        cout<< "Enter (v, u, weight) :" << endl;
        for (int i = 0; i < edges; i++) {
            cin >> u >> v >> w;
            adj[u][v] = w;
            adj[v][u] = w;
        }
    }
    
    void display()  {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                cout << adj[i][j] <<  "\t";
            }
            cout << endl;
        }
    }
    
    void dijkstra(int start) {
        int r[vertices][vertices], minDest,cnt,  next, i, j, visited[vertices], distance[vertices], from[vertices];
        
        for (i = 0; i < vertices; i++) {
            for (j = 0; j < vertices; j++) {
                if (adj[i][j] == 0) {
                    r[i][j] = 9999;
                } else {
                    r[i][j] = adj[i][j];
                }
            } 
        }
        
        for (i = 0; i < vertices; i++) {
            visited[i] = 0;
            from[i] = start;
            distance[i] = r[start][i];
        }
        
        distance[start] = 0;
        visited[start] = 1;
        
        for (int count = 1;  count < vertices; count++) {
            minDest = 9999;
            for (i = 0; i < vertices; i++) {
                if (!visited[i]  && distance[i] < minDest) {
                    minDest = distance[i];
                    next = i;
                }
            }
            visited[next] = 1;
        
        for (i = 0; i< vertices; i++) {
            if(!visited[i] && distance[i] > minDest + r[next][i]) {
                distance[i] = minDest + r[next][i];
                from[i] = next;
            }
        }
        
        }
        
        for (int i = 0; i < vertices; i++) {
            cout << "\nDistance of " << i << " form " << start << " is " << distance[i];
            cout << "\nPath :" << i;
            
            j= i;
            while (j != start) {
                j = from[j];
                cout << " <- " << j;
            }
            cout << endl;
        }
    }
};
int main() {
    graph g;
    g.accept();
    g.display();
    
    int start;
    cout << "\nEnter starting node : ";
    cin >> start;
    
    g.dijkstra(start);
    
    return 0;
}
