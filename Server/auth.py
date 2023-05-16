from enum import Enum
import hashlib
import socket
import jwt
from datetime import datetime, timedelta
import time

from typing import Any, Tuple, Union

from config import JWT_KEY, JWT_EXP, TOKEN_LENGTH
from modles import session, Computer, User


LENGTH_HEADER = 1


class TokenStatus(Enum):
    """An enumeration representing the different status codes
    of a token validation."""

    valid = 0
    invalid = 1
    expired = 2


class CredentialsStatus(Enum):
    """An enumeration representing the different status codes
    of a credentials validation."""

    valid = 0
    invalid = 1


def get_ip(sock: socket.socket) -> str:
    """Returns the IP address of the connected socket.

    Args:
        sock: A `socket.socket` object representing the
            connected socket.

    Returns:
        A string representing the IP address of the socket.
    """

    return sock.getsockname()[0]


def read_token_data(token: str) -> Tuple[TokenStatus, dict[str, Any]]:
    """Returns the computer ID from a JWT token.

    This function checks the expiration and signature of the token,
    and returns a `TokenStatus` code indicating the validation status
    along with a dictionary of the decoded token payload.

    Args:
        token: A string representing the JWT token.

    Returns:
        A tuple containing a `TokenStatus` status code and a dictionary
        of the decoded token payload.
    """

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


def send_error(sock: socket.socket, status_code: int) -> None:
    """Sends an error response to a connected socket.

    This function sends a status code indicating an error to the
    connected socket.

    Args:
        sock: A `socket.socket` object representing the connected socket.
        status_code: An integer representing the status code of the error.
    """

    sock.send(str(status_code).encode())


def send_success(sock: socket.socket) -> None:
    """Sends a success response to a connected socket.

    This function sends a success response (status code 0) to the
    connected socket.

    Args:
        sock: A `socket.socket` object representing the connected socket.
    """

    sock.send(b"0")


def authenticate(
    sock: socket.socket,
    token: str
) -> Tuple[TokenStatus, Union[Computer, None]]:
    """Authenticates a client via token.

    This function checks if the expiration date of the token is valid,
    and if the 'computer_id' associated with the token matches the IP address
    of the connected socket. It returns a `TokenStatus` code indicating the
    validation status along with a `Computer` object representing the
    authenticated computer, or `None` if the validation failed.

    Args:
        sock: A `socket.socket` object representing the connected socket.
        token: A string representing the JWT token.

    Returns:
        A tuple containing a `TokenStatus` status code and a `Computer`
        object representing the authenticated computer, or `None`.
    """

    status, payload = read_token_data(token)

    match status:
        case TokenStatus.valid:
            user_id = payload["user_id"]
            comp = get_computer_by_id(payload["computer_id"])
            
            if (comp.ip == get_ip(sock)) and (comp.user_id == user_id):
                return TokenStatus.valid, comp
            
            return TokenStatus.invalid, None
        
        case TokenStatus.expired:
            return TokenStatus.expired, None
        
        case TokenStatus.invalid:
            return TokenStatus.invalid, None


def get_computer_by_id(comp_id: int) -> Computer:
    """Returns the Computer by id.

    Args:
    comp_id (int): The id of the computer.

    Returns:
    Computer: The computer with the specified id.
    """
    return session.query(Computer).get(comp_id)


def read_msg_length(sock: socket.socket) -> int:
    """Reads the length of a message from the socket.

    Args:
    sock (socket.socket): The socket object to read from.

    Returns:
    int: The length of the message.
    """

    return int.from_bytes(sock.recv(LENGTH_HEADER), byteorder='big')


def read_credentials(sock: socket.socket) -> Tuple[str, str]:
    """Reads username and password from the socket.

    Args:
    sock (socket.socket): The socket object to read from.

    Returns:
    Tuple[str, str]: A tuple containing the username and password.
    """

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
    """Validates a username and password.

    Args:
    username (str): The username to validate.
    password (str): The password to validate.

    Returns:
    CredentialsStatus: A status indicating if the credentials are valid or invalid.
    """

    user: User = session.query(User).filter_by(
        username=username,
        password=get_hash(password)
    ).scalar()

    if user is None:
        return CredentialsStatus.invalid

    return CredentialsStatus.valid


def create_new_computer(sock: socket.socket, user_id: int) -> int:
    """Creates a new computer in the database and returns it.

    Args:
    sock (socket.socket): The socket object.
    user_id (int): The user id associated with the computer.

    Returns:
    int: The id of the newly created computer.
    """

    comp = Computer(ip=get_ip(sock), user_id=user_id)
    session.add(comp)
    session.commit()

    return comp


def get_computer(sock: socket.socket) -> Union[Computer, None]:
    """Returns a computer by the socket's IP.

    Args:
    sock (socket.socket): The socket object.

    Returns:
    Union[Computer, None]: The computer with the socket's IP, or None if not found.
    """
    return session.query(Computer).filter_by(ip=get_ip(sock)).scalar()


def get_user(sock: socket.socket) -> Union[User, None]:
    """Returns a user by the socket's IP.

    Args:
    sock (socket.socket): The socket object.

    Returns:
    Union[User, None]: The user with the socket's IP, or None if not found.
    """

    comp = get_computer(sock)
    if comp is not None:
        return comp.user

    return None


def get_user_by_username(username: str) -> Union[User, None]:
    """Returns a user by their username.

    Args:
    username (str): The username to search for.

    Returns:
    Union[User, None]: The user with the specified username, or None if not found.
    """

    return session.query(User).filter_by(username=username).scalar()


def generate_token(sock: socket.socket, username: str) -> str:
    """
    Generate a new JWT token.

    Args:
        sock (socket.socket): The socket object.
        username (str): The username for which the token is generated.

    Returns:
        str: The generated JWT token.
    """

    comp = get_computer(sock)
    user = get_user_by_username(username)

    if comp is None:
        # New computer
        comp = create_new_computer(sock, user.id)
        print("created new comp")
    
    elif comp.user.username != username:
        print("edited comp to new user")
        comp.user = user
        session.commit()


    payload = {
        "user_id": user.id,
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
    """
    Reads the token from the user.

    Args:
        sock (socket.socket): The socket object.

    Returns:
        str: The token string.
    """

    return sock.recv(TOKEN_LENGTH).decode()


def send_token(sock: socket.socket, token: str) -> None:
    """
    Sends the token to the user.

    Args:
        sock (socket.socket): The socket object.
        token (str): The token string to be sent.
    """

    send_success(sock)
    sock.send(token.encode())


def create_user(username: str, password: str) -> None:
    """
    Creates a new user in the db.

    Args:
        username (str): The username of the user to be created.
        password (str): The password of the user to be created.
    """

    user = User(username=username, password=get_hash(password))
    session.add(user)
    session.commit()


def username_exists(username: str) -> bool:
    """
    Checks if username exists.

    Args:
        username (str): The username to check.

    Returns:
        bool: True if the username exists, False otherwise.
    """

    user = session.query(User).filter_by(username=username).first()
    return user is not None


def get_hash(password: str):
    sha = hashlib.sha256()
    sha.update(password.encode("utf-8"))
    return sha.hexdigest()
