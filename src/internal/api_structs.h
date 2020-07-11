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
/**
 * @brief The Department struct
 *
 * Describles the department in the university
 */
struct Department {
    declare_struct(Department)
    declare_field(int_t,id); /**< id of department */
    declare_field(string_t,short_name); /**< short name of department */
    declare_field(string_t,full_name); /**<  full name of department */
    declare_field(container_t<Teacher>,teachers); /**< teachers working in the department */
};
/**
 * @brief The Teacher struct
 *
 * Describes the teacher in the university
 */
struct Teacher {
    declare_struct(Teacher)
    declare_field(int_t,id); /**< teacher id */
    declare_field(string_t,short_name); /**< initials of the teacher */
    declare_field(string_t,full_name); /**<  full name of the teacher */
};
/**
 * @brief The Faculty struct
 *
 * Describes the faculty in the university
 */
struct Faculty {
    declare_struct(Faculty)
    declare_field(int_t,id); /**< faculty id */
    declare_field(string_t,short_name); /**< Short name of the faculty */
    declare_field(string_t,full_name); /**<  Full name of the faculty */
    declare_field(container_t<Department>,departments); /**< Departments of faculty */
    declare_field(container_t<Direction>,directions);/**<  Directions of faculty */
};
/**
 * @brief The AuditoryType struct
 *
 * Describes type of auditory
 */
struct AuditoryType {
    declare_struct(AuditoryType)
    declare_field(string_t,id); /**<  Auditory id */
    declare_field(string_t,short_name); /**< Auditory short name */
};
/**
 * @brief The AuditoryTypes struct
 *
 * Describes the container of AuditoryType
 * Used for JSON parsing
 */
struct AuditoryTypes {
    declare_struct(AuditoryTypes)
    declare_field(container_t<AuditoryType>,auditory_types); /**<  Container of AuditoryType */
};
/**
 * @brief The Auditory struct
 *
 * Describes the auditory in the university
 */
struct Auditory {
    declare_struct(Auditory)
    declare_field(string_t,id); /**<  Auditory id */
    declare_field(string_t,short_name); /**<  Auditory short name */
    declare_field(string_t,floor); /**< Auditory floor */
    declare_field(string_t,is_have_power); /**< Check if auditory has power inside  */
    declare_field(container_t<AuditoryType>,auditory_types); /**< Types of the auditory */
};
/**
 * @brief The University struct
 *
 * Describes the whole university with faculties, departments, teachers  etc.
 */
struct University {
    declare_struct(University)
    declare_field(string_t,short_name); /**< Short name of the university */
    declare_field(string_t,full_name); /**<  Full name of the university*/
    declare_field(container_t<Faculty>,faculties); /**< Faculties of the university */
};
/**
 * @brief The Group struct
 *
 * Describes the group in the university
 */
struct Group {
    declare_struct(Group)
    declare_field(int_t,id); /**<  Group id*/
    declare_field(string_t,name); /**<  Group name */
};
/**
 * @brief The Speciality struct
 *
 * Describes the speciality in the university
 */
struct Speciality {
    declare_struct(Speciality)
    declare_field(int_t,id); /**< Speciality id */
    declare_field(string_t,short_name); /**<  Speciality shortened name */
    declare_field(string_t,full_name); /**<  Speciality full name */
    declare_field(container_t<Group>,groups); /**< groups of the speciality */
};
/**
 * @brief The Direction struct
 *
 * Describes the diretion in the university
 */
struct Direction {
    declare_struct(Direction)
    declare_field(int_t,id); /**< Direction id */
    declare_field(string_t,short_name); /**< Direction shortened name */
    declare_field(string_t,full_name); /**<  Direction full name*/
    declare_field(container_t<Group>,groups); /**<  Diretion groups */
    declare_field(container_t<Speciality>,specialities); /**<  Direction specialities \note Can contain another groups*/
};
/**
 * @brief The UniversityWrapper struct
 *
 * Wrapper for JSON parsing and transfering between models
 */
struct UniversityWrapper {
    declare_struct(UniversityWrapper)
    declare_field(University,university); /**<  University object */
};
/**
 * @brief The DepartmentWrapper struct
 *
 * Wrapper for JSON parsing and transfering between models
 */
struct DepartmentWrapper {
    declare_struct(DepartmentWrapper)
    declare_field(Department,department); /**< Department object */
};
/**
 * @brief The FacultyWrapper struct
 *
 * Wrapper for JSON parsing and transfering between models
 */
struct FacultyWrapper {
    declare_struct(FacultyWrapper)
    declare_field(Faculty,faculty); /**<  */
};
/**
 * @brief The Timetable struct
 *
 * Describes the timetable (generalization of Group and Teacher object)
 */
struct Timetable {
    declare_struct(Timetable)
    declare_field(int,id); /**<  Timetable id */
    declare_field(string_t,title); /**<  Timetable title */
};
/**
 * @brief The Lesson struct
 *
 * Describes the Lesson in the timetable (Displayed in a form of cell)
 * Also used for serialization
 */
struct Lesson {
    declare_struct(Lesson)
    declare_field(QDate,date); /**< Date of lesson */
    declare_field(QString,subject); /**<  Lesson subject */
    declare_field(QString,type); /**< Lesson type (Lecture/Practice/Credit/Exam etc. */
    declare_field(QString,groups); /**< Groups that must attend this lesson */
    declare_field(QString,auditory); /**< Auditory where the lesson takes place */
    declare_field(QTime,timeStart); /**< Start time of the lesson */
    declare_field(QTime,timeEnd); /**< End time of the lesson */
    Lesson() = default;
    /**
     * @brief Parses CSV line in order to get Lesson object
     * \note static function
     * @param csvLine line to parse
     * @return Lesson object
     * \note If line does not contain any information, the empty Lesson object will be returned
     */
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
/**
 * @brief The SavedTimetable struct
 *
 * Struct that contains information about user-saved timetable
 * Used for serialization
 */
struct SavedTimetable {
    declare_struct(SavedTimetable)
    declare_field(int,id); /**< Timetable id */
    declare_field(string_t,title); /**< Timetable title */
    declare_field(string_t,lastUpdate); /**< Last update QDateTime of this timetable */
    declare_field(bool,isTeacher); /**< Check whether this is teacher timetable or group one
                                    \note NURE API do not distinguish them, so there is a need for boolean flag */
    declare_field(std::vector<Lesson>,lessons); /**< Container of Lesson objects */
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
