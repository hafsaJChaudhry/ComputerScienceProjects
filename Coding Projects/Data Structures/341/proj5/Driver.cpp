//project 5 Driver.cpp
//Hafsa Chaudhry

#include "Anahash.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {

    Anahash table( 1 << 4 );  // Table of size 2^3
    table.insert("aliens");
    table.insert("saline");
    table.insert("alines");
    table.insert("san lie");
    table.insert("sale in");
    table.insert("resting");
    table.insert("stinger");
    table.insert("gent sir");
    table.insert("set ring");
    table.insert("auctioned");
    table.insert("cautioned");
    table.insert("education");
    table.insert("a telescope");
    table.insert("escapee lot");
    table.insert("celesta ope");
    table.insert("ace sleep to");
    table.insert("to see place");
    table.insert("the eyes");
    table.insert("they see");
    table.insert("waitress");
    table.insert("a stew sir");
    table.insert("i run to escape");
    table.insert("a persecution");
    table.insert("a gentleman");
    table.insert("elegant man");

    cout << "aliens: " << endl;
    auto ana = table.getAnagrams("aliens");
    for (auto itr = ana.begin(); itr != ana.end(); itr++)
        cout << "   " << *itr << endl;
    cout << endl;

    // Dump the contents of the hash table.
    cout << "Full dump of table:\n";
    table.dump();

    return 0;
}


