import os


HOST = '0.0.0.0'
SYNC_PORT = 33456
LOGIN_PORT = 33457
SIGNUP_PORT = 33458

TIME_FORMAT = '%d-%m-%y %H:%M:%S'
HEADER_LENGTH = 4

DB_PATH = 'sqlite:///Server/database.db'
DATA_PATH = './Server/data'

TOKEN_LENGTH = 143

JWT_KEY = exe = os.getenv("FileSyncJWT")
JWT_EXP = 60 * 60 * 24
