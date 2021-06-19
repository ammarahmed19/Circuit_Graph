#include <iostream>
#include <list>
#include <vector>
#include "graph.h"

using namespace std;

Graph::Graph(int V)
{
    this->V = V;
    adj = new vector <pair<int, int>>[V];
}
 
void Graph::addEdge(int u,int v, int wt)
{
    adj[u].push_back(make_pair(v, wt));
    adj[v].push_back(make_pair(u, wt));
}

 
bool Graph::isCyclicUtil(int v, bool visited[], bool *recStack)
{
    if(visited[v] == false)
    {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;
 
        // Recur for all the vertices adjacent to this vertex
        for(auto i = adj[v].begin(); i != adj[v].end(); i++)
        {
            if ( !visited[i->first] && isCyclicUtil(i->first, visited, recStack) )
                return true;
            else if (recStack[i->first])
                return true;
        }
 
    }
    recStack[v] = false;  // remove the vertex from recursion stack
    return false;
}
 
// Returns true if the graph contains a cycle, else false.
bool Graph::isCyclic()
{
    // Mark all the vertices as not visited and not part of recursion
    // stack
    bool *visited = new bool[V];
    bool *recStack = new bool[V];
    for(int i = 0; i < V; i++)
    {
        visited[i] = false;
        recStack[i] = false;
    }
 
    // Call the recursive helper function to detect cycle in different
    // DFS trees
    for(int i = 0; i < V; i++)
        if (isCyclicUtil(i, visited, recStack))
            cout << i << ", ";
 
    return false;
}

