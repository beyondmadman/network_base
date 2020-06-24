#include "CSThreadPool.h"

CSThreadPool::CSThreadPool()
	: m_bRun(false)
{
}


CSThreadPool::~CSThreadPool()
{
	Destroy();
}

void CSThreadPool::AddTask(const Task& task)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_conEmpty.wait(lock, [this] {
		return !m_bRun.load() || (m_tasks.size() < m_taskNum);
	});

	if (!m_bRun.load())
	{
		return;
	}
		
	m_tasks.emplace(std::move(task));
	m_conFull.notify_one();
}

void CSThreadPool::Create(size_t taskNum, size_t threadNum)
{
	m_taskNum = taskNum;
	m_threadNum = threadNum > 1 ? threadNum : std::thread::hardware_concurrency();
	m_bRun.store(true);

	for (size_t i = 0; i < m_threadNum; ++i)
	{
		m_workers.emplace_back(std::thread (&CSThreadPool::Work, this));
	}
}
void CSThreadPool::Destroy()
{
	m_bRun.store(false);

	m_conFull.notify_all();
	m_conEmpty.notify_all();

	for (std::thread &worker : m_workers)
	{
		if (worker.joinable())
		{
			worker.join();
		}
	}

	m_workers.clear();
}

void CSThreadPool::Work()
{
	while (true)
	{
		Task task;

		std::unique_lock<std::mutex> lock(m_mutex);
		m_conFull.wait(lock, [this] {
			return !m_bRun.load() || !m_tasks.empty();
		});

		if (!m_bRun.load() && m_tasks.empty())
			break;

		//task = std::move(m_tasks.front());
		//m_tasks.pop();
		//m_conEmpty.notify_one();

		do
		{

			task = std::move(m_tasks.front());
			m_tasks.pop();

			task();
		} while (m_tasks.size() > 0);

		m_conEmpty.notify_one();

		/*if (task)
		{
			task();
		}*/
	}
}