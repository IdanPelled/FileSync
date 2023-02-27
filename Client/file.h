#pragma once
#include "config.h"
#include "logger.h"


#pragma comment(lib,"Cabinet.lib")

#include <Windows.h>
#include <time.h>
#include <sys/stat.h>
#include <compressapi.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include "miniz.h"

using std::string;


bool get_file_information(struct stat& data);
const string get_last_modification_date();
bool save_folder(const char* folder);
void extract_zip(const char* zip_buffer, size_t zip_size, const char* directory);
char* compress_folder(const char* folder_path, size_t* zip_size);