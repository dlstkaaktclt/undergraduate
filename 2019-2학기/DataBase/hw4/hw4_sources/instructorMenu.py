# instructorMenu.py

from userAcc import *
from DBconnection import *

def instructor_menu():
    menu_num = "-1"

    while (menu_num != "0"):
        print("\n\nWelcome %s" % user_acc.name)
        print("Please select instructor menu")
        print("1) Course report")
        print("2) Advisee report")
        print("0) Exit")
        menu_num = input("Enter : ")

        switcher = {
            "0": exit_menu,
            "1": print_course_report,
            "2": print_advisee_report
        }

        selected_func = switcher.get(menu_num, print_wrong)

        selected_func()

    return


def print_course_report():
    c = user_acc.conn.cursor()

    # 강의한 학기, 연도정보 모두 가져오기 (distinct로 중복 계산 방지)

    c.execute("SELECT DISTINCT year, semester FROM teaches \
                   WHERE ID = \"%s\" \
                   ORDER BY year DESC, \
                   case when semester like 'Winter' then 1 \
                   when semester like 'Fall' then 2 \
                   when semester like 'Summer' then 3 \
                   when semester like 'Spring' then 4 \
                   else 5 end" % (user_acc.ID))

    year_and_semester = c.fetchall()

    if year_and_semester:
        # get most recent semester.
        year = year_and_semester[0][0]
        semester = year_and_semester[0][1]

        # couser report - year semester 출력.
        print("\nCourse report - %d %6s\n" % (year,semester))

        # find courses at that semester
        c.execute("SELECT course_id from teaches \
            where ID = \"%s\" and semester = \"%s\" and year = %s" % (user_acc.ID, semester, year))

        most_recent_courses = c.fetchall()

        for most_recent_course in most_recent_courses:
            recent_course_id = most_recent_course[0]

            # 각 course에 대해 course가 열렸던 위치, course의 정보 출력.
            c.execute("SELECT title, building, room_number, day, start_hr, start_min, end_hr, end_min FROM course natural join section natural join time_slot \
                       WHERE course_id = \"%s\" and semester = \"%s\" and year = %s" % (recent_course_id, semester, year))

            course_datas = c.fetchall()

            #course 정보 출력
            if course_datas:
                days = []
                for course_data in course_datas: # course가 열린 day들 추출
                    days.append(course_data[3])

                title = course_datas[0][0]
                building = course_datas[0][1]
                room_number = course_datas[0][2]
                day = ", ".join(days)
                start_time = str(course_datas[0][4]) + " : " + str(course_datas[0][5])
                end_time = str(course_datas[0][6]) + " : " + str(course_datas[0][7])

                print("%10s\t%40s\t[%s %s]\t(%s %7s - %7s)" % (recent_course_id, title, building, room_number, day, start_time, end_time))
            else:
                # 만약 section table의 time_slot_id의 데이터가 timeslot table의 없는값인 경우, 부분 정보만 받아옴.
                c.execute("SELECT title, building, room_number FROM course natural join section \
                                       WHERE course_id = \"%s\" and semester = \"%s\" and year = %s" % (recent_course_id, semester, year))
                course_datas = c.fetchall()
                if course_datas:
                    title = course_datas[0][0]
                    building = course_datas[0][1]
                    room_number = course_datas[0][2]
                    print("%10s\t%40s\t[%s %s]\t" % (recent_course_id, title, building, room_number))
                else:
                # 아예 해당하는 데이터가 없는경우, 오류메시지 출력.
                    print("course_id %s have missing data. we can't get course data." % recent_course_id)

            #course들을 수강했던 학생들 데이터 출력

            c.execute("SELECT ID, name, dept_name, grade FROM student natural join takes \
                        WHERE course_id = \"%s\" and semester = \"%s\" and year = %s" % (recent_course_id, semester, year))

            students = c.fetchall()

            print("%10s\t%20s\t%15s\t%8s" % ("ID", "name", "dept_name", "grade"))

            for student in students:
                student_id = student[0]
                student_name = student[1]
                dept_name = student[2]
                grade = student[3]

                if(grade == None) : grade = "" # handle grade is null

                print("%10s\t%20s\t%15s\t%8s" % (student_id, student_name, dept_name, grade))

            print("")
    else:
        print("there is no course instructor %s teaches" % user_acc.name)

    c.close()

    return


def print_advisee_report():
    c = user_acc.conn.cursor()
    
    # 교수의 ID를 이용해 그 교수가 advisor인 학생들의 정보를 가져옴
    c.execute("SELECT student.ID, student.name, student.dept_name, student.tot_cred FROM student, advisor\
               WHERE student.ID = advisor.s_id and advisor.i_id = \"%s\"" % user_acc.ID)

    students = c.fetchall()

    print("%10s\t%20s\t%15s\t%8s" % ("ID", "name", "dept_name", "tot_cred"))

    for student in students:
        student_id = student[0]
        student_name = student[1]
        dept_name = student[2]
        tot_cred = student[3]
        if(tot_cred == None) : tot_cred = ""  # deal with if tot_cred is null
        print("%10s\t%20s\t%15s\t%8s" % (student_id, student_name, dept_name, tot_cred))

    c.close()

    return


def exit_menu():
    global user_acc  # global 변수 write할때는 명시 필요

    # user가 사용하던 connection 반납
    return_connect(user_acc.conn)

    del user_acc

    return


def print_wrong():
    print("\nwrong menu number.")
    return