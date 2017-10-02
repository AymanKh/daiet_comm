#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "bosen_headers/ps_msgs.hpp"

#define PORT 8080

using namespace petuum;

/*
 * Socket address, internet style.
 */



int main(int argc, char const *argv[])
{
	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;

	char *bosen_msg = "bosen message";


	ClientSendOpLogMsg client_send_oplog_msg(bosen_msg);

	char *client_data = (char *) client_send_oplog_msg.get_data();

	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	send(sock , client_data - client_send_oplog_msg.get_header_size(), 20 , 0 );
	printf("Hello message sent\n");

	/* ======================== */

	valread = read( sock , buffer, 1024);
	printf("%s\n",buffer );


	return 0;
}
