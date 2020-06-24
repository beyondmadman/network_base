#ifndef CSBUFFER_H
#define CSBUFFER_H

#include<cstdint>
#include<string>

class CSBuffer
{
public:
        CSBuffer(size_t len = 1024)
	{
		m_pBegin = new char[len];
		m_pEnd = m_pBegin + len;
		m_pReader = m_pWriter = m_pBegin;
	}

        ~CSBuffer()
	{
		if (NULL != m_pBegin)
		{
			delete[] m_pBegin;
			m_pBegin = m_pEnd = m_pReader = m_pWriter = NULL;
		}
	}

	size_t GetWritableBytes() const
	{
		return m_pEnd - m_pWriter;
	}

	const char* BeginWrite() const
	{
		return m_pWriter;
	}

	void setWritePos(size_t len)
	{
		m_pWriter += len;
	}

	bool Write(const void *pData, size_t len)
	{
		if (EnsureWritable(len))
		{
			memcpy(m_pWriter, (void *)pData, len);
			setWritePos(len);
			return true;
		}
		return false;
	}

	//char
	bool WriteInt8(int8_t n)
	{
		return Write(n);
	}
	//short
	bool WriteInt16(int16_t n)
	{
		return Write(n);
	}

	//int
	bool WriteInt32(int32_t n)
	{
		return Write(n);
	}
	
	//float
	bool WriteFloat(float n)
	{
		return Write(n);
	}

	//double
	bool WriteDouble(double n)
	{
		return Write(n);
	}

	//array
	template<typename T>
	bool WriteArray(T* pData, size_t len)
	{
		return Write(pData, len);
	}

	size_t GetReadableBytes() const
	{
		return m_pWriter - m_pReader;
	}

	const char* BeginRead() const
	{
		return m_pReader;
	}

	void setReadPos(size_t len)
	{
		if (len < GetReadableBytes())
		{
			m_pReader += len;
		}	
		else
		{
			m_pReader = m_pBegin;
			m_pWriter = m_pBegin;
		}
			
	}

	bool Read(void *pData, size_t len)
	{
		if (EnsureReadable(len))
		{
			memcpy(pData, m_pReader, len);
			setReadPos(len);
			return true;
		}
		return false;
	}

	//char 
	int8_t ReadInt8(int8_t n = 0)
	{
		Read(n);
		return n;
	}

	//short
	int16_t ReadInt16(int16_t n = 0)
	{
		Read(n);
		return n;
	}

	//int
	int32_t ReadInt32(int32_t n = 0)
	{
		Read(n);
		return n;
	}

	//float
	float ReadFloat(float n = 0.0f)
	{
		Read(n);
		return n;
	}
	
	//doule
	double ReadDouble(double n = 0.0f)
	{
		Read(n);
		return n;
	}

	//array
	template<typename T>
	bool ReadArray(T* pArr, size_t len)
	{
		return Read(pArr, len * sizeof(T));
	}

	bool EnsureWritable(size_t len)
	{
		if (GetWritableBytes() < len)
		{
			if (GetWritableBytes() + (m_pReader - m_pBegin) < len )
			{
				size_t readable = GetReadableBytes();
				size_t newSize = (m_pWriter - m_pBegin) + len;
				char *pTemp = new char[newSize];
				if (NULL == pTemp)
				{
					return false;
				}
				memcpy(pTemp, m_pReader, readable);

				delete [] m_pBegin;
				m_pBegin = pTemp;
				m_pReader = m_pBegin;
				m_pWriter = m_pReader + readable;
				m_pEnd = m_pBegin + newSize;
			}
			else
			{
				size_t readable = GetReadableBytes();
				memmove(m_pBegin, m_pReader, readable);
				m_pReader = m_pBegin;
				m_pWriter = m_pReader + readable;
			}
		}

		return true;
	}

	bool EnsureReadable(size_t len)
	{
		if (GetReadableBytes() < len)
		{
			return false;
		}

		return true;
	}

private:
		template<typename T>
		bool Write(T n)
		{
			return Write(&n, sizeof(n));
		}

		template<typename T>
		void Read(T& n)
		{
			Read(&n, sizeof(n));
		}
private:
	char					*m_pBegin;
	char					*m_pReader;
	char					*m_pWriter;
	char					*m_pEnd;
};

#endif

