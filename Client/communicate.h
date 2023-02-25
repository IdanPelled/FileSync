#pragma once

// winsock2 requirement
#pragma comment(lib,"ws2_32.lib")
#define WS_VERSION 0x0202

#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

#include <string>
#include <stdlib.h>
#include <iostream>


#include "file.h"
#include "config.h"
#include "logger.h"


using std::string;
using std::cout;
using std::endl;


class Socket {
public:
	Socket(int port);
	~Socket();

	bool create_socket(SOCKET& sock, int port);
	void close_socket();
	bool read_data(char* buffer, int length);
	bool send_data(void* data, const int length);
	bool send_data(const string& data);

	SOCKET sock;
};
