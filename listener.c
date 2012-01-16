/* program:     listener
   author:      Oscar Sanchez
   description: Binds to a given port, listens for any sent 
                package and echoes it back to whoever sent it.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define MaxStringLen 256
int main(int argc, char ** argv) {
  int verbose=0;              //verbose output?
  int port=0;                 //what port we are talking to
  int c;                      //for processing input arguments
  
  /* run through the program arguments and error check------------------------*/
  while((c=getopt(argc,argv,"vp:"))!=-1)
    switch(c) {
      case 'v':
        verbose=1;            //verbose output true
        break;
      case 'p':
        port=atoi(optarg);    //the port we're binding to
        break;
      case '?':               //error checking
        fprintf(stderr,"Usage is: listener [-v] -p port\n");
        return 1;
      default:
        abort();
    }
  
  /* network connection-------------------------------------------------------*/
  int sockfd;
  struct sockaddr_in my_addr;                 // my address information
  struct sockaddr_in their_addr;              // connector’s address information
  int addr_len, numbytes;
  char buf[255];
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }
  my_addr.sin_family = AF_INET;               // host byte order
  my_addr.sin_port = htons(port);             // short, network byte order
  my_addr.sin_addr.s_addr = INADDR_ANY;       // automatically fill with my IP
  memset(&(my_addr.sin_zero), '\0', 8);       // zero the rest of the struct
  if (bind(sockfd, (struct sockaddr *)&my_addr,
    sizeof(struct sockaddr)) == -1) {
    perror("bind");
    exit(1);
  }
  addr_len = sizeof(struct sockaddr);
  if ((numbytes=recvfrom(sockfd,buf, MaxStringLen-1, 0,
    (struct sockaddr *)&their_addr, &addr_len)) == -1) {
    perror("recvfrom");
    exit(1);
  }
	
  if(verbose)
    printf("got packet from %s port %d\n",inet_ntoa(their_addr.sin_addr),port);
	
	//printf("packet is %d bytes long\n",numbytes);
	buf[numbytes] = '\0';                      // make sure it ends...
	printf("packet contains \"%s\"\n",buf);
  close(sockfd);
  
  return 0;
}
