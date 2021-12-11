// File: Grader.h
//
// CMSC 341 Spring 2019
//
// Header file for Grader class
//

#ifndef _GRADER_H
#define _GRADER_H

#include <iostream>
#include <stdexcept>  // for throwing out_of_range exceptions
#include <tuple>      // Edges are respresented as tuple<int,int,int>
#include <queue>      // Errors and decuctions are saved in a queue
#include <vector>     // Vectors are used to store Edge sequences and
		      // may be used in test implementations
#include <set>        // Sets may be used in test implementations

using namespace std;

typedef tuple<int,int,int> Edge;   // Edge type definition

// Error - a small class to organize error messages and deductions.
// Defined in-line.
class Error {
 public:

  // Constructor.  deductions must be >= 0
  Error(int deduct = 0, int id = 0, string description = "") {
    if (deduct < 0) {
      throw std::out_of_range("deduction for error must be non-negative");
    }
    m_deduction = deduct;
    m_id = id;
    m_description = description;
  }

  // deduction getter
  int getDeduction() { return m_deduction; }

  // id getter
  int getId() { return m_id; }
  
  // description getter
  string getDescription() { return m_description; }

  // formatted output of Errors
  friend ostream& operator<<(ostream& outStream, const Error& err);

private:
  int m_deduction;            // points to deduct
  int m_id;                   // id number of error
  string m_description;       // description of error
};

class Grader {

 public:

  // Constructor; may provide optional descriptive name
  Grader(string name = "");

  // In the following test functions, the parameters are:
  //
  //   int numVert: number of vertices in test graph
  //
  //   vector<Edge> edgeSeq: a sequence of edges to insert in the
  //   graph
  
  // Test for *exact* correctness of m_re, m_nz, and m_ci.  Assume
  // that m_re is initialized to all zeroes
  bool testCSRExact(int numVert, vector<Edge> edgeSeq);

  // Test for *unordered* correctness of m_re, m_nz, and m_ci.  An
  // implementation passes if (1) the values in m_re are correct, and
  // (2) the values in m_nz and m_ci for each row are correct, but not
  // necessarily ordered by increasing m_ci values.
  bool testCSRUnordered(int numVert, vector<Edge> edgeSeq);

  // Test that for *every* vertex in the Graph, NbIterator produces
  // the correct list of neighbor vertices.
  bool testNbIterator(int numVert, vector<Edge> edgeSeq);

  // Test that the edge iterator returns the correct list of edges.
  // Each edge should occur exactly once even though it is represented
  // twice in the data structure; that is, only one of (u, v, w) and
  // (v, u, w) should be included in the list produced by the
  // iterator.
  bool testEgIterator(int numVert, vector<Edge> edgeSeq);

  // Test that operations that are supposed to throw an out_of_range
  // exception do so.
  bool testExceptions(int numVert, vector<Edge> edgeSeq);

  // Print all the deductions and errors in the error queue and total
  // deductions.  The queue should be empty after the call.
  void printAllErrors();

  // Clear the error queue
  void resetErrorQueue();

 private:
  string m_name;                 // Optional descriptive name
  std::queue<Error> m_errors;    // Queue to hold errors and
				 // deductions
  
  // ******************************************************//
  // You may add private helper functions here.  They must //
  // be declared inside the private section of the class.  // 
  //*******************************************************//

};

#endif
