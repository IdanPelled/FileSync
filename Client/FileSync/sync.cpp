#include "sync.h"


Sync::Sync()
{
	if (!update())
		cout << "did not sync" << endl;
	else
		cout << "syncronized successfuly" << endl;
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
		cout << "no action" << endl;
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
	if (token == "")
		if (Auth::renew_token()) {
			cout << "renewed token" << endl;
			return false;
		}
	
	TokenStatus status = send_token(token);
	switch (status)
	{
	case TokenStatus::Valid:
		cout << "valid token" << endl;
		break;
	
	case TokenStatus::Outdated:
		if (Auth::renew_token()) {
			cout << "renewed token" << endl;
			return false;
		}

		cout << "login error - invalid username or password" << endl;
		return false;

	case TokenStatus::Invalid:
	default:
		cout << "login error - invalid token" << endl;
		return false;
	}

	cout << "logged in successfully" << endl;
	return true;

}


bool Sync::upload()
{
	cout << "Uploading...." << endl;
	return true;
}


bool Sync::download()
{
	cout << "Downloading...." << endl;
	return true;
}

TokenStatus Sync::send_token(const string& token)
{
	char status;

	sock.send_data(token);

	if (sock.read_data(&status, 1)) {
		return TokenStatus(status - '0');
	}

	cout << "error reading response" << endl;
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