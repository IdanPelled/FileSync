#include "sync.h"


extern std::shared_ptr<spdlog::logger> logger;


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
	return true;
}


bool Sync::download()
{
	logger->info("Downloading....");
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
	char response;
	const string date = get_last_modification_date();

	if (date != "") {
		sock.send_data(date);
		sock.read_data(&response, 1);
		return Action(response - '0');
	}
	
	return Action::None;
}