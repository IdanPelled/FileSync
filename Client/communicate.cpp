#include "communicate.h"
#include "config.h"

#define LENGTH_HEADER 4

extern std::shared_ptr<spdlog::logger> logger;


Socket::Socket(int port)
{
	// creates a socket
	if (!create_socket(sock, port)) {
		logger->error("unable to create the socket");
		close_socket();
	}
}

void Socket::close_socket()
{
	// safely close the program
	this->~Socket();
	exit(1);
}

Socket::~Socket()
{
	// closes the socket and winsock dll
	if (sock)
		closesocket(sock);
	else
		logger->error("Can not close socket - alreay closed");
	
	WSACleanup();
}

bool Socket::read_data(char* buffer, int length)
{
	// Reads data and checks for errors
	int status_code = recv(sock, buffer, length, 0);

	if (status_code < 0) {
		logger->error("Faild reading data with status code {}", WSAGetLastError());
		close_socket();
	}

	return true;
}

bool Socket::send_data(const string& data)
{
	return send_data((void*) data.c_str(), data.size());
}


bool Socket::send_data(void* data, const int length)
{
	// Sends data and checks for errors	
	int status_code = send(sock, (const char*) data, length, 0);
	
	if (status_code == SOCKET_ERROR) {
		logger->error("Faild sending data with status code {}", WSAGetLastError());
		close_socket();
	}

	return true;
}


bool Socket::create_socket(SOCKET& sock, int port)
{
	// Creates a socket and connects to server
	WSADATA WSAData;
	SOCKADDR_IN server;
	int status_code;

	// init winsock dll
	status_code = WSAStartup(WS_VERSION, &WSAData);
	if (status_code != NO_ERROR) {
		logger->error("WSAStartup faild");
		return false;
	}

	// fill connection info
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	string ip = get_config("server", "ip");
	std::wstring ip_w = { ip.begin(), ip.end() };
	InetPton(AF_INET, ip_w.c_str(), &server.sin_addr.s_addr);

	// create socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		logger->error("Socket creation faild");
		return false;
	}

	// connect to server
	status_code = connect(sock, (SOCKADDR*) &server, sizeof(server));
	if (status_code == SOCKET_ERROR) {
		logger->error("Socket connection faild");
		return false;
	}

	return true;
}

