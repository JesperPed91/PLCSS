#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std; 

void man()
{
	//Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
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


	//binding the socket to an ip. and port

	//WS2 the socket is to listen to 

	//waiting for connection 

	//Close listening socket

	//accept and echo message back

	//close the socket



	//shutdown for Ws2(winsock)


}
