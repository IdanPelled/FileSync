#pragma once
#include "communicate.h"

#define TOKEN_LENGTH 127

enum class TokenStatus {
	Valid,
	Invalid,
	Outdated,
};

class Auth {
public:
	static bool renew_token(const string& username, const string& password);
	static bool login(const string& username, const string& password);
	static bool signup(const string& username, const string& password);

private:
	static void save_token(string& token);
	static bool ask_for_token(char* token, const string& username, const string& password);
	static bool send_user_info(Socket& sock, string field);
};
