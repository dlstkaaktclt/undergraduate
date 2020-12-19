#studentMenu.py

from userAcc import *
from DBconnection import *

def student_menu():

    menu_num = "-1"

    while(menu_num != "0"):
        print("\n\nWelcome %s" % user_acc.name)
        print("select student menu")
        print("1) Student Report")
        print("2) View Time Table")
        print("0) Exit")
        menu_num = input("Enter : ")

        switcher = {
            "0" : exit_menu,
            "1" : print_stud_report,
            "2" : print_time_table
        }

        selected_func = switcher.get(menu_num, print_wrong)

        selected_func()

    return


# ID,name으로 소속학과와 총 수강한 학점 가져와서 출력

def print_stud_report():

    c = user_acc.conn.cursor()
    c.execute("SELECT * FROM student \
               WHERE ID = \"%s\" and name = \"%s\"" % (user_acc.ID, user_acc.name) )
    data = c.fetchone()

    print("You are a member of %s" % data[2])
    if(data[3] == None): # if tot_cred is null, print nothing
        print("You have taken total %s credits\n" % "")
    else:
        print("You have taken total %s credits\n" % data[3])
    print("Semester report\n")

    #평점 구하는 과정
    #수강한 학기, 연도정보 모두 가져오기 (distinct로 중복 계산 방지)

    c.execute("SELECT DISTINCT year, semester FROM takes \
               WHERE ID = \"%s\" \
               ORDER BY year DESC, \
               case when semester like 'Winter' then 1 \
               when semester like 'Fall' then 2 \
               when semester like 'Summer' then 3 \
               when semester like 'Spring' then 4 \
               else 5 end" % (user_acc.ID))  #

    year_and_semester = c.fetchall()

    for year_semester in year_and_semester:
        year = year_semester[0]
        semester = year_semester[1]
        # 학기정보와 학생 ID를 이용해서 GPA 계산을 위해 grade와 credit을 가져옴
        c.execute("SELECT grade, credits FROM course natural join takes \
                           WHERE ID = \"%s\" and semester = \"%s\" and year = %s" % (user_acc.ID, semester, year))

        grade_credits = c.fetchall()

        credit_sum = 0
        grade_mul_credit_sum = 0

        for grade_credit in grade_credits:
            grade = grade_credit[0]
            if(grade is None) : continue    # null값을 가지고 있으면 cursor에서 리턴된것은 None값을 가짐. 이럴땐 무시.
            if(grade_credit[1] != None): credit = float(grade_credit[1])
            else: credit = 0

            credit_sum += credit
            grade_mul_credit_sum += gp_to_float(grade) * credit

        ## gpa가 NULL인경우 처리.
        if(credit_sum == 0) :
            print("\n%s\t%s\tGPA : " % (year, semester))
        else:
            gpa = grade_mul_credit_sum / credit_sum
            print("\n%s\t%s\tGPA : %f" % (year, semester, gpa))
        ## 강의 정보 가져오기
        print("%10s\t%40s\t%15s\t%8s\t%8s" % ("course_id", "title", "dept_name", "credit", "grade"))
        c.execute("SELECT course_id, title, dept_name, credits, grade FROM course natural join takes \
                   WHERE ID = \"%s\" and semester = \"%s\" and year = %s" % (user_acc.ID, semester, year))
        courses = c.fetchall()

        for course in courses:
            course_id = course[0]
            title = course[1]
            dept_name = course[2]
            credit = course[3]
            grade = course[4]

            # grade가 null인 경우 출력하지 않음
            if(grade is None):
                grade = ""

            print("%10s\t%40s\t%15s\t%8s\t%8s" % (course_id, title, dept_name, credit, grade))

        print("")

    c.close()

    return

def print_time_table():
    c = user_acc.conn.cursor()

    # 수강한 학기, 연도정보 모두 가져오기 (distinct로 중복 계산 방지)

    c.execute("SELECT DISTINCT year, semester FROM takes \
                   WHERE ID = \"%s\" \
                   ORDER BY year DESC, \
                   case when semester like 'Winter' then 1 \
                   when semester like 'Fall' then 2 \
                   when semester like 'Summer' then 3 \
                   when semester like 'Spring' then 4 \
                   else 5 end" % (user_acc.ID))  #

    year_and_semester = c.fetchall()

    if year_and_semester:
        print("Please select semester to view")
        for (i,year_semester) in enumerate(year_and_semester):
            year = year_semester[0]
            semester = year_semester[1]
            print("%d) %s %s" % (i+1, year, semester))
        input_num = input()

        # handle if input is uncorrect value
        try:
            num = int(input_num)
            if(num > len(year_and_semester) or num <= 0):
                print("Please insert correct value")
                c.close()
                return

        except Exception as e:
            print("Please insert correct value")
            c.close()
            return


        # 타임테이블 출력
        print("\nTime Table\n")
        print("%10s\t%40s\t%15s\t%10s\t%10s" % ("course_id", "title", "day", "start_time", "end_time"))
        year = year_and_semester[num-1][0]
        semester = year_and_semester[num-1][1]

        # time_table 데이터 받아오기
        c.execute("SELECT course_id, title, day, start_hr, start_min, end_hr, end_min FROM takes natural join course natural join section natural join time_slot \
                           WHERE ID = \"%s\" and semester = \"%s\" and year = %s ORDER BY start_hr desc, start_min desc" % (user_acc.ID, semester, year))

        time_table_rows = c.fetchall()
        if time_table_rows:
            # course에 해당하는 section table의 time_slot_id의 값이 timeslot table에 없는경우, 제외하고 출력함.
            for time_table_row in time_table_rows:
                course_id = time_table_row[0]
                title = time_table_row[1]
                day = time_table_row[2]
                start_time = str(time_table_row[3]) + " : " + str(time_table_row[4])
                end_time = str(time_table_row[5]) + " : " + str(time_table_row[6])

                print("%10s\t%40s\t%15s\t%10s\t%10s" % (course_id, title, day, start_time, end_time))
        else:
            # 모든 course들의 time_slot_id의 값이 timeslot table에 없는 경우
            print("there is no course that has a valid time_slot id")
    else:
        print("No option to Select")


    #사용한 cursor 닫기
    c.close()

    return

def exit_menu():
    global user_acc # global 변수 write할때는 명시 필요

    # user가 사용하던 connection 반납
    return_connect(user_acc.conn)

    del user_acc

    return

def print_wrong():
    print("\nwrong menu number.")
    return


def gp_to_float(grade):
    if (grade is None): return 0
    else:
        return {
            "A+" : 4.3,
            "A" : 4,
            "A " : 4,
            "A-" : 3.7,
            "B+" : 3.3,
            "B" : 3,
            "B " : 3,
            "B-" : 2.7,
            "C+" : 2.3,
            "C" : 2,
            "C " : 2,
            "C-" : 1.7,
            "D+" : 1.3,
            "D" : 1,
            "D " : 1,
            "D-" : 0.7,
            "F" : 0,
            "F " : 0
        }[grade]
