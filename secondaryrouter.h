#ifndef __Secondaryrouter_header__
#define __Secondaryrouter_header__

#include<string>
#include<fstream>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/select.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<fcntl.h>
#include<unistd.h>
#include "base.h"


#define BUF_MAX_SIZE 2048
using namespace std;

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
        Secondary(int index,string inputfile,int portnumber);
        Base base;
        string secondaryrouterfilename = "";
        void cancellationpoint(int value,string errormessage);
		void logfilewrite(string inputfile);
        void secondaryudpsetup();
        unsigned int checksum(char *addr, short count);
        ofstream outfile_iterator;

};

#endif 
