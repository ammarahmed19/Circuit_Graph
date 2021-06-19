#include <iostream>
#include <vector>

using namespace std;

class Graph
{
    int V;    // No. of vertices
    vector <pair<int, int> > *adj;   // Pointer to an array containing adjacency lists
    bool isCyclicUtil(int v, bool visited[], bool *rs);  // used by isCyclic()
public:
    Graph(int V);   // Constructor
    void addEdge(int u,int v, int wt);
    bool isCyclic();    // returns true if there is a cycle in this graph
    
};