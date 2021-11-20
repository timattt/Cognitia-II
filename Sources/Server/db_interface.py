import sqlite3


class DBQuery:
    def __init__(self, db_file_path):
        self.connection = sqlite3.connect(db_file_path)
        self.cursor = self.connection.cursor()

    def get_users(self, user_names, role=None):
        if type(user_names) != type(()):
            user_names = (user_names,)

        for user in user_names:
            if not user.isalpha():
                return None

        user_names = ('"' + user + '"' for user in user_names)

        if role is None:
            role_limit = ""
        else:
            if not role.isalpha():
                return None

            if role not in ("student", "mentor"):
                return None

            role_limit = ' and role = "' + role + '"'

        users_list = "(" + ", ".join(user_names) + ")"
        self.cursor.execute(
            "select * from users where name in " + users_list + role_limit
        )
        return self.cursor.fetchall()

    def get_students_folders_pathes(self, students):
        if type(students) != type(()):
            students = (students,)
        students = ('"' + student + '"' for student in students)

        students_list = "(" + ", ".join(students) + ")"
        self.cursor.execute(
            "select * from student_x_progress_file where student in " + students_list
        )
        return self.cursor.fetchall()

    def get_mentor_students(self, mentor):
        self.cursor.execute(
            'select * from mentor_x_student where mentor = "' + mentor + '"'
        )
        return self.cursor.fetchall()

    def __del__(self):
        self.cursor.close()
        self.connection.close()
