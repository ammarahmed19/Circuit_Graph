#include <iostream>
#include <vector>
#include <set>
#include <map>
#include "edge.h"
#include "node.h"
#include "circuit.h"
#include "etypes.h"
#include "circuit.h"

using namespace std;

void Circuit1(void);
void Circuit2(void);
int main(void) {
    Circuit1();
    //Circuit2();
}

void Circuit1(void) {
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
    circuit.nodal();
}

void Circuit2(void) {
    Circuit circuit;
    circuit.addNode("GND", 0);
    circuit.addNode("VCC", 9);
    circuit.addNode("A");
    circuit.addNode("O");
    circuit.setGND(circuit.findNode("GND"));

    circuit.addVS("VS", 9, circuit.findNode("VCC"), circuit.findNode("GND"));    
    circuit.addRes("R1", 3, circuit.findNode("VCC"), circuit.findNode("A"));
    circuit.addRes("R2", 6, circuit.findNode("A"), circuit.findNode("GND"));
    circuit.addRes("R3", 2, circuit.findNode("A"), circuit.findNode("O"));
    circuit.addRes("R4", 4, circuit.findNode("O"), circuit.findNode("GND"));

    circuit.printCircuit();
    circuit.nodal();
}