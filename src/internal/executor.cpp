#include "executor.h"

Promise::Promise(std::function<QVariant ()> executor)
    : m_state{std::make_shared<State>(State{executor})}
{}

void Promise::then(std::function<void (QVariant)> resolve) { m_state->resolve = resolve; }

void Promise::then(std::function<void (QVariant)> resolve, std::function<void (std::exception_ptr)> reject)
{
    m_state->resolve = resolve;
    m_state->reject = reject;
}

void Promise::execute()
{
    try {
        m_state->result = m_state->executor();
    } catch (...) {
        m_state->exception = std::current_exception();
    }
}

void Promise::notify()
{
    if (m_state->exception) {
        if (m_state->reject)
            m_state->reject(m_state->exception);
    } else {
        if (m_state->resolve)
            m_state->resolve(m_state->result);
    }
}

ExecutorService::ExecutorService(QObject *parent)
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

ExecutorService::~ExecutorService()
{
    m_thread.quit();
    m_thread.wait();
}

QThread *ExecutorService::thread() {
    return &m_thread;
}

Promise ExecutorService::submit(std::function<QVariant ()> executor)
{
    Promise promise{std::move(executor)};
    emit execute(promise);
    return promise;
}
