#include "file.h"


size_t create_tar_from_folder(const string& folder_path, char*& out_tar, const size_t buffer_size)
{
    std::size_t tar_size = 0;

    // Initialize the libarchive writer
    struct archive* a = archive_write_new();
    archive_write_add_filter_gzip(a);
    archive_write_set_format_pax_restricted(a);

    // Initialize the in-memory buffer to hold the tar archive
    char* buffer = new char[buffer_size];
    struct archive* memory_writer = archive_write_new();
    archive_write_set_format_ustar(memory_writer);
    archive_write_open_memory(memory_writer, buffer, buffer_size, &tar_size);

    try {
        // Recursively iterate through the folder and its subdirectories
        std::filesystem::path root_path(folder_path);
        for (const auto& entry : std::filesystem::recursive_directory_iterator(folder_path)) {
            struct archive_entry* file_entry = archive_entry_new();

            // Get the relative path of the file/directory
            std::filesystem::path relative_path = std::filesystem::relative(entry.path(), root_path);

            archive_entry_copy_pathname(file_entry, relative_path.string().c_str());

            if (entry.is_regular_file()) {
                archive_entry_set_filetype(file_entry, AE_IFREG);
                archive_entry_set_size(file_entry, entry.file_size());

                archive_write_header(memory_writer, file_entry);

                std::ifstream file_stream(entry.path().string(), std::ios::binary);
                char* file_buffer = new char[entry.file_size()];
                file_stream.read(file_buffer, entry.file_size());
                archive_write_data(memory_writer, file_buffer, entry.file_size());
                delete[] file_buffer;
            }
            else if (entry.is_directory()) {
                archive_entry_set_filetype(file_entry, AE_IFDIR);
                archive_write_header(memory_writer, file_entry);
            }

            archive_entry_free(file_entry);
        }

    }
    
    catch (const std::filesystem::filesystem_error& ex) {
        logger->error(ex.what());
    }
    // Finalize the libarchive writer and extract the tar archive from the in-memory buffer
    archive_write_close(memory_writer);
    archive_write_free(memory_writer);

    archive_write_open_filename(a, buffer);
    archive_write_open(a, nullptr, nullptr, nullptr, nullptr);
    archive_write_data(a, buffer, tar_size);
    archive_write_free(a);

    // Set the output tar archive and its size
    out_tar = buffer;

    return tar_size;
}

bool extract_tar_to_folder(const string& output_folder, const char* tar_data, const size_t tar_size)
{
    // Initialize the libarchive reader
    struct archive* a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    // Initialize the in-memory buffer to hold the tar archive
    struct archive* memory_reader = archive_read_new();
    archive_read_support_format_all(memory_reader);
    archive_read_open_memory(memory_reader, tar_data, tar_size);

    // Read the tar archive from the in-memory buffer
    struct archive_entry* entry;
    while (archive_read_next_header(memory_reader, &entry) == ARCHIVE_OK) {
        const char* entry_path = archive_entry_pathname(entry);
        std::filesystem::path output_path = std::filesystem::path(output_folder) / std::filesystem::path(entry_path);
        std::filesystem::create_directories(output_path.parent_path());
        std::ofstream output_file(output_path, std::ios::binary | std::ios::trunc);

        if (archive_entry_size(entry) > 0) {
            char* buffer = new char[archive_entry_size(entry)];
            archive_read_data(memory_reader, buffer, archive_entry_size(entry));
            output_file.write(buffer, archive_entry_size(entry));
            delete[] buffer;
        }

        output_file.close();
    }

    // Finalize the libarchive reader and free memory
    archive_read_close(memory_reader);
    archive_read_free(memory_reader);
    archive_read_free(a);

    return true;
}

bool compress(const char* input, size_t input_len, string& output) {
    // Allocate space for the compressed output
    size_t max_output_len = snappy::MaxCompressedLength(input_len);
    output.resize(max_output_len);

    // Compress the input
    size_t compressed_len = snappy::Compress(input, input_len, &output);
    if (compressed_len == 0)
        return false;

    // Resize the output buffer to match the actual compressed length
    output.resize(compressed_len);

    return true;
}

bool decompress(const char* input, size_t input_len, string& output) {
    // Allocate space for the decompressed output
    size_t uncompressed_len;
    if (!snappy::GetUncompressedLength(input, input_len, &uncompressed_len)) {
        logger->error("error in GetUncompressedLength");
        return false;
    }
    output.resize(uncompressed_len);

    // Decompress the input
    if (!snappy::Uncompress(input, input_len, &output)) {
        logger->error("error in Uncompress");
        return false;
    }

    return true;
}

string xor_crypt(const string& data, const string& key) {
    std::string result;

    for (size_t i = 0; i < data.length(); i++) {
        result += data[i] ^ key[i % key.length()];
    }

    return result;
}

bool load_folder(const string path, const string key, string& encrypted_file)
{
    size_t buffer_size = 1024 * 1024 * 10;
    char* buffer = new char[buffer_size];
    string zip_file;

    size_t tar_size = create_tar_from_folder(path, buffer, buffer_size);
    if (tar_size <= 0) {
        delete[] buffer;
        logger->error("error occured while creating tar archive");
        return false;
    }

    if (!compress(buffer, tar_size, zip_file)) {
        delete[] buffer;
        logger->error("error occured while compressing folder");
        return false;
    }

    encrypted_file = xor_crypt(zip_file, key);
    delete[] buffer;

    logger->info("sucessfully compressed tar folder");
    return true;
}

bool save_folder(const string path, const string key, const string& encrypted_file)
{
    string zip_file, tar_file;

    zip_file = xor_crypt(encrypted_file, key);
    if (!decompress(zip_file.c_str(), zip_file.length(), tar_file)) {
        logger->error("error occured while decompressing folder");
        return false;
    }

    if (!extract_tar_to_folder(path, tar_file.c_str(), tar_file.length())) {
        logger->error("error occured while extracting tar archive");
        return false;
    }


    logger->info("sucessfully extracted tar archive");
    return true;
}

void refresh_file_system()
{
    string path = get_config("app", "app_path") + "UpdateModDate.ps1 ";
    system(("powershell.exe " + path + get_config("app", "folder")).c_str());
}

bool get_file_information(struct stat& data) {
    return stat(get_folder_path().c_str(), &data) == 0;
}

const string get_last_modification_date()
{
    // returns the folders last modification date
    struct stat data;
    tm time;
    char* buffer = new char[FORMAT_LENGTH];

    refresh_file_system();
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


