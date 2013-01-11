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
		void showVertexIndex(int index);
		vector<double> gl_vertices;   // empty vector of glcoordinates for ports
		Parser();							//Constructor	
		
	private:
		
		sniff_ip *ip;					//Instance of camera structure
		sniff_tcp *tcp;
		sniff_udp *udp;
		SNIFF_ETHERNET *ethernet;

			
		//void dispatcher_handler(unsigned char *, const struct pcap_pkthdr *, const unsigned char *);
		void glCalcCoord(struct sniff_ip *ip, unsigned long ip_src, unsigned long ip_dst, unsigned short sport, unsigned short dport);
		

    };

#endif