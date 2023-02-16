#pragma once
#pragma comment(lib,"Cabinet.lib")

#include <Windows.h>
#include <time.h>
#include <sys/stat.h>
#include <compressapi.h>


int get_file_size(const char* path);
bool get_file_information(struct stat& data);
bool get_last_modification_date(char* buffer);
size_t get_buffer_max_size(const char* buffer, size_t buffer_length);
bool zip_folder(
	const char* in_buffer,
	char* out_buffer,
	size_t& out_data_length
);

//TODO:
bool zip(
	const char* in_buffer,
	char* out_buffer,
	size_t& out_data_length
);
bool save_folder(const char* folder);