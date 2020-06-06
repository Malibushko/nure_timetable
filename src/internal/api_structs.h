#include <QMetaObject>
#include <QObject>
#include <QDate>

#include <vector>
#include <string>

#include "macro.h"

namespace timetable::internal {

using string_t = std::string;
using int_t = int32_t;
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
    declare_field(std::vector<Teacher>,teachers);
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
    declare_field(std::vector<Department>,departments);
    declare_field(std::vector<Direction>,directions);
};
struct AuditoryType {
    declare_struct(AuditoryType)
    declare_field(string_t,id);
    declare_field(string_t,short_name);
};
struct AuditoryTypes {
    declare_struct(AuditoryTypes)
    declare_field(std::vector<AuditoryType>,auditory_types);
};
struct Auditory {
    declare_struct(Auditory)
    declare_field(string_t,id);
    declare_field(string_t,short_name);
    declare_field(string_t,floor);
    declare_field(string_t,is_have_power);
    declare_field(std::vector<AuditoryType>,auditory_types);
};

struct University {
    declare_struct(University)
    declare_field(string_t,short_name);
    declare_field(string_t,full_name);
    declare_field(std::vector<Faculty>,faculties);
};
struct Group {
    declare_struct(Group);
    declare_field(int_t,id);
    declare_field(string_t,name);
};

struct Speciality {
    declare_struct(Speciality)
    declare_field(int_t,id);
    declare_field(string_t,short_name);
    declare_field(string_t,full_name);
    declare_field(std::vector<Group>,groups);
};

struct Direction {
    declare_struct(Direction)
    declare_field(int_t,id);
    declare_field(string_t,short_name);
    declare_field(string_t,full_name);
    declare_field(std::vector<Speciality>,specialities);
    declare_field(std::vector<Group>,groups);
};

struct Lesson {
    QDate date;
    qint32 lesInd;
    QString subject;
    QString type;
    QString auditory;
    QString timeStart;
    QString note;
    Lesson() = default;
    static Lesson fromCSV(const QString & csvLine) {
        QString editText;
        QStringList innerList = csvLine.split('\"',QString::SplitBehavior::SkipEmptyParts);
        innerList.erase(std::remove_if(innerList.begin(),innerList.end(),[](QString string){
                            return string.size() <= 1;
                        }),innerList.end());
        
        QString firstToken = innerList.first();
        QRegExp exp("[,]\\s+");
        firstToken.replace(exp,",");
        Lesson lesson;
        
        auto tokens = firstToken.split(' ');
        while (tokens.size() >= 4) {
            tokens.removeLast();
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
        lesson.date = QDate::fromString(innerList.at(1),"dd.MM.yyyy");
        auto time_start = QTime::fromString(innerList.at(2),"h:mm:ss");
        lesson.timeStart = innerList.at(2).chopped(3)+ '\n'+ innerList.at(4).chopped(3);
        //     lesson.lesInd = lesson.detectLessonType(time_start);
        return  lesson;    
    }
};

}
