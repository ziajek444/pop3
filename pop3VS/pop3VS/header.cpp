#include "header.h"
#include "pch.h"
#include <string>
#include <string.h>
#include <winsock2.h>
#include <iostream>
#include <conio.h>
#include <thread>         // std::thread
#include <chrono>         // std::chrono::seconds


namespace GLOBALS {
	extern bool escape;
	extern int index;
}

std::string ReadByte(SOCKET s)
{
	std::string ELO = "";
	int recv_size = 0;
	char server_reply[512] = {};
	if ((recv_size = recv(s, server_reply, 512, 0)) == SOCKET_ERROR) {
		printf("Receiving information failed. Error Code : %d", WSAGetLastError());
		return "Error";
	}
	for (int i = 0; i < recv_size; ++i) {
		ELO += server_reply[i];
	}
	return ELO;
}

void closeConnectPop3(SOCKET s)
{
	std::string mess = "QUIT\n\r";
	send(s, mess.c_str(), mess.size(), 0);
	//Receiving information from the server
	std::cout << "Server (bye...): " << ReadByte(s);
}

std::string getUIDL(SOCKET s)
{
	std::string mess = "UIDL\n\r";
	send(s, mess.c_str(), mess.size(), 0);
	//Receiving information from the server
	if (ReadByte(s) == "-ERR") return "UIDL ERR";
	else {
		std::string tmp = "";
		mess = ReadByte(s);
		while (mess.length() == 512) {
			tmp += mess;
			mess = ReadByte(s);
		}
		tmp += mess;
		return (tmp != "") ?  tmp : "PUSTO!!" ;
	}
}

void ReadMessage(SOCKET s, int nr_of_message)
{
	// Odczyt wiadomosci
	std::string mess = "RETR ";
	try {
		mess += std::to_string(nr_of_message);
	}
	catch(int i)
	{
		std::cout << "It have to be a number!\r\n";
		return;
	}
	mess += "\n\r";
	std::cout << "Mess: " << mess << "\r\n";
	send(s, mess.c_str(), mess.size(), 0);
	//Receiving information from the server
	mess = ReadByte(s);
	if (mess[0] != '+') {
		std::cout << "RETR ERR\r\n"<<mess<<"\r\n";
		return;
	}
	std::cout << "Message: \r\n";
	mess = ReadByte(s);
	while (mess.length() == 512) {
		if (mess.find("-ERR") != -1) {
			break; //-1 to std::string::npos
		}
		std::cout << mess ;
		mess = ReadByte(s);
	}
	std::cout << mess;
	std::cout << "\r\nkoniec wiadomosci\r\n";
}

void SingIn(std::string Login, std::string CertainlyNotPassword, SOCKET s)
{

}

bool Press_Q()
{
	while (1)
	{
		switch (_getch())
		{
		case 'q':
			GLOBALS::escape = false;
			//printf("Hurray %d", GLOBALS::escape);
			return true;
			break;
		default:
			//printf("Do ur job again man ");
			break;
		}
	}
}

void pause_thread_ms(int n)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(n));
	//std::cout << "pause of " << n << " seconds ended\n";
}


