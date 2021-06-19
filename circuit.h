#ifndef CIRCUIT
#define CIRCUIT

#include <iostream>
#include <vector>
#include <set>
#include "etypes.h"
#include "edge.h"
#include "node.h"

using namespace std;


class Circuit {
    private:
    vector<Node*> nodes;
    vector<Edge*> edges;
    Node* GND;

    public:
    Circuit() = default;
    void addNode(string label, float voltage);

    void setGND(Node* g);
    Node* getGND();
    void addNode(string label);

    Node* findNode(string label);

    int countIndSrc();
    
    void addRes(string label, float resistance, Node* n1, Node* n2);

    void addVS(string label, float voltage, Node* n1, Node* n2);

    vector<vector<float>> transpose(vector<vector<float>> mat, int n, int m);

    void printCircuit();

    bool hasIndepSrc();

    void nodal();

};

#endif