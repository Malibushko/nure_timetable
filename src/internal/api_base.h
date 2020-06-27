#pragma once
#include "api_structs.h"
namespace timetable::internal::api {

class ApiBase : public QObject {
    Q_OBJECT
public:
    virtual void groups() = 0;
    virtual void teachers() = 0;
signals:
    void groupResponse(const QVariant& group);
    void teacherResponse(const QVariant& teacher);
    void error(const QString& description);
};

}
