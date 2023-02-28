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
//#include "miniz.h"

using std::string;

/**
* Get the last modification date of a folder.
* @return A string containing the last modification date of the folder.
* If an error occurs while retrieving the information, an empty string is returned.
*/
bool get_file_information(struct stat& data);


/**
 * Retrieves the last modification date of a file.
 *
 * This function retrieves the last modification date of a file by calling the get_file_information
 * function to get the file's stat data and then converting it to a formatted string.
 *
 * @return A string containing the last modification date in the format "yyyy-mm-dd hh:mm:ss".
 * If the function fails to retrieve the file information, an empty string is returned.
 */
const string get_last_modification_date();

/**
 * Extracts the contents of a ZIP archive from memory to a directory.
 *
 * This function uses the miniz library to read the zip archive from memory
 * and extract its contents to the specified directory.
 *
 * @param zip_buffer A pointer to the buffer containing the zip archive.
 * @param zip_size The size of the zip archive buffer in bytes.
 * @param directory The directory to which the zip archive contents should be extracted.
 */
void extract_zip(const char* zip_buffer, size_t zip_size, const char* directory);

/**
* Compress a folder into a zip archive.
* @param folder_path A pointer to a string containing the path of the folder to compress.
* @param zip_size A pointer to a size_t variable that will store the size of the resulting zip archive.
* @return A pointer to a char array containing the compressed zip archive.
* If an error occurs while creating the zip archive, NULL is returned.
*/
char* compress_folder(const char* folder_path, size_t * zip_size);
