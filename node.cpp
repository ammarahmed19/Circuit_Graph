#include <iostream>
#include <vector>
#include <set>
#include "etypes.h"
#include "edge.h"
#include "node.h"

using namespace std;

Node::Node(string l, float v) {
    label = l;
    voltage = v;
}

Node::Node(string l) {
    label = l;
    //voltage = NULL;
}

void Node::setLabel(string l) {label = l;}
void Node::setVoltage(float v) {voltage = v;}
string Node::getLabel() {return label;}
float Node::getVoltage() {return voltage;}
void Node::addConnection(Edge* edge) {edges.insert(edge);}
void Node::addAdj(Node* node) {adj.insert(node);}
bool Node::known() {if (voltage) {return true;} else {return false;}}
set<Edge*> Node::getConnections() {return edges;}
set<Node*> Node::getAdj() {return adj;}
bool Node::hasEdge(Edge* e) {
    set<Edge*>::iterator i;
    for (i = edges.begin(); i != edges.end(); i++) {
        if (*i == e) {
            return true;
        }
    }
    return false;
}
set<Edge*> Node::findEdges(Node* node) {
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
int Node::searchVector(vector<Node*> v, Node* elem) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == elem) {
            return i;
        }
    }
    return -1;
}