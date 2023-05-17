import os


FILESYNC_PATH = "C:\\Users\\pelle\\Desktop\\FileSync\\Client\\x64\\Release\\Client.exe"


def _login(username, password):
    cmd = f"{FILESYNC_PATH} -l -u {username} -p {password}"
    return os.system(cmd) == 0


def _signup(username, password):
    cmd = f"{FILESYNC_PATH} -s -u {username} -p {password}"
    return os.system(cmd) == 0


