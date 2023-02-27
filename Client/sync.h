#pragma once
#include "communicate.h"
#include "auth.h"
#include "config.h"


enum class Action {
	None,
	Upload,
	Download,
};


class Sync {
public:
	Sync();

private:
	bool update();
	bool authenticate();
	bool upload();
	bool download();
	TokenStatus send_token(const string& token);
	Action get_action();

	Socket sock { get_int_config("server", "sync_port") };
};