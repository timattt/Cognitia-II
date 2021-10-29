import socket
import os
from dotenv import load_dotenv
from PyQt6 import QtCore

import server_codes as scodes


def send_files(sock, master_dir, files):
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


def send_progress(sock, name):
    PROGRESS_DIR = os.getenv("PROGRESS_DIR")
    STUDENT_PROGRESS_FILE_EXTENSION = os.getenv("STUDENT_PROGRESS_FILE_EXTENSION")

    PROGRESS = os.listdir(PROGRESS_DIR)

    PROGRESS = list(
        filter(lambda x: x.endswith(STUDENT_PROGRESS_FILE_EXTENSION), PROGRESS),
    )

    send_files(sock, PROGRESS_DIR, PROGRESS)


def reg_usr_handler(sock, name, str):
    send_cource(sock, name)
    print("send_cource done")

    send_skillpack(sock, name)
    print("send_skillpack done")

    send_progress(sock, name)
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


if __name__ == "__main__":
    load_dotenv()
    print(os.getenv("VAR"))

    sock = socket.socket()
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(("127.0.0.1", 1917))
    sock.listen(1)

    # Wating for clients
    while True:
        conn, addr = sock.accept()
        print("New client connected!")
        print(addr)

        data = conn.recv(2048)

        byte_array = QtCore.QByteArray(data)
        FLAG = QtCore.QIODeviceBase.OpenModeFlag.ReadOnly
        qdata_stream = QtCore.QDataStream(byte_array, FLAG)

        BLOCK_SIZE = qdata_stream.readUInt32()
        SOME_STR0 = qdata_stream.readQString()
        CODE = qdata_stream.readUInt16()
        SOME_STR1 = qdata_stream.readQString()

        reg_usr_handler(conn, SOME_STR0, SOME_STR1)

        conn.close()
        print("Connection closed")
