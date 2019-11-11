#ifndef __base_header_included_
#define __base_header_included_


#include<fstream>
#include<iostream>
#include<regex>
using namespace std;



class Base
{
    public:
    int stage = 0;
    int number_of_routers = 0;
    string s;
    void readinputfile(string input);
    void cancellationpoint(int value,string errormessage);
    unsigned int checksum(char *addr, short count);

};
#endif //__base_header_included__
