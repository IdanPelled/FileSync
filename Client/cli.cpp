#include "cli.h"


/**
 * Perform a login operation with the given username and password.
 * 
 * Args:
 *  username (string): The username of the user.
 *  password (string): The password of the user.
 * 
 * Returns:
 *  int: 0 if the login was successful, otherwise 1. */
int cli_login(string username, string password) {
    logger->info("Action: login");

    if (Auth::login(username, password)) {
        logger->info("Success");
        return 0;
    }

    logger->error("Failed");
    return 1;
}

/**
 * Perform a signup operation with the given username and password.
 * 
 * Args:
 *  username (string): The username of the user.
 *  password (string): The password of the user.
 * 
 * Returns:
 *  int: 0 if the signup was successful, otherwise 1. */
int cli_signup(string username, string password) {
    logger->info("Action: signup");

    if (Auth::signup(username, password)) {
        logger->info("Success");
        return 0;
    }

    logger->error("Failed");
    return 1;
}


/**
 * Perform a sync operation.
 * 
 * Returns:
 *  int: 0 if the sync was successful. */
int cli_sync() {
    logger->info("Action: sync");
    Sync();
    return 0;
}


/**
 * Runs the command line interface with the given arguments.
 * 
 * Args:
 *  argc (int): The number of arguments.
 *  argv (char**): The array of arguments.
 * 
 * Returns:
 *  int: 0 if the operation was successful, otherwise 1. */
int cli(int argc, char** argv) {
    try {
        TCLAP::CmdLine cmd("FileSync", ' ', "0.1");

        TCLAP::SwitchArg loginArg("l", "login", "Login with username and password", false);
        TCLAP::SwitchArg signupArg("s", "signup", "Signup with username and password", false);
        TCLAP::SwitchArg syncArg("y", "sync", "Sync data", false);

        cmd.add(loginArg);
        cmd.add(signupArg);
        cmd.add(syncArg);

        TCLAP::ValueArg<std::string> usernameArg("u", "username", "Username", false, "", "string");
        TCLAP::ValueArg<std::string> passwordArg("p", "password", "Password", false, "", "string");

        cmd.add(usernameArg);
        cmd.add(passwordArg);

        cmd.parse(argc, argv);

        if (loginArg.isSet())
            return cli_login(usernameArg.getValue(), passwordArg.getValue());

        else if (signupArg.isSet())
            return cli_signup(usernameArg.getValue(), passwordArg.getValue());

        else if (syncArg.isSet())
            return cli_sync();

        else {
            std::cerr << "Error: no command specified" << std::endl;
            return 1;
        }

        return 0;
    }
    catch (TCLAP::ArgException& e) {
        std::cerr << "Error: " << e.error() << " for argument " << e.argId() << std::endl;
        return 1;
    }
}