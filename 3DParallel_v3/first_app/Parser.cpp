/*
 *  pcapOfflineParser.c
 *  
 *
 *  Created by Troy Nunnally on 9/21/11.
 *  Copyright 2011 Brain Rain Solutions. All rights reserved.
 *
 */






#include "Parser.h"

#include <pcap.h>
#include <Winsock2.h>	//inet_ntoa

Parser::Parser():
totalNumberPackets(0),
totalPacketSize(0)
{

}

int Parser::readPacketsFromFile(char *filename) 
{

    pcap_t *fp;
	char errbuf[PCAP_ERRBUF_SIZE];
	
	/*********temporary packet buffers **********/


	/*  struct pcap_pkthdr {
		 struct timeval ts;		  time stamp 
		 bpf_u_int32 caplen;	  length of portion present 
		 bpf_u_int32 len;         length this packet (off wire) 
		 }
	*/
	struct pcap_pkthdr *header;					// The header that pcap gives us 			
	const u_char *pkt_data;						// The actual packet 
	
	/***** Declare pointers to packet headers*******/
	//const struct SNIFF_ETHERNET *ethernet;	// The ethernet header [1] 
	//const struct sniff_ip *ip;				// Declare an instance to the IP header structure
	//const struct sniff_tcp *tcp;				// The TCP header
	//const struct sniff_udp *udp;				// The TCP header
	
	const u_char *payload;						// Packet payload
	
	u_int i=0;									//used to initialize loops
	int res;									//used for the pcap_next loop
	
	//filename = "test.pcap";

	
	/* Open the capture file */
	if ((fp = pcap_open_offline(filename,		// name of the device
						 errbuf					// error buffer
						 )) == NULL)
	{
		//fprintf(stderr,"\nUnable to open the file %s.\n", filename);
		return -1;
	}

	/* read and dispatch packets until EOF is reached */
    /* Retrieve the packets from the file */
    while((res = pcap_next_ex( fp, &header, &pkt_data)) >= 0)
    {


		// header contains information about the packet (e.g. timestamp) 
		u_char *pkt_ptr = (u_char *)pkt_data;		//cast a pointer to the packet data 


		/* print pkt timestamp and pkt len */
		printf("%ld.%ld seconds (%ld bytes)\n", header->ts.tv_sec, header->ts.tv_usec, header->len);	//Print Packet Timestamp value in seconds:milliseconds (packet length)	
		
		
		

		ethernet = (SNIFF_ETHERNET *)(pkt_data);		//Ethernet Header

		/*parse the first (ethernet) header, grabbing the type field*/
		int ether_type = ntohs(ethernet->ether_type);		//ntohs converts  the 16-bit u_short from TCP/IP network byte order to host byte order (which is little-endian on Intel processors).
		int ether_offset = 0; 
				
		if (ether_type == ETHERTYPE_IP4){ //most common 
			ether_offset = SIZE_ETHERNET; 
			//printf("Ethernet Type: IP\n");
		}	
		else if (ether_type == ETHERTYPE_8021Q) // 
			ether_offset = 18; 
		//else if (ether_type == ETHERTYPE_ARP)
			//printf("Ethernet Type: ARP\n");
		//else if (ether_type == ETHERTYPE_REVARP)
			//printf("Ethernet Type: RARP \n");
		//else if (ether_type == ETHERTYPE_IP6)
			//printf("Ethernet Type: IPv6 \n");
		//else 
			//fprintf(stderr, "Unknown ethernet type, %04X, skipping...\n", ether_type); 
				
		if (ether_type == ETHERTYPE_IP4){ //most common 
			//parse the IP header 
			ip = (struct sniff_ip*)(pkt_data + ether_offset);		// Typecast packet into a ip structure
			int size_ip = IP_HL(ip)*4;							// IP Header Size in bytes
			
			unsigned long ip_src = ntohl(ip->ip_src.S_un.S_addr);
			unsigned long ip_dst = ntohl(ip->ip_dst.S_un.S_addr);
			
			char buf[2048];

			//sprintf(buf, "***** \n");
			//OutputDebugString(buf);

			//sprintf(buf,"IP Source: %s or %u\n", inet_ntoa(ip->ip_src), ip_src);				// IP source address Internet standard dotted format as string). 
			//OutputDebugString(buf);
			//sprintf(buf,"IP Destination: %s or %u\n", inet_ntoa(ip->ip_dst), ip_dst);		// IP destination address Internet standard dotted format as string. 
			//OutputDebugString(buf);

			//printf("IP Protocol: %u \n", ip->ip_p);				// IP destination address (in network format). 
			
			int packet_length = ntohs(ip->ip_len);				// Total length of the packet
			
			totalNumberPackets++;				
			totalPacketSize = totalPacketSize + packet_length;

			if(ip->ip_p == TRANSPORT_TYPE_TCP)
			{
			//parse the TCP header 
			tcp = (struct sniff_tcp*)(pkt_data + ether_offset + size_ip);		//typecast packet into a ip structure						//IP Header Size in bytes

			//sprintf(buf,"Source Port: %u \n", ntohs(tcp->th_sport));
			//OutputDebugString(buf); 
			//sprintf(buf,"Destination Port: %u \n", ntohs(tcp->th_dport));
			//OutputDebugString(buf);

			glCalcCoord(ip, ip_src, ip_dst, ntohs(tcp->th_sport), ntohs(tcp->th_dport));
			}
			else if(ip->ip_p == TRANSPORT_TYPE_UDP)
			{
			//parse the TCP header 
			udp = (struct sniff_udp*)(pkt_data + ether_offset + size_ip);		//typecast packet into a ip structure						//IP Header Size in bytes

			//sprintf(buf,"Source Port: %u \n", ntohs(udp->uh_sport));	
			//OutputDebugString(buf); 
			//IP source address (in network format). 
			//sprintf(buf,"Destination Port: %u \n", ntohs(udp->uh_dport));		//	IP destination address (in network format). 
			//sprintf(buf,"UDP Length: %u bytes \n", ntohs(udp->uh_len));
			//OutputDebugString(buf); 
			glCalcCoord(ip, ip_src, ip_dst, ntohs(udp->uh_sport), ntohs(udp->uh_dport));
			}
		}

		/* Print the packet */
		//for (i=1; (i < header->caplen + 1 ) ; i++)			//length of portion present 
		//{
		//	printf("%.2x ", pkt_data[i-1]);
		//	if ( (i % LINE_LEN) == 0) printf("\n");
		//}
		
		//printf("\n\n");		
		
	}


    if (res == -1)
    {
       // printf("Error reading the packets: %s\n", pcap_geterr(fp));
    }
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
//	dispatcher_handler 
//		- Parses network information and stores it into a array 
//		
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
//	glCalcCoord 
//		- Converts Ports and IPs in openGL Coordianates 
//		  and store it into a vertex array
///////////////////////////////////////////////////////////////////////////////////////
void Parser::glCalcCoord(struct sniff_ip *ip, unsigned long ip_src, unsigned long ip_dst, unsigned short sport, unsigned short dport)
{
	
	double gl_x_pos, gl_y_pos, gl_z_pos, gl_x_pos2, gl_y_pos2, gl_z_pos2;
	//char buf[2048];

	double total_ports = 65535;
	//double total_ports = 500;
	//double total_ip = 4294967295;
	double total_ip = 4294967295;
	
	double port_ratio;

	//Perform some type-casting on the property of the the structure
	double ip_dstd = (double)(ip_dst);
	double ip_srcd = (double)(ip_src);
	char *ip_src_dot = inet_ntoa(ip->ip_src);
	char *ip_dst_dot = inet_ntoa(ip->ip_dst);

	
	char buf[2048];

	unsigned long D = ip->ip_src.S_un.S_addr;
	//Convert from network to host (big-endian)
    D = ((D>>24)&0xff)|		//move byte 3 to byte 0
			  ((D<<8)&0xff0000)|		//move byte 1 to byte 2
			  ((D>>8)&0xff00)|		//move byte 2 to byte 1
			  ((D>>24)&0xff000000);		//move byte 0 to byte 3



	//sprintf(buf, "D = %f \n",(double)sport/total_ports*9.0f);
	//OutputDebugString(buf);


	//sprintf(buf,"IP Source: %s or %u\n", ip_src_dot, ntohl(ip->ip_src.S_un.S_addr));				// IP source address Internet standard dotted format as string). 
	//OutputDebugString(buf);

	//sprintf(buf,"IP Destination: %s or %u\n", ip_dst_dot, ntohl(ip->ip_dst.S_un.S_addr));		// IP destination address Internet standard dotted format as string. 
	//OutputDebugString(buf);

	if(sport>50000){
		gl_x_pos = (double)(-5.5f);
		gl_y_pos = ((double)sport/total_ports*500-383-.3)*9.0f - 5.0f+1.0f;
		gl_z_pos = (double)ip_src/total_ip*9.0f-5.0f;
	}else
	{
		gl_x_pos = (double)(-5.5f);
		gl_y_pos = (double)sport/total_ports*9.0f - 5.0f+1.0;
		gl_z_pos = (double)ip_src/total_ip*9.0f-5.0f;
	}
	
	//if(gl_x_pos<-5.0f)
	//{
	//push 1st gl vertex coordinates on the back of the vector array
	Parser::gl_vertices.push_back (gl_x_pos);
	Parser::gl_vertices.push_back (gl_y_pos);
	Parser::gl_vertices.push_back (gl_z_pos);
	//}

	
	if(dport>53800){
		gl_x_pos2 = (double)(5.5f);
		gl_y_pos2 = ((double)dport/total_ports*500-410+0)*9.0f - 5.0f+1.0f;
		gl_z_pos2 = (double)ip_dst/total_ip*9.0f-5.0f;
	}else{
		gl_x_pos2 = (double)(5.5f);
		gl_y_pos2 = (double)dport/total_ports*9.0f - 5.0f+1.0f;
		gl_z_pos2 = (double)ip_dst/total_ip*9.0f-5.0f;
	}
	
	//if(gl_x_pos2>5.0f)
	//{
	//push 2nd gl vertex coordinates on the back of the vector array
	Parser::gl_vertices.push_back (gl_x_pos2);
	Parser::gl_vertices.push_back (gl_y_pos2);
	Parser::gl_vertices.push_back (gl_z_pos2);
	//}
	

	/*DRAW A SECOND LINE*/
		Parser::gl_coordinates_left_plane.push_back(gl_x_pos-1.0f);
		Parser::gl_coordinates_left_plane.push_back(gl_y_pos);
		Parser::gl_coordinates_left_plane.push_back(gl_z_pos);

	//sprintf(buf," src IP: %u  -  dst IP: %u\n", ip_src, ip_dst);
	//OutputDebugString(buf);

	//sprintf(buf," sPort: %u  -  dport: %u \n Line: (%f, %f, %f) to (%f, %f, %f) \n", sport, dport, gl_x_pos, gl_y_pos,gl_z_pos, gl_x_pos2, gl_y_pos2, gl_z_pos2);
	//OutputDebugString(buf);

}

void Parser::showAveragePacketSize()
{


	float averagePacketSize=(float)totalPacketSize/(float)totalNumberPackets;				
			
	wchar_t buf[2048];
	wsprintf(buf,L"Average Packet Size (in bytes) = (%f) \n",averagePacketSize);
	OutputDebugString(buf);

}	

void Parser::glCalcCoord_averageY_totalZ()
{
	
	for(unsigned int i = 0; i < gl_coordinates_left_plane.size(); i=i+3)
	{
		/*VARIABLES*/
			double gl_x_pos, gl_y_pos, gl_z_pos;

		/*COMPUTE COORDINATES*/
		if(i < (gl_coordinates_left_plane.size()-3)){
			Parser::gl_vertices2.push_back (gl_coordinates_left_plane[i]);
			Parser::gl_vertices2.push_back (gl_coordinates_left_plane[i+1]);
			Parser::gl_vertices2.push_back (gl_coordinates_left_plane[i+2]);

			double averagePacketSize=(double)totalPacketSize/(double)totalNumberPackets;

			gl_x_pos = (double)(-17.5f);
			gl_y_pos = averagePacketSize/1500*9.0f - 5.0f+1.0f;			//1500 is the MTU size of ethernet frame
			//gl_y_pos = (double)1490/1600.0*9.0f - 5.0f+1.0f;
			gl_z_pos = (double)1444/1500*9.0f-5.0f-.5f;		//1500
			//gl_z_pos = (double)2696/150000.0*9.0f-5.0f;		//150000-147304

			Parser::gl_vertices2.push_back (gl_x_pos);
			Parser::gl_vertices2.push_back (gl_y_pos);
			Parser::gl_vertices2.push_back (gl_z_pos);

		}
		
	}


}
						 

void Parser::showVertexSize()
{

	int vertex_size = Parser::gl_vertices.size();

	wchar_t buf[2048];
	wsprintf(buf,L"Vertex Size = (%d) \n",vertex_size);
	OutputDebugStringW(buf);

}	

void Parser::showVertexIndex(int index)
{
	double vertex_ind = Parser::gl_vertices[index];

	wchar_t buf[2048];
	wsprintf(buf,L"Vertex Index (%u) = (%f) \n",index, vertex_ind);
	OutputDebugStringW(buf);
}

void Parser::showTotalNumberPackets()
{
	wchar_t buf[2048];
	wsprintf(buf,L"Total Number of Packets %u... \n", totalNumberPackets);
	OutputDebugStringW(buf);
}

int Parser::getVertexSize()
{
	int vertex_size = Parser::gl_vertices.size();

	return vertex_size;
}