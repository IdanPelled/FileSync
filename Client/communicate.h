#pragma once

// winsock2 requirement
#pragma comment(lib,"ws2_32.lib")
#define WS_VERSION 0x0202

#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

#include <string>
#include <stdlib.h>
#include <iostream>


#include "file.h"
#include "config.h"
#include "logger.h"


using std::string;
using std::cout;
using std::endl;


/**
 * @brief Represents a socket that can be used to communicate over the network.
 */
class Socket {
public:
    /**
     * @brief Constructs a `Socket` object.
     *
     * @param port The port to bind the socket to.
     */
    Socket(int port);

    /**
     * @brief Destroys the `Socket` object.
     */
    ~Socket();

    /**
     * @brief Creates a socket and binds it to the specified port.
     *
     * @param sock The socket to create and bind to the port.
     * @param port The port to bind the socket to.
     *
     * @return `true` if the socket was successfully created and bound, `false` otherwise.
     */
    bool create_socket(SOCKET& sock, int port);

    /**
     * @brief Closes the socket.
     */
    void close_socket();

    /**
     * @brief Reads data from the socket into the specified buffer.
     *
     * @param buffer The buffer to read the data into.
     * @param length The maximum number of bytes to read.
     *
     * @return `true` if the data was successfully read, `false` otherwise.
     */
    bool read_data(char* buffer, int length);

    /**
     * @brief Sends data over the socket.
     *
     * @param data The data to send.
     * @param length The length of the data to send.
     *
     * @return `true` if the data was successfully sent, `false` otherwise.
     */
    bool send_data(void* data, const int length);

    /**
     * @brief Sends data over the socket.
     *
     * @param data The data to send.
     *
     * @return `true` if the data was successfully sent, `false` otherwise.
     */
    bool send_data(const string& data);

    /**
     * @brief The socket object that represents the connection.
     */
    SOCKET sock;
};

