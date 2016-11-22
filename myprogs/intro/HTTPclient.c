#include "unp.h"

int
main(int argc, char **argv)
{
	int sockfd, n, counter = 0;
	char recvline[MAXLINE + 1], buff[MAXLINE];
	struct sockaddr_in servaddr;
	char *ptr, **pptr;
	struct hostent *hptr;

	ptr = argv[1];

	if (argc != 4)
	{
		err_quit("usage: a.out <IPaddress>");
	}

	if ( (hptr = gethostbyname(ptr)) == NULL)
	{
		err_msg("gethostbyname error for host: %s: %s",
			ptr, hstrerror(h_errno));

	}

	pptr = hptr->h_addr_list;

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		err_sys("socket error");
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2])); /*daytime server port 
					taken from the command line */

	//memcpy(&servaddr.sin_addr, *pptr, sizeof(*pptr));

	//if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <=0) /*convert dotted decimal 
//							IP address to network byte ord*/
	//{
	//	err_quit("inet_pton error for %s", argv[1]);
	//}

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
	{
		err_sys("connect error");
	}

	snprintf(buff, sizeof(buff), "GET %s HTTP/1.1\r\n"
				"Host: www.dit.ie\r\n"
				"Connection: Close\r\n\r\n", argv[3]);
	Write(sockfd, buff, strlen(buff));
	memcpy(&servaddr.sin_addr, *pptr, sizeof(*pptr));
	while ( (n = read(sockfd, recvline, 10)) > 0)
	{
		counter++; /*count the number of loops completed*/
		recvline[n] = 0; //null terminate
		if ( fputs(recvline, stdout) == EOF ) /*print data (time 
					and date) returned from server*/
		{
			err_sys("fputs error");
		}
	}

	if (n < 0)
	{
		err_sys("read error");
	}

	printf("\ncounter = %d\n", counter);

	exit(0);
}
