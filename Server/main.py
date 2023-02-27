from datetime import datetime, timedelta
from threading import Thread


from modles import User, Computer, session
from server import sync_server, authorization_server, signup_server



def main():
    Thread(target=authorization_server).start()
    Thread(target=signup_server).start()
    
    sync_server()


def create_user():
    user = User(username="username12345", password="pass1", backup_date=datetime.now())
    session.add(user)
    session.commit()
    
    session.commit()


def create_token():
    import jwt
    print(jwt.encode(
        {
            "computer_id": 1,
            "exp": datetime.utcnow() + timedelta(seconds=500)
        },
        "key",
        algorithm="HS256"
    ))


if __name__ == "__main__":
    # create_user()
    # create_token()

    main()