import datetime
import socket
import time
import enum
import struct
from typing import Union
from modles import session
from auth import get_user_by_username


import files
from config import TIME_FORMAT, HOST, DATA_PATH


class Action(enum.Enum):
    NONE = 0
    UPLOAD = 1
    DOWNLOAD = 2


def get_action(comp_id: int, client_mod_date: datetime.datetime) -> Action:
    """
    Reads the client's versions and returns the appropriate action.

    Parameters:
        sock (socket.socket): The client's socket.
        comp_id (int): The ID of the client's computer.

    Returns:
        Action: The appropriate action to perform.

    """
    server_mod_date = files.get_folder_mod_date(comp_id)

    if client_mod_date is None:
        return Action.NONE

    if server_mod_date is None:
        return Action.UPLOAD
    
    if client_mod_date > server_mod_date:
        return Action.UPLOAD

    elif client_mod_date < server_mod_date:
        return Action.DOWNLOAD
    
    return Action.NONE


def get_client_mod_date(sock: socket.socket) -> Union[datetime.datetime, None]:
    try:
        return datetime.datetime.fromtimestamp(
            time.mktime(time.strptime(
                bytes.decode(sock.recv(len(TIME_FORMAT))),
                TIME_FORMAT
            )
        ))

    except ValueError:
        print("Invalid date format.")
        return None


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
    return conn


def read_length_header(sock: socket.socket) -> int:
    return struct.unpack('>I', sock.recv(4))[0]


def send_length_header(sock: socket.socket, length: int) -> None:
    sock.send(struct.pack('>I', length))


def upload(sock: socket.socket, username: str, mod_date: datetime.datetime) -> bool:
    """
    Upload the client's folder.

    Parameters:
        sock (socket.socket): The client's socket.
        username (str): the client's username

    Returns:
        bool: True if the upload finished sucessfully, False otherwise
    
    """

    length = read_length_header(sock)
    if length <= 0:
        return False
    
    data = sock.recv(length)
    with open(f'{DATA_PATH}/{username}', 'wb') as f:
        f.write(data)

    
    user = get_user_by_username(username)
    user.backup_date = mod_date
    session.commit()
    return True


def download(sock: socket.socket, username: str) -> bool:
    """
    Download the server's folder.

    Parameters:
        sock (socket.socket): The client's socket.
        username (str): the client's username
    
    Returns:
        bool: True if the download finished sucessfully, False otherwise
    
    """

    with open(f'{DATA_PATH}/{username}', 'rb') as f:
        data = f.read()

    send_length_header(sock, len(data))
    sock.send(data)
    return True


def take_action(
    sock: socket.socket,
    action: Action,
    username: str,
    mod_date: datetime.datetime
) -> bool:
    """
    Perform the appropriate action based on the given action enum.

    Parameters:
        sock (socket.socket): The client's socket.
        action (Action): The appropriate action to perform.
        username (str): username

    Returns:
        bool: True if the action finished sucessfully, False otherwise
    """

    match action:
        case Action.NONE:
            return True

        case Action.UPLOAD:
            return upload(sock, username, mod_date)

        case Action.DOWNLOAD:
            return download(sock, username)
