#include "sync.h"


Sync::Sync()
{
	if (!update())
		logger->error("did not sync");
	else
		logger->info("syncronized successfuly");
}

bool Sync::update()
{
	/* If the token is valid, executes an action. */

	if (!authenticate()) {
		return false;
	}

	Action action = get_action();
	switch (action)
	{
	case Action::Upload:
		return upload();

	case Action::Download:
		return download();

	case Action::None:
	default:
		logger->info("No action");
		break;
	}

	return true;
}

inline const string read_token()
{
	return get_config("app", "token");
}

bool Sync::authenticate()
{
	/* Authenticates the user via token.
	* If the token is invaid, tries to renew it.
	* Returns true if auth worked, false otherwise. */

	const string token = read_token();
	const string username = get_username();
	const string password = get_password();

	if (username == "" || password == "" || token == "") {
		logger->error("authentication errror: missing credentials");
		return false;
	}

	
	TokenStatus status = send_token(token);
	switch (status)
	{
	case TokenStatus::Valid:
		logger->info("valid token");
		break;
	
	case TokenStatus::Outdated:
		if (Auth::renew_token(username, password)) {
			logger->info("renewed token");
			return false;
		}

		logger->info("error renewing token");
		return false;

	case TokenStatus::Invalid:
	default:
		logger->error("login error - invalid token");
		return false;
	}

	logger->info("logged in successfully");
	return true;

}

uint32_t read_length_header(Socket& sock) {

	char length_buffer[HEADER_LENGTH];
	uint32_t length;

	sock.read_data(length_buffer, HEADER_LENGTH);
	memcpy(&length, length_buffer, HEADER_LENGTH);
	length = ntohl(length);

	if (length == 0) {
		logger->error("error reading archive length form socket");
		return 0;
	}

	return length;
}

void send_length_header(uint32_t length, Socket& sock) {
	length = htonl(length);
	sock.send_data(&length, HEADER_LENGTH);
}

bool Sync::upload()
{
	string encrypted_file;

	logger->info("Uploading....");
	string path = get_config("app", "folder");
	string key = get_config("app", "password");
	

	if (!load_folder(path, key, encrypted_file)) {
		logger->info("error loading folder");
		return false;
	}
	
	send_length_header(encrypted_file.length(), sock);
	sock.send_data(encrypted_file);
	
	logger->info("Done");
	return true;
}

bool read_archive(Socket& sock, string& encrypted_file)
{
	size_t length = read_length_header(sock);
	char* file_buffer = new char[length];
	sock.read_data(file_buffer, length);

	encrypted_file = string(file_buffer, length);
	delete[] file_buffer;

	if (encrypted_file == "")
		return false;
	return true;
}

bool Sync::download()
{
	string encrypted_file;

	logger->info("Downloading....");
	string path = get_config("app", "folder");
	string key = get_config("app", "password");
	

	if (!read_archive(sock, encrypted_file)) {
		logger->error("error reading archive form socket");
		return false;
	}

	if (!save_folder(path, key, encrypted_file)) {
		logger->info("error extracting folder");
		return false;
	}

	logger->info("Done");
	return true;
}

TokenStatus Sync::send_token(const string& token)
{
	char status;

	sock.send_data(token);

	if (sock.read_data(&status, 1)) {
		return TokenStatus(status - '0');
	}

	logger->error("error reading response");
	return TokenStatus::Invalid;
}

Action Sync::get_action()
{
	/* sends the last modification date and 
	return the action to preform */

	char response;
	const string date = get_last_modification_date();

	sock.send_data(date);
	sock.read_data(&response, 1);

	return Action(response - '0');
}