#ifndef CSTHREADPOOL_H
#define CSTHREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>

class CSThreadPool
{
private:
	typedef std::function<void()> Task;

public:
	CSThreadPool();
	~CSThreadPool();

	void AddTask(const Task& task);
	void Create(size_t queueNum, size_t threadNum);
	void Destroy();

private:
	void Work();

private:
	
	size_t                          m_threadNum;
	std::vector<std::thread>	    m_workers;
	std::queue<Task>				m_tasks;
	size_t                          m_taskNum;
	std::mutex						m_mutex;
	std::condition_variable			m_conFull;
	std::condition_variable			m_conEmpty;
	std::atomic<bool>               m_bRun;
};

#endif

