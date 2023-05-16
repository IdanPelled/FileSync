import logging
from threading import Thread

from server import sync_server, authorization_server, signup_server


def configure_logger():
    logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')


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
