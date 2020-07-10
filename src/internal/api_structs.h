#pragma once
#include <QMetaObject>
#include <QObject>
#include <QDate>
#include <QDebug>
#include <vector>
#include <string>

#include "macro.h"

namespace timetable::internal {

using string_t = QString;

template <class T>
using container_t = QList<T>;

using int_t = int;
using int_long_t = int64_t;

struct Department;
struct Teacher;
struct Faculty;
struct Direction;

struct Department {
    declare_struct(Department)
    declare_field(int_t,id);
    declare_field(string_t,short_name);
    declare_field(string_t,full_name);
    declare_field(container_t<Teacher>,teachers);
};

struct Teacher {
    declare_struct(Teacher)
    declare_field(int_t,id);
    declare_field(string_t,short_name);
    declare_field(string_t,full_name);
};
struct Faculty {
    declare_struct(Faculty)
    declare_field(int_t,id);
    declare_field(string_t,short_name);
    declare_field(string_t,full_name);
    declare_field(container_t<Department>,departments);
    declare_field(container_t<Direction>,directions);
};
struct AuditoryType {
    declare_struct(AuditoryType)
    declare_field(string_t,id);
    declare_field(string_t,short_name);
};
struct AuditoryTypes {
    declare_struct(AuditoryTypes)
    declare_field(container_t<AuditoryType>,auditory_types);
};
struct Auditory {
    declare_struct(Auditory)
    declare_field(string_t,id);
    declare_field(string_t,short_name);
    declare_field(string_t,floor);
    declare_field(string_t,is_have_power);
    declare_field(container_t<AuditoryType>,auditory_types);
};

struct University {
    declare_struct(University)
    declare_field(string_t,short_name);
    declare_field(string_t,full_name);
    declare_field(container_t<Faculty>,faculties);
};
struct Group {
    declare_struct(Group)
    declare_field(int_t,id);
    declare_field(string_t,name);
};

struct Speciality {
    declare_struct(Speciality)
    declare_field(int_t,id);
    declare_field(string_t,short_name);
    declare_field(string_t,full_name);
    declare_field(container_t<Group>,groups);
};

struct Direction {
    declare_struct(Direction)
    declare_field(int_t,id);
    declare_field(string_t,short_name);
    declare_field(string_t,full_name);
    declare_field(container_t<Speciality>,specialities);
    declare_field(container_t<Group>,groups);
};

struct UniversityWrapper {
    declare_struct(UniversityWrapper)
    declare_field(University,university);
};
struct DepartmentWrapper {
    declare_struct(DepartmentWrapper)
    declare_field(Department,department);
};
struct FacultyWrapper {
    declare_struct(FacultyWrapper)
    declare_field(Faculty,faculty);
};
struct Timetable {
    declare_struct(Timetable)
    declare_field(int,id);
    declare_field(string_t,title);
};

struct Lesson {
    declare_struct(Lesson)
    declare_field(QDate,date);
    declare_field(QString,subject);
    declare_field(QString,type);
    declare_field(QString,groups);
    declare_field(QString,auditory);
    declare_field(QTime,timeStart);
    declare_field(QTime,timeEnd);
    Lesson() = default;
    static Lesson fromCSV(const QString & csvLine) {
        /*
         * Many parts of this function are really hard to understand
         * This is because NURE API sends wrong and oftenly
         * unparsable data (like different type of separators in a single (!) string)
         * I used heuristic with regex and some other fixes
         * that should work in most cases but not always
         * Do not try to understand it, this function will be replaced
         * as soon as new NURE API will be available (current one is legacy)
         */
        QString editText;
        QStringList innerList = csvLine.split('\"',Qt::SkipEmptyParts);
        if (innerList.size() < 4)
            return {};
        innerList.erase(std::remove_if(innerList.begin(),innerList.end(),[](QString string){
                            return string.size() <= 1;
                        }),innerList.end());

        QString firstToken = innerList.first();
        QRegExp exp("[,]\\s+");
        firstToken.replace(exp,",");
        qDebug() << innerList;
        Lesson lesson;
        
        auto tokens = firstToken.split(' ');
        while (tokens.size() >= 4) {
            lesson.groups = tokens.takeLast();
            lesson.auditory += (lesson.auditory.size()? QString("/")+ tokens.takeLast() : tokens.takeLast());
            lesson.type += (lesson.type.size()? QString("/")+ tokens.takeLast() : tokens.takeLast());
            if (lesson.subject.size())
                lesson.subject += "/";
            QStringList buff;
            while (!tokens.empty() && !tokens.last().contains(';')) {
                buff << tokens.takeLast();
            }
            for (auto it = buff.rbegin();it != buff.rend();++it)
                lesson.subject += ' ' + (*it);
        }
        lesson.subject = lesson.subject.trimmed();
        lesson.date = QDate::fromString(innerList.at(1),"dd.MM.yyyy");
        auto timeParsed = {innerList.at(2).chopped(3),innerList.at(4).chopped(3)};
        assert(timeParsed.size() == 2);
        lesson.timeStart = QTime::fromString(*timeParsed.begin());
        lesson.timeEnd = QTime::fromString(*(timeParsed.begin()+1));
        return  lesson;    
    }
};
struct SavedTimetable {
    declare_struct(SavedTimetable)
    declare_field(int,id);
    declare_field(string_t,title);
    declare_field(string_t,lastUpdate);
    declare_field(bool,isTeacher);
    declare_field(std::vector<Lesson>,lessons);
};
}
Q_DECLARE_METATYPE(timetable::internal::Department)
Q_DECLARE_METATYPE(timetable::internal::Teacher)
Q_DECLARE_METATYPE(timetable::internal::Faculty)
Q_DECLARE_METATYPE(timetable::internal::AuditoryType)
Q_DECLARE_METATYPE(timetable::internal::AuditoryTypes)
Q_DECLARE_METATYPE(timetable::internal::Auditory)
Q_DECLARE_METATYPE(timetable::internal::University)
Q_DECLARE_METATYPE(timetable::internal::Group)
Q_DECLARE_METATYPE(timetable::internal::Speciality)
Q_DECLARE_METATYPE(timetable::internal::Direction)
Q_DECLARE_METATYPE(timetable::internal::Timetable)
Q_DECLARE_METATYPE(timetable::internal::Lesson)
Q_DECLARE_METATYPE(timetable::internal::SavedTimetable)
