#include <iostream>
#include <vector>
#include <set>
#include <map>
using namespace std;

enum etypes{EDG, ER, EVS};

void gaussSeidel(vector<vector<float>> a, vector<float> b, int m, vector<float> x) {
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


void printMatrix(vector<vector<float>> mat) {
    int n = mat.size();
    for(int i=0;i<n;i++)  // loop 3 times for three lines
    {
        for(int j=0;j<n;j++)  // loop for the three elements on the line
        {
            cout<<mat[i][j]<<" ";  // display the current element out of the array
        }
    cout<<endl;  // when the inner loop is done, go to a new line
    }
}

class Node {
    private:
    string label;
    float voltage;
    set<Edge*> edges;
    set<Node*> adj;
    public:
    Node(string l, float v) {
        label = l;
        voltage = v;
    }
    Node(string l) {
        label = l;
        //voltage = NULL;
    }
    Node() = default;

    void setLabel(string l) {label = l;}
    void setVoltage(float v) {voltage = v;}
    string getLabel() {return label;}
    float getVoltage() {return voltage;}
    void addConnection(Edge* edge) {edges.insert(edge);}
    void addAdj(Node* node) {adj.insert(node);}
    bool known() {if (voltage) {return true;} else {return false;}}
    set<Edge*> getConnections() {return edges;}
    set<Node*> getAdj() {return adj;}
    bool hasEdge(Edge* e) {
        set<Edge*>::iterator i;
        for (i = edges.begin(); i != edges.end(); i++) {
            if (*i == e) {
                return true;
            }
        }
        return false;
    }
    set<Edge*> findEdges(Node* node) {
        set<Edge*> out;
        set<Edge*>::iterator i;
        for (i = edges.begin(); i != edges.end(); i++) {
            if(((*i)->getStart() == this && (*i)->getEnd() == node) || ((*i)->getEnd() == this && (*i)->getStart() == node)) {
                out.insert((*i));
            }
        }
        return out;
    }

    // Linear Search
    static int searchVector(vector<Node*> v, Node* elem) {
        for (int i = 0; i < v.size(); i++) {
            if (v[i] == elem) {
                return i;
            }
        }
        return -1;
    }
};

class Edge {
    protected:
    string label;
    Node* start;
    Node* end;
    //float resistance;

    public:
    //Edge(string l, float r) {
    //    label = l;
    //    resistance = r;
    //}
    //Edge() = default;
    void setLabel(string l) {label = l;}
    //void setResistance(float r) {resistance = r;}
    string getLabel() {return label;}
    void setStart(Node* n) {start = n;}
    void setEnd(Node* n) {end = n;} 
    Node* getStart() {return start;}
    Node* getEnd() {return end;} 
    virtual etypes etype() {return EDG;}
    virtual float getResistance() {return 0;}
    virtual float getVoltage() {return 0;}



    //float getResistance() {return resistance;}
};

class Resistance : public Edge {
    private:
    float resistance;
    public:
    Resistance(string l, float r) {
        setLabel(l);
        resistance = r;
    }

    float getResistance() {
        return resistance;
    }

    etypes etype() {
        return ER;
    }
};

class VS : public Edge {
    private: 
    float voltage;

    public:
    VS(string l, float v) {
        setLabel(l);
        voltage = v;
    }

    float getVoltage() {
        return voltage;
    }
    etypes etype() {
        return EVS;
    }
};


class Circuit {
    private:
    vector<Node*> nodes;
    vector<Edge*> edges;
    Node* GND;

    public:
    Circuit() = default;
    void addNode(string label, float voltage) {
        Node* node = new Node(label, voltage);
        nodes.push_back(node);
    }

    void setGND(Node* g) {
        GND = g;
    }
    void addNode(string label) {
        Node* node = new Node(label);
        nodes.push_back(node);
    }

    Node* findNode(string label) {
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i]->getLabel() == label) {
                return nodes[i];
            }
        }
        return NULL;
    }
    
    void addRes(string label, float resistance, Node* n1, Node* n2) {
        Resistance* edge = new Resistance(label, resistance);
        edge->setStart(n1);
        edge->setEnd(n2);
        edges.push_back(edge);

        n1->addAdj(n2);
        n2->addAdj(n1);
        n1->addConnection(edge);
        n2->addConnection(edge);
    }

    void addVS(string label, float voltage, Node* n1, Node* n2) {
        VS* edge = new VS(label, voltage);
        edges.push_back(edge);
        n1->addAdj(n2);
        n2->addAdj(n1);
        n1->addConnection(edge);
        n2->addConnection(edge);

        n2->setVoltage(n1->getVoltage()-voltage);
    }

    void printCircuit() {
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

    bool hasIndepSrc() {
        for (int i = 0; i < edges.size(); i ++) {
            if (edges[i]->etype() == EVS) {
                return true;
            }
        }
        return false;
    }
    /******
    void currentKCL() {
        vector<float> b;
        vector<vector<float>> mat;
        for (int i = 0; i < nodes.size(); i++) {
            set<Node*> adj = nodes[i]->getAdj();
            set<Node*>::iterator j;
            for (j = adj.begin(); j != adj.end(); j++) {
                vector<float> row;
                for (int k = 0; k < edges.size(); k++) {
                    if ((*j)->hasEdge(edges[k])) {
                        //float col = (nodes[i]->getVoltage()-(*j)->getVoltage())/edges[k]->getResistance();
                        //row.push_back(col);
                    }
                    else {
                        row.push_back(0);
                    }
                }
                mat.push_back(row);
                b.push_back(0);
            }
    }
    printMatrix(mat);
    gaussSeidel(mat, b, 1, b);
    }
    ****/

    void nodal() {
        vector<vector<float>> G; // G Matrix
        for (int i = 0; i < nodes.size(); i++) {
            //map<string , float> row;
            set<Node*> adj = nodes[i]->getAdj();
            vector<Node*> vadj(adj.begin(), adj.end());
            //set<Node*>::iterator j;
            //set<Node*>::iterator k;
            if (!this->hasIndepSrc()) {
                cout << "The circuit has no independent sources" << endl;
                return; 
            }
            for (int j = 0; j < adj.size(); j++) {
               set<Edge*> branches = nodes[i]->findEdges((*j));
               vector<Edge*> vbranches(branches.begin(), branches.end());
               if (branches.size() > 0) {
                   float conductance;
                   for (int k = 0; k < branches.size(); k++) {
                       if (vbranches[k]->etype() == ER) {
                           conductance += 1/(vbranches[k])->getResistance();
                           Node* n1 = vbranches[k]->getStart();
                           Node* n2 = vbranches[k]->getEnd();

                           if (n1 != GND && n2 != GND) {
                               int a = Node::searchVector(vadj, n1);
                               int b = Node::searchVector(vadj, n2);

                               G[a][b] = conductance*-1;
                               G[b][a] = conductance*-1;
                           }
                       }
                       else if (vbranches[k]->etype() == EVS) {
                           
                       }
                   }
                   G[j][j] = conductance;
               }
            }
        }
        printMatrix(G);
    }
};

int main(void) {
    Circuit circuit;
    circuit.addNode("GND", 0);
    circuit.addNode("VCC", 9);
    circuit.addNode("A");
    circuit.setGND(circuit.findNode("GND"));
    
    circuit.addRes("R1", 1000, circuit.findNode("GND"), circuit.findNode("VCC"));
    circuit.addRes("R2", 2000, circuit.findNode("GND"), circuit.findNode("VCC"));
    circuit.addVS("VS", 9, circuit.findNode("VCC"), circuit.findNode("GND"));
    circuit.addRes("R3", 250, circuit.findNode("VCC"), circuit.findNode("A"));
    circuit.addRes("R4", 500, circuit.findNode("A"), circuit.findNode("GND"));
    

    circuit.printCircuit();
    //circuit.currentKCL();
}

//void inputCircuit(string fname) {
//
//}