#pragma once
#include "api_structs.h"
/**
 *  \brief Base class for API
 *
 * This is the interface for API classes
 * NURE has 3 differen API so a user may want to
 * any of them, because they differ in reply speed
 */
namespace timetable::internal::api {

class ApiBase : public QObject {
    Q_OBJECT
public:
    /**
     * @brief request for groups list
     * \return no value, emit a groupResponse signal
     */
    virtual void groups() = 0;
    /**
     * @brief request for teachers list
     * \return no value, emit a teacherRespone signal
     */
    virtual void teachers() = 0;
signals:
    /**
     * @brief signal emit when a group was parsed and ready to be added to list
     * @param group the group that was parsed from JSON
     */
    void groupResponse(const QVariant& group);
    /**
     * @brief signal emit when a teacher was parsed and ready to be added to list
     * @param teacher the teacher that was parsed from JSON
     */
    void teacherResponse(const QVariant& teacher);
    /**
     * @brief signal describes the exceptional situation and contains it`s description
     * @param description description to be shown to the user
     */
    void error(const QString& description);
};

}
