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
		logger->info("no action");
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

	if (token == "")
	{
		if (Auth::renew_token(username, password))
			logger->info("renewed token");
		
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

		logger->error("login error - invalid username or password");
		return false;

	case TokenStatus::Invalid:
	default:
		logger->error("login error - invalid token");
		return false;
	}

	logger->info("logged in successfully");
	return true;

}


bool Sync::upload()
{
	logger->info("Uploading....");
	
	string path, key, encrypted_file;

	if (!load_folder(path, key, encrypted_file)) {
		logger->info("error loading folder");
		return false;
	}

	sock.send_data(std::to_string(encrypted_file.length()));
	sock.send_data(encrypted_file);
	
	logger->info("Done");
	return true;
}

bool read_archive(Socket& sock, string& encrypted_file)
{
	char* length_buffer = new char[10];
	char* file_buffer;
	long length;

	sock.read_data(length_buffer, 10);
	length = std::stoi(length_buffer);
	if (length == 0) {
		delete[] length_buffer;
		logger->error("error reading archive length form socket");
	}

	file_buffer = new char[length];
	sock.read_data(file_buffer, length);

	encrypted_file = string(file_buffer);
	delete[] length_buffer;
	delete[] file_buffer;

	if (encrypted_file == "") {
		logger->error("error reading archive form socket");
		return false;
	}

	return true;
}

bool Sync::download()
{
	string path, key, encrypted_file;
	logger->info("Downloading....");

	read_archive(sock, encrypted_file);
	
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