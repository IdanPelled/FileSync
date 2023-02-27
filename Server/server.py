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


def server(port):
    """A decorator for accepting
    requests on a given port."""

    def decorator(f):

        def wrapper(*args, **kwargs) -> Any:
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
    """Syncs the client's folder."""

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
    """authorize a computer by
    assigning it a new token."""

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
    """create a new user."""

    username, password = read_credentials(sock)
    if username_exists(username): 
        if create_user(username, password):
            send_success(sock)
        
        send_error(sock, 2)
    
    send_error(sock, 1)
