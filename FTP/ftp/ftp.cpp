// ftp.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include <string.h>



using namespace std;
#pragma warning(disable : 4996)


int main()
{
    string User = "";
    string Pass = "";
    string Server = "";
    std::cout << "Hello World!\n"; 
	SOCKET sockfd;
	WSADATA wsaData;
	hostent *host;
	sockaddr_in dest;

	SOCKET return_socket;
	sockaddr_in dest2;

	int sent;
	char line[1024];
	char buferek[512];
	memset(buferek, 0, 512);

	if(WSAStartup(0x202, &wsaData) != SOCKET_ERROR) std::cout << "oksy" << std::endl;
	else return -1;

	char *server = Server.c_str();
	host = gethostbyname(server);
	std::cout << "host: " << host  << std::endl;

	// SOCKET 21
	memset(&dest, 0, sizeof(dest));
	memcpy(&(dest.sin_addr), host->h_addr, host->h_length);

	dest.sin_family = host->h_addrtype;
	dest.sin_port = htons(21);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	sent = connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));

	cout << "sent21: " << sent << endl;
	memset(buferek, 0, 512);

	sent = recv(sockfd, buferek, 512, 0);
	cout << "begin21 rec: " << buferek << endl;
	memset(buferek, 0, 512);

	// SOCKET 20
	memset(&dest2, 0, sizeof(dest2));
	memcpy(&(dest2.sin_addr), host->h_addr, host->h_length);

	dest2.sin_family = host->h_addrtype;
	dest2.sin_port = htons(22);

	return_socket = socket(AF_INET, SOCK_STREAM, 0);
	sent = connect(return_socket, (struct sockaddr*)&dest2, sizeof(dest2));

	cout << "sent20: " << sent << endl;
	memset(buferek, 0, 512);

	//----------------------------


	strcpy_s(line, "USER ");
    strcat_s(line, User.c_str());
    strcat_s(line, "\r\n");
    
	sent = send(sockfd, line, strlen(line), 0);
	Sleep(5);

	sent = recv(sockfd, buferek, 512, 0);
	cout << "log rec: " << buferek << endl;
	memset(buferek, 0, 512);

	strcpy_s(line, "PASS ");
    strcat_s(line, Pass.c_str());
    strcat_s(line, "\r\n");
	sent = send(sockfd, line, strlen(line), 0);
	Sleep(5);

	sent = recv(sockfd, buferek, 512, 0);
	cout << "pass rec: " << buferek << endl;
	memset(buferek, 0, 512);
	//....


	//sent = recv(sockfd, buferek, 512, 0);
	//cout << "22 odbior: " << buferek << endl;
	//memset(buferek, 0, 512);

	strcpy_s(line, "LIST\r\n");
	sent = send(sockfd, line, strlen(line), 0);
	Sleep(5);

	sent = recv(return_socket, buferek, 512, 0);
	cout << "20 list: " << buferek << endl;
	memset(buferek, 0, 512);


	//quit
	strcpy_s(line, "QUIT\n");
	sent = send(sockfd, line, strlen(line), 0);
	Sleep(5);

	sent = recv(sockfd, buferek, 512, 0);
	cout << "S2 rec: " << buferek << endl;
	memset(buferek, 0, 512);

	closesocket(sockfd);
	WSACleanup();

	return 0;

}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
