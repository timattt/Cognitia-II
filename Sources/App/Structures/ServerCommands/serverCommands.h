#ifndef SERVERCOMMANDS_H
#define SERVERCOMMANDS_H

enum CommandsToserv{
    sendCourse = 0,
    sendSkillpack,
    sendStudentProgress,

    saveStudentProgress,
    saveCourse,
    saveSkillpack,

    getUserName
};

enum ServerReplies{
    retrieveSkillpack = 0,
    retrieveCourseUnit,
    retrieveStudentProgress,
    retrieveSuccess,
    retrieveFail
};

#endif // SERVERCOMMANDS_H