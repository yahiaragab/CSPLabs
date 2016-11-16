
//include appropriate header file(s) for in-built socker functions etc.
#include "unp.h"
//requires to calculate date and time
#include <time.h>
//#include <ncurses.h>
#include <stdio.h>

#define HOME_PAGE "<html><head><title>Home</title></head></html>"

#define ERROR_PAGE "<html><head><title>404: Not Found</title></head></html>"

int main(int argc, char **argv)
{
	/*socket IDs; one for the listening socket and one for the
	connected socket */
	int listenfd, connfd, char_in, count = 0;

	/*address structure to hold this server's address*/
	struct sockaddr_in servaddr, cliaddr;

	socklen_t len;

	//verifies number of command-line args; two in this instance
	char readbuff[MAXLINE];
	char writebuff[MAXLINE];

	//buffer for client address
	char buff[MAXLINE];

	//LAB4
	char cmd[16], path[64], vers[16], path1[64] = {'.'};

	FILE * fp;

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
		len = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &len);

		printf("cnnection from %s, port %d\n",
			inet_ntop(AF_INET, &cliaddr.sin_addr, buff, 
					sizeof(buff)),
			ntohs(cliaddr.sin_port));

		while ( (n = read(connfd, readbuff, MAXLINE)) > 0 )
		{
			//readbuff[n] = 0;

			if (fputs(readbuff, stdout) == EOF)
			{
				err_sys("fputs error");
			}

			if (strstr(readbuff, "\r\n\r\n"))
			{
				break;
			}
		}

		if (n < 0)
		{
			err_sys("read error");
		}

		//WHAT IS n?
		readbuff[n] = 0;
                sscanf(readbuff, "%s %s %s", cmd, path, vers);

		strcat(path1, path);

		if  (strcmp(path1, "./") == 0)
		{
			strcpy(path1, "./index.html");
		}

		fp = fopen(path1, "r");

		if (fp == NULL)
		{
			fp = fopen("error.html", "r");
		}

		strcpy(writebuff, "");

		while ((char_in = fgetc(fp)) != EOF)
		{
			writebuff[count] = char_in;
			count++;
		}

		writebuff[count] = 0;

		Write(connfd, writebuff, strlen(writebuff));
		count = 0;
		fclose(fp);
		strcpy(path1, ".");

		Close(connfd);
	}

}
/*
Note the use of upper-case letters at the start of the socket primities names.
These are wrapper functions tht include the error checking functionality for
each call. The actual socket primitives use lower case names.
*/
