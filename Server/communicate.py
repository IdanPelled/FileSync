from curses.ascii import isdigit
from typing import Union
import socket
import time
import enum
import struct


import files
from config import TIME_FORMAT, HOST


class Action(enum.Enum):
    NONE = 0
    UPLOAD = 1
    DOWNLOAD = 2


def get_action(sock: socket.socket, comp_id: int) -> Action:
    """Reads the client's versions,
    and returns the appropriate action."""

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
    """Create and bind."""

    sock = socket.socket(
        socket.AF_INET,
        socket.SOCK_STREAM
    )
    sock.bind((HOST, port))
    return sock


def connect(sock: socket.socket) -> socket.socket:
    """Connect the socket."""

    sock.listen()
    conn, _ = sock.accept()
    print(_, "!!!")
    return conn


def upload(sock: socket.socket) -> None:
    """Upload the client's folder."""
    
    print("Upload")
    sock.recv(1024)


def download(sock: socket.socket) -> None:
    """Download the server's folder."""

    print("Download")
    msg = b"Folder"
    sock.send(b"6")
    sock.send(msg)


def take_action(sock: socket.socket, action: Action) -> None:
    match action:
        case Action.NONE:
            return

        case Action.UPLOAD:
            upload(sock)

        case Action.DOWNLOAD:
            download(sock)