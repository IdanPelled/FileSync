#include "auth.h"

extern std::shared_ptr<spdlog::logger> logger;


bool Auth::renew_token(const string& username, const string& password) {
	char* token_buffer = new char[TOKEN_LENGTH + 1];
	token_buffer[TOKEN_LENGTH] = '\0';

	if (ask_for_token(token_buffer, username, password)) {
		string token = string(token_buffer);
		logger->info(token);
		set_config("app", "token", token);
		delete[] token_buffer;
		return true;
	}

	logger->error("wrong username or password");
	delete[] token_buffer;
	return false;

	
}

bool Auth::login(const string& username, const string& password) {
	if (renew_token(username, password)) {
		set_config("app", "username", username);
		set_config("app", "password", password);
		return true;
	}

	return false;
}


bool Auth::signup(const string& username, const string& password) {
	Socket sock{ get_int_config("server", "signup_port") };
	char status_code;

	return (
		send_user_info(sock, username)
		&& send_user_info(sock, password)
		&& sock.read_data(&status_code, 1)
		&& status_code == '0'
	);
}


bool Auth::send_user_info(Socket& sock, string field) {
	uint32_t length = field.length();
	sock.send_data(reinterpret_cast<char*>(&length));
	sock.send_data(field);

	char status;
	if (sock.read_data(&status, 1))
		return status == '0';

	return false;
}


bool Auth::ask_for_token(
	char* token,
	const string& username,
	const string& password
) {
	Socket sock { get_int_config("server", "login_port") };
	char status_code;

	
	return (
		send_user_info(sock, username)
		&& send_user_info(sock, password)
		&& sock.read_data(&status_code, 1)
		&& status_code == '0'
		&& sock.read_data(token, TOKEN_LENGTH)
	);
}
