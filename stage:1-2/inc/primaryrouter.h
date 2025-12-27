#ifndef __Primary_Header__
#define __Primary_Header__

#include<string>
#include<fstream>
#include<stdlib.h>
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
#include<map>
#include<vector>
#include "base.h"


#define BUF_MAX_SIZE 2048

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
		std::map<std::string, int>secondaryrouterinfo;


	public:
		Primary_Router(std::string inputfile);
		std::string primaryrouterfilename = "";
		std::ofstream outfile_iterator;
		int udpsocketcreation();
		void tunnelreader();
		void logfilewrite(std::string inputfile);
		std::vector<std::string> cleanupstring(std::string input);

};

#endif
