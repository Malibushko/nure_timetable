#pragma once
#include "api_structs.h"
namespace timetable::internal::api {

class ApiBase {

public:
    virtual void faculties() = 0;
    virtual void departments(int32_t p_id_faculty) = 0;
    virtual void teachers(int32_t p_id_department) = 0;
    virtual void directions(int32_t p_id_faculty) = 0;
    virtual void specialities(int32_t p_id_faculty,int32_t p_id_department) = 0;
    virtual void schedule(int32_t p_group_id) = 0;

signals:
    void facultiesResponse(const QList<Faculty> & faculties);
    void departmentsResponse(const QList<Department> & faculties);
    void teachersResponse(const QList<Teacher> & faculties);
    void directionsResponse(const QList<Direction> & faculties);
    void specialitiesResponse(const QList<Speciality> & faculties);
    void scheduleResponse(const QList<Lesson> & faculties);

    void error(const QString& description);
};

}
