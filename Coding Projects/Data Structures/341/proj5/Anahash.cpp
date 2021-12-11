// CMSC 341, Spring 2019
// Project 5: Finding anagrams by hashing
//hafsa chaudhry

#include "Anahash.h"
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

/************************************************************************
  // Create a Anahash object with hash table of size 'htSize'.  If
  // 'htSize' is greater than HT_SIZE_LIMIT or less than 1, throw a
  // range_error exception.
  ************************************************************************/
Anahash::Anahash( int htSize ){
    unsigned int size = htSize;
    if(size > HT_SIZE_LIMIT){
        std::range_error(" ");
    }
    //create an empty string set
    m_htSize = htSize;
    m_ht = new set<string>[m_htSize] {};
}

/************************************************************************
  // Copy constructor
  ************************************************************************/
Anahash::Anahash( const Anahash& rhs ){
    m_htSize = rhs.m_htSize;

    //new anahash object with hash table of size 'htSize'
    m_ht = new set<string>[m_htSize];

    int count = 0;
    while(count < rhs.m_htSize){
        m_ht[count] = rhs.m_ht[count];
        count++;
    }
}

/************************************************************************
  // Assignment operator
  ************************************************************************/
Anahash& Anahash::operator=( const Anahash& rhs ){
    if(this != &rhs){
        //destructor essentially
        delete [] m_ht;
        m_htSize = rhs.m_htSize;

        //new anahash object with hash table of size 'htSize'
        m_ht = new set<string>[m_htSize];

        int count = 0;
        while(count < rhs.m_htSize){
            m_ht[count] = rhs.m_ht[count];
            count++;
        }
    }
    return *this;
}

/************************************************************************
  // Destructor
  ************************************************************************/
Anahash::~Anahash(){
    delete [] m_ht;
}

/************************************************************************
  // Insert 'line' into hash table.  Return true if a new entry is
  // created, false if the line is already in the table.
  ************************************************************************/
bool Anahash::insert( string line ){
    //check if line is already in the hash table
    if(search(line) != false)
        return false;

    int lineIndex = hashFunction(line) % m_htSize;

    //if the position is empty, insert
    if(m_ht[lineIndex].empty() != false){
        m_ht[lineIndex].insert(line);
        return true;
    }while(!m_ht[lineIndex].empty()){
        //else linear probing if == false
        if(getAnagram(line, lineIndex) == true){
            m_ht[lineIndex].insert(line);
            return true;
        }lineIndex++;
        lineIndex = lineIndex % m_htSize;
    }if(m_ht[lineIndex].empty() != false){
        //if you find an empty spot
        lineIndex = lineIndex % m_htSize;
        m_ht[lineIndex].insert(line);
    }return true;

}

/************************************************************************
  // Search for 'line' in the hash table.  Return true if found, false
  // otherwise.
  ************************************************************************/
bool Anahash::search( string line ){
    int count = 0;
    while(m_htSize > count){
        set<string>::iterator wordSet;
        wordSet = m_ht[count].begin();

        while(wordSet != m_ht[count].end()){
            //if "word" == line
            if(*wordSet == line){
                return true;
            }
            wordSet++;
        }
        count++;
    }
    return false;
}

/************************************************************************
  // If 'line' is in the the hash table, return its set of anagrams,
  // including 'line'.  If 'line' is not in the hash table, return an
  // emtpy set of strings.
  ************************************************************************/
set<string> Anahash::getAnagrams( string line ){
    //line is IN hash table
    if(search(line) != false){
        int lineIndex = hashFunction(line) % m_htSize;
        //linear probing
        while(!getAnagram(line, lineIndex)){
            lineIndex = (lineIndex + 1) % m_htSize;
        }
        return m_ht[lineIndex];
    }
    //line is NOT IN hash table
    //....return empty string set
    set<string> emptySet {};
    return emptySet;
}


/************************
  //private: checks line at an index. helper function to getAnagram. sort in order of Alphabet and made uppercase using toupper()
  ***********************/
bool Anahash::getAnagram(string line, int index){

    //sorted first string in the set at the index of the array
    string sString = "";
    //sorted version of the line entered in the function
    string sLine = "";

    string wordSet = *m_ht[index].begin();
    for(string::iterator iter = wordSet.begin(); iter != wordSet.end(); iter++){
        *iter = toupper(*iter);
        if(*iter != ' '){
            sString = sString + *iter;
        }
    }

    for(string::iterator iter = line.begin(); iter != line.end(); iter++){
        *iter = toupper(*iter);
        if(*iter != ' '){
            sLine = sLine + *iter;
        }
    }
    sort(sString.begin(),sString.end());
    sort(sLine.begin(),sLine.end());

    return(sString == sLine);
}
/************************
  //private: hash function to make hash values into strings
  //collision rate == #of sets NOT where its hash to/ total number of none empty sets in the table
  //try to get a low collision rate
  ***********************/
unsigned int Anahash::hashFunction(string line){

    int totalNum = 0;
    int wordLength = line.length();
    char charTemp[wordLength];

    int count = 0;
    while(count < wordLength){
        charTemp[count] = line[count];

        //convert ASCII to string
        if(122 <= charTemp[count] and 97 <= charTemp[count]){
            //add current ASCII values to char array
            charTemp[count] = charTemp[count] -32;
        }
        //add current ASCII values to totalNum
        totalNum = totalNum + charTemp[count];
        count++;
    }

    int hash = ((9453*totalNum + 9453)%9453);
    return hash;
}
/************************************************************************
  // Dump all non-empty lists from the hash table.  Print the hash
  // value in hexadecimal and the strings in the list.
  ************************************************************************/
void Anahash::dump(){
    int count = 0;
    while(count<m_htSize){

        if(!m_ht[count].empty()){
            //print out the hexadecimal number
            cout << hex << count << endl;

            //create iterator set of strings
            set<string>::iterator wordSet;
            wordSet = m_ht[count].begin();

            while(wordSet != m_ht[count].end()){
                //print out the strings in the list
                cout << " " << *wordSet << endl;
                wordSet++;
            }
        }count++;
    }
}