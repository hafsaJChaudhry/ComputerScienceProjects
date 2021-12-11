// File: Grader.cpp
//
// CMSC 341 Spring 2019                                                                             
//                                                                                                  
// Method stubs for Grader class
//         

#include "Grader.h"
#include "BaselineGraph.h"
#include "Graph.h"

Grader::Grader(string name) {
  m_name = name;
}

bool Grader::testCSRExact(int numVert, vector<Edge> edgeSeq) {
  return true;
}

bool Grader::testCSRUnordered(int numVert, vector<Edge> edgeSeq) {
  return true;
}

bool Grader::testNbIterator(int numVert, vector<Edge> edgeSeq) {
  return true;
}

bool Grader::testEgIterator(int numVert, vector<Edge> edgeSeq) {
  return true;
}

bool Grader::testExceptions(int numVert, vector<Edge> edgeSeq) {
  return true;
}

void Grader::resetErrorQueue() {
}

// Formatted output of Error object:
// deduction: (id) description
ostream& operator<<(ostream& outStream, const Error& err) {
  if (err.m_deduction > 0) {
    outStream << "-" << err.m_deduction << ": (" << err.m_id
	      << ") " << err.m_description;
  } else {
    outStream << err.m_deduction << ": (" << err.m_id
	      << ") " << err.m_description;
  }
  return outStream;
}

