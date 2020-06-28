#pragma once
#include <QObject>
#include <QDebug>
#include <QThread>

#include <exception>
#include <functional>
#include <memory>

class ExecutorService;
class Promise
{
    friend class ExecutorService;
    struct State
    {
        std::function<QVariant()> executor;
        std::function<void(QVariant)> resolve;
        std::function<void(std::exception_ptr)> reject;
        QVariant result;
        std::exception_ptr exception;
    };
    std::shared_ptr<State> m_state;
public:
    Promise() = default;
    explicit Promise(std::function<QVariant()> executor);
    void then(std::function<void(QVariant)> resolve);
    void then(std::function<void(QVariant)> resolve, std::function<void(std::exception_ptr)> reject);
private:
    void execute();
    void notify();
};

Q_DECLARE_METATYPE(Promise)


class ExecutorService : public QObject
{
    Q_OBJECT
private:
    QThread m_thread;

public:
    explicit ExecutorService(QObject *parent = nullptr);
    ~ExecutorService();
    QThread * thread();
    Promise submit(std::function<QVariant()> executor);
signals:
    void execute(Promise);
    void ready(Promise);
};
