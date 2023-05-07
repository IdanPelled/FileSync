#pragma once
#include "logger.h"

#include <mini\ini.h>
#include <string>
#include <iostream>


using std::string;
using std::cout;
using std::endl;

#define CONFIG_PATH "C:\\ProgramData\\FileSync\\Config\\config.ini"

/**
 * @brief Returns a configuration value as a string from a specified section and name.
 *
 * @param section The name of the section in the configuration file.
 * @param name The name of the configuration value to retrieve.
 *
 * @return The configuration value as a string.
 */
const string get_config(string section, string name);

/**
 * @brief Sets a configuration value in the specified section and name.
 *
 * @param section The name of the section in the configuration file.
 * @param name The name of the configuration value to set.
 * @param val The value to set the configuration value to.
 */
void set_config(string section, string name, string val);

/**
 * @brief Returns a configuration value as an integer from a specified section and name.
 *
 * @param section The name of the section in the configuration file.
 * @param name The name of the configuration value to retrieve.
 *
 * @return The configuration value as an integer.
 */
const int get_int_config(string section, string name);

/**
 * @brief Returns the folder path configuration value from the application section.
 *
 * @return The folder path configuration value.
 */
const string get_folder_path();

/**
 * @brief Returns the username configuration value from the application section.
 *
 * @return The username configuration value.
 */
const string get_username();

/**
 * @brief Returns the password configuration value from the application section.
 *
 * @return The password configuration value.
 */
const string get_password();