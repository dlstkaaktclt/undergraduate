#DBConnection.py

# DB connectoin을 담당하는 DBconnection.py
# 다른 파일에서는 이 파일이 제공하는 DB connection을 사용하고 반납한다
# 사용시 get_connect(), 반납시 return_connect()
# EX) login.py의 auth(), studentMenu.py의 quit_menu()에서 사용.

import MySQLdb

db_host = "localhost"
db_user = "root"
db_pw = "as2354"
db_name = "python_testdb"

connect_pool = []

def connectDB():
    connect = MySQLdb.connect(db_host, db_user, db_pw, db_name)
    return connect

def get_connect():
    global connect_pool
    if not connect_pool:
        connect_tmp = connectDB()
        connect_pool.append(connect_tmp)
    return connect_pool.pop()

def return_connect(conn):
    global connect_pool
    connect_pool.append(conn)
    return

def close_db(db):
    db.close()
    return
