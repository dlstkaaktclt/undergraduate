#userAcc.py


# UserAcc class
# Attribute : id, name, role(학생인지 교수인지), conn(연결된 Connection object)
# user_acc는 연결된 user의 계정 object
# 한 user당 하나의 connection을 이용하므로, user object가 conn을 attribute로 가짐!

class UserAcc():
    ID = 0
    name = ""
    role = 0
    conn = None

    def __init__(self, ID = 0, name = "", role = 0, conn = None):
        self.ID = ID
        self.name = name
        self.role = role
        self.conn = conn

    def set_attrs(self, ID, name, role, conn):
        self.ID = ID
        self.name = name
        self.role = role
        self.conn = conn

#user account object declare
user_acc = UserAcc()