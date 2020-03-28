#include "icmpacket.h"
void ICMP_PACKET::icmpsocketcreation()
{
    icmpsocket = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
    cancellationpoint(icmpsocket,"Error in creating icmp raw socket");
	
    sock_addr_icmp.sin_family = AF_INET;
    sock_addr_icmp.sin_addr.s_addr = inet_addr("192.168.201.2");/*wht will go here = in_addr_t source_icmp*/
    icmpsocketlength = sizeof(sock_addr_icmp);
    icmpbindvalue = bind(icmpsocket, (struct sockaddr *) &sock_addr_icmp,icmpsocketlength);
    cancellationpoint(icmpbindvalue,"Error in binding icmpsocket");

	
}
void ICMP_PACKET::messageforward(uint32_t destaddr)
{
    
    sock_addr_icmp.sin_addr.s_addr = destaddr;

    struct Packet {
			struct icmphdr icmp;
			struct timeval tv;
    			} p;

    struct iovec iov;
    struct msghdr m = { &sock_addr_icmp, sizeof(struct sockaddr_in), &iov,1,0,0,0};

    p.icmp.type = ICMP_ECHO;
    p.icmp.code = 0;
    p.icmp.un.echo.id = 0;
 
    iov.iov_base = &p ;
    iov.iov_len = sizeof(struct icmphdr) + sizeof(timeval);
    
 

    p.icmp.checksum = 0;
    p.icmp.un.echo.sequence = htons (1);
    gettimeofday(&p.tv, NULL);
 
 	unsigned short check = sendmsgchecksum((uint16_t*)&p, iov.iov_len); 
    p.icmp.checksum = check;
    
    sendmsgresult = sendmsg(icmpsocket, &m, 0);
    cancellationpoint(sendmsgresult,"Error in sendmsg");

    
}
