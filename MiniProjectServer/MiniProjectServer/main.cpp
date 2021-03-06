#include <iostream>
#include <WS2tcpip.h>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")

using namespace std; 

void main()
{
	//Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int WinSock = WSAStartup(ver, &wsData);
	if (WinSock != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl; //if we can't create winsock
		return;
	}

	// Create a socket (just a number)
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	//bind ip address and port to socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // could also use inet_pton ....

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	fd_set master;
	FD_ZERO(&master);

	FD_SET(listening, &master);

	while (true)
	{
		fd_set copy = master;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		
		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];
			if (sock == listening)
			{
				//accept new connection
				SOCKET client = accept(listening, nullptr, nullptr);

				//add new connection to the list of connected clients
				FD_SET(client, &master);
				//send welcome message to the connected client
				string welcomeMsg = "Welcome maddafkkas!\r\n"; //Perhaps change this
				send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
				
			}
			else
			{
				//buffer for recived data
				char buff[4096];
				ZeroMemory(buff, 4096);
				//accepting external messages 
				int byteIN = recv(sock, buff, 54096, 0);
				if(byteIN <= 0)
				{
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				
				//echo message to all other clients NOT THE socket that send it
				for (int i = 0; i < master.fd_count; i++)
				{
					SOCKET outputSock = master.fd_array[i];
					if (outputSock != listening && outputSock != sock)
					{
						ostringstream stringStream;
						stringStream << "Testbadder " << sock << ": " << buff << "\r\n"; //text standing in front of each message from every other klient than yourself
						string stringOutput = stringStream.str(); 
						
						send(outputSock, stringOutput.c_str(), stringOutput.size()+1, 0); //sending the message out to the remaining clients, +1 is because in c++ there is a trailing 0 so the size will be +1
					}
				}
			}
		}
	}
 

	//cleanup for winsock
	WSACleanup();


}
