#include "Cstring.h"

Cstring::Cstring()
{
}

Cstring::Cstring(const string& str) : string(str)
{
}

Cstring::Cstring(const char* pstr) : string(pstr)
{
}

Cstring::Cstring(const Cstring& str) : string(str)
{
}

Cstring::Cstring(const TCHAR *pstr)
{
	int nSize = _tcslen(pstr);
	char *pBuff = new char[nSize * 2]();
	TcharToChar(pstr, pBuff);
	this->clear();
	this->append(pBuff);
	delete[] pBuff;
}

Cstring::Cstring(const CString& str)
{
	int nSize = str.GetLength();
	char *pStr = new char[nSize * 2]();
	TcharToChar(str.GetString(), pStr);
	this->clear();
	this->append(pStr);
	delete[] pStr;
	pStr = NULL;
}

Cstring::~Cstring(void)
{
}

Cstring& Cstring::operator =(const Cstring& str)
{
	this->clear();
	this->append(str.c_str());

	return *this;
}

void Cstring::TcharToChar(const TCHAR * tchar, char * _char)
{
    int iLength = 0;
    //获取字节长度   
    iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
    //将tchar值赋给_char    
    WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}

void Cstring::CharToTchar(const char * _char, TCHAR * tchar)
{
    int iLength = 0;
    iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}

Cstring& Cstring::operator =(const TCHAR* pStr)
{
	int nSize = _tcslen(pStr);
	char *pBuff = new char[nSize * 2]();
	TcharToChar(pStr, pBuff);

	m_str.Empty();
	this->clear();
	this->append(pBuff);
	delete[] pBuff;
	return *this;
}

Cstring& Cstring::operator =(const CString& str)
{
	int nSize = str.GetLength();
	char *pStr = new char[nSize * 2]();
	TcharToChar(str.GetString(), pStr);
	this->clear();
	this->append(pStr);
	delete[] pStr;
	pStr = NULL;
	return *this;
}

Cstring::operator TCHAR*()
{
	int nSize = this->size();
	m_str.Empty();
	CharToTchar(this->c_str(), m_str.GetBuffer(nSize));
	m_str.ReleaseBuffer();

	return m_str.GetBuffer();
}

Cstring::operator CString()
{
	int nSize = this->size();
	m_str.Empty();
	CharToTchar(this->c_str(), m_str.GetBuffer(nSize));
	m_str.ReleaseBuffer();

	return m_str;
}

Cstring::operator CString&()
{
	int nSize = this->size();
	m_str.Empty();
	CharToTchar(this->c_str(), m_str.GetBuffer(nSize));
	m_str.ReleaseBuffer();

	return m_str;
}

void Cstring::clear()
{	
	string::clear();
	m_str.Empty();
}

//清除字符串所有的空格
Cstring& Cstring::trim_all()
{
	trim_all(' ');
	return *this;
}

//清除字符串所有指定的字符
Cstring& Cstring::trim_all(const char ch)
{
	int nSize = this->size();
	for (int i = 0; i < nSize; i++)
	{
		if (ch == this->at(i))
		{
			this->erase(i, 1);
			i--;
			nSize = this->size();
		}
	}
	return *this;
}

//清除字符串所有指定的所有字符
Cstring& Cstring::trim_all(const string str)
{
	return *this;
}


Cstring& Cstring::trim_left()
{
	trim_left(' ');
	return *this;
}

Cstring& Cstring::trim_left(const char ch)
{
	int nSize = this->size();
	for (int i = 0; i < nSize; i++)
	{
		if (ch == this->at(i))
		{
			this->erase(i, 1);
			i--;
		}
		else
		{
			break;
		}
	}
	return *this;
}

Cstring& Cstring::trim_left(const string str)
{
	int nSize = str.size();
	int j = 0;
	while (j < this->size())
	{
		int i = 0;
		for (; i < nSize; i++)
		{
			//包含了字符，则清除左边包含的字符
			if (this->at(j) == str.at(i))
			{
				trim_left(str.at(i));
				j = 0;

				break;
			}
		}

		//不包含字符，则退出
		if (i == nSize)
		{
			return *this;
		}
	}
	return *this;
}

Cstring& Cstring::trim_right()
{
	trim_right(' ');
	return *this;
}

Cstring& Cstring::trim_right(const char ch)
{
	int nSize = this->size();
	for (int i = nSize - 1; i >= 0; i--)
	{
		if (ch == this->at(i))
		{
			this->erase(i, 1);
		}
		else
		{
			break;
		}
	}
	return *this;
}

Cstring& Cstring::trim_right(const string str)
{
	int nSize = str.size();
	int j = this->size() - 1;
	while(j >= 0)
	{
		int i = 0;
		for (; i < nSize; i++)
		{
			//包含了字符，则清除右边包含的字符
			if (this->at(j) == str.at(i))
			{
				trim_right(str.at(i));
				j = this->size() - 1;
				
				break;
			}
		}

		//不包含字符，则退出
		if (i == nSize)
		{
			return *this;
		}
	}
	return *this;
}


