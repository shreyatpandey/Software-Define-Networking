#include "primaryrouter.h"
#include "tunnel.h"

Primary_Router::Primary_Router(string inputfile):Base()
{
	readinputfile(inputfile);
	socket_primary_udp = 0;
	length_udp_socket = 0;
	primaryrouterfilename = "stage"+to_string(stage)+".r0.out";
}

void Primary_Router::logfilewrite(string info)
{
	outfile_iterator.open(primaryrouterfilename,ios_base::app);
	outfile_iterator<<info<<endl;
	outfile_iterator.close();

}

int Primary_Router::udpsocketcreation()
{
    socket_primary_udp = socket(AF_INET,SOCK_DGRAM,0);
    cancellationpoint(socket_primary_udp,"Error in creating primary udp socket");

    bzero((char *)&primaryaddr,sizeof(primaryaddr));
	memset(&primaryaddr,0,sizeof(struct sockaddr_in)) ;
    primaryaddr.sin_family = AF_INET;
    primaryaddr.sin_port = htons(0);

    int bind_value = bind(socket_primary_udp, (struct sockaddr *) &primaryaddr, sizeof(struct sockaddr_in));
    cancellationpoint(bind_value,"Error in binding the primary socket");

    	length_udp_socket  = sizeof(struct sockaddr*);
	getsockname(socket_primary_udp,(struct sockaddr*)&primaryaddr,&length_udp_socket);
	port_number = ntohs(primaryaddr.sin_port);
	primaryaddr.sin_port = htons(port_number);
	//cout<<"void Primary_Router::udpsocketcreation() :"<<port_number<<endl;
	logfilewrite("primary port: "+to_string(port_number));
	return port_number;

}

void Primary_Router::tunnelreader()
{
    char tun_name[IFNAMSIZ];
 
    /* Connect to the tunnel interface (make sure you create the tunnel interface first) */
    strcpy(tun_name, "tun1");
    int tun_fd = tun_alloc(tun_name, IFF_TUN | IFF_NO_PI); 
    cancellationpoint(tun_fd,"Error in creating tunnel");
    
    
    while(1)
    {
    	fd_set fs;	
	tv.tv_sec = 15;
    	tv.tv_usec = 0;
    
		FD_ZERO(&fs);
		FD_SET(tun_fd, &fs);
		FD_SET(socket_primary_udp,&fs);
		int select_retval = select(tun_fd>socket_primary_udp?tun_fd+1:socket_primary_udp+1, &fs, NULL, NULL,&tv);
		cancellationpoint(select_retval,"Error in select statement");
		
		/*send a message to secondary router to close down*/
		 if (select_retval == 0)
		{
			memset(shutdownmessage,0,BUF_MAX_SIZE);
			snprintf(shutdownmessage,BUF_MAX_SIZE,"Close");
			for(auto &x: secondaryrouterinfo)
			{
				primaryaddr.sin_port = htons(x.second);
				int closedownvalue = sendto(socket_primary_udp,shutdownmessage,BUF_MAX_SIZE,0,(struct sockaddr*)&primaryaddr,length_udp_socket);
				cancellationpoint(closedownvalue,"Error in sending close down message from primary udp to secondary udp ");
			}
			
			logfilewrite("router 0 closed");
			cout<<"closing primary"<<endl;
			exit(0);
		}

		else
		{
		if(FD_ISSET(tun_fd,&fs))
		{
			memset(buffertunnel,0,BUF_MAX_SIZE);
			int nread = read(tun_fd,buffertunnel,sizeof(buffertunnel));
			cancellationpoint(nread,"Error in reading from tunnel");
			if(nread > 0)
			{	
				
				if(icmp_pac->type== ICMP_ECHO)
				{
					struct sockaddr_in source, dest;
					struct iphdr* ip_udptu = (struct iphdr*)buffertunnel;
					struct icmphdr* icmp_udptu = (struct icmphdr *)(ip_udptu+1);

					source.sin_addr.s_addr = ip_udptu->saddr;
					dest.sin_addr.s_addr = ip_udptu->daddr;
					string source_addr = inet_ntoa(source.sin_addr);
					string dest_addr = inet_ntoa(dest.sin_addr);
					string filewrite = "ICMP from tunnel, src: " +source_addr + ",  dest:" + dest_addr +", type: "+to_string((int)(icmp_udptu->type));
					logfilewrite(filewrite);

				int send_to_secondary =  sendto(socket_primary_udp,buffertunnel,sizeof(buffertunnel),0,(struct sockaddr*)&primaryaddr,length_udp_socket);
				cancellationpoint(send_to_secondary,"Error in sending from primary udp to secondary udp ");

				}
			}
		}
		 

		
		if(FD_ISSET(socket_primary_udp,&fs))
		{
			
			char buf_udptu[BUF_MAX_SIZE];
			int value_read_udp = recvfrom(socket_primary_udp,buf_udptu,sizeof(buf_udptu),0,(struct sockaddr*)&primaryaddr,&length_udp_socket);
			cancellationpoint(value_read_udp,"Error in reading from primary udp socket");
			int incomingsourceport = ntohs(primaryaddr.sin_port);
			struct sockaddr_in source, dest;
			struct iphdr* ip_udptu = (struct iphdr*)buf_udptu;
			struct icmphdr* icmph_udptu = (struct icmphdr *)(ip_udptu + 1);
			//cout<<"protocol_value:"<<(int)ip_udptu->protocol<<endl;
			switch(ip_udptu->protocol)
			{
				case 1:
					{
					
					icmph_udptu->checksum = 0;
					unsigned short check_here = checksum((char *)buf_udptu,sizeof(buf_udptu));
					icmph_udptu->checksum = check_here;
					icmph_udptu->type = (unsigned int)0;
					


					if(icmph_udptu->type == ICMP_ECHOREPLY)
				{
					source.sin_addr.s_addr = ip_udptu->saddr;
					dest.sin_addr.s_addr = ip_udptu->daddr;
					string source_port = to_string(incomingsourceport);
					string source_addr = inet_ntoa(source.sin_addr);
					string dest_addr = inet_ntoa(dest.sin_addr);
					string filewrite = "ICMP from port: "+ source_port +", src: " +source_addr + ",  dest:" + dest_addr +
										", type: "+to_string((int)(icmph_udptu->type));
					logfilewrite(filewrite);
					int tun_write = write(tun_fd,buf_udptu,2048);
					cancellationpoint(tun_write,"error in writing to tunnel");
				}
					}
					break;
			
					/*write secondary router information into primaryrouter log*/
				default:
					{
					//printf("buf_udptu:%s\n",buf_udptu);
					string secondaryinfo = string(&buf_udptu[0],strlen(buf_udptu));
					vector<string>primarylogwritesecondaryinfo= cleanupstring(secondaryinfo);
					string secondaryrouterinformation = "router: "+ primarylogwritesecondaryinfo[0] +", pid: "+ primarylogwritesecondaryinfo[1] 
														+", port:"+ primarylogwritesecondaryinfo[2];
					
					secondaryrouterinfo[primarylogwritesecondaryinfo[0]] = incomingsourceport;
					logfilewrite(secondaryrouterinformation);

					memset(buf_udptu,0,BUF_MAX_SIZE);
					}
					break;
					
			}

		}
		}
	}
}

vector<string>Primary_Router::cleanupstring(string input)
{
	vector<string>secondaryinfo;
	string delimiter = ":";
	size_t position = 0;
    	string token = "";
	while((position = input.find(delimiter))!= string::npos)
	  {
	    token = input.substr(0,position);
	    secondaryinfo.push_back(token);
	    input.erase(0,position + delimiter.length());
		}
	secondaryinfo.push_back(input);
	return secondaryinfo;

}




