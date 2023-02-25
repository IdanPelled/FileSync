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
	static bool renew_token();

private:
	static void save_token(string& token);
	static bool ask_for_token(char* token);
};
