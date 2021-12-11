// File: driver.cpp
//
// CMSC 341 Spring 2019
//
// Sample driver for Grader class
//

#include <tuple>
#include <thread>
#include "Grader.h"
#include "Graph.h"

using namespace std ;

int main() {

  Grader grader("test");

  int numVert = 5;

  vector<Edge> edgeSeq;
  
  edgeSeq.push_back(Edge(3,4,11));
  edgeSeq.push_back(Edge(1,4,12));
  edgeSeq.push_back(Edge(0,3,13));
  edgeSeq.push_back(Edge(0,4,14));
  edgeSeq.push_back(Edge(0,1,15));
  edgeSeq.push_back(Edge(1,2,16));
  edgeSeq.push_back(Edge(2,4,17));

  //  edgeSeq.push_back(Edge(0,1,15)); // a duplicate edge

  grader.testCSRExact(numVert, edgeSeq);
  grader.testCSRUnordered(numVert, edgeSeq);
  grader.testNbIterator(numVert, edgeSeq);
  grader.testEgIterator(numVert, edgeSeq);
  grader.testExceptions(numVert, edgeSeq);
  
  cout << "\n--------------- Errors and Deductions ---------------\n\n";
  
  grader.printAllErrors();

  cout << endl;
  
  return 0;
}
