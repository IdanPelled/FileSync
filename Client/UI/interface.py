import os


def _login(username, password):
    exe = os.getenv("FileSyncPath")
    if exe:
        cmd = f"{exe} -l -u {username} -p {password}"
        return os.system(cmd) == 0
    
    raise EnvironmentError("missing FileSyncPath env variable")


def _signup(username, password):
    exe = os.getenv("FileSyncPath")
    if exe:
        cmd = f"{exe} -s -u {username} -p {password}"
        return os.system(cmd) == 0

    raise EnvironmentError("missing FileSyncPath env variable")

