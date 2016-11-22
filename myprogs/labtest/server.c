//include appropriate header file(s) for in-built socker functions etc.
#include "unp.h"
//requires to calculate date and time
#include <time.h>

int main(int argc, char **argv)
{
	/*socket IDs; one for the listening socket and one for the 
	connected socket */
	int listenfd, connfd;

	/*address structure to hold this server's address*/
	struct sockaddr_in servaddr;

	//verifies number of command-line args; two in this instance
	char buff[MAXLINE];

	//required to calculate dat and time
	time_t ticks;

	if (argc != 2)
	{
		err_quit("usage: <Program Name> <Port No.>");
	}

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; )
	{
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
/*
Note the use of upper-case letters at the start of the socket primities names.
These are wrapper functions tht include the error checking functionality for
each call. The actual socket primitives use lower case names.
*/
