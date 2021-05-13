// Receives data from sender-client through the MQ service
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h> 
#include <io.h>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <string>
#include <conio.h>
#include <thread>
#include <chrono>

#pragma comment(lib,"ws2_32.lib") // Winsock Library

#define IP "127.0.0.1"	// Ip address of udp server
#define BUFLEN 512	// Max length of buffer
#define PORT 8808	// The port on which to listen for incoming data
#define SERV_PORT 8888	// The port on which to listen for incoming data

int main(void)
{
	struct sockaddr_in si_this, si_other;
	int s, slen = sizeof(si_this), slen_other = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	WSADATA wsa;

	// Initialise winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// Create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// Setup address structure
	memset((char *)&si_this, 0, sizeof(si_this));
	si_this.sin_family = AF_INET;
	si_this.sin_port = htons(PORT);
	si_this.sin_addr.s_addr = inet_addr(IP);

	if (bind(s, (struct sockaddr *)&si_this, sizeof(si_this)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");

	int recv_len = 0;

	// Start communication
	while (true) {
		
		memset(buf, '\0', BUFLEN);

		// Try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			throw std::exception("Connection lost\n");
			exit(EXIT_FAILURE);
		}

		std::cout << "\n" << buf << "\n";

	}

	closesocket(s);
	WSACleanup();

	_getch();
}