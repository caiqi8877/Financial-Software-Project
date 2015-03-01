#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "io.h"
#include "clock.h"
#include "protocol.h"
#include "socket.h"

SBB_clock* timer;
SBB_instrument_fields* opening_records;
SBB_instrument_fields* closing_records;
int opening_records_count;
int closing_records_count;

int main(int args, char* argv[ ])
{
	/* 
	 * get an internet domain socket 
	 */
        bool flag=true;
	timer=new SBB_clock();

        tradingBookCollection* my_tdbook= new tradingBookCollection(argv[1]);
        my_tdbook->import_curve(argv[3]);
        opening_records = my_tdbook->get_records(opening_records_count);

	delete my_tdbook;
	my_tdbook = NULL;
	my_tdbook = new tradingBookCollection(argv[2]);
	my_tdbook->import_curve(argv[3]);
	closing_records = my_tdbook->get_records(closing_records_count);


	int sd;
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* 
	 * set up the socket structure 
	 */
	struct sockaddr_in	sock_addr;

	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;

#ifdef SBB_ANY
	/* set to INADDR_ANY if want server to be open to any client on any machine */
	sock_addr.sin_addr.s_addr = INADDR_ANY;
#else
	char	hostname[128];
	/*
	 *  we'll default to this host and call a section 3 func to get this host
	 */
	if( gethostname(hostname,sizeof(hostname)) ){
		fprintf(stderr," SBB gethostname(...) failed errno: %d\n", errno);
		exit(1);
	}
	printf("SBB gethostname() local hostname: \"%s\"\n", hostname);

	/*
	 * set up socket structure for our host machine
	 */
	struct hostent *hp;
	if ((hp = gethostbyname(hostname)) == 0) {
		fprintf(stderr,"SBB gethostbyname(...) failed errno: %d exiting...\n", errno);
		exit(1);
	}
	sock_addr.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
#endif
	sock_addr.sin_port = htons(PORT);

	/* 
	 * bind the socket to the port number 
	 */
	if (bind(sd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) == -1) {
		perror("bind");
		exit(1);
	}

	/* 
	 * advertise we are available on this socket/port
	 */
	if (listen(sd, 5) == -1) {
		perror("listen");
		exit(1);
	}

	/* 
	 * wait for a client to connect 
	 */
	struct sockaddr_in	sock_addr_from_client;
        socklen_t addrlen = sizeof(sock_addr_from_client); 
	int sd_current;
	if ((sd_current = accept(sd, (struct sockaddr *)  &sock_addr_from_client, &addrlen)) == -1) {
		fprintf(stderr,"SBB accept(...) failed errno: %d  exiting...\n", errno);
		exit(1);
	}
	printf("SBB client ip address: %s port: %x\n",
		inet_ntoa(sock_addr_from_client.sin_addr),
		PORT);

//		ntohs(sock_addr_from_client.sin_port));

	/*
	 * block on socket waiting for client message
	 */
	int ret = 0;
        char	msg[MSGSIZE];
	msg[0]=0;
	fprintf(stderr," SBB: sizeof msg: %d\n", sizeof(msg));

	parser* myparser = new parser(argv[3]);
	message mymessage;
	char* reply;

	while (ret = recv(sd_current, msg, sizeof(msg), 0) > 0) { 
	  printf("SBB server received msg: [%s] from client\n", msg);
	  if (flag)
	    {
	      timer->start_clock();
	      flag=false;
	    }
	  mymessage = myparser->parse(msg);
	  reply=myparser->handle(mymessage);
	  /* 
	   * ack back to the client 
	   */
	  if (send(sd_current, reply, strlen(reply), 0) == -1) {
	    fprintf(stderr,"SBB send(...) failed errno: %d exiting...\n", errno);
	    exit(1);
	  }
	  delete reply;
	  reply=NULL;
	  msg[0] = '\0';
	}

	if( 0 == ret ) {
		printf("SBB ciient exited...\n");
		/* For TCP sockets	
		 * the return value 0 means the peer has closed its half side of the connection 
		 */
	}
	else if( -1 == ret ) {
		fprintf(stderr,"SBB recv(...) returned failed - errno: %d exiting...\n", errno);	
		exit(1);
	}

	close(sd_current); 
	close(sd);
	return 0;
}
