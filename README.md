# Circuit_Graph
# Electric Circuit Simulation

## ECEN204 (Data Structures and Algorithms) Project – Ammar Ahmed, Deyaa Khaled
# Code

The code is composed of the following OOP C++ Classes:

- Node
- Circuit
- Edge (Abstract Class)
  - VS (Voltage Source)
  - Resistance

Below are the attributes of each Class

## Node
```
class Node {

    private:

    string label;

    float voltage;

    set\&lt;Edge\*\&gt; edges;

    set\&lt;Node\*\&gt; adj;
```
## Edge
```
class Edge {

    protected:

    string label;

    Node\* start;

    Node\* end;
```
## Resistance
```
class Resistance : public Edge {

    private:

    float resistance;
```
## VS
```
class VS : public Edge {

    private:

    float voltage;
```
## Circuit
```
class Circuit {

    private:

    vector\&lt;Node\*\&gt; nodes;

    vector\&lt;Edge\*\&gt; edges;

    Node\* GND;
```
# Testing Case

We want to test and simulate the above circuit using our Algorithm.

![Circuit](https://github.com/ammarahmed19/Circuit_Graph/blob/main/gallery/img1.png)

The results from LTSpice tells us that the VCC voltage = 9V, and the Voltage at Node A = 6V, and the total current in the circuit equals = -0.025500 A

## Drawing the circuit in our code
```
int main(void) {

    Circuit circuit;

    circuit.addNode(&quot;GND&quot;, 0);

    circuit.addNode(&quot;VCC&quot;, 9);

    circuit.addNode(&quot;A&quot;);

    circuit.setGND(circuit.findNode(&quot;GND&quot;));

    circuit.addRes(&quot;R1&quot;, 1000, circuit.findNode(&quot;GND&quot;), circuit.findNode(&quot;VCC&quot;));

    circuit.addRes(&quot;R2&quot;, 2000, circuit.findNode(&quot;GND&quot;), circuit.findNode(&quot;VCC&quot;));

    circuit.addVS(&quot;VS&quot;, 9, circuit.findNode(&quot;VCC&quot;), circuit.findNode(&quot;GND&quot;));

    circuit.addRes(&quot;R3&quot;, 250, circuit.findNode(&quot;VCC&quot;), circuit.findNode(&quot;A&quot;));

    circuit.addRes(&quot;R4&quot;, 500, circuit.findNode(&quot;A&quot;), circuit.findNode(&quot;GND&quot;));

    circuit.printCircuit();

    circuit.nodal();

}
```
# Result

This is the output from our program

![Result](https://github.com/ammarahmed19/Circuit_Graph/blob/main/gallery/img2.png)
Which is a correct output because it is identical to the solution from LTSpice

# Video

![Watch the video](https://i.imgur.com/vKb2F1B.png)](https://drive.google.com/file/d/1ENeIBy3wX_MymQOL6J_eMppwXk3FHja7/view?usp=sharing)