#ifndef PARSER_H
#define PARSER_H

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
#include <vector>
#include <cstdlib>

#define MAXFLDS 200 /* maximum possible number of fields */
#define MAXFLDSIZE 32 /* longest possible field + 1 = 31 byte field */

class Parser
{

public:
    Parser();
    void printrecords(char* filename);
    void printProperties();
   std::vector<double> ReadCoordinates(char* filename);
    bool cleanup();
    bool writeRecord(int userID, char* bioID, char* filename);
    bool writeCoordinates(float coordinates[], int size, char* filename);
    std::vector<double> coordinates;    //Vector of x, y double cooordinates 
    int num_coord;      //Number of coordinates
    
    
    
    
    
    
    
    
    
    
    
    

private:
    bool trimWhiteSpace;
    char fieldDelim;
    char recordDelim;
    char quote;
    

};

#endif // PARSER_H
