#pragma once
#include "mINI\ini.h"
#include <string>
#include <iostream>


using std::string;
using std::cout;
using std::endl;

#define CONFIG_PATH "config.ini"


const string get_config(string section, string name);
void set_config(string section, string name, string val);
const int get_int_config(string section, string name);
const string get_folder_path();
const string get_username();
const string get_password();