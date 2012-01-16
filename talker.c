/* program:     talker
   author:      Oscar Sanchez
   description: Sends a message to be echoed by the listener on a given port.
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

int main(int argc, char ** argv) {
  int verbose=0;              //verbose output?
  int port=0;                 //what port we are talking to
  char* host;                //where the host is located
  char* message = argv[argc-1];             //what we are sending
  int c;                      //for processing input arguments
  
  /* run through the program arguments and error check------------------------*/
  while((c=getopt(argc,argv,"vp:h:"))!=-1)
    switch(c) {
      case 'v':
        verbose=1;            //verbose output true
        break;
      case 'p':
        port=atoi(optarg);    //the port we're binding to
        break;
      case 'h':
        host=optarg;          //the host
        break;
      case '?':               //error checking
        fprintf(stderr,"Usage is: talker [-v] -p port -h hostname\n");
        return 1;
      default:
        abort();
    }
    //check port again, just in case the input arguments were misread
    if(port<=0) {
      fprintf(stderr,"Usage is: talker [-v] -p port -h hostname\n");
      return 1;
    }
  
  /* network connection-------------------------------------------------------*/
  int sockfd;
  struct sockaddr_in their_addr; //Connector’s address information
  struct hostent *he;
  int numbytes;
  if ((he=gethostbyname(host)) == NULL) { // get the host info
    perror("gethostbyname");
    exit(1);
  }
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }
  their_addr.sin_family = AF_INET; // host byte order
  their_addr.sin_port = htons(port); // short, network byte order
  their_addr.sin_addr = *((struct in_addr *)he->h_addr_list[0]);
  memset(&(their_addr.sin_zero), '\0', 8); // zero the rest of the struct
  if ((numbytes=sendto(sockfd, message, strlen(message), 0,
  (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
    perror("sendto");
    exit(1);
  }
  if(verbose) {
    printf("sent packet to %s port %d\n",inet_ntoa(their_addr.sin_addr),port);
  }
	printf("packet contents:\n%s\n",message);
	close(sockfd);                //Close the socket!
  return 0;
}

