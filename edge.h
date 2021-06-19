#ifndef EDGE
#define EDGE

#include <iostream>
#include <vector>
#include <set>
#include "etypes.h"
#include "node.h"

using namespace std;

class Node;
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
    void setLabel(string l);
    //void setResistance(float r) {resistance = r;}
    string getLabel();
    void setStart(Node* n);
    void setEnd(Node* n);
    Node* getStart();
    Node* getEnd();
    virtual etypes etype();
    virtual float getResistance();
    virtual float getVoltage();



    //float getResistance() {return resistance;}
};

class Resistance : public Edge {
    private:
    float resistance;
    public:
    Resistance(string l, float r);

    float getResistance();

    etypes etype();
};

class VS : public Edge {
    private: 
    float voltage;

    public:
    VS(string l, float v);

    float getVoltage();
    etypes etype();
};

#endif