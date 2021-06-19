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
    circuit.nodal();
    //circuit.currentKCL();
}

//void inputCircuit(string fname) {
//
//}