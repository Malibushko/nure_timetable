#pragma once
#include <QObject>
#include <QDebug>
#include <QThread>

#include <exception>
#include <functional>
#include <memory>

class ExecutorService;

/**
 * @brief The Promise class
 *
 * Used for asynchronous requests for Database access
 *
 * \note The need in this class stems from the fact, that
 * QSqlDatabase does not allow to make requests from other threads,
 * expect for one, that is was created in. So we have to move the
 * whole database to a single thread and use Promises for asycnhronous
 * requests (and QEventloop when synchronization is required)
 */
class Promise
{
    friend class ExecutorService;
    /**
     * @brief The State struct
     *
     * Contains all neccessary information about executing
     */
    struct State
    {
        std::function<QVariant()> executor; /**< Code to execute */
        std::function<void(QVariant)> resolve;  /**< Action that must happen after execution */
        std::function<void(std::exception_ptr)> reject;  /**< Action that must happen if exception is thrown */
        QVariant result;  /**< QVariant object that contain the result of execution */
        std::exception_ptr exception;  /**< Exception that has been thrown during execution */
    };
    std::shared_ptr<State> m_state;
public:
    /**
      Default ctor
      */
    Promise() = default;
    /**
     * @brief Promise construction with one parameter
     * @param executor code to execute
     */
    explicit Promise(std::function<QVariant()> executor);
    /**
     * @brief add action for execution pipeline
     * @param action to add
     */
    void then(std::function<void(QVariant)> resolve);
    /**
     * @brief add action for execution pipeline
     * @param action to add
     * @param action in order if exception is thrown
     */
    void then(std::function<void(QVariant)> resolve, std::function<void(std::exception_ptr)> reject);
private:
    void execute();
    void notify();
};

Q_DECLARE_METATYPE(Promise)

/**
 * @brief The ExecutorService class
 *
 * Class performs all neccessary operations on Promise object
 */
class ExecutorService : public QObject
{
    Q_OBJECT
private:
    QThread m_thread;

public:
    explicit ExecutorService(QObject *parent = nullptr);
    ~ExecutorService();
    /**
     * @brief thread
     * @return execution thread object
     */
    QThread * thread();
    /**
     * @brief start execution with executor param
     * @param code to execute
     * @return Promise object that contains the result
     */
    Promise submit(std::function<QVariant()> executor);
signals:
    void execute(Promise);
    /**
     * @brief signal emited done
     */
    void ready(Promise);
};
