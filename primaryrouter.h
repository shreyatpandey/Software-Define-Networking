#ifndef __Primary_Header__
#define __Primary_Header__

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
#include<map>
#include "base.h"


#define BUF_MAX_SIZE 2048
using namespace std;



class Primary_Router:public Base
{
    private:
		int socket_primary_udp = 0;
		struct sockaddr_in primaryaddr;
		socklen_t length_udp_socket;
		unsigned int port_number;
		char buffertunnel[BUF_MAX_SIZE];
		char shutdownmessage[BUF_MAX_SIZE];
		struct timeval tv;
		struct iphdr* ip_pac = (struct iphdr*)buffertunnel;
		struct icmphdr* icmp_pac = (struct icmphdr *)(ip_pac + 1);
		map<string,int>secondaryrouterinfo;
		map<string,int>::iterator it;

	public:
		Primary_Router(string inputfile);
		Base base;
		string primaryrouterfilename = "";
		ofstream outfile_iterator;
        int udpsocketcreation();
        void tunnelreader();
		void cancellationpoint(int value,string errormessage);
		void logfilewrite(string inputfile);
		vector<string> cleanupstring(string input);
		unsigned int checksum(char *addr, short count);
        
};

#endif
