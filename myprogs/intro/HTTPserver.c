//include appropriate header file(s) for in-built socker functions etc.
#include "unp.h"
//requires to calculate date and time
#include <time.h>
//#include <ncurses.h>

#define HOME_PAGE "<html><head><title>Home</title></head></html>"

#define ERROR_PAGE "<html><head><title>404: Not Found</title></head></html>"

int main(int argc, char **argv)
{
	/*socket IDs; one for the listening socket and one for the
	connected socket */
	int listenfd, connfd;

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
	FILE *fp;
//CANT GET FILES WOKING	
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
			if (strstr(readbuff, "\r\n\r\n"))
			{
				break;
			}
		}
		//WHAT IS n?
		readbuff[n] = 0;
                sscanf(readbuff, "%s %s %s", cmd, path, vers);

		strcat(path1, path);

		if ( (fp = fopen(path1, "r")) )
		{
			printf("OOOPPEEENN!!!\n\n");
/*		//putc(
	}

        if ( fopen(path1, "rb") )
        {
*/
			char file[MAXLINE];
			int g = 0;
			while(!feof(fp))
			{
				fscanf(fp, "%s", file[g]);
				g++;
			}
			file[g] = 0;

			if ( strcmp(path,"/index.html") == 0 )
			{
				snprintf(writebuff, sizeof(writebuff), file);
				//snprintf(writebuff, sizeof(writebuff), fp);
				writebuff[n] = 0;
				Write(connfd, writebuff, strlen(writebuff));
			}
			else
			{
				snprintf(writebuff, sizeof(writebuff), file);
				writebuff[n] = 0;
				Write(connfd, writebuff, strlen(writebuff));
			}


		}
		else
		{
			snprintf(writebuff, sizeof(writebuff),
				"FILE NOT FOUND");
                        writebuff[n] = 0;
                 	Write(connfd, writebuff, strlen(writebuff));
		}
		fclose(fp);
		Close(connfd);
	}

}
/*
Note the use of upper-case letters at the start of the socket primities names.
These are wrapper functions tht include the error checking functionality for
each call. The actual socket primitives use lower case names.
*/
