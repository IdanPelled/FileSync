#include "auth.h"

extern std::shared_ptr<spdlog::logger> logger;


bool Auth::renew_token() {
	// asks and saves the new token

	char* token_buffer = new char[TOKEN_LENGTH + 1];
	token_buffer[TOKEN_LENGTH] = '\0';

	if (ask_for_token(token_buffer)) {
		string str(token_buffer);
		save_token(str);
		delete[] token_buffer;
		return true;
	}

	logger->error("could not get token");
	delete[] token_buffer;
	return false;

	
}

void Auth::save_token(string& token) {
	// saves token to config file

	set_config("app", "token", token);
}

bool send_user_info(Socket& sock, string field) {
	
	uint32_t length = field.length();
	sock.send_data(reinterpret_cast<char*>(&length));
	sock.send_data(field);

	char status;
	if (sock.read_data(&status, 1))
		return status == '0';

	return false;
}


bool Auth::ask_for_token(char* token) {
	// sends the username and password and returns the token
	Socket sock { get_int_config("server", "login_port") };
	char status_code;

	string username = get_username();
	string password = get_password();
	
	return (
		send_user_info(sock, username)
		&& send_user_info(sock, password)
		&& sock.read_data(&status_code, 1)
		&& status_code == '0'
		&& sock.read_data(token, TOKEN_LENGTH)
	);
}
