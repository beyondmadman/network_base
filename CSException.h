#include <exception>
#include <string>

class CSException : public std::exception
{
public:
	explicit CSException(const char* msg)
		: m_message(msg)
	{
	}

	explicit CSException(const std::string& msg)
		: m_message(msg)
	{
	}

	virtual ~CSException() throw()
	{
	}

	virtual const char* what() const throw()
	{
		return m_message.c_str();
	}

private:
	std::string				m_message;
};
