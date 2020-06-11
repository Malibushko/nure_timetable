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
    explicit Promise(std::function<QVariant()> executor)
        : m_state{std::make_shared<State>(State{executor})}
    {}
    void then(std::function<void(QVariant)> resolve) { m_state->resolve = resolve; }
    void then(std::function<void(QVariant)> resolve, std::function<void(std::exception_ptr)> reject)
    {
        m_state->resolve = resolve;
        m_state->reject = reject;
    }
private:
    void execute()
    {
        try {
            m_state->result = m_state->executor();
        } catch (...) {
            m_state->exception = std::current_exception();
        }
    }
    void notify()
    {
        if (m_state->exception) {
            if (m_state->reject)
                m_state->reject(m_state->exception);
        } else {
            if (m_state->resolve)
                m_state->resolve(m_state->result);
        }
    }
};

Q_DECLARE_METATYPE(Promise)


class ExecutorService : public QObject
{
    Q_OBJECT
private:
    QThread m_thread;

public:
    explicit ExecutorService(QObject *parent = nullptr)
        : QObject{parent}
    {
        QObject *context = new QObject;
        context->moveToThread(&m_thread);
        connect(&m_thread, &QThread::finished, context, &QObject::deleteLater);
        connect(this, &ExecutorService::execute, context, [this](Promise p) {
            p.execute();
            emit ready(std::move(p));
        });
        connect(this, &ExecutorService::ready, this, [](Promise p) { p.notify(); });
        m_thread.start();
    }
    ~ExecutorService()
    {
        m_thread.quit();
        m_thread.wait();
    }
    QThread * thread() {
        return &m_thread;
    }
    Promise submit(std::function<QVariant()> executor)
    {
        Promise promise{std::move(executor)};
        emit execute(promise);
        return promise;
    }
signals:
    void execute(Promise);
    void ready(Promise);
};

