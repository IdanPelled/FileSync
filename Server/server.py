from socket import socket
from typing import Any, Optional

from config import LOGIN_PORT, SYNC_PORT, SIGNUP_PORT
from communicate import connect, create_cocket, get_action, take_action
from auth import (
    TokenStatus,
    CredentialsStatus,
    authenticate,
    read_token,
    read_credentials,
    is_valid,
    generate_token,
    send_token,
    send_success,
    send_error,
    username_exists,
    create_user,
)


def server(port: int):
    """A decorator for accepting requests on a given port.

    Args:
        port: An integer representing the port number on which to listen for requests.

    Returns:
        A decorator function.
    """

    def decorator(f):
        """A decorator that wraps the input function and listens for incoming requests on the given port.

        Args:
            f: A function to be wrapped.

        Returns:
            A wrapper function.
        """

        def wrapper(*args, **kwargs) -> Any:
            """The wrapper function that listens for incoming requests on the given port.

            Args:
                *args: Variable length argument list.
                **kwargs: Arbitrary keyword arguments.

            Returns:
                The result of calling the input function with the given arguments.

            Raises:
                Exception: If any exception is encountered while listening for incoming requests.
            """
            s = create_cocket(port)

            while True:
                try:
                    sock = connect(s)
                    f(sock, *args, **kwargs)
                
                except Exception as e:
                    sock.close()
                    print(e)

        return wrapper

    return decorator


@server(SYNC_PORT)
def sync_server(sock: Optional[socket] = None) -> None:
    """Syncs the client's folder.

    Args:
        sock: A socket representing the connection to the client. Defaults to None.

    Returns:
        None.
    """

    token = read_token(sock)
    status_code, comp = authenticate(sock, token)

    if status_code == TokenStatus.valid:
        # authenticated success
        send_success(sock)

        action = get_action(sock, comp.id)
        sock.send(str(action.value).encode())

        take_action(sock, action)
    
    else:
        print("auth error")
        send_error(sock, status_code.value)


@server(LOGIN_PORT)
def authorization_server(sock: Optional[socket] = None) -> None:
    """Authorize a computer by assigning it a new token.

    Args:
        sock: A socket representing the connection to the client. Defaults to None.

    Returns:
        None.
    """

    username, password = read_credentials(sock)
    status_code = is_valid(username, password)

    if status_code == CredentialsStatus.valid:
        token = generate_token(sock, username)
        send_token(sock, token)
    
    else:
        print("login error")
        send_error(sock, status_code.value)


@server(SIGNUP_PORT)
def signup_server(sock: Optional[socket] = None) -> None:
    """Create a new user.

    Args:
        sock: A socket representing the connection to the client. Defaults to None.

    Returns:
        None.
    """

    username, password = read_credentials(sock)
    if username_exists(username): 
        create_user(username, password)
        send_success(sock)
    
    else:
        send_error(sock, 1)
