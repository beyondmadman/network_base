#ifndef CSOBJECTPOOL_H
#define CSOBJECTPOOL_H

#include <vector>
#include <mutex>
#include <assert.h>

template<typename T>
class CSMemoryBlocks
{
public:
	CSMemoryBlocks()
	{
		InitPool();
	}

	~CSMemoryBlocks()
	{
		ReleaseChunk();
	}

	T *Get()
	{
		T *ret = NULL;

		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_freeBlocks.empty())
		{
			ret = m_freeBlocks.back();
			m_freeBlocks.pop_back();
		}
		else
		{
			if (m_pFreeHead == (m_chunks.back() + (m_chunks.size() * DefaultBlockSize)))
			{
				AllocateChunk();
				m_pFreeHead = m_chunks.back();
			}
			ret = m_pFreeHead;
			++m_pFreeHead;
		}
		return ret;
	}

	void Put(T *pBlock)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_freeBlocks.push_back(pBlock);
	}

private:

	void InitPool()
	{
		AllocateChunk();
		m_pFreeHead = m_chunks.back();
	}
	void AllocateChunk()
	{
		T* pChunk = reinterpret_cast< T* >(malloc(DefaultBlockSize * sizeof(T)));
		if (NULL == pChunk)
		{
			assert(false);
			return;
		}

		m_chunks.push_back(pChunk);
	}

	void ReleaseChunk()
	{
		while (!m_chunks.empty())
		{
			T *pointer = m_chunks.back();
			m_chunks.pop_back();
			free(pointer);
		}

		m_freeBlocks.clear();
		m_pFreeHead = NULL;
	}

	std::mutex						m_mutex;
	T*                              m_pFreeHead;
	std::vector<T *>				m_chunks;
	std::vector<T *>				m_freeBlocks;

	static const size_t DefaultBlockSize = 128;
};

template<typename T>
class ObjectPool
{
public:
	template<typename ...Args>
	T *Get(Args ... args)
	{
		T *pObj = m_pool.Get();
		new(pObj) T(args...);
		return pObj;
	}

	void Put(T *pObj)
	{
		if (nullptr == pObj)
		{
			assert(false);
			return;
		}

		pObj->~T();
		m_pool.Put(pObj);
	}

private:
	CSMemoryBlocks<T>				  m_pool;
};

#endif
