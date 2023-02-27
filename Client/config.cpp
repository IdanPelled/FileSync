#include "config.h"

extern std::shared_ptr<spdlog::logger> logger;

const string get_config(string section, string name)
{
	/* return a configuration as a string */

	mINI::INIFile file(CONFIG_PATH);
	mINI::INIStructure ini;
	file.read(ini);
	return ini[section][name];
}

void set_config(string section, string name, string val)
{
	/* return a configuration as a string */

	mINI::INIFile file(CONFIG_PATH);
	mINI::INIStructure ini;
	file.read(ini);
	ini[section][name] = val;
	file.write(ini);
}

const int get_int_config(string section, string name)
{
	/* return a configuration as an int */

	string s = get_config(section, name);
	int i;

	if (sscanf_s(s.c_str(), "%d", &i) == EOF) {
		logger->error("invalid config type ({}, {})", section, name);
		return int();
	}
	return i;
}

const string get_folder_path() {
	return get_config("app", "folder");
}

const string get_username() {
	return get_config("app", "username");
}

const string get_password() {
	return get_config("app", "password");
}