import os



def _login(username, password):
    exe = os.getenv("FileSyncPath")
    cmd = f"{exe} -l -u {username} -p {password}"
    return os.system(cmd) == 0


def _signup(username, password):
    exe = os.getenv("FileSyncPath")
    cmd = f"{exe} -s -u {username} -p {password}"
    print(cmd)
    return os.system(cmd) == 0

print(_signup("bob", "12341234"))