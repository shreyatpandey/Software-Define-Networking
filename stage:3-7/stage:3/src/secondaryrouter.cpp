#include "secondaryrouter.h"

Secondary::Secondary(int index,string inputfile,int portnumber): Base()  /* this is very important */
{
	readinputfile(inputfile);
	this->primaryportnumber = portnumber;
	routerindex = index;
	secondaryrouterfilename = "stage"+to_string(stage)+".r"+to_string(routerindex)+".out";
}
void Secondary::logfilewrite(string info)
{
	outfile_iterator.open(secondaryrouterfilename,ios_base::app);
	outfile_iterator<<info<<endl;
	outfile_iterator.close();

}

void Secondary::secondaryudpsetup()
{
	//cout<<"Secondary::secondaryudpsetup()"<<endl;
	socketsecondary= socket(AF_INET,SOCK_DGRAM,0);
	cancellationpoint(socketsecondary,"Error in creating secondary socket");

	memset(&secondary_socket,0,sizeof(struct sockaddr_in));
    secondary_socket.sin_family = AF_INET;
    secondary_socket.sin_port = htons(0);
    

    int bindsecondary = bind(socketsecondary, (struct sockaddr *) &secondary_socket, sizeof(struct sockaddr_in));
	cancellationpoint(bindsecondary,"Error in binding secondary socket");
   
	lengthudpsocketsecondary  = sizeof(struct sockaddr);
	getsockname(socketsecondary,(struct sockaddr*)&secondary_socket,&lengthudpsocketsecondary);
	secondaryportnumber = (int)ntohs(secondary_socket.sin_port);
	secondary_socket.sin_port = htons(primaryportnumber); 

	//cout<<"Secondary::secondaryudpsetup() secondaryportnumber:"<<secondaryportnumber<<endl;
	
	secondarypidvalue = getpid();
	//cout<<"Secondary::secondaryudpsetup() secondarypidvalue: "<<secondarypidvalue<<endl;

	/*Sending the routeriindex,pid value,portnumber of secondary router */
	string secondaryrouterinformation = "router: "+to_string(routerindex)+", pid: "+to_string(secondarypidvalue)+", port: "+to_string(secondaryportnumber);
	logfilewrite(secondaryrouterinformation);
	memset(buffer,0,BUF_MAX_SIZE);
	sprintf(buffer,"%d",routerindex);
	sprintf(buffer+strlen(buffer),":");
	sprintf(buffer+strlen(buffer),"%d",secondarypidvalue);
	sprintf(buffer+strlen(buffer),":");
	sprintf(buffer+strlen(buffer),"%d",secondaryportnumber);

	int secondaryinfosend = sendto(socketsecondary,buffer,sizeof(buffer),0,(struct sockaddr*)&secondary_socket,lengthudpsocketsecondary);
	cancellationpoint(secondaryinfosend,"Error in sending secondary information");

	secondarymessageforwarding();
	
}
void Secondary::secondarymessageforwarding()
{
	/*ICMP-Socket Call*/
	icmpobject.icmpsocketcreation();


	while(1)
	{
		fd_set fs;
		FD_ZERO(&fs);
		FD_SET(socketsecondary,&fs);
		FD_SET(icmpobject.icmpsocket,&fs);
		
		int select_retval = select(socketsecondary>icmpobject.icmpsocket?socketsecondary:icmpobject.icmpsocket+1, &fs, NULL, NULL,NULL);
		cancellationpoint(select_retval,"Error in secondary socket retval");

		if(FD_ISSET(socketsecondary,&fs))
		{
		
			memset(buffer,0,BUF_MAX_SIZE);
			int value_read_udp = recvfrom(socketsecondary,buffer,2048,0,(struct sockaddr*)&secondary_socket,&lengthudpsocketsecondary);
			cancellationpoint(value_read_udp,"Error in reading from secondary udp");
			/*check for close down message and close*/
			if(buffer[0] == 'C')
			{
				cout<<"closing secondary router :" <<routerindex << endl;
				logfilewrite("router " + to_string(routerindex) +" is closed");
				exit(0);
			}

			struct iphdr* ip_udptu = (struct iphdr*)buffer;
			struct icmphdr* icmp_udptu = (struct icmphdr *)(ip_udptu+1);
			struct sockaddr_in src_ip_udptu, dest_ip_udptu;
			cout<<"ip_udptu->protocol:"<<(int)ip_udptu->protocol<<endl;
			switch(ip_udptu->protocol)
			{
				case 1:
					{
						src_ip_udptu.sin_addr.s_addr = ip_udptu->saddr;
						dest_ip_udptu.sin_addr.s_addr = ip_udptu->daddr;

						string source_address_udptu = inet_ntoa(src_ip_udptu.sin_addr);
						string dest_address_udptu = inet_ntoa(dest_ip_udptu.sin_addr);

						/*write tunnelinfo to log*/
						string tunnelinfo = "ICMP from port: " + to_string(primaryportnumber) + ", src: " + source_address_udptu + ", dest: " + dest_address_udptu+
											", type: " + to_string((int)icmp_udptu->type); 
						
						logfilewrite(tunnelinfo);

						/*sending to icmp*/
						icmpobject.messageforward(dest_ip_udptu.sin_addr.s_addr);

						/*Sending to Primary*/
						char client_updtu[BUF_MAX_SIZE];
						memcpy(client_updtu,buffer,BUF_MAX_SIZE); 

						struct iphdr* ip_client_udptu = (struct iphdr*)client_updtu;
						struct icmphdr* icmp_client_udptu = (struct icmphdr *)(ip_client_udptu+1);

						/*This is main for reversal */
						ip_client_udptu->daddr = ip_udptu->saddr;
						ip_client_udptu->saddr = ip_udptu->daddr;


						icmp_client_udptu->checksum = 0;
						unsigned short check_here = checksum((char *)client_updtu,sizeof(struct icmphdr));
						icmp_client_udptu->checksum = check_here;
						icmp_client_udptu->type = (unsigned int)0;

						ip_client_udptu->check = 0;
						unsigned short check_observe = checksum((char *)client_updtu,sizeof(struct iphdr));
						ip_client_udptu->check = check_observe;


						if(icmp_client_udptu->type == ICMP_ECHOREPLY)
						{
							int udp_to_tunnel = sendto(socketsecondary,client_updtu,2048,0,(struct sockaddr*)&secondary_socket,sizeof(struct sockaddr_in));
							cancellationpoint(udp_to_tunnel,"Error in sending secondary udp to primary udp");
						}
					}
					break ;

			}

		}
		if(FD_ISSET(icmpobject.icmpsocket,&fs))
		{
			char buficmpreceive[2048];
			int icmprecv = recvfrom(icmpobject.icmpsocket,buficmpreceive,2048,0,(struct sockaddr*)&icmpobject.sock_addr_icmp,&icmpobject.icmpsocketlength);
			cancellationpoint(icmprecv,"Error in receiving icmp message");
			struct iphdr* ip = (struct iphdr*)buficmpreceive;
			struct icmphdr* icmp = (struct icmphdr *)(ip + 1);
			struct sockaddr_in src_addr, dest_addr;
			src_addr.sin_addr.s_addr = ip->saddr;
			dest_addr.sin_addr.s_addr = ip->daddr;
			string source_address = inet_ntoa(src_addr.sin_addr);
			string dest_address = inet_ntoa(dest_addr.sin_addr);
			/*write icmpinfo to log*/
			string icmpinfo = "ICMP from raw sock, src: " + source_address + ", dest: " + dest_address +
								", type: " + to_string((int)icmp->type); 
						
			logfilewrite(icmpinfo);


		}
	}
}


