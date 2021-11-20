import socket
import os
from dotenv import load_dotenv
from PyQt6 import QtCore

from db_interface import DBQuery

import server_codes as scodes


def send_files(sock, master_dir, files):
    """Send files  'files' from 'master_dir' directory to client"""
    for file in files:
        ba = QtCore.QByteArray()
        FLAG = QtCore.QIODeviceBase.OpenModeFlag.WriteOnly
        dstream = QtCore.QDataStream(ba, FLAG)

        dstream.writeUInt32(0)
        dstream.writeUInt16(scodes.ServerReplies.retrieveCourseUnit)
        dstream.writeQString(file)  # ("dedCourse")

        with open(master_dir + "/" + file, "r") as f:
            data = f.read()

        dstream.writeString(data.encode("utf-8"))
        dstream.device().seek(0)
        dstream.writeUInt32(ba.size() - 4)

        sock.send(ba.data())


def send_cource(sock, name):
    """Send course to client"""
    COURSE_DIR = os.getenv("COURSE_DIR")
    MAIN_COURSEUNIT_FILE_EXTENSION = os.getenv("MAIN_COURSEUNIT_FILE_EXTENSION")
    COURSE_UNIT_FILE_EXTENSION = os.getenv("COURSE_UNIT_FILE_EXTENSION")

    COURSE = os.listdir(COURSE_DIR)

    COURSE = list(
        filter(
            lambda x: x.endswith(MAIN_COURSEUNIT_FILE_EXTENSION)
            or x.endswith(COURSE_UNIT_FILE_EXTENSION),
            COURSE,
        ),
    )

    send_files(sock, COURSE_DIR, COURSE)


def send_skillpack(sock, name):
    """Send skillpack to client"""
    SKILL_DIR = os.getenv("SKILL_DIR")
    SKILL_PACK_FILE_EXTENSION = os.getenv("SKILL_PACK_FILE_EXTENSION")
    SKILL_FILE_EXTENSION = os.getenv("SKILL_FILE_EXTENSION")

    SKILLS = os.listdir(SKILL_DIR)

    SKILLS = list(
        filter(
            lambda x: x.endswith(SKILL_PACK_FILE_EXTENSION)
            or x.endswith(SKILL_PACK_FILE_EXTENSION),
            SKILLS,
        ),
    )

    send_files(sock, SKILL_DIR, SKILLS)


def send_progress(sock, user_info):
    """Send progress file to client"""
    PROGRESS_DIR = (
        os.getenv("PROGRESS_DIR")
        + "/"
        + dbq.get_students_folders_pathes(user_info[1])[0][1]
    )
    STUDENT_PROGRESS_FILE_EXTENSION = os.getenv("STUDENT_PROGRESS_FILE_EXTENSION")

    PROGRESS = os.listdir(PROGRESS_DIR)

    PROGRESS = list(
        filter(lambda x: x.endswith(STUDENT_PROGRESS_FILE_EXTENSION), PROGRESS),
    )

    send_files(sock, PROGRESS_DIR, PROGRESS)


def student_handler(sock, student_info, str):
    """Handle student connection
    Send cource, skillpack and progress files
    """
    send_cource(sock, student_info)
    print("send_cource done")

    send_skillpack(sock, student_info)
    print("send_skillpack done")

    send_progress(sock, student_info)
    print("send_progress done")

    ba = QtCore.QByteArray()
    FLAG = QtCore.QIODeviceBase.OpenModeFlag.WriteOnly
    dstream = QtCore.QDataStream(ba, FLAG)

    dstream.writeUInt32(0)
    dstream.writeUInt16(scodes.ServerReplies.firstConnectionSuccess)
    dstream.writeQString("")

    dstream.device().seek(0)
    dstream.writeUInt32(ba.size() - 4)

    sock.send(ba.data())


def mentor_handler(sock, mentor_info, str):
    """Handle mentor connection
    Send cource, skillpack and progress of each student files
    """
    send_cource(sock, mentor_info)
    print("send_cource done")

    send_skillpack(sock, mentor_info)
    print("send_skillpack done")

    students = dbq.get_mentor_students(mentor_info[0])
    for student_info in students:
        send_progress(sock, student_info)
        print("send_progress of student", student_info[0], "done")

    ba = QtCore.QByteArray()
    FLAG = QtCore.QIODeviceBase.OpenModeFlag.WriteOnly
    dstream = QtCore.QDataStream(ba, FLAG)

    dstream.writeUInt32(0)
    dstream.writeUInt16(scodes.ServerReplies.firstConnectionSuccess)
    dstream.writeQString("")

    dstream.device().seek(0)
    dstream.writeUInt32(ba.size() - 4)

    sock.send(ba.data())


def user_handler(sock, name, param):
    """Handle client connection
    Authentication, authorization and call right handler for client
    """
    if not name.isalpha() or not param.isalpha():
        print("ERR: letters only!!!")

    user_info = dbq.get_users(name)

    if len(user_info) == 1:
        user_info = user_info[0]
    else:
        print("ERR: too many users:", user_info)

    if user_info[1] == "student":
        student_handler(sock, user_info, param)
    elif user_info[1] == "mentor":
        mentor_handler(sock, user_info, param)
    else:
        print("ERR: something went wrong with user:", user_info)


if __name__ == "__main__":
    # Load .env file
    load_dotenv()
    print(os.getenv("VAR"))

    global dbq
    dbq = DBQuery(os.getenv("DB_PATH"))

    PORT = int(os.getenv("PORT"))

    # Server configuration
    sock = socket.socket()
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(("127.0.0.1", PORT))
    sock.listen(1)

    while True:
        # Wating for clients
        conn, addr = sock.accept()
        print("New client connected!")
        print(addr)

        # Get client request
        data = conn.recv(2048)

        byte_array = QtCore.QByteArray(data)
        FLAG = QtCore.QIODeviceBase.OpenModeFlag.ReadOnly
        qdata_stream = QtCore.QDataStream(byte_array, FLAG)

        BLOCK_SIZE = qdata_stream.readUInt32()
        SOME_STR0 = qdata_stream.readQString()
        CODE = qdata_stream.readUInt16()
        SOME_STR1 = qdata_stream.readQString()

        user_handler(conn, SOME_STR0, SOME_STR1)

        conn.close()
        print("Connection closed\n")
