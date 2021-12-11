//hafsa chaudhry

#include "CTree.h"
#include <iostream>
#include <cstring>
#include <string>
#include <math.h>
#include <algorithms>
using namespace std;

int main(){
    CTree T;
    //INSERT
    cout << "\n\n***** Insert 20 *****\n" ;
    T.insert(20) ;
    T.inorder() ; cout << endl ;
    cout << "\n\n***** Insert 60 *****\n" ;
    T.insert(60) ;
    T.inorder() ; cout << endl ;
    cout << "\n\n***** Insert 30 *****\n" ;
    T.insert(30) ;
    T.inorder() ; cout << endl ;
    cout << "\n\n***** Insert 40 *****\n" ;
    T.insert(40) ;
    T.inorder() ; cout << endl ;
    cout << "\n\n***** Insert 90 *****\n" ;
    T.insert(90) ;
    T.inorder() ; cout << endl ;
    cout << "\n\n***** Insert 100 *****\n" ;
    T.insert(100) ;
    T.inorder() ; cout << endl ;

    //REMOVE
    cout << "removing ..." << endl;
    n = 20 ; cout << "removing " << n << endl ; T.remove(n) ; T.inorder() ; cout << endl ;
    n = 60 ; cout << "removing " << n << endl ; T.remove(n) ; T.inorder() ; cout << endl ;
    n = 30 ; cout << "removing " << n << endl ; T.remove(n) ; T.inorder() ; cout << endl ;
    n = 40 ; cout << "removing " << n << endl ; T.remove(n) ; T.inorder() ; cout << endl ;
    n = 90 ; cout << "removing " << n << endl ; T.remove(n) ; T.inorder() ; cout << endl ;
    n = 100 ; cout << "removing " << n << endl ; T.remove(n) ; T.inorder() ; cout << endl ;

    //FIND
    int n;
    n = 40  ;
    cout << "Find " << n << endl ;
    if ( T.find(n) ) {
        cout << "found = " << n << endl ;
    } else {
        cout << "did not find = " << n << endl ;
    }
    cout << endl ;

    //COPY
    cout << "Original T1:         " ;
    T1.inorder() ; cout << endl ;

    // Use copy constructor
    CTree *Tptr = new CTree(T1) ;

    cout << "Copied T1:           " ;
    Tptr->inorder() ; cout << endl;

    CTree T2 ;

    T2.insert(50) ;
    T2.insert(25) ; T2.insert(75) ;
    // T2.inorder() ; cout << endl ;

    T2 = *Tptr ;
    cout << "Second copy:         " ;
    T2.inorder() ; cout << endl ;

    cout << "Delete first copy...\n" ;
    delete Tptr ;

    cout << "Recheck original:    " ;
    T1.inorder() ; cout << endl ;
    cout << "Recheck second copy: " ;
    T2.inorder() ; cout << endl ;

    //LOCATE
    int key = 9999 ;
    bool ans ;
    const char *str ;

    ans = T.locate(str="", key=-1) ;
    cout << str << ": " << ans << " " << key << endl ;

    ans = T.locate(str="LL", key=-1) ;
    cout << str << ": " << ans << " " << key << endl ;

    ans = T.locate(str="LLR", key=-1) ;
    cout << str << ": " << ans << " " << key << endl ;

    ans = T.locate(str="RLLR", key=-1) ;
    cout << str << ": " << ans << " " << key << endl ;


    return 0;
}

