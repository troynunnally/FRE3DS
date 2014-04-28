#ifndef _PARSER_H
#define _PARSER_H

#include <stdio.h>
//#include <stdint.h>
//#include <iostream>
//#include <windows.h>



#include "inaddr.h"				//Revised inaddr.h 
#include <vector>				/*Used for storing the gl coordinates*/
#include "protocols.h"			//Structures for all the protocols


//Use the vector space within the standard library (std)
using std::vector;

#define LINE_LEN 16				/* Line Length for print hex packets */

    class Parser
    {
    public:
		
		int readPacketsFromFile(char *filename);
		void showVertexSize();
		int getVertexSize();
		void showAveragePacketSize();		//Show the average packet size

		void glCalcCoord_averageY_totalZ();
		void showVertexIndex(int index);
		void showTotalNumberPackets();
		vector<double> gl_vertices;			// empty vector of glcoordinates for ports
		vector<double> gl_vertices2;

		vector<double> gl_coordinates_left_plane;			//  vector of glcoordinates for left_plane

		Parser();							//Constructor	
		unsigned int totalNumberPackets;	//Total Number of Packets
		unsigned int totalPacketSize;	//Average Packet Size
		
	private:
		
		sniff_ip *ip;					//Instance of camera structure
		sniff_tcp *tcp;
		sniff_udp *udp;
		SNIFF_ETHERNET *ethernet;

			
		//void dispatcher_handler(unsigned char *, const struct pcap_pkthdr *, const unsigned char *);
		void glCalcCoord(struct sniff_ip *ip, unsigned long ip_src, unsigned long ip_dst, unsigned short sport, unsigned short dport);
		

    };

#endif