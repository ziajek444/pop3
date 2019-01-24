// SMTP As simple as possible
// based on:
// https://ep.com.pl/files/3382.pdf
// http://namarginesie.blox.pl/2007/03/HOW-TO-telnet-do-serwera-SMTP.html

#include "pch.h"
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <algorithm>
#include <string.h>
#include <string>
#include <vector>
#include <iterator>
#include "base64.h"

#define CUC const unsigned char 

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#pragma warning(disable : 4996)

void SendEmail(char *server, char *to, char* from, char *subject, char *message, std::string in_login, std::string in_haslo);
bool serwerOK(std::string str);
bool adresatOK(std::string str);
bool nadawcaOK(std::string str);
bool hasloOK(std::string str);

// Przykladowa lista argumentow:
// smtp.wp.pl grouplong0@wp.pl grouplong0@wp.pl haslo1234 "Tesoway temat" "testowa wiadomosc"

int main(int args, char ** argv)
{
	std::cout << args << " - "<<argv[1]; 
	if (args < 7) {
		std::cout << "Za mala liczba argumentow !\r\n";
		std::cout << "wprowadz:\r\n";
		std::cout << "serwer adresat nadawca haslo temat wiadomosc\r\n";
	}
	else if (args > 7)
	{
		std::cout << "Za duzo liczba argumentow !\r\n";
		std::cout << "wprowadz:\r\n";
		std::cout << "serwer adresat nadawca haslo temat wiadomosc\r\n";
	}

	std::string in_serwer    = argv[1];
	std::string in_adresat   = argv[2];
	std::string in_nadawca   = argv[3];
	std::string in_haslo     = argv[4];
	std::string in_temat     = argv[5];
	std::string in_wiadomosc = argv[6];
	
	if (!serwerOK(in_serwer)) std::cout<<"nie poprawna nazwa serwera smtp\r\n";
	if (!adresatOK(in_adresat)) std::cout << "nie poprawna nazwa adresata (adresat@dostawca.pl)\r\n";
	if (!nadawcaOK(in_nadawca)) std::cout << "nie poprawna nazwa nadawcy (nadawca@dostawca.pl)\r\n";
	if (!hasloOK(in_haslo)) std::cout << "za krotkie haslo\r\n";

	std::string in_login = in_nadawca.substr(0, in_nadawca.find_first_of("@"));
	
	SendEmail(const_cast<char*>(in_serwer.c_str()), const_cast<char*>(in_adresat.c_str()), const_cast<char*>(in_nadawca.c_str()), const_cast<char*>(in_temat.c_str()), const_cast<char*>(in_wiadomosc.c_str()),in_login,in_haslo);
	

	return 0;
}

void SendEmail(char *server, char *to, char* from, char *subject, char *message, std::string in_login, std::string in_haslo)
{
	SOCKET sockfd;
	WSADATA wsaData;
	hostent *host;
	sockaddr_in dest;

	int sent;
	char line[200];
	char buferek[512];
	memset(buferek, 0, 512);
	int mp = sizeof(in_login);
	std::string LOGIN = base64_encode(in_login);
	std::string PASSW = base64_encode(in_haslo);
	// dodaje tu eby nie dodawac do wiadomosci (upraszcza)
	LOGIN += "\n";
	PASSW += "\n";

	if (WSAStartup(0x202, &wsaData) != SOCKET_ERROR)
	{
		if ((host = gethostbyname(server)) != NULL) {
			memset(&dest, 0, sizeof(dest));
			memcpy(&(dest.sin_addr), host->h_addr, host->h_length);

			dest.sin_family = host->h_addrtype;
			dest.sin_port = htons(25);

			sockfd = socket(AF_INET, SOCK_STREAM, 0);

			connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));

			strcpy_s(line,"ehlo komputer\n");
			sent = send(sockfd, line, strlen(line), 0);
			Sleep(5);

			sent = recv(sockfd,buferek,512,0);
			memset(buferek,0,512);

			//---------
			//---------
			strcpy_s(line, "AUTH LOGIN\n");
			sent = send(sockfd, line, strlen(line), 0);
			Sleep(5);

			sent = recv(sockfd, buferek, 512, 0);
			memset(buferek, 0, 512);

			//---------

			strcpy_s(line, LOGIN.c_str());
			sent = send(sockfd, line, strlen(line), 0);
			Sleep(5);

			sent = recv(sockfd, buferek, 512, 0);
			memset(buferek, 0, 512);

			//---------

			strcpy_s(line, PASSW.c_str());
			sent = send(sockfd, line, strlen(line), 0);
			Sleep(5);

			sent = recv(sockfd, buferek, 512, 0); 
			memset(buferek, 0, 512);
			//---------
			//---------

			strcpy_s(line, "MAIL FROM:<");
			strncat_s(line, from, strlen(from));
			strncat_s(line, ">\n",3);
			sent = send(sockfd, line, strlen(line), 0);
			Sleep(5);

			sent = recv(sockfd, buferek, 512, 0);
			memset(buferek, 0, 512);

			strcpy_s(line, "RCPT TO:<");
			strncat_s(line, to, strlen(to));
			strncat_s(line, ">\n", 3);
			sent = send(sockfd, line, strlen(line), 0);
			Sleep(5);

			sent = recv(sockfd, buferek, 512, 0);
			memset(buferek, 0, 512);

			strcpy_s(line, "DATA\r\n");
			sent = send(sockfd, line, strlen(line), 0);
			Sleep(5);

			sent = recv(sockfd, buferek, 512, 0);
			memset(buferek, 0, 512);

			strcpy_s(line, "To: ");
			strcat_s(line, to);
			strcat_s(line, "\n");

			strcat_s(line, "From: ");
			strcat_s(line, from);
			strcat_s(line, "\n");

			strcat_s(line, "Subject: ");
			strcat_s(line, subject);
			strcat_s(line, "\n");

			strcat_s(line, "Date: Fri, 25 Jul 2014 18 : 29 : 02 + 0200");
			strcat_s(line, "\n");

			strcat_s(line, "Content - Type: text / plain; charset = \"UTF - 8\"\r\n\r\n");
			strcat_s(line, message);
			strcat_s(line, "\r\n\r\n.\r\n");

			sent = send(sockfd, line, strlen(line), 0);
			Sleep(50);

			sent = recv(sockfd, buferek, 512, 0);
			memset(buferek, 0, 512);

			strcpy_s(line, "quit\n");
			sent = send(sockfd, line, strlen(line), 0);
			Sleep(50);

			sent = recv(sockfd, buferek, 512, 0);
			memset(buferek, 0, 512);


			closesocket(sockfd);
			WSACleanup();

		}
	}
}

bool serwerOK(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::cout << str << "\r\n";
	return (str.find("smtp") == std::string::npos) ? false : true;
}

bool adresatOK(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::cout << str << "\r\n";
	return (str.find("@") == std::string::npos || str.find(".") == std::string::npos) ? false : true;
}

bool nadawcaOK(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::cout << str << "\r\n";
	return (str.find("@") == std::string::npos || str.find(".") == std::string::npos) ? false : true;
}

bool hasloOK(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::cout << str << "\r\n";
	return (str.size() > 6) ? false : true;
}