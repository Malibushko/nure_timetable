#pragma once
#include "api_structs.h"
namespace timetable::internal::api {

class ApiBase : public QObject {
    Q_OBJECT
public:
    virtual void faculties() = 0;
    virtual void departments(int p_id_faculty) = 0;
    virtual void teachers(int p_id_department) = 0;
    virtual void directions(int p_id_faculty) = 0;
    virtual void specialities(int p_id_faculty,int p_id_department) = 0;

    virtual void groups() = 0;
    virtual void teachers() = 0;

signals:
    void facultiesResponse(const QList<Faculty> & faculties);
    void departmentsResponse(const QList<Department> & departments);
    void teachersResponse(const QList<Teacher> & teachers);
    void directionsResponse(const QList<Direction> & direction);
    void specialitiesResponse(const QList<Speciality> & specialities);
    void groupResponse(const QVariant& group);
    void teacherResponse(const QVariant& teacher);
    void error(const QString& description);
};

}
