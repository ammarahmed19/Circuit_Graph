#include <iostream>
#include <vector>
#include <set>
#include "etypes.h"
#include "edge.h"
#include "node.h"
#include "circuit.h"
#include <math.h>

using namespace std;


// HELPER FUNCTIONS
void printMatrix(vector<vector<float>> mat, int n, int m);
void gaussJordan(vector<vector<float>> a, int n);
vector<float> findSolution(vector <vector<float>> coeff, int n);
void gaussSeidel(vector<vector<float>> a, int n, int l, vector<float> b, int m, vector<float> x);
float determinant(vector<vector<float>> matrix, int n);
vector<vector<float>> initMatrix(int n, int m);

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
    vector<Node*> vnodes;
    for (int i = 0; i < nodes.size(); i++) {
        if(nodes[i] != getGND()) vnodes.push_back(nodes[i]);
    }
    int n = vnodes.size();
    int m = countIndSrc();
    vector<vector<float>> A = initMatrix(m+n,m+n+1);
    vector<vector<float>> G = initMatrix(n, n); // G Matrix
    vector<vector<float>> B = initMatrix(n, m); // B Matrix
    vector<vector<float>> C; // C Matrix
    vector<vector<float>> D = initMatrix(m, m); // D Matrix
    vector<float> X;
    vector<float> Z;
    // FILL Z Matrix
    for (int i = 0; i < n; i++) Z.push_back(0);
    for (int i = 0; i < m+n; i++) X.push_back(0);
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i]->etype() == EVS) {
            Z.push_back(edges[i]->getVoltage());
        }
    }
    if (!this->hasIndepSrc()) {
            cout << "The circuit has no independent sources" << endl;
            return; 
        }
    for (int i = 0; i < vnodes.size(); i++) {
        //map<string , float> row;
        set<Node*> adj = vnodes[i]->getAdj();
        //vector<Node*> vadj(adj.begin(), adj.end());
        //set<Node*>::iterator j;
        //set<Node*>::iterator k;
        set<Edge*> branches = vnodes[i]->getConnections();
        vector<Edge*> vbranches(branches.begin(), branches.end());
        float conductance = 0;
        for (int k = 0; k < vbranches.size(); k++) {
            if (vbranches[k]->etype() == ER) {
                conductance += 1/(vbranches[k])->getResistance();
                Node* n1 = vbranches[k]->getStart();
                Node* n2 = vbranches[k]->getEnd();
                if (n1 != getGND() && n2 != getGND()) {
                    int a = Node::searchVector(vnodes, n1);
                    int b = Node::searchVector(vnodes, n2);

                    G[a][b] = conductance*-1;
                    G[b][a] = conductance*-1;
                }
            }
            else if (vbranches[k]->etype() == EVS) {
                if (vbranches[k]->getStart() == vnodes[i]) {
                    B[i][0] = 1;
                }
                else if (vbranches[k]->getEnd() == vnodes[i]) {
                    B[i][0] = -1;
                }
            }
        }
        G[i][i] = conductance;
        }
    C = transpose(B, n, m);

    // Create A Matrix
    // ADD G
    for (int i = 0; i < n; i ++) {
        for(int j = 0; j < n; j ++) {
            A[i][j] = G[i][j];
        }
    }

    // ADD B
    for (int i = 0; i < n; i++) {
        for(int j = n; j < n+m; j++) {
            A[i][j] = B[i][j-n];
        }
    }
    
    // ADD C
    for (int i = n; i < n+m; i ++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = C[i-n][j];
        }
    }
    // ADD D
    for (int i = n; i < n+m; i ++) {
        for (int j = n; j < n+m; j++) {
            A[i][j] = D[i-n][j-n];
        }
    }

    // ADD Z
    for (int i = 0; i < n+m; i++) {
        A[i][n+m] = Z[i];
    }
    //printMatrix(G, n, n);
    //printMatrix(B, n, m);
    //gaussSeidel(A, n+m, n+m, Z, 10, X);
    //printMatrix(A, n+m, n+m+1);
    cout << "MNA MATRIX" << endl;
    printMatrix(A, n+m, n+m);
    cout << "------------------" << endl;
    cout << "Solutions: " << endl;
    vector<float> x = findSolution(A, n+m);
    for (int i = 0; i < vnodes.size(); i++) {
        cout << vnodes[i]->getLabel() << " = " << x[i] << endl;
    }
    cout << "Total Current = " << x[x.size()-1] << endl;
}

vector<vector<float>> initMatrix(int n, int m) {
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

void gaussSeidel(vector<vector<float>> a, int n, int l, vector<float> b, int m, vector<float> x) {
    float y[100];
    float o[100];
    while (m > 0)
    {
        for (int i = 0; i < n; i++)
        {
            y[i] = (b[i] / a[i][i]);
            for (int j = 0; j < l; j++)
            {
                if (j == i)
                    continue;
                y[i] = y[i] - ((a[i][j] / a[i][i]) * x[j]);
                x[i] = y[i];
            }
            o[i] = y[i];
            //printf("x%d = %f    ", i + 1, y[i]);
        }
       // cout << "\n";
        m--;
    }
    for (int i = 0; i < n; i++) {
        cout << o[i] << endl;
    }
}


float determinant(vector<vector<float>> A, int n)
{

   float det = 1;

   // Row operations for i = 0, ,,,, n - 2 (n-1 not needed)
   for ( int i = 0; i < n - 1; i++ )
   {
      // Partial pivot: find row r below with largest element in column i
      int r = i;
      float maxA = abs( A[i][i] );
      for ( int k = i + 1; k < n; k++ )
      {
         float val = abs( A[k][i] );
         if ( val > maxA )
         {
            r = k;
            maxA = val;
         }
      }
      if ( r != i )
      {
         for ( int j = i; j < n; j++ ) swap( A[i][j], A[r][j] );
         det = -det;
      }

      // Row operations to make upper-triangular
      float pivot = A[i][i];
      if ( abs( pivot ) < 1.0E-30 ) return 0.0;              // Singular matrix

      for ( int r = i + 1; r < n; r++ )                    // On lower rows
      {
         float multiple = A[r][i] / pivot;                // Multiple of row i to clear element in ith column
         for ( int j = i; j < n; j++ ) A[r][j] -= multiple * A[i][j];
      }
      det *= pivot;                                        // Determinant is product of diagonal
   }

   det *= A[n-1][n-1];

   return det;
}

vector<float> findSolution(vector <vector<float>> coeff, int n)
{
    // Matrix d using coeff as given in cramer's rule
    vector<vector<vector<float>>> dets;
    vector<float> x;
    for (int i = 0; i < n+1; i++) {
        vector<vector<float>> d = initMatrix(n, n);
        dets.push_back(d);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < dets.size(); k++) {
                dets[k][i][j] = coeff[i][j];
            }
        }
    }

    for (int i = 0; i < n; i++) {
        //d1[i][0] = coeff[i][3];
        //d2[i][1] = coeff[i][3];
        //d3[i][2] = coeff[i][3];
        for (int j = 1; j < dets.size(); j++) {
                dets[j][i][j-1] = coeff[i][n];
            }
    }
    // Calculating Determinant of Matrices d, d1, d2, d3
    float D = determinant(dets[0], n);
    if (D==0) {
        cout << "No solution or infinite solutions" << endl;
        return x;
    }
    for (int i = 1; i < dets.size(); i++) {
        float Dx = determinant(dets[i], n) / D;
        x.push_back(Dx);
        //cout << "x" << i << " = " << Dx << endl;
    }
    return x;
}

void gaussJordan(vector<vector<float>> a, int n) {
    int ratio;
    float x[100];
    /* Applying Gauss Jordan Elimination */
     for(int i=1;i<=n;i++)
     {
          if(a[i][i] == 0.0)
          {
               cout<<"Mathematical Error!" << endl;;
               exit(0);
          }
          for(int j=1;j<=n;j++)
          {
               if(i!=j)
               {
                    ratio = a[j][i]/a[i][i];
                    for(int k=1;k<=n+1;k++)
                    {
                        a[j][k] = a[j][k] - ratio*a[i][k];
                    }
               }
          }
     }
     /* Obtaining Solution */
     for(int i=1;i<=n;i++)
     {
        x[i] = a[i][n+1]/a[i][i];
     }

	 /* Displaying Solution */
	 cout<< endl<<"Solution: "<< endl;
	 for(int i=1;i<=n;i++)
	 {
	  	cout<<"x["<< i<<"] = "<< x[i]<< endl;
	 }
}

void printMatrix(vector<vector<float>> mat, int n, int m) {
    for(int i=0;i<n;i++)  // loop 3 times for three lines
    {
        for(int j=0;j<m;j++)  // loop for the three elements on the line
        {
            cout<<mat[i][j]<<" ";  // display the current element out of the array
        }
    cout<<endl;  // when the inner loop is done, go to a new line
    }
}