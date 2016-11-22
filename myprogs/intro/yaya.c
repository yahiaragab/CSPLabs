#include "unp.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	int listenfd, connfd, n = 0, count = 0, char_in = 0;
	struct sockaddr_in servaddr, cliaddr;
	char buff[40], rbuff[MAXLINE], wbuff[MAXLINE];
	char cmd[16], path[64], path1[64]=".", vers[16];
	socklen_t len;

	FILE * fp;

	if (argc != 2)
	{
		err_quit("usage: ./<progname> <port>");
	}

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for( ; ; )
	{
		len = sizeof(cliaddr);

		connfd = Accept(listenfd, (SA *) &cliaddr, &len);

		printf("Connection from %s, port %d\n",
			Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
			ntohs(cliaddr.sin_port));

		while ( (n = read(connfd, rbuff, MAXLINE)) > 0 )
		{
			rbuff[n] = 0;
			if ( fputs(rbuff, stdout) == EOF )
			{
				err_sys("fputs error");
			}
			if (strstr(rbuff, "\r\n\r\n"))
			{
				break;
			}
		}

		if ( n < 0 )
		{
			err_sys("read error");
		}
		sscanf(rbuff, "%s %s %s", cmd, path, vers);

		strcat(path1, path);

		if (strcmp(path1, "./") == 0)
		{
			strcpy(path1, "./index.html");
			printf("path1 = %s\n", path1);
		}

		fp = fopen(path1, "r");

		if (fp == NULL)
		{
			fp = fopen("error.html", "r");
		}

		strcpy(wbuff, "");

		//count = char_in = 0;

		while ( (char_in = fgetc(fp)) != EOF)
		{
			wbuff[count] = char_in;
			count++;
		}

		wbuff[count] = 0;

		Write(connfd, wbuff, strlen(wbuff));

		count = 0;
		strcpy(path1, ".");
		fclose(fp);
		Close(connfd);

	}//end outer loop


}
