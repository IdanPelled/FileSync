#include "communicate.h"
#include "file.h"
#include "config.h"

#include <iostream>

using std::cout;
using std::endl;

Socket::Socket()
{
	// creates a socket and authenticates the connection
	if (!create_socket(sock)) {
		close_socket();
	}

	if (!handshake()) {
		cout << "Can not complate handshake" << endl;
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

bool Socket::update()
{
	Action action = get_action();
	
	switch (action)
	{
	case Action::None:
		cout << "Action: None" << endl;
		break;

	case Action::Upload:
		cout << "Action: Upload" << endl;
		upload();
		break;

	case Action::Download:
		cout << "Action: Download" << endl;
		download();
		break;

	default:
		cout << "Invalid action" << endl;
		close_socket();
	}

	return true;
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

bool auth_handshake(const char* data, int length)
{
	return strncmp(HANDSHAKE, data, length) == 0;
}

bool Socket::handshake()
{
	// Handshake between the client and the server
	// TODO: auth
	const string msg = HANDSHAKE;
	const int length = 14;
	char buffer[length];

	return {
		send_data(msg)
		&& read_data(buffer, length)
		&& auth_handshake(buffer, length)
	};
}

bool validate_action(const char* action)
{
	return isdigit(action[0]);
}

Action Socket::get_action()
{
	char date[FORMAT_SIZE];
	char buffer[1];

	if (!get_last_modification_date(date)) {
		cout << "Can not get file information for PATH" << endl;
		close_socket();
	}
	
	if (
		send_data(string(date))
		&& read_data(buffer, 1)
		&& validate_action(buffer)
	) {
		return Action(*buffer - '0');
	}
		
	
	else {
		cout << "Can not get action from server" << endl;
		close_socket();
	}
}

bool Socket::upload()
{
	char folder[] = "jn45k3jk53blk2308423kj4n24k234 m2b 4mn23 423k4ndmfsfd9ssgsfsd , mn52 3n4m 23nm423 n45j35j35b232342mnzd;lgsdglsgdmsdl;gsmgk;325n2l35n2m352352jn45k3jk53blk2308423kj4n24k234 m2b 4mn23 423k4ndmfsfd9ssgsfsd , mn52 3n4m 23nm423 n45j35j35b232342mnzd;lgsdglsgdmsdl;gsmgk;325n2l35n2m352352jn45k3jk53blk2308423kj4n24k234 m2b 4mn23 423k4ndmfsfd9ssgsfsd , mn52 3n4m 23nm423 n45j35j35b232342mnzd;lgsdglsgdmsdl;gsmgk;325n2l35n2m352352jn45k3jk53blk2308423kj4n24k234 m2b 4mn23 423k4ndmfsfd9ssgsfsd , mn52 3n4m 23nm423 n45j35j35b232342mnzd;lgsdglsgdmsdl;gsmgk;325n2l35n2m352352";
	char* zip_buffer = (char*) malloc(get_buffer_max_size(folder, strlen(folder)));
	size_t buffer_size;
	long long index = 0;

	if (!zip_folder(folder, zip_buffer, buffer_size)) {
		std::cout << GetLastError() << std::endl;
		close_socket();
		free(zip_buffer);
	}

	std::cout << buffer_size << std::endl;
	send_data(zip_buffer, buffer_size);
	send_data("Done", 5);

	std::cout << "W!" << std::endl;
	free(zip_buffer);
	return true;
}

bool Socket::download()
{
	return false;
}

bool Socket::create_socket(SOCKET& sock)
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
	server.sin_port = htons(PORT);
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

