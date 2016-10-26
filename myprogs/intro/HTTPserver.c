//include appropriate header file(s) for in-built socker functions etc.
#include "unp.h"
//requires to calculate date and time
#include <time.h>

#define HOME_PAGE "<html><head><title>Home</title></head></html>"

#define ERROR_PAGE "<html><head><title>404: Not Found</title></head></html>"

int main(int argc, char **argv)
{
	/*socket IDs; one for the listening socket and one for the 
	connected socket */
	int listenfd, connfd;

	/*address structure to hold this server's address*/
	struct sockaddr_in servaddr;

	//verifies number of command-line args; two in this instance
	char readbuff[MAXLINE];
	char writebuff[MAXLINE];

	//LAB4
	char cmd[16], path[64], vers[16];


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

	int n;

	for ( ; ; )
	{
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		while ( (n = read(connfd, readbuff, MAXLINE)) > 0 )
		{
			if (strstr(readbuff, "\r\n\r\n"))
			{
				break;
			}
		}
		//WHAT IS n?
		readbuff[n] = 0;
                sscanf(readbuff, "%s %s %s", cmd, path, vers);

		if ( strcmp(path,"/index.html") == 0 )
		{
			snprintf(writebuff, sizeof(writebuff), HOME_PAGE);
			writebuff[n] = 0;
			Write(connfd, writebuff, strlen(writebuff));
		}
		else
		{
			snprintf(writebuff, sizeof(writebuff), ERROR_PAGE);
			writebuff[n] = 0;
			Write(connfd, writebuff, strlen(writebuff));
		}
		Close(connfd);
	}

}
/*
Note the use of upper-case letters at the start of the socket primities names.
These are wrapper functions tht include the error checking functionality for
each call. The actual socket primitives use lower case names.
*/
