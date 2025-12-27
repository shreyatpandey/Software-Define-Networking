#ifndef __Secondaryrouter_header__
#define __Secondaryrouter_header__

#include<string>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/select.h>
#include<netdb.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<fcntl.h>
#include<unistd.h>
#include "base.h"


#define BUF_MAX_SIZE 2048

class Secondary:public Base
{
	private:
		int socketsecondary ;
		socklen_t lengthudpsocketsecondary ;
		uint16_t secondaryportnumber ;
		int secondarypidvalue ;
		int primaryportnumber;
		char buffer[BUF_MAX_SIZE];
		int routerindex;
	public:
		Secondary(int index, std::string inputfile, int portnumber);
		std::string secondaryrouterfilename = "";
		void secondaryudpsetup();
		void logfilewrite(std::string info);
		std::ofstream outfile_iterator;

};

#endif
