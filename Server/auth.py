from ctypes import sizeof
from enum import Enum
import socket
import jwt
from datetime import datetime, timedelta
import time

from typing import Any, Tuple, Union

from config import JWT_KEY, JWT_EXP, TOKEN_LENGTH
from modles import session, Computer, User


LENGTH_HEADER = 1


class TokenStatus(Enum):
    valid = 0
    invalid = 1
    expired = 2


class CredentialsStatus(Enum):
    valid = 0
    invalid = 1


def get_ip(sock: socket.socket) -> str:
    """Returns the ip of the socket connection."""
    return sock.getsockname()[0]


def read_token_data(token: str) -> Tuple[TokenStatus, dict[str, Any]]:
    """Returns the computer id from the token."""
    try:
        payload = jwt.decode(token, JWT_KEY, algorithms=["HS256"])
        return TokenStatus.valid, payload

    except jwt.exceptions.ExpiredSignatureError:
        return TokenStatus.expired, {}
    
    except (
        jwt.exceptions.DecodeError,
        jwt.exceptions.InvalidTokenError,
    ):
        return TokenStatus.invalid, {}


def send_error(sock: socket.socket, status_code: TokenStatus) -> None:
    # make sure that the status code is one digit
    sock.send(str(status_code.value % 10).encode())


def send_success(sock: socket.socket) -> None:
    sock.send(b"0")


def authenticate(
    sock: socket.socket, token: str
) -> Tuple[TokenStatus, Union[Computer, None]]:
    # To do: fix documentation
    """Authenticate the client via token. Checks if
    the exp date is valid, and if the 'computer_id'
    is associated with the socket's ip. returns a
    `TokenStatus` status code."""

    status, payload = read_token_data(token)
    if status == TokenStatus.valid:

        comp_id = payload["computer_id"]
        user = get_computer_by_id(comp_id)

        
        return TokenStatus.valid, user

    return status, None
    

    # if ip == get_ip(sock):
    #     return TokenStatus.valid
    
    # return TokenStatus.invalid


def get_computer_by_id(comp_id: int) -> Computer:
    """Returns the Computer by id."""

    return session.query(Computer).get(comp_id)


def read_msg_length(sock: socket.socket) -> int:
    return int.from_bytes(sock.recv(LENGTH_HEADER), byteorder='big')


def read_credentials(sock: socket.socket) -> Tuple[str, str]:
    """Reads username and password."""

    username_length = read_msg_length(sock)
    username = sock.recv(username_length)
    send_success(sock)

    password_length = read_msg_length(sock)
    password = sock.recv(password_length)
    send_success(sock)

    return (
        username.decode(),
        password.decode(),
    )


def is_valid(username: str, password: str) -> CredentialsStatus:
    """Validated username and password."""
    
    print("username + password: ", username, password)
    user: User = session.query(User).filter_by(
        username=username,
        password=password
    ).scalar()

    if user is None:
        return CredentialsStatus.invalid
    
    return CredentialsStatus.valid


def create_new_computer(sock: socket.socket, user_id: int) -> int:
    """Create a new computer in the db and returns it"""
    comp = Computer(ip=get_ip(sock), user_id=user_id)
    session.add(comp)
    session.commit()

    return comp


def get_computer(sock: socket.socket) -> Union[Computer, None]:
    """Returns comp by ip of the socket."""

    return session.query(Computer).filter_by(ip=get_ip(sock)).scalar()


def get_user(sock: socket.socket) -> Union[User, None]:
    """Returns the user by ip of the socket."""

    comp = get_computer(sock) is not None
    if comp is not None:
        return comp.user
    
    return None

def get_user_by_username(username: str) -> Union[User, None]:
    """Returns the user by username."""

    return session.query(User).filter_by(username=username).scalar()


def generate_token(sock: socket.socket, username: str) -> str:
    """Generate a new JWT token."""
    
    comp = get_computer(sock)
    if comp is None:
        # New computer
        user = get_user_by_username(username)
        comp = create_new_computer(sock, user.id)
        print("created new comp")
    
    payload = {
        "computer_id": comp.id,
        "exp": datetime.utcnow() + timedelta(seconds=JWT_EXP)
    }
    
    print("created new token")
    return jwt.encode(
        payload,
        JWT_KEY,
        algorithm="HS256"
    )

def read_token(sock: socket.socket) -> str:
    """Reads the token from the user."""

    return sock.recv(TOKEN_LENGTH).decode()


def send_token(sock: socket.socket, token: str) -> str:
    """Sends the token to the user."""

    send_success(sock)
    sock.send(token.encode())