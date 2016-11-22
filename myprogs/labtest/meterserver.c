//Name: Yahia Ragab
//Student Number: D14122773

//include needed header files to use their functions 
//(netwok programming, standard I/O and string functions)
#include "unp.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	/*
	declaring 2 socket ids, listenfd will be used with the
	Socket primitive, and connfd with the Accept pimitive
	*/
	//n will be used for reading in from the client
	//reading will store the meter reading sent from the client
	int listenfd, connfd, n, reading = 0;
	//cost variable will store the result of reading * 1.3, which
	// is why it is a floating point variable
	float cost = 0;

	//address structure to hold this server's address
	struct sockaddr_in servaddr;

	//declaring 2 buffers, a read (receive) and write (send) buffer
	char readbuff[MAXLINE], writebuff[MAXLINE];
	//temporary variable to be used with strstr to break out of loop
	char *tempreading;

	//verify that the correct number of arguments (2) was passed from cmd line
	if (argc != 2)
	{
		//quit the program and print instructions of how to run
		err_quit("usage: <Program Name> <Port No.>");
	}

	//create connection with TCP layer
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	//bzero initializes the structure to 0
	bzero(&servaddr, sizeof(servaddr));
	//specify the sin_family (protocol to be used) is AF_INET which is TCP
	servaddr.sin_family = AF_INET;
	//convert port given in command line from presentation to 
	// network format [pton] (in bits) and store it
	servaddr.sin_port = htons(atoi(argv[1]));
	//convert and store INADDR_ANY, which is predefined in unp.h, into the
	// address part of the  address structure
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind the server to its address
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	//change this from an active socket to a passive socket, and
	// listen for connections
	Listen(listenfd, LISTENQ);

	//start infinite loop
	for ( ; ; )
	{
		//Accept incoming connections from clients, and make connfd the id
		connfd = Accept(listenfd, (SA *) NULL, NULL);
		//start nested loop
		// while there is still data to be read, from connection
		// place it in the read buffer, at chunks of MAXLINE size
		while ( ( n = read(connfd, readbuff, MAXLINE) ) > 0 )
		{
			//null terminate readbuffer
			readbuff[n] = 0;
			//display readbuff on screen and check for errors
			if ( fputs(readbuff, stdout) == EOF)
			{
				printf("There was an fputs error!\n");
			}
			//check for Reading in read buffer (needle in haystack
			tempreading = strstr(readbuff, "Reading:");
			//if reading: found, break from loop
			if ( tempreading != NULL )
			{
				break;
			}
		}
		//if no fata was read, print error
		if ( n < 0)
		{
			printf("There was a read error!\n");
		}

		//place the string after reading in readbuffer into tempreading
		sscanf(readbuff, "Reading: %s\r\n", tempreading);
		//convert from ascii to integer and place in reading
		reading = atoi(tempreading);
		//calculate cost and store it
		cost = (float)reading * 1.3;
		//put reading and cost details in the write buffer
		snprintf(writebuff, sizeof(writebuff),
			"Reading: %d Cost: %.2f\n",
		 	reading, cost);
		//send the write buffer to the client
		Write(connfd, writebuff, strlen(writebuff));
		//show cntents of write buffer
		printf("Written: %s\n", writebuff);
		fflush(stdout);
		//close the connection
		Close(connfd);
	}
}

