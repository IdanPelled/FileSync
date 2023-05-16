from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, String, ForeignKey, DateTime, create_engine
from sqlalchemy.orm import sessionmaker, relationship

from config import DB_PATH


Base = declarative_base()


class User(Base):
    __tablename__ = 'users'

    id = Column(Integer, primary_key=True)
    username = Column(String, unique=True, nullable=False)
    password = Column(String, nullable=False)

    backup_date = Column(DateTime)


class Computer(Base):
    __tablename__ = 'computers'

    id = Column(Integer, primary_key=True)
    ip = Column(String, unique=True, nullable=False)
    
    user_id = Column(Integer, ForeignKey('users.id'), nullable=False)
    user = relationship("User", back_populates="computers")

User.computers = relationship("Computer", order_by=Computer.id, back_populates="user")


engine = create_engine(DB_PATH, connect_args={'check_same_thread': False})
Base.metadata.create_all(engine, checkfirst=True)
Session = sessionmaker(bind=engine)
session = Session()
