/*
  A skeleton to build on for low level socket programming.
  This skeleton will open a socket of type PF_PACKET,
  SOCK_RAW, suitable for constructing a frame from scratch.
*/

#include <string.h>
#include <errno.h>
#include <iostream>

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

unsigned short calcsum(unsigned short *buffer, int length);

main (){
  ssize_t ioRtnCode ;

  /*
  Open a packet level raw socket. Only the root id has
  sufficient privilege to do this.
  */
  int sd = 0 ;

  std::cout << "Attempting to create socket; " ;
  sd = socket(PF_PACKET,SOCK_RAW,ETHERTYPE_IP) ;
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
    std::cout << "index is " << ifNdx << "\n" ; }

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
  struct ether_header *ethernetHeader;
  struct ip *ipHeader;
  struct icmp *icmpHeader;

  //finger the pointers to corresponding positions in frame
  ethernetHeader=(struct ether_header*)frame;
  ipHeader=(struct ip*)(ethernetHeader+sizeof(struct ether_header));
  icmpHeader=(struct icmp*)(ipHeader+sizeof(struct ip));
  icmpHeader->icmp_type=ICMP_ECHO;
  icmpHeader->icmp_code=0;
  char greeting[]="Hello World. Greetings from Rui Zheng.";
  strcpy(icmpHeader->icmp_data,greeting);
  std::cout<<"data:"<<icmpHeader->icmp_data;
  std::cout<<"\necho.icmp_cksum="<<icmpHeader->icmp_cksum;
  echo->icmp_cksum=calcsum((unsigned short*)icmpHeader,sizeof(icmp));

  std::cout<<"\necho.icmp_cksum="<<icmpHeader->icmp_cksum;


  // echo.icmp_id=1
  // std::cout<<"id"<<echo.icmp_id<<"\nseq"<<echo.icmp_seq;

  /*
  Send the frame. Don't forget to set the length of the
  finished frame. The sockaddr data type is a horribly
  overloaded union. The code is a bit more readable with an
  intermediate variable of type sockaddr.
  */

  std::cout << "Attempting to send " << frameLen << " bytes ... " ;

  const struct sockaddr *saPtr ;
  size_t saLen ;
  saPtr = reinterpret_cast<const sockaddr *>(&dllAddr) ;
  saLen = sizeof(dllAddr) ;
  ioRtnCode = sendto(sd,frame,frameLen,0,saPtr,saLen) ;

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