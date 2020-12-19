# login.py

# auth()에서 현재 접속한 user의 role(user_acc.role)을 설정해야
# 그에따라 student_menu, instructor_menu가 실행


from userAcc import *
from DBconnection import *
from studentMenu import *
from instructorMenu import *


def login():
    print("Welcome")

    while (user_acc.conn is None):
        print("Please sign in")

        ID = input("%-10s : " % "ID")
        name = input("%-10s : " % "Name")

        auth(ID, name)

    switcher = {
        0: student_menu,
        1: instructor_menu
    }

    role_menu = switcher.get(user_acc.role)

    role_menu()


def auth(ID, name):
    user_connect = get_connect()  # 로그인이 성공하면 user_acc의 conn에 connection object를 대입해줘야 한다.

    c = user_connect.cursor()

    print("")

    # 입력 받은 ID, name이 student DB에 존재하는지 확인
    c.execute("SELECT * FROM student \
               WHERE ID = \"%s\" and name = \"%s\"" % (ID, name))
    data = c.fetchone()

    if data is not None:
        user_acc.set_attrs(ID, name, 0, user_connect)
        return

    # 입력 받은 ID, name이 instructor DB에 존재하는지 확인
    c.execute("SELECT * FROM instructor \
               WHERE ID = \"%s\" and name = \"%s\"" % (ID, name))
    data = c.fetchone()

    if data is not None:
        user_acc.set_attrs(ID, name, 1, user_connect)
        return

    # print that the id and the name are wrong.
    print("Wrong authentication.")
    return_connect(user_connect)

    return

