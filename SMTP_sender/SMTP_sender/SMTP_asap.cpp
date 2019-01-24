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
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#pragma warning(disable : 4996)

void SendEmail(char *server, char *to, char* from, char *subject, char *message);

int main()
{
	std::cout << "Hello World!\n";
	SendEmail(const_cast<char*>("smtp.wp.pl"), const_cast<char*>("grouplong0@wp.pl"), const_cast<char*>("yinyang69@wp.pl"), const_cast<char*>("Tesoway temat"), const_cast<char*>("testowa wiadomosc"));

	return 0;
}

void SendEmail(char *server, char *to, char* from, char *subject, char *message)
{
	SOCKET sockfd;
	WSADATA wsaData;
	hostent *host;
	sockaddr_in dest;

	int sent;
	char line[200];
	char buferek[512];
	memset(buferek, 0, 512);

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

			sent = recv(sockfd, buferek, 512, 0); // VXNlcm5hbWU6 -> username:
			memset(buferek, 0, 512);

			//---------

			strcpy_s(line, "eWlueWFuZzY5\n"); //yinyang69 -> eWlueWFuZzY5
			sent = send(sockfd, line, strlen(line), 0);
			Sleep(5);

			sent = recv(sockfd, buferek, 512, 0); // UGFzc3dvcmQ6 -> password:
			memset(buferek, 0, 512);

			//---------

			strcpy_s(line, "cnV3bm93YWdhNjk=\n"); //ruwnowaga69 -> cnV3bm93YWdhNjk=
			sent = send(sockfd, line, strlen(line), 0);
			Sleep(5);

			sent = recv(sockfd, buferek, 512, 0); // 
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

