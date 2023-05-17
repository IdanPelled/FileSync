#pragma once
#include "communicate.h"
#include "config.h"
#include "logger.h"



#include <Windows.h>
#include <time.h>
#include <sys/stat.h>
#include <compressapi.h>
#include <string>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>
#include <snappy.h>
#include <cstdlib>


#define TIME_FORMAT "%d-%m-%y %H:%M:%S"
#define FORMAT_LENGTH 18

using std::string;

/**
 * Creates a tar archive from a given folder.
 * @param folder_path the path to the folder to be archived
 * @param out_tar a pointer to a char array that will hold the tar archive data
 * @param buffer_size the size of the buffer allocated for the tar archive data
 * @return the size of the tar archive data
 */
size_t create_tar_from_folder(const string& folder_path, char*& out_tar, const size_t buffer_size);

/**
 * Extracts a tar archive to a given folder.
 * @param output_folder the path to the output folder
 * @param tar_data a pointer to the tar archive data
 * @param tar_size the size of the tar archive data
 * @return true if extraction succeeded, false otherwise
 */
bool extract_tar_to_folder(const string& output_folder, const char* tar_data, const size_t tar_size);

/**
 * Compresses a character array using Snappy.
 *
 * @param input A pointer to the character array to be compressed.
 * @param input_len The length of the character array to be compressed.
 * @param output A reference to a string that will store the compressed output.
 * @return true if compression succeeded, false otherwise.
 */
bool compress(const char* input, size_t input_len, string& output);

/**
 * Decompresses a character array that was compressed using Snappy.
 *
 * @param input A pointer to the compressed character array.
 * @param input_len The length of the compressed character array.
 * @param output A reference to a string that will store the decompressed output.
 * @return true if decompression succeeded, false otherwise.
 */
bool decompress(const char* input, size_t input_len, string& output);

/**
 * Performs XOR encryption on a given string using the given key.
 *
 * @param data The string to be encrypted.
 * @param key The key to use for encryption.
 * @return The encrypted string.
 */
string xor_crypt(const string& data, const string& key);

/**
 * Creates a tar archive from a given folder, compresses it using
 * Snappy, and performs XOR encryption on the resulting data.
 *
 * @param path The path of the folder to be archived.
 * @param key The key to use for encryption.
 * @param encrypted_file A reference to a string that will store the encrypted archive data.
 * @return true if the process succeeded, false otherwise.
 */
bool load_folder(const string path, const string key, string& encrypted_file);

/**
 * Decrypts the given string using XOR, decompresses it using Snappy,
 * and extracts the contents to the given folder.
 *
 * @param path The path of the folder to extract to.
 * @param key The key to use for decryption.
 * @param encrypted_file The encrypted archive data.
 * @return true if the process succeeded, false otherwise.
 */
bool save_folder(const string path, const string key, const string& encrypted_file);

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