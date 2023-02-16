#include "communicate.h"


Socket::Socket(int port)
{
	// creates a socket and authenticates the connection
	if (!create_socket(sock, port)) {
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
		cout << "Can not close socket - alreay closed" << endl;
	
	WSACleanup();
}

bool Socket::read_data(char* buffer, const int length)
{
	// Reads data and checks for errors
	int status_code = recv(sock, buffer, length, 0);

	if (status_code < 0) {
		cout << "Faild reading data with status code " << WSAGetLastError() << endl;
		close_socket();
	}

	return true;
}

bool Socket::send_data(const string& data)
{
	return send_data(data.c_str(), data.size());
}

bool Socket::send_data(const char* data, const int length)
{
	// Sends data and checks for errors
	int status_code = send(sock, data, length, 0);
	
	if (status_code == SOCKET_ERROR) {
		cout << "Faild sending data with status code " << WSAGetLastError() << endl;
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
		cout << "WSAStartup faild" << endl;
		return false;
	}

	// fill connection info
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	InetPton(AF_INET, _T(IP), &server.sin_addr.s_addr);

	// create socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		cout << "Socket creation faild" << endl;
		return false;
	}

	// connect to server
	status_code = connect(sock, (SOCKADDR*) &server, sizeof(server));
	if (status_code == SOCKET_ERROR) {
		cout << "Socket connection faild" << endl;
		return false;
	}

	return true;
}

