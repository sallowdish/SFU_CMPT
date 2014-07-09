/*
  A skeleton to build on for low level socket programming.
  This skeleton will open a socket of type PF_PACKET,
  SOCK_RAW, suitable for constructing a frame from scratch.
*/

#include <string.h>
#include <errno.h>
#include <iostream>

// needed bt setw() and setfill()
#include <iomanip>
#include <sstream>
/* Basic data types u_int[8,16,32]_t */

#include <sys/types.h>

/* Socket interface definitions: constants and functions */

#include <sys/socket.h>

/* The sockaddr_ll structure */

#include <netpacket/packet.h>

/* ifreq, among other things. */

#include <net/if.h>

/* ioctl definitions */

#include <sys/ioctl.h>

/*
  Needed for ETHERTYPE_IP. Also contains a structure
  definition for the Ethernet frame header, if you care to
  use it.
*/

#include <net/ethernet.h>

/*
  Ethernet, IP and ICMP header struct definition
*/
#include </usr/include/net/ethernet.h>
#include </usr/include/netinet/ip_icmp.h>
#include </usr/include/netinet/ip.h>

// Needed to use inet_addr() which convert the Internet host address
// from IPv4 numbers-and-dots notation into binary data 
// in network byte orde
#include <netinet/in.h>
#include <arpa/inet.h>

/* Macro the length of msg in ICMP Echo Request*/
#define DATALEN 50
// #define PREAMBLELEN 8

unsigned short calcsum(unsigned short *buffer, int length);

main (int argc, char **argv){
  if (argc!=1 && argc!=3 && argc!=5)
  {
    printf("usage: echo [ src_IPv4_address dst_IPv4_address [src_Ether_address dst_Ether_address] ]\n");
    exit(1);
  }

  ssize_t ioRtnCode ;

  /*
  Open a packet level raw socket. Only the root id has
  sufficient privilege to do this.
  */
  int sd = 0 ;

  std::cout << "Attempting to create socket; " ;

  //if only network layer info is given, craft IP packet rather than Ethernet frame
  if (argc==3)
  {
    sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW) ;
  }else{
    sd = socket(PF_PACKET,SOCK_RAW,ETHERTYPE_IP) ;
  }
  
  // sd = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP) ;
  if (sd < 0)
  { std::cout << "failed.\n  Result is " << sd
  	      << ". Are you running as root?\n  " ;
    std::cout << strerror(errno) << "\n" ;
    exit (1) ; }
  else
  { std::cout << "descriptor is " << sd << ".\n" ; }

  /*
  Get the interface number corresponding to an interface
  name. We'll need this in order to send the frame. The
  name `eth1' is hardwired. On one of the hosts acting as
  routers, eth2 will also work.
  */
  struct ifreq ifioctl ;
  int ifNdx ;
  u_int8_t localMAC[ETH_ALEN];
  ifioctl.ifr_ifrn.ifrn_name[0] = 'e' ;
  ifioctl.ifr_ifrn.ifrn_name[1] = 't' ;
  ifioctl.ifr_ifrn.ifrn_name[2] = 'h' ;
  ifioctl.ifr_ifrn.ifrn_name[3] = '1' ;
  ifioctl.ifr_ifrn.ifrn_name[4] = '\0' ;
  std::cout << "Attempting to obtain interface number for "
      << ifioctl.ifr_ifrn.ifrn_name << " ... " ;
  ioRtnCode = ioctl(sd,SIOCGIFINDEX,&ifioctl) ;
  if (ioRtnCode < 0)
  { std::cout << "failed.\n  Result is " << ioRtnCode
  	      << ". Are you running as root?\n  " ;
    std::cout << strerror(errno) << "\n" ;
    exit (2) ; }
  else
  { ifNdx = ifioctl.ifr_ifru.ifru_ivalue ;
    memcpy(localMAC,ifioctl.ifr_hwaddr.sa_data,ETH_ALEN);
    std::cout << "index is " << ifNdx << "\n" ;
  }

  /*
  The address structure used for a packet socket. Family
  and interface index (ifindex) are necessary. sll_addr
  is unimportant for a raw packet socket. It's the address
  that's loaded in the frame that counts.
  */
  struct sockaddr_ll dllAddr ;

  dllAddr.sll_family = AF_PACKET ;
  dllAddr.sll_protocol = 0 ;
  dllAddr.sll_ifindex = ifNdx ;
  dllAddr.sll_hatype = 0 ;
  dllAddr.sll_pkttype = 0 ;
  dllAddr.sll_halen = 6 ;
  dllAddr.sll_addr[0] = 0x00 ;
  dllAddr.sll_addr[1] = 0x00 ;
  dllAddr.sll_addr[2] = 0x00 ;
  dllAddr.sll_addr[3] = 0x00 ;
  dllAddr.sll_addr[4] = 0x00 ;
  dllAddr.sll_addr[5] = 0x00 ;
  dllAddr.sll_addr[6] = 0 ;
  dllAddr.sll_addr[7] = 0 ;

  /*
  Declare a generous amount of buffer space in which we can
  build the frame, and a variable to hold the length of the
  finished frame.
  */
  char frame[1000] ;
  int frameLen = 0 ;

  /*
  Here, you need to write code to build your ICMP echo request,
  encapsulated in an IP packet, encapsulated in an Ethernet
  frame.
  */

  //Declaer varibles
  struct ether_header ethernetHeader;
  struct ip ipHeader;
  struct icmphdr icmpHeader;

  //finger the pointers to corresponding positions in frame
  // ethernetHeader=(struct ether_header*)frame;
  // ipHeader=(struct ip*)(ethernetHeader+sizeof(struct ether_header));
  // ipHeader=(struct ip*)frame;
  // icmpHeader=(struct icmp*)(frame+sizeof(struct ip));
  // std::cout<<"IP header:"<<ipHeader<<"\tICMP header:"<<icmpHeader<<std::endl;
  // std::cout<<"Diff:"<<(long)icmpHeader-(long)ipHeader<<std::endl;


  //config icmp packet
  icmpHeader.type=ICMP_ECHO;
  icmpHeader.code=0;
  icmpHeader.checksum=0;

  //msg carried by ICMP Echo request
  char greeting[DATALEN]="Hello World. Greetings from Rui Zheng.";

  //config the IP header

  ipHeader.ip_v     = 4;  /*IPv4*/
  ipHeader.ip_hl    = 5;  /* This is the smallest possible value, our IP header is only 20 bytes */
  ipHeader.ip_tos   = 0;  /* low delay*/
  ipHeader.ip_len  = htons(sizeof(struct ip)+sizeof(struct icmphdr)+DATALEN);
  ipHeader.ip_id    = htons(0x777);
  ipHeader.ip_off = 0;  /* do not fragement flag */
  ipHeader.ip_ttl   = 255;  /* packets should pass thru at most 4 routers to arrive at the destination in VNL*/
  ipHeader.ip_p    = 1;  /* #1 stands for ICMP */
  ipHeader.ip_sum    = 0;  /* initialize this to zero to properly calculate checksum */

  //if both src and dst addr are provide, use them
  if (argc==3 || argc==5)
  {
    ipHeader.ip_src.s_addr   = inet_addr(argv[1]); /* 1st argument is src address */
    ipHeader.ip_dst.s_addr   = inet_addr(argv[2]); /* 2nd argument is dst address */
    // std::cout<<"From "<<argv[1]<<" to "<<argv[2]<<"...\n";
    printf("From %s\nTo %s\n",argv[1],argv[2] );
  }
  //else, then the echo request from august to year
  else
  {
    ipHeader.ip_src.s_addr   = inet_addr("172.17.1.8"); /* 1st argument is src address */
    ipHeader.ip_dst.s_addr   = inet_addr("172.19.1.18"); /* 2nd argument is dst address */
    // std::cout<<"Default From 172.17.1.8 to 172.19.1.18...";
    printf("From\t%s\nTo\t%s\n","172.17.1.8","172.19.1.18" );
  }


  //calculate the ip header checksum
  ipHeader.ip_sum=calcsum((unsigned short*)&ipHeader,sizeof(struct ip));


  ethernetHeader.ether_type=htons(ETHERTYPE_IP);
  u_int8_t srcMAC[ETH_ALEN],dstMAC[ETH_ALEN];
  //configur the Ethernet header 
  if (argc==1)
  {
    //default ethernet frame
    //august 00:50:56:a4:05:33
    //year  00:50:56:a4:2b:16
    //january 00:50:56:a4:0b:bb
    sscanf("00:50:56:a4:05:33", "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &srcMAC[0], &srcMAC[1], &srcMAC[2], &srcMAC[3], &srcMAC[4], &srcMAC[5]);
    sscanf("00:50:56:a4:0b:bb", "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dstMAC[0], &dstMAC[1], &dstMAC[2], &dstMAC[3], &dstMAC[4], &dstMAC[5]);
    printf("srcMAC:%02x:%02x:%02x:%02x:%02x:%02x\ndstMAC:%02x:%02x:%02x:%02x:%02x:%02x\n",srcMAC[0], srcMAC[1], srcMAC[2], srcMAC[3], srcMAC[4], srcMAC[5],dstMAC[0], dstMAC[1], dstMAC[2], dstMAC[3], dstMAC[4], dstMAC[5] );
    memcpy(ethernetHeader.ether_shost,srcMAC,ETH_ALEN);
    memcpy(ethernetHeader.ether_dhost,dstMAC,ETH_ALEN);
  }
  else if (argc==5)
  {
    //if Link-Layer info is given
    

    sscanf(argv[3], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &srcMAC[0], &srcMAC[1], &srcMAC[2], &srcMAC[3], &srcMAC[4], &srcMAC[5]);
    sscanf(argv[4], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dstMAC[0], &dstMAC[1], &dstMAC[2], &dstMAC[3], &dstMAC[4], &dstMAC[5]);
    printf("srcMAC:%02x:%02x:%02x:%02x:%02x:%02x\ndstMAC:%02x:%02x:%02x:%02x:%02x:%02x\n",srcMAC[0], srcMAC[1], srcMAC[2], srcMAC[3], srcMAC[4], srcMAC[5],dstMAC[0], dstMAC[1], dstMAC[2], dstMAC[3], dstMAC[4], dstMAC[5] );
    memcpy(ethernetHeader.ether_shost,srcMAC,ETH_ALEN);
    memcpy(ethernetHeader.ether_dhost,dstMAC,ETH_ALEN);
  }else{
  }


  //Index of different header inside buffer
  char *etherOffset, *ipOffset,*icmpOffset,*dataOffset;
  //if Ethernet Header is crafted
  //copy the Ethernet header, ip header,ICMP header and data into buffer in turn
  if (argc==1 || argc==5)
  {
    etherOffset=frame;
    ipOffset=frame+sizeof(struct ether_header);
    icmpOffset=frame+sizeof(struct ether_header)+sizeof(struct ip);
    dataOffset=icmpOffset+sizeof(struct icmphdr);

    //copy into buffer
    memcpy(etherOffset,&ethernetHeader,sizeof(struct ether_header));
    memcpy(ipOffset,&ipHeader,sizeof(struct ip)); 
    memcpy(icmpOffset,&icmpHeader,sizeof(struct icmphdr));
    memcpy(dataOffset,greeting,DATALEN); 

    //calculate the total length of crafted Ethernet frame
    frameLen=sizeof(struct ether_header)+sizeof(struct ip)+sizeof(struct icmphdr)+DATALEN;
  }
  //otherwise, send IP packet rather than Ethernet frame
  else{
    ipOffset=frame;
    icmpOffset=frame+sizeof(struct ip);
    dataOffset=frame+sizeof(struct ip)+sizeof(struct icmphdr);

    //copy into buffer
    memcpy(ipOffset,&ipHeader,sizeof(struct ip)); 
    memcpy(icmpOffset,&icmpHeader,sizeof(struct icmphdr));
    memcpy(dataOffset,greeting,DATALEN);

    frameLen=sizeof(struct ip)+sizeof(struct icmphdr)+DATALEN;
  }

  //calculate ICMP checksum
    struct icmphdr *icmpPtr=(struct icmphdr *)icmpOffset;
    icmpPtr->checksum=calcsum((unsigned short*)icmpOffset,sizeof(icmphdr)+DATALEN);

  /*
  Send the frame. Don't forget to set the length of the
  finished frame. The sockaddr data type is a horribly
  overloaded union. The code is a bit more readable with an
  intermediate variable of type sockaddr.
  */

  // std::cout << "Attempting to create listen socket; " ;
  // int recv=socket(AF_INET,SOCK_ICMP,0)
  // // sd = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP) ;
  // if (recv < 0){ 
  //   std::cout << "failed to create listen socket.\n  Result is " << recv<< ".\n";
  //   std::cout << strerror(errno) << "\n";
  //   exit (1);
  // }
  // else{
  //   std::cout << "descriptor is " << recv << ".\n" ; 
  // }



  std::cout<<std::dec << "Attempting to send " << frameLen << " bytes ... " ;

//if only network layer info is given, construct socketaddr using IP address
if (argc==3)
{
  struct sockaddr_in dst;
  dst.sin_family=AF_INET;
  dst.sin_addr.s_addr=ipHeader.ip_dst.s_addr;
  ioRtnCode=sendto(sd,frame,frameLen,0,(struct sockaddr*)&dst,sizeof(struct sockaddr));
}
//if link layer info is given, construct socketaddr using MAC address
else{
  const struct sockaddr *saPtr ;
  size_t saLen ;
  saPtr = reinterpret_cast<const sockaddr *>(&dllAddr) ;
  saLen = sizeof(dllAddr) ;
  ioRtnCode = sendto(sd,frame,frameLen,0,saPtr,saLen) ;
}
  
  if (ioRtnCode < 0)
  { std::cout << "failed.\n  Result is " << ioRtnCode << "\n  " ;
    std::cout << strerror(errno) << "\n" ;
    exit (3) ; }
  else
  { std::cout << "success!\n" ; }

  exit (0) ; 
}


/*
Sample Code from sendicmp.c.txt ofDickBits.
http://sickbits.net/code2/sendicmp.c.txt
*/

/* calcsum - used to calculate IP and ICMP header checksums using
 * one's compliment of the one's compliment sum of 16 bit words of the header
 */
unsigned short calcsum(unsigned short *buffer, int length)
{
  unsigned long sum;  

  // initialize sum to zero and loop until length (in words) is 0 
  for (sum=0; length>1; length-=2) // sizeof() returns number of bytes, we're interested in number of words 
    sum += *buffer++; // add 1 word of buffer to sum and proceed to the next 

  // we may have an extra byte 
  if (length==1)
    sum += (char)*buffer;

  sum = (sum >> 16) + (sum & 0xFFFF);  // add high 16 to low 16 
  sum += (sum >> 16);        // add carry 
  return ~sum;
}
