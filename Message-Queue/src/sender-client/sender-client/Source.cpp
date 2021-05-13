// Sending messages to the receiver client through the intermediate MQ service 
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

#define SERVER "127.0.0.1"	// Ip address of udp server
#define BUFLEN 512	// Max length of buffer
#define PORT 8888	// The port on which to listen for incoming data

int main(void)
{
	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
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
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	int i = 0;
	std::cin.get();	// Press key to start sending

	while (true) {
		std::string info = "";
		info = "Message: " + std::to_string(i);
		std::cout << "Entered message for the server: " << info << "\n";

		// Send the message
		if (sendto(s, info.c_str(), strlen(info.c_str()), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		i++;
	
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	closesocket(s);
	WSACleanup();

	_getch();
}