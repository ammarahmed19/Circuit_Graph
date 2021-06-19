#include <iostream>
#include <vector>
#include <set>
#include "etypes.h"
#include "edge.h"
#include "node.h"

using namespace std;

void Edge::setLabel(string l) {label = l;}
//void setResistance(float r) {resistance = r;}
string Edge::getLabel() {return label;}
void Edge::setStart(Node* n) {start = n;}
void Edge::setEnd(Node* n) {end = n;} 
Node* Edge::getStart() {return start;}
Node* Edge::getEnd() {return end;} 
etypes Edge::etype() {return EDG;}
float Edge::getResistance() {return 0;}
float Edge::getVoltage() {return 0;}

Resistance::Resistance(string l, float r) {
        setLabel(l);
        resistance = r;
    }

float Resistance::getResistance() {
        return resistance;
    }

etypes Resistance::etype() {
        return ER;
    }


VS::VS(string l, float v) {
    setLabel(l);
    voltage = v;
}

float VS::getVoltage() {
    return voltage;
}

etypes VS::etype() {
    return EVS;
}
