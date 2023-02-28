#pragma once
#include "communicate.h"
#include "auth.h"
#include "config.h"


/**
 * @brief An enum class representing actions to be performed in synchronization.
 */
enum class Action {
	None,
	Upload,
	Download,
};


class Sync {
public:
	/**
	 * @brief Constructor for the Sync class that initializes the synchronization process.
	 */
	Sync();

private:
	/**
	 * @brief Checks for a valid authentication token and performs the required action based on the token.
	 * 
	 * @return true if the synchronization was successful, false otherwise.
 	 */
	bool update();
	
	/**
	 * @brief Authenticates the user via token.
	 * 
	 * If the token is invalid, it tries to renew it by calling the renew_token() function from the Auth class.
	 * 
	 * @return true if authentication succeeded, false otherwise.
	 */
	bool authenticate();

	/**
	 * @brief Uploads data to a remote server.
	 * 
	 * @return true if the upload was successful.
	 */
	bool upload();

	/**
	 * @brief Downloads data from a remote server.
	 * 
	 * @return true if the download was successful.
	 */
	bool download();

	/**
	 * @brief Sends a token to the server and returns its status.
	 * 
	 * @param token The token to send.
	 * @return The status of the token.
	 */
	TokenStatus send_token(const string& token);

	/**
	 * @brief Gets the action to be performed on the remote server.
	 * 
	 * @return The action to be performed.
	 */
	Action get_action();


	Socket sock { get_int_config("server", "sync_port") };
};