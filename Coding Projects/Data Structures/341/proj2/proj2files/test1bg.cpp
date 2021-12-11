// File: test1bg.cpp
//
// CMSC 341 Spring 2019
// Project 2 
//
// Basic testing of Baseline::Graph class
//

#include <iostream>
#include <tuple>

using namespace std ;

#include "BaselineGraph.h"

int main() {

  // G has 5 vertices numbered 0 thru 4
  Baseline::Graph G(5) ;

  // add some edges
  G.addEdge(3,4,11);
  G.addEdge(1,4,12);
  G.addEdge(0,3,13);
  G.addEdge(0,4,14);
  G.addEdge(0,1,15);
  G.addEdge(1,2,16);
  G.addEdge(2,4,17);

  // dump out data structure
  G.dump();

  // Test neighbor iterator
   
  Baseline::Graph::NbIterator nit ;

  cout << "\nThe neighbors of vertex 4 are:\n" ;
  for (nit = G.nbBegin(4); nit != G.nbEnd(4) ; nit++) {
    cout << *nit << " " ;
  }
  cout << endl ;

  // Test edge iterator

  Baseline::Graph::EgIterator eit ;
  std::tuple<int,int,int> edge ;

  cout << "\nThe edges in the graph are:\n" ;
  for (eit = G.egBegin() ; eit != G.egEnd() ; eit++) {

    edge = *eit ;   // get current edge

    cout << "(" << get<0>(edge) << ", "
	 << get<1>(edge) << ", "
	 << get<2>(edge) << ") " ;

  }
  cout << endl ;
}
