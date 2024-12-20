#include <iostream>
#include "BaselineGraph.h"

namespace Baseline {
  // Baseline::Graph implementations go here
  //friend Grader;
/***********************************************************************/
  // Graph constructor; must give number of vertices                           
/***********************************************************************/
  Baseline::Graph::Graph(int n){
    /*    if(n <= 0){
      throw out_of_range("out of range");
      }*/
    /*    m_nz = new int[n];
    m_re = new int[n+1];
    m_ci = new int[n];
    */
    
    for(int i = 0; i < n; i++){
      m_re.push_back(0);
      m_nz.push_back(0);
      m_ci.push_back(0);
    }
    m_re.push_back(0);

    
    //    m_cap = n;
    m_numVert = n;
    m_numEdge = 0;
  } 
/***********************************************************************/
  //return number of vertices
/***********************************************************************/
  int Baseline::Graph::numVert(){
    return m_numVert;
  }
/***********************************************************************/
  // return number of edges
/***********************************************************************/
  int Baseline::Graph::numEdge(){
  return m_numEdge/2;
  }
/***********************************************************************/
  // add edge between u and v with weight x                                    
/***********************************************************************/
  void Baseline::Graph::addEdge(int u, int v, int x){
    //out of range exception                                                   
    /*if(u < 0 || u > m_numVert){
      throw out_of_range("out of range");
    }
    if(v < 0 || v > m_numVert){
      throw out_of_range("out of range");
      }*/
    //start and end position                                                   
    int start = m_re[u];
    int end = m_re[u+1]; //m_nz.size();equal to NNZ                              
    //double size                                                              
    /*    if(m_cap - 2 < m_numEdge){
      //create new temp queue                                                    
      int *nzTemp = new int[m_cap*2];
      int *ciTemp = new int[m_cap*2];
      //loop through and copy data into temp queue                               
      for(int i = 0; i < m_cap*2; i++){
	if(i >= m_cap){
	  nzTemp[i] = 0;
	  ciTemp[i] = 0;
	}
	else{
	  nzTemp[i] = m_nz[i];
	  ciTemp[i] = m_ci[i];
	}
      }
      delete[] m_nz;
      delete[] m_ci;
      //create new arrays with twice the capacity                             
      m_nz = nzTemp;
      m_ci = ciTemp;
      m_cap = m_cap*2;
      }*/
    
    //add edge                                                                 
    //boolean check                                                            
    bool check = true;
    int count = start;
    while(count < end){
      //if the column in ci is greater than v, change start position           
      if(m_ci[count] > v and check == true){
        start = count;
        check = false;
      }
      count++;
    }
    if(check == true){
      start = m_re[u+1];
    }
    /*
    //loop backwards to shift                                                  
    for(int i = m_cap - 1 ; i > start; i --){
      //change nz and ci values by 1 index                                     
      m_nz[i] = m_nz[i-1];
      m_ci[i] = m_ci[i-1];
      }*/
    
    for(int i = u + 1; i < m_numVert +1; i++){
      m_re[i] = m_re[i] + 1;
    }

    /*    m_ci.insert(m_ci.cbegin()+i, v);
	  m_nz.insert(m_nz.cbegin()+i, x);*/
    m_ci.insert(m_ci.cbegin(), v);
    m_nz.insert(m_nz.cbegin(), x);
    //    m_ci[start] = v;
    //    m_nz[start] = x;
    m_numEdge++;
    
    //duplicate edges with same code but u and v swapped                        
    if(u != v){
      start = m_re[v];
      end = m_re[v + 1];
      for(int i = start; i < end; i ++){
        if(m_ci[i] > u and check == true){
          start = i;
          check = false;
        }
      }
      if(check == true){
        start = m_re[v+1];
      }
      /*
      //loop backwards to shift                                                
      for(int i = m_cap-1; i > start; i--){
        //change nz and ci values by 1 index                                   
        m_nz[i] = m_nz[i-1];
        m_ci[i] = m_ci[i-1];
	}*/
      for(int i = v+1; i < m_numVert+1; i++){
        m_re[i] = m_re[i]+1;
      }
      /*
      m_ci.insert(m_ci.cbegin()+i, u);
      m_nz.insert(m_nz.cbegin()+i, x);*/
      m_ci.insert(m_ci.cbegin(), u);
      m_nz.insert(m_nz.cbegin(), x);
      //      m_ci[start] = u;
      //      m_nz[start] = x;
      m_numEdge++;
    }
/***********************************************************************/
    // print out data structure for debugging                                  
/***********************************************************************/
    void Graph::dump(){
      cout << "Dump of graph (num Vert = " << numVert() << ", num Edge = " << numEd \
	ge() << ", m_cap = " << m_cap << ")" << endl;
      
      //print nz, re, and ci                                                        
      cout << "m_nz: " << endl ;
      for(int i = 0; i < m_cap; i++){
	if(m_nz[i] != 0){
	  cout << m_nz[i] << " ";
	}
      }
      
      cout << endl;
      cout << "m_re: " << endl;
      for(int i = 0; i < numVert()+1; i++){
	cout << m_re[i] << " ";
      }
      cout << endl;
      cout << "m_ci: " << endl;
      for(int i = 0; i < numEdge()*2; i++){
	cout << m_ci[i] << " ";
      }
      cout << endl;
    }
/********************************************************************/
//Edge Iterator class                                                          
/********************************************************************/
/********************************************************************/
  // Edge Iterator constructor; indx can be used to                           
  // set m_indx for begin and end iterators.                                   
/********************************************************************/
    Baseline::Graph::EgIterator::EgIterator(Graph *Gptr, int indx){
      m_Gptr = Gptr;
      m_indx = indx;
    }
/********************************************************************/
// Compare iterators; only makes sense to compare with                         
    // end iterator                                                            
/********************************************************************/
    bool Baseline::Graph::EgIterator::operator!= (const EgIterator& rhs){
      return rhs.m_indx != m_indx;
    }
/********************************************************************/
// Move iterator to next printable edge                                        
// post increment                                                              
/********************************************************************/
    void Baseline::Graph::EgIterator::operator++(int dummy){
      /*
      m_indx++;
      if(m_indx != m_Gptr->m_numEdge){
	while((m_row && m_Gptr->m_numEdge != m_indx) > m_Gptr->m_ci[m_indx]){
	  while(m_Gptr->m_re[m_row] == m_indx){
	    m_row++;
	  }
	  m_indx++;
	}
      }
      }*/
      // Already at end; just return
      if ( m_indx == m_Gptr->m_numEdge ) return;
      int numEdge = m_Gptr->m_numEdge; 
      do {
	m_indx++;
	while ( (m_indx < numEdge) && (m_indx >= m_Gptr->m_re[m_row + 1]) )
	  m_row++;
      } while ( (m_indx < numEdge) && (m_row > m_Gptr->m_ci[m_indx]) );
    }
  }
/********************************************************************/
    // return edge at iterator location                                                  
/********************************************************************/
  std::tuple<int,int,int>Baseline::Graph::EgIterator::operator*(){
    return std::tuple<int,int,int>(m_row, m_Gptr->m_ci[m_indx], m_Gptr->m_nz[m_indx]);
  }
  /********************************************************************/
  // Make an end iterator for edge iterator                                              
/********************************************************************/
  Baseline::Graph::EgIterator Graph::egEnd(){
    return Graph::EgIterator(this, m_re[m_numVert]);
  }
/********************************************************************/
  // Make an initial edge Iterator                                                      
/********************************************************************/
  Baseline::Graph::EgIterator Graph::egBegin(){
    return Graph::EgIterator(this, m_re[0]);
  }
/********************************************************************/
/********************************************************************/
// Neighbor Iterator class                                                               
/********************************************************************/
/********************************************************************/
    // Constructor for iterator for vertices adjacent to vertex v;                       
    // indx can be used to set m_indx for begin and end iterators                        
/********************************************************************/
  Baseline::Graph::NbIterator::NbIterator(Graph *Gptr, int v, int indx){
    m_Gptr = Gptr;
    m_row = v;
    m_indx = indx;
  }
/********************************************************************/
    // Compare iterators; only makes sense to compare with                               
    // end iterator                                                                      
/********************************************************************/
  bool Baseline::Graph::NbIterator::operator!=(const NbIterator& rhs){
    return rhs.m_indx != m_indx;
  }

/********************************************************************/
    // Move iterator to next neighbor                                                    
/********************************************************************/
  void Baseline::Graph::NbIterator::operator++(int dummy){
    m_indx++;
  }
/********************************************************************/
    // Return neighbor at current iterator position                                      
/********************************************************************/
  int Baseline::Graph::NbIterator::operator*(){
    return m_Gptr->m_ci[m_indx];
  }

/********************************************************************/
  // Make an end neighbor iterator                                                       
/********************************************************************/
  Baseline::Graph::NbIterator Graph::nbEnd(int v){
    return Graph::NbIterator(this, v, m_re[v+1]);
  }
/********************************************************************/
  // Make an initial neighbor iterator                                                   
/********************************************************************/
  Baseline::Graph::NbIterator Graph::nbBegin(int v){
    return Graph::NbIterator(this, v, m_re[v]);
  }
}
/********************************************************************/

