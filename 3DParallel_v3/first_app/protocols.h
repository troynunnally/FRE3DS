//defines for the packet type code in an ETHERNET header

///////////////////////////////////////////////////////////////
//Ethernet Header
///////////////////////////////////////////////////////////////
/* Ethernet protocol ID's */
#define	ETHERTYPE_PUP		0x0200      /* Xerox PUP */
#define	ETHERTYPE_IP4		0x0800		/* IP v4*/
#define	ETHERTYPE_ARP		0x0806		/* Address resolution */
#define	ETHERTYPE_REVARP	0x8035		/* Reverse ARP */
#define ETHERTYPE_8021Q		0x8100
#define ETHERTYPE_IP6		0x86DD		/* IP v6*/	

/* ethernet headers are always exactly 14 bytes [1] */
#define SIZE_ETHERNET 14

/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6

/* Ethernet header */
typedef struct sniff_ethernet{
	unsigned char ether_dhost[ETHER_ADDR_LEN];    /* destination host address (6 bytes)*/
	unsigned char  ether_shost[ETHER_ADDR_LEN];    /* source host address (6 bytes)*/
	unsigned short ether_type;                     /* IP? ARP? RARP? etc (2 bytes)*/
} SNIFF_ETHERNET;

///////////////////////////////////////////////////////////////
//IP Header
///////////////////////////////////////////////////////////////

/* IPv4 header */
typedef struct sniff_ip{
	unsigned char  ip_vhl;					/* version << 4 | header length >> 2 */
	unsigned char  ip_tos;					/* type of service */
	unsigned short ip_len;					/* total length */
	unsigned short ip_id;					/* identification */
	unsigned short ip_off;					/* fragment offset field */
	#define IP_RF 0x8000			/* reserved fragment flag */
	#define IP_DF 0x4000            /* dont fragment flag */
	#define IP_MF 0x2000            /* more fragments flag */
	#define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
	unsigned char   ip_ttl;                 /* time to live */
	unsigned char   ip_p;                   /* protocol */
	unsigned short ip_sum;                 /* checksum */
	struct  in_addr ip_src,ip_dst;  /* source and dest address need netinet/in.h*/
} SNIFF_IP;


//Header Length is the first byte logically anded by 0f
#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
//Header Version is the first byte (ip_vh1) by shifting bits 4 bits to the right 
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)

///////////////////////////////////////////////////////////////
//IPv6 Header - have not been tested
///////////////////////////////////////////////////////////////
/* Structure of IPv6 header */
//typedef struct ipv6_header
//{
//  u_int8_t       version:4, 
//                 traffic_class_high:4;
//  u_int8_t       traffic_class_low:4,
//                 flow_label_high:4;
//  u_int16_t      flow_label_low;
//  u_int16_t      payload_length;
//  u_int8_t       next_header;
//  u_int8_t       hop_limit;
//  u_int8_t       src_addr[16];
//  u_int8_t       dst_addr[16];
//} ipv6_header_t;


///////////////////////////////////////////////////////////////
//TCP Header
///////////////////////////////////////////////////////////////
/* TCP header */
typedef unsigned int tcp_seq;
#define TRANSPORT_TYPE_TCP 0x06
#define TRANSPORT_TYPE_UDP 0x11
#define TRANSPORT_TYPE_ICMP 0x01
#define TRANSPORT_TYPE_IGMP 0x02
#define TRANSPORT_TYPE_OSPF 0x59

typedef struct sniff_tcp{
	unsigned short th_sport;               /* source port */
	unsigned short th_dport;               /* destination port */
	tcp_seq th_seq;                 /* sequence number */
	tcp_seq th_ack;                 /* acknowledgement number */
	unsigned char   th_offx2;               /* data offset, rsvd */
	#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
	unsigned char   th_flags;
	#define TH_FIN  0x01
	#define TH_SYN  0x02
	#define TH_RST  0x04
	#define TH_PUSH 0x08
	#define TH_ACK  0x10
	#define TH_URG  0x20
	#define TH_ECE  0x40
	#define TH_CWR  0x80
	#define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
	unsigned short th_win;                 /* window */
	unsigned short th_sum;                 /* checksum */
	unsigned short th_urp;                 /* urgent pointer */
} SNIFF_TCP;



///////////////////////////////////////////////////////////////
//UDP Header
///////////////////////////////////////////////////////////////

// UDP header's structure

typedef struct sniff_udp{
	unsigned short int uh_sport;
	unsigned short int uh_dport;
	unsigned short int uh_len;
	unsigned short int uh_chksum;
} SNIFF_UDP;

// total udp header length: 8 bytes (=64 bits)
 