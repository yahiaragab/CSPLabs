//include appropriate header file(s) for in-built socker functions etc.
#include "unp.h"
//requires to calculate date and time
#include <time.h>

#define HOME_PAGE "GET /index.html HTTP/1.1\r\nHost: www.dit.ie\r\nConnection: close\r\n\r\n"

#define ERROR_PAGE "ERROR MESSAGE"

int main(int argc, char **argv)
{
	/*socket IDs; one for the listening socket and one for the 
	connected socket */
	int listenfd, connfd;

	/*address structure to hold this server's address*/
	struct sockaddr_in servaddr;

	//verifies number of command-line args; two in this instance
	char buff[MAXLINE];
	char socket[MAXLINE];

	//LAB4
	char cmd[16], path[64], vers[16];

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

		while ( (int n = read(connfd, socket, MAXLINE)) > 0 )
		{
			read(connfd, buff, MAXLINE);
			buff[n] = 0;
			sscanf(buff, "%s %s %s", cmd, path, vers);
			if (strstr(buff, "\r\n\r\n")
			{
				break;
			}
		}
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
