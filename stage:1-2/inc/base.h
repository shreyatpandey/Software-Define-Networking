#ifndef __base_header_included_
#define __base_header_included_


#include<fstream>
#include<iostream>
#include<regex>
#include<string>

class Base
{
	public:
	int stage = 0;
	int number_of_routers = 0;
	std::string s;
	void readinputfile(std::string input);
	void cancellationpoint(int value, std::string errormessage);
	unsigned int checksum(char *addr, short count);
};
#endif //__base_header_included__
