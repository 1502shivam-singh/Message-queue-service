/*
------------------------------------------------------------------------------------------------------------------------------------------------
Message queue system																															
																																				
Design overview -																														    	
																																				
------------- t1	dequeueing recevied messages and sending to receiver after some wait (randomised, generated to mimick actual wait)			
																																				
------------- t2 receiving message from client and enqueuing in the message queue													            
																																				
System for now -																																
Client keeps sending data to this service on server																										
Server receives the data and queues it in a queue on one thread and other thread routes these messages after dequeuing							
																																				
------------------------------------------------------------------------------------------------------------------------------------------------
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h> 
#include <io.h>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <conio.h>
#include <queue>
#include <condition_variable>
#pragma comment(lib,"ws2_32.lib") // Winsock Library

#define BUFLEN 512	// Max length of buffer
#define PORT 8888	// The port on which to listen for incoming data
#define PORT_SEND 8808	// The port on which to send incoming data
#define IP "127.0.0.1"	// The ip of server

using namespace std::chrono_literals;
std::mutex m;
std::condition_variable sem;
std::queue<std::string> queue;

int lower = 5, upper = 80;

void funcPut(SOCKET& s, int intflag, sockaddr_in& si_other_send, int& slen_send) {
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds((std::rand() % (upper - lower + 1)) + lower));	// Simulating wait time by making this thread wait for randomised time

		std::unique_lock<std::mutex> _lck(m);
		sem.wait(_lck, [&] { return !queue.empty(); });
		std::cout << "Putted out: ";
		std::cout << queue.front() << "\n";
		std::string pass(queue.front());
		queue.pop();
		_lck.unlock();

		std::stringstream sendData;

		sendData << pass;

		// Pass message to receiver node

		if (sendto(s, sendData.str().c_str(), strlen(sendData.str().c_str()), intflag, (struct sockaddr*) &si_other_send, slen_send) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
	}
};

int main()
{
	SOCKET s;
	struct sockaddr_in server, si_other, si_other_send;
	int slen, recv_len, slen_send;
	char buf[BUFLEN];
	WSADATA wsa;

	slen = sizeof(si_other);
	slen_send = sizeof(si_other_send);

	// Initialise winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	memset((char *)&si_other_send, 0, sizeof(si_other_send));
	si_other_send.sin_family = AF_INET;
	si_other_send.sin_port = htons(PORT_SEND);
	si_other_send.sin_addr.S_un.S_addr = inet_addr(IP);

	// Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	std::cout<<"Connected...";

	fflush(stdout);

	memset(buf, '\0', BUFLEN);

	auto funcRec = [&] {
		while (true)
		{
			if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (sockaddr *)&si_other, &slen)) == SOCKET_ERROR)
			{
				printf("recvfrom() failed with error code  %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
			std::stringstream ss;

			{	// Critical section
				std::lock_guard<std::mutex> _mtx(m);
				for (int i = 0; i < recv_len; i++) {
					ss << buf[i];
				}
				queue.push(ss.str());
			}

			sem.notify_one();

			memset(buf, '\0', BUFLEN);
		}
	};

	std::thread t1(funcPut, std::ref(s), 0, std::ref(si_other_send), std::ref(slen_send));
	std::thread t2(funcRec);

	t1.join();
	t2.join();

	closesocket(s);
	WSACleanup();

	_getch();
}
