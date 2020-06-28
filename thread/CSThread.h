#ifndef CSTHREAD_H
#define CSTHREAD_H

#include <thread>
#include <functional>
#include <atomic>
#include <assert.h>

class CSThread
{
public:
	static void Sleep(time_t dt)
	{
		std::chrono::milliseconds t(dt);
		std::this_thread::sleep_for(t);
	}

private:
	typedef std::function<void()> ThreadFunc;

public:
	CSThread()
		: m_isRun(false)
	{
	}

	~CSThread()
	{
		if (!IsRun())
		{
			m_isRun = false;
		}

		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}

	void Start(const ThreadFunc& func)
	{
		assert(!m_isRun);
		m_isRun = true;
		m_func = func;
		std::thread t(&CSThread::Run, this);
		m_thread = std::move(t);
		m_thread.join();
	}

	bool IsRun() const { return m_isRun; }

	void Run()
	{
		   m_func();
	}

	std::thread::id GetId()
	{
		return m_thread.get_id();
	}


private:
	std::thread                     m_thread;
	ThreadFunc                      m_func;
	std::atomic<bool>				m_isRun;
};

#endif
