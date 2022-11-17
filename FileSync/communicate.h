#pragma once

// winsock2 requirement
#pragma comment(lib,"ws2_32.lib")
#define WS_VERSION 0x0202

#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

#include <string>


#define HANDSHAKE "HandshakeHello"

using std::string;

enum class Action {
	None,
	Upload,
	Download,
};

class Socket {
public:
	Socket();
	~Socket();

	bool update();

private:
	bool create_socket(SOCKET& sock);
	void close_socket();
	bool read_data(char* buffer, const int length);
	bool send_data(const char* data, const int length);
	bool send_data(const string& data);
	bool handshake();
	bool upload();
	bool download();
	
	Action get_action();

	

	SOCKET sock;
};
