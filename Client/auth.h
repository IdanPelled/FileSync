#pragma once
#include "communicate.h"

#define TOKEN_LENGTH 127


// Enumerates possible token statuses.
enum class TokenStatus {
	Valid,
	Invalid,
	Outdated,
};


// This is a module for authentication functions, including login, signup, and token renewal.
class Auth {
public:
	/* Asks for a new token using the given username and password, saves the token, and returns true if successful.
	Otherwise, logs an error and returns false.
	Args:
	  username (const string&): the user's username
	  password (const string&): the user's password
	Returns:
	  bool: true if the token was successfully renewed and saved, false otherwise */
	bool renew_token(const string& username, const string& password);

	/* Logs in the user using the given username and password, saves the username and password to the config file, 
	and returns true if successful. Otherwise, returns false.
	Args:
	  username (const string&): the user's username
	  password (const string&): the user's password
	Returns:
	  bool: true if the user was successfully logged in, false otherwise */
	bool login(const string& username, const string& password);

	/* Signs up the user using the given username and password, and returns true if successful. 
	Otherwise, returns false.
	Args:
	  username (const string&): the user's username
	  password (const string&): the user's password
	Returns:
	  bool: true if the user was successfully signed up, false otherwise */
	bool signup(const string& username, const string& password);

private:
	/* Saves the given token to the config file.
	Args:
	  token (string&): the token to be saved */
	void save_token(string& token);

	/* Sends the given user information to the given socket, and returns true if the data was sent successfully 
	and the server responded with a status code of '0'. Otherwise, returns false.
	Args:
	  sock (Socket&): the socket to send the data to
	  field (string): the user information to be sent
	Returns:
	  bool: true if the user information was sent successfully and the server responded with a status code of '0',
	  false otherwise */
	bool send_user_info(Socket& sock, string field);

	/* Asks the server for a new token using the given username and password, and returns true if successful. 
	Otherwise, returns false.
	Args:
	  token (char*): a pointer to a buffer where the token will be stored
	  username (const string&): the user's username
	  password (const string&): the user's password
	Returns:
	  bool: true if the token was successfully obtained from the server, false otherwise */
	bool ask_for_token(char* token, const string& username, const string& password);
};
