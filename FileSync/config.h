#pragma once
#include "mINI\ini.h"
#include <string>
#include <iostream>


using std::string;
using std::cout;
using std::endl;

#define CONFIG_PATH "config.ini"


string get_config(string section, string name);
int get_int_config(string section, string name);
string get_folder_path();