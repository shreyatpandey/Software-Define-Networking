#ifndef _ICMP_PACKETS_
#define _ICMP_PACKETS_

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
#include<time.h>
#include<sys/time.h>
#include "base.h"

class ICMP_PACKET : public Base
{
    public:
        int icmpsocket ;
        int icmpbindvalue ;
        ssize_t sendmsgresult ;
        struct sockaddr_in sock_addr_icmp;
        socklen_t icmpsocketlength;
        void icmpsocketcreation();
        void messageforward(uint32_t destaddr);


};

#endif 
