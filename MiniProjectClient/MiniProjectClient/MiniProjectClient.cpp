#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

//function used for creating a thread that receives data from the server and thereby ignores the blocking call
void infiniteReceive(SOCKET s){
	char buf[4096];
	while (true){
		ZeroMemory(buf, 4096);
		int bytesReceived = recv(s, buf, 4096, 0);
		if (bytesReceived > 0)
		{
			cout << "SERVER > " << string(buf, 0, bytesReceived) << endl;
		}
	}
}

void main()
{
	string ipAddress = "192.168.0.101";
	int port = 54000;

	//int winsock

	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "cant start winsock" << wsResult << endl;
		return;
	}

	//create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "cant create socket" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	//fill in a structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//con. to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "cant connect to server" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}


	//Send recieve data 
	char buf[4096];
	string userInput;

	//start receive thread
	thread t1(infiniteReceive, sock);

	do
	{
		if (userInput.size() > 0)
		{

			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0); //+1 because of c++ every string is terminated with a 0
			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buf, 4096);

			}
			userInput = "";
		}
		cout << "> ";
		getline(cin, userInput);
	} while (userInput.size() > 0);

	closesocket(sock);
	WSACleanup();

}