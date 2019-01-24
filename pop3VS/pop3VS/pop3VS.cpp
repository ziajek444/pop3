// pop3VS.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
#include "pch.h"
#include "header.h"
#include <windows.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h> // getaddrinfo
//#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string> // std::string display by std::cout<<
#include <algorithm>

#include <conio.h> //my favour
//Zabawa z watkami :(
#include <thread>         // std::thread
#include <atomic>         // std::atomic
#include <chrono>         // std::chrono::seconds

/*g++ main.cpp -o program.exe -lWs2_32*/

int main(int argc, char *argv[])
{
	// dane z pliku App.config
	std::vector<std::string> dane = GetParameters();
	std::string ADRSERV = dane[0];
	std::string USRNAME = dane[1];
	std::string PSSWRD = dane[2];
	std::string PORT = dane[3];
	int REFTIME = atoi(dane[4].c_str());
	if (REFTIME < 5000) REFTIME = 6000;
	


	WSADATA wsa;
	SOCKET s;
	char *hostname = const_cast<char*>(ADRSERV.c_str()); // "pop3.wp.pl"
	struct addrinfo hints, *result = NULL; 
	int err;
	//struct sockaddr_in addr;

	//Initializing winsock
	printf("Initialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Initialising Winsock f ailed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Initializing address
	ZeroMemory(&hints, sizeof(hints)); 
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_family = AF_INET; 
	hints.ai_protocol = IPPROTO_TCP; 
	if ((err = getaddrinfo(hostname, PORT.c_str(), &hints, &result)) != 0) { // "110"
		printf("error %d\n", err);
		return -1;
	}
	printf("Set data.\n");
	
	//Creating socket
	s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (s == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	printf("Socket created.\n");

	//Connecting socket to the address
	err = connect(s, result->ai_addr, (int)result->ai_addrlen);
	if (err == SOCKET_ERROR) {
		closesocket(s);
		s = INVALID_SOCKET;
		printf("Connect problem\n");
		return -1;
	}
	printf("Socket connected\n");

	freeaddrinfo(result);
	

	//Checking if socket is connected
	int error_code;
	int error_code_size = sizeof(error_code);
	getsockopt(s, SOL_SOCKET, SO_ERROR, (char*)&error_code, &error_code_size);
	if (!error_code) {
		printf("Socket: Valid connect \n");
	} else printf("Socket: Invalid connect ! Error ! Something gone wrong!\n");

	//Receiving information from the server
	std::cout << "Server (are u ready?): " << ReadByte(s);


	//------------------------------------------------------------------------------------------------
	
	//Sending login
	std::string mess = "USER " + USRNAME + "\n\r"; //USER grouplong0@wp.pl\n\r
	send(s, mess.c_str(), mess.size(), 0);
	//Receiving information from the server
	std::cout<< "login: " << ReadByte(s);

	//sending password
	mess = "PASS " + PSSWRD + "\n\r"; // PASS haslo1234\n\r
	send(s, mess.c_str(), mess.size(), 0);
	//Receiving information from the server
	std::cout <<"password: "<< ReadByte(s);

	//sending command list
	mess = "LIST\n\r";
	send(s, mess.c_str(), mess.size(), 0);
	//Receiving information from the server
	//recived = ReadByte(s);
	std::cout <<"list: "<< ReadByte(s);
	// LIST required double reciv
	//Receiving information from the server
	std::cout <<"LIST result: \n"<< ReadByte(s);
	
	std::cout << "-----------------------\n";
	
	//ReadMessage(s,1);
	
	//std::cout << "UIDL result: \n" << getUIDL(s) << std::endl;

	
	//------------------------------------------------------------------------------------------------
	char nr_wiad;
	
	//std::cin >> nr_wiad;
	//std::cout << nr_wiad;

	GLOBALS::escape = true;
	//std::thread TD(Press_Q);
	
	
	
	//pause_thread_ms(2000);
	system("cls");
	std::cout << "Ktora wiadomosc chcialbys odczytac?\r\n";
	std::cout << "UIDL result: \n" << getUIDL(s) << std::endl;

	GLOBALS::UIDL = getUIDL(s);
	std::string tmp_UIDL = GLOBALS::UIDL;
	
	std::thread REFR(f, REFTIME, s);

	while (GLOBALS::escape)
	{
		//system("cls");
		std::cout << "\r\nKtora wiadomosc chcialbys odczytac?\r\n";
		std::cout << "UIDL result: \n" << tmp_UIDL << std::endl; //szybszy dostep do tmp_UIDL

		if (GLOBALS::UIDL != tmp_UIDL)
		{
			tmp_UIDL = GLOBALS::UIDL;
			while(tmp_UIDL.find("+OK")) tmp_UIDL = GLOBALS::UIDL;
			std::cout << "Nowe Wiadomosci\r\n";
		}
		else
		{
			//std::cout << "\r\nNie ma nowych wiadomosci\r\n";
		}
		
 
		std::cin >> nr_wiad;

		if (nr_wiad == 'q') {
			std::cout << "\r\npoczekaj na zamkniecie wszystkich watkow\r\nlub zabij program natychmiast\r\n\r\n";
			GLOBALS::escape = false; 

			while (!(REFR.joinable())) { 
				continue;

			}
			REFR.join();
			
			std::cout << "break\r\n";
			break;
		}
		else
		{
			int ix = static_cast<int>(nr_wiad - 48);
			ReadMessage(s, ix);
			std::cout << "\r\nWciśnij cokolwiek aby kontunuwać\r\n";
			_getch();

		}
		pause_thread_ms(1000);

	}

	//TD.join(); 
	if(REFR.joinable()) REFR.join();

	// escaping 
	closeConnectPop3(s);

	//_getch();
	closesocket(s);
	WSACleanup();

	return 0;
}
