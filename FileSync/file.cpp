#include "file.h"
#include "config.h"

#include <iostream>

// File info
bool get_file_information(struct stat& data) {
	return stat(PATH, &data) == 0;
}

bool get_last_modification_date(char* buffer)
{
	// returns the folders last modification date
	struct stat data;
	tm time;

	if (!get_file_information(data))
		return false;

	gmtime_s(&time, &(data.st_mtime));
	strftime(
		buffer,
		FORMAT_SIZE,
		TIME_FORMAT,
		&time
	);

	return true;
}






// File zip
int get_file_size(const char* path) {
	HANDLE file = CreateFileA(
		path, 0, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL
	);

	long size = GetFileSize(file, NULL);
	CloseHandle(file);

	return size;
}

void get_compressor(COMPRESSOR_HANDLE& file) {
	CreateCompressor(
		COMPRESS_ALGORITHM_XPRESS_HUFF,
		NULL, &file
	);
}

size_t get_buffer_max_size(const char* buffer, size_t buffer_length) {
	size_t max_size;
	COMPRESSOR_HANDLE file;

	get_compressor(file);
	Compress(
		file, buffer, buffer_length,
		NULL, 0,
		(SIZE_T*) &max_size
	);

	return max_size;
}

bool zip_folder(
	const char* in_buffer,
	char* out_buffer,
	size_t& out_data_length
) {
	COMPRESSOR_HANDLE file;
	size_t max_size;
	bool res;

	get_compressor(file);
	res = Compress(
		file, in_buffer, strlen(in_buffer),
		out_buffer, strlen(out_buffer),
		(SIZE_T*) &out_data_length
	);
	CloseCompressor(file);

	return res;
}

//TODO:
bool zip(
	const char* in_buffer,
	char* out_buffer,
	size_t& out_data_length
) {
	const char* str = "ZIPED_DIR";
	memcpy(out_buffer, str, strlen(str));
	out_data_length = strlen(str);
	return true;
}

bool save_folder(const char* folder) {
	return true;
}