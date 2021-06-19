#ifndef NODE
#define NODE

#include <iostream>
#include <vector>
#include <set>
#include "etypes.h"
#include "edge.h"

using namespace std;

class Edge;

class Node {
    private:
    string label;
    float voltage;
    set<Edge*> edges;
    set<Node*> adj;
    public:
    Node(string l, float v);
    Node(string l);
    Node() = default;

    void setLabel(string l);
    void setVoltage(float v);
    string getLabel();
    float getVoltage();
    void addConnection(Edge* edge);
    void addAdj(Node* node);
    bool known();
    set<Edge*> getConnections();
    set<Node*> getAdj();
    bool hasEdge(Edge* e);
    set<Edge*> findEdges(Node* node);

    // Linear Search
    static int searchVector(vector<Node*> v, Node* elem);
};

#endif