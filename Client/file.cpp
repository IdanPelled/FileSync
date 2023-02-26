#include "file.h"

#define BUF_SIZE 1024
#define TIME_FORMAT "%d-%m-%y %H:%M:%S"
#define FORMAT_LENGTH 18


extern std::shared_ptr<spdlog::logger> logger;


bool get_file_information(struct stat& data) {
	return stat(get_folder_path().c_str(), &data) == 0;
}


const string get_last_modification_date()
{
	// returns the folders last modification date
	struct stat data;
	tm time;
	char* buffer = new char[FORMAT_LENGTH];
	
	if (get_file_information(data)) {
		gmtime_s(&time, &(data.st_mtime));
		strftime(
			buffer,
			FORMAT_LENGTH,
			TIME_FORMAT,
			&time
		);

		string date = { buffer };
		delete[] buffer;
		return date;
	}
	
	logger->error("can not get file information");
	delete[] buffer;
	return string();
}


bool save_folder(const char* folder) {
	return true;
}


void add_directory_to_zip(mz_zip_archive* zip_archive, const char* path) {
    mz_zip_writer_add_path(zip_archive, path, NULL, MZ_BEST_COMPRESSION);
    char search_path[BUF_SIZE];
    snprintf(search_path, BUF_SIZE, "%s/*", path);
    void* dir_handle = mz_os_open_dir(search_path);
    if (dir_handle) {
        mz_dir_entry dir_entry;
        while (mz_os_read_dir(dir_handle, &dir_entry)) {
            if (dir_entry.m_is_dir && strcmp(dir_entry.m_filename, ".") != 0 && strcmp(dir_entry.m_filename, "..") != 0) {
                char sub_path[BUF_SIZE];
                snprintf(sub_path, BUF_SIZE, "%s/%s", path, dir_entry.m_filename);
                add_directory_to_zip(zip_archive, sub_path);
            }
            else if (!dir_entry.m_is_dir) {
                char file_path[BUF_SIZE];
                snprintf(file_path, BUF_SIZE, "%s/%s", path, dir_entry.m_filename);
                mz_zip_writer_add_file(zip_archive, dir_entry.m_filename, file_path, NULL, 0, MZ_BEST_COMPRESSION);
            }
        }
        mz_os_close_dir(dir_handle);
    }
}


char* compress_folder(const char* folder_path, size_t* zip_size) {
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));
    if (!mz_zip_writer_init_heap(&zip_archive, 0)) {
        return NULL;
    }
    add_directory_to_zip(&zip_archive, folder_path);
    void* zip_data = mz_zip_writer_finalize_heap(&zip_archive, zip_size);
    char* buffer = new char[*zip_size];
    memcpy(buffer, zip_data, *zip_size);
    mz_zip_writer_end(&zip_archive);
    return buffer;
}


void extract_zip(const char* zip_buffer, size_t zip_size, const char* directory) {
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));
    if (!mz_zip_reader_init_mem(&zip_archive, zip_buffer, zip_size, 0)) {
        return;
    }
    for (int i = 0; i < mz_zip_reader_get_num_files(&zip_archive); i++) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
            continue;
        }
        char* path = new char[BUF_SIZE];
        snprintf(path, BUF_SIZE, "%s/%s", directory, file_stat.m_filename);
        if (file_stat.m_is_directory) {
            mz_dir_make(path);
        }
        else {
            void* file_handle = mz_os_fopen(path, "wb");
            if (file_handle) {
                mz_zip_reader_extract_to_callback(&zip_archive, i, [](void* pOpaque, mz_uint64 file_ofs, const void* pBuf, size_t n) {
                    mz_os_fwrite(pBuf, n, 1, pOpaque);
                    return true;
                    }, file_handle, 0);
                mz_os_fclose(file_handle);
            }
        }
        delete[] path;
    }
    mz_zip_reader_end(&zip_archive);
}

