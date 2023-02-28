import socket
import time
import enum


import files
from config import TIME_FORMAT, HOST, DATA_PATH


class Action(enum.Enum):
    NONE = 0
    UPLOAD = 1
    DOWNLOAD = 2


def get_action(sock: socket.socket, comp_id: int) -> Action:
    """
    Reads the client's versions and returns the appropriate action.

    Parameters:
        sock (socket.socket): The client's socket.
        comp_id (int): The ID of the client's computer.

    Returns:
        Action: The appropriate action to perform.

    """

    try:
        client_mod_date = time.strptime(bytes.decode(sock.recv(len(TIME_FORMAT))), TIME_FORMAT)

    except ValueError:
        print("Invalid date format.")
        return Action.NONE

    server_mod_date = files.get_folder_mod_date(comp_id)
    
    if client_mod_date > server_mod_date:
        return Action.UPLOAD

    elif client_mod_date < server_mod_date:
        return Action.DOWNLOAD
    
    return Action.NONE


def create_cocket(port: int) -> socket.socket:
    """
    Create and bind a socket.

    Parameters:
        port (int): The port number to bind the socket to.

    Returns:
        socket.socket: The created socket.

    """

    sock = socket.socket(
        socket.AF_INET,
        socket.SOCK_STREAM
    )
    sock.bind((HOST, port))
    return sock


def connect(sock: socket.socket) -> socket.socket:
    """
    Connect the socket to a client.

    Parameters:
        sock (socket.socket): The server's socket.

    Returns:
        socket.socket: The client's socket.

    """

    sock.listen()
    conn, _ = sock.accept()
    print(_, "!!!")
    return conn


def upload(sock: socket.socket, username: str) -> None:
    """
    Upload the client's folder.

    Parameters:
        sock (socket.socket): The client's socket.
        username (str): the client's username

    """

    length = sock.recv(10).decode()
    data = sock.recv(length)
    
    with open(f'{DATA_PATH}/{username}.zip', 'wb') as f:
        f.write(data)


def download(sock: socket.socket, username: str) -> None:
    """
    Download the server's folder.

    Parameters:
        sock (socket.socket): The client's socket.
        username (str): the client's username
    """

    with open(f'{DATA_PATH}/{username}.zip', 'rb') as f:
        data = f.read()

    sock.send(len(data))
    sock.send(data)


def take_action(sock: socket.socket, action: Action) -> None:
    """
    Perform the appropriate action based on the given action enum.

    Parameters:
        sock (socket.socket): The client's socket.
        action (Action): The appropriate action to perform.

    """

    match action:
        case Action.NONE:
            return

        case Action.UPLOAD:
            upload(sock)

        case Action.DOWNLOAD:
            download(sock)
