#include <iostream>
#include <vector>
#include <set>
#include "etypes.h"
#include "edge.h"
#include "node.h"
#include "circuit.h"

using namespace std;

void Circuit::gaussSeidel(vector<vector<float>> a, vector<float> b, int m, vector<float> x) {
    int n = a.size();
    float y[100];
    float o[100];
    while (m > 0)
    {
        for (int i = 0; i < n; i++)
        {
            y[i] = (b[i] / a[i][i]);
            for (int j = 0; j < n; j++)
            {
                if (j == i)
                    continue;
                y[i] = y[i] - ((a[i][j] / a[i][i]) * x[j]);
                x[i] = y[i];
            }
            o[i] = y[i];
            //printf("x%d = %f    ", i + 1, y[i]);
        }
        //cout << "\n";
        m--;
    }
    for (int i = 0; i < n; i++) {
        cout << o[i] << endl;
    }
}


void Circuit::printMatrix(vector<vector<float>> mat, int n, int m) {
    for(int i=0;i<n;i++)  // loop 3 times for three lines
    {
        for(int j=0;j<m;j++)  // loop for the three elements on the line
        {
            cout<<mat[i][j]<<" ";  // display the current element out of the array
        }
    cout<<endl;  // when the inner loop is done, go to a new line
    }
}

vector<vector<float>> Circuit::initMatrix(int n, int m) {
    vector<vector<float>> mat; 
    for (int i = 0; i < n; i ++) {
        vector<float> row;
        for (int j = 0; j < m; j++) {
            row.push_back(0);
        }
        mat.push_back(row);
    }
    return mat;
}

vector<vector<float>> Circuit::transpose(vector<vector<float>> mat, int n, int m) {
    vector<vector<float>> out = initMatrix(m, n);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j) {
         out[j][i] = mat[i][j];
      }
      return out;
}

void Circuit::addNode(string label, float voltage) {
    Node* node = new Node(label, voltage);
    nodes.push_back(node);
}

void Circuit::setGND(Node* g) {
    GND = g;
}

Node* Circuit::getGND() {
    return GND;
}
void Circuit::addNode(string label) {
    Node* node = new Node(label);
    nodes.push_back(node);
}

Node* Circuit::findNode(string label) {
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i]->getLabel() == label) {
            return nodes[i];
        }
    }
    return NULL;
}

void Circuit::addRes(string label, float resistance, Node* n1, Node* n2) {
    Resistance* edge = new Resistance(label, resistance);
    edge->setStart(n1);
    edge->setEnd(n2);
    edges.push_back(edge);

    n1->addAdj(n2);
    n2->addAdj(n1);
    n1->addConnection(edge);
    n2->addConnection(edge);
}

void Circuit::addVS(string label, float voltage, Node* n1, Node* n2) {
    VS* edge = new VS(label, voltage);
    edge->setStart(n1);
    edge->setEnd(n2);
    edges.push_back(edge);
    n1->addAdj(n2);
    n2->addAdj(n1);
    n1->addConnection(edge);
    n2->addConnection(edge);

    n2->setVoltage(n1->getVoltage()-voltage);
}

int Circuit::countIndSrc() {
    int count = 0;
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i]->etype() == EVS) {
            count++;
        }
    }
    return count;
}

void Circuit::printCircuit() {
    for (int i = 0; i < nodes.size(); i++) {
        cout << nodes[i]->getLabel() << ", " << nodes[i]->getVoltage() << ", ";
        set<Edge*> edges = nodes[i]->getConnections();
        set<Edge*>::iterator j;
        for (j = edges.begin(); j != edges.end(); j++) {
            if ((*j)->etype() == ER) {
            cout << (*j)->getLabel() << ", " << (*j)->getResistance() << ", ";
            }
            else if ((*j)->etype() == EVS) {
            cout << (*j)->getLabel() << ", " << (*j)->getVoltage() << ", ";
            }
        }
        cout << endl;
    }
}

bool Circuit::hasIndepSrc() {
    for (int i = 0; i < edges.size(); i ++) {
        if (edges[i]->etype() == EVS) {
            return true;
        }
    }
    return false;
}

void Circuit::nodal() {
    int n = nodes.size();
    int m = countIndSrc();
    vector<vector<float>> G = initMatrix(n, n); // G Matrix
    vector<vector<float>> B = initMatrix(n, m); // B Matrix
    vector<vector<float>> C; // C Matrix
    vector<vector<float>> D = initMatrix(m, m); // D Matrix
    vector<vector<float>> V = initMatrix(n, 1); // V Matrix
    vector<vector<float>> J = initMatrix(m, 1); // J Matrix
    for (int i = 0; i < nodes.size(); i++) {
        //map<string , float> row;
        set<Node*> adj = nodes[i]->getAdj();
        //vector<Node*> vadj(adj.begin(), adj.end());
        //set<Node*>::iterator j;
        //set<Node*>::iterator k;
        if (!this->hasIndepSrc()) {
            cout << "The circuit has no independent sources" << endl;
            return; 
        }
        set<Edge*> branches = nodes[i]->getConnections();
        vector<Edge*> vbranches(branches.begin(), branches.end());
        float conductance;
        for (int k = 0; k < vbranches.size(); k++) {
            if (vbranches[k]->etype() == ER) {
                conductance += 1/(vbranches[k])->getResistance();
                Node* n1 = vbranches[k]->getStart();
                Node* n2 = vbranches[k]->getEnd();
                if (n1 != getGND() && n2 != getGND()) {
                    int a = Node::searchVector(nodes, n1);
                    int b = Node::searchVector(nodes, n2);

                    G[a][b] = conductance*-1;
                    G[b][a] = conductance*-1;
                }
            }
            else if (vbranches[k]->etype() == EVS) {
                if (vbranches[k]->getStart() == nodes[i]) {
                    B[i][0] = 1;
                }
                else if (vbranches[k]->getEnd() == nodes[i]) {
                    B[i][0] = -1;
                }
            }
        }
        G[i][i] = conductance;
        }
    C = transpose(B, n, m);
    //printMatrix(G, n, n);
    //printMatrix(B, n, m);
}