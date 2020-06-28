#ifndef CSINGLETON_H
#define CSINGLETON_H

#include <mutex>

template <class T>
class CSSingleton
{
private:
	CSSingleton() = default;
	~CSSingleton() = default;
	CSSingleton(const CSSingleton&) = delete;
	CSSingleton& operator=(const CSSingleton&) = delete;

public:
	template <typename... Args>
	static T* Instance(Args&&... args)
	{
		std::call_once(m_flag, [&]() {
			m_instance = new T(std::forward<Args>(args)...);
		});
		return m_instance;
	}

	static void Destroy()
	{
		if (m_instance)
		{
			delete m_instance;
			m_instance = NULL;
		}
	}

private:
	static T*						m_instance;
	static std::once_flag			m_flag;
};

template <class T>
T* CSSingleton<T>::m_instance = NULL;

template <class T>
std::once_flag CSSingleton<T>::m_flag;

#endif
