#pragma once
/*Author: Marcin Ziajkowski pop3.h */

#include <string.h>
#include <winsock2.h>
#include <iostream>
#include <conio.h>
#include <thread>         // std::thread
#include <chrono>         // std::chrono::seconds
#include <vector>

namespace GLOBALS {
	bool escape = false;
	int index = 0;
	std::string UIDL;
}

std::string ReadByte(SOCKET s);
void closeConnectPop3(SOCKET s);
std::string getUIDL(SOCKET s);
void ReadMessage(SOCKET s, int nr_of_message);
void SingIn(std::string Login, std::string CertainlyNotPassword, SOCKET s);
void f(int ms, SOCKET s);
bool Press_Q();
void pause_thread_ms(int n);
std::vector<std::string> GetParameters();
