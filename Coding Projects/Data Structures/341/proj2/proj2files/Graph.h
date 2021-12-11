// File: Graph.h
//
// CMSC 341 Spring 2019
//
// Header file for Graph class
//

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdexcept>  // for throwing out_of_range exceptions
#include <tuple>      // for tuple template

class Grader;  // Forward declaration needed for 'friend'

class Graph {

public:

  friend Grader;  // Allows Grader class to access private variables

  // Graph constructor; must give number of vertices
  Graph(int n);

  // Graph copy constructor
  Graph(const Graph& G);

  // Graph destructor
  ~Graph();

  // Graph assignment operator
  const Graph& operator= (const Graph& rhs);

  // return number of vertices
  int numVert();

  // return number of edges
  int numEdge();

  // add edge between u and v with weight x
  void addEdge(int u, int v, int x);

  // print out data structure for debugging
  void dump();

  // Edge Iterator inner class
  class EgIterator {

  public: 
    // Edge Iterator constructor; indx can be used to
    // set m_indx for begin and end iterators.
    EgIterator(Graph *Gptr = nullptr, int indx = 0);

    // Compare iterators; only makes sense to compare with
    // end iterator
    bool operator!= (const EgIterator& rhs);
	 
    // Move iterator to next printable edge
    void operator++(int dummy);   // post increment

    // return edge at iterator location
    std::tuple<int,int,int> operator*();

  private:
    Graph *m_Gptr;    // pointer to associated Graph
    int m_indx;       // index of current edge in m_nz
    int m_row;        // corresponding row of m_nz[m_indx]
  };

  // Make an initial edge Iterator
  EgIterator egBegin();

  // Make an end iterator for edge iterator
  EgIterator egEnd();

  // Neighbor Iterator inner class
  class NbIterator {
    
  public: 
    // Constructor for iterator for vertices adjacent to vertex v;
    // indx can be used to set m_indx for begin and end iterators
    NbIterator(Graph *Gptr = nullptr, int v = 0, int indx = 0);

    // Compare iterators; only makes sense to compare with
    // end iterator
    bool operator!=(const NbIterator& rhs);

    // Move iterator to next neighbor
    void operator++(int dummy);

    // Return neighbor at current iterator position
    int operator*();

  private:
    Graph *m_Gptr;  // pointer to the associated Graph
    int m_row;      // row (source) for which to find neighbors
    int m_indx;     // current index into m_nz of Graph
  };

  // Make an initial neighbor iterator
  NbIterator nbBegin(int v);

  // Make an end neighbor iterator
  NbIterator nbEnd(int v);

private:

  int *m_nz;  // non-zero elements array
  int *m_re;  // row extent array
  int *m_ci;  // column index array
  int m_cap;  // capacity of m_nz and m_ci

  int m_numVert;  // number of vertices
  int m_numEdge;  // ptr to number of edges

};

#endif
