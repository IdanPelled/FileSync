from datetime import datetime, timedelta
from threading import Thread

from modles import User, Computer, session
from server import sync_server, authorization_server, signup_server


def main() -> None:
    """The main entry point of the application.
    Starts the authorization, sign up, and sync servers on separate threads."""

    # Start the authorization server on a separate thread
    Thread(target=authorization_server).start()

    # Start the sign up server on a separate thread
    Thread(target=signup_server).start()

    # Start the sync server on the main thread
    sync_server()


if __name__ == "__main__":
    main()
