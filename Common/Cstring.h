#pragma once
#include <AFX.h>
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;

class Cstring : public string
{
public:
	Cstring();
	Cstring(const string& str);
	Cstring(const char* pstr);
	Cstring(const Cstring& str);
	Cstring(const TCHAR *pstr);
	Cstring(const CString& str);
	~Cstring(void);

	Cstring& operator =(const Cstring& str);
	Cstring& operator =(const TCHAR* pStr);
	Cstring& operator =(const CString& str);
	operator TCHAR*();
	operator CString();
	//赋值给一个CString类型的引用时，当值改变，引用变量的值不会随之改变，需重新赋值
	operator CString&();
	void clear();
	//清除字符串所有的空格
	Cstring& trim_all();
	//清除字符串所有指定的字符
	Cstring& trim_all(const char ch);
	//清除字符串所有指定的所有字符
	Cstring& trim_all(const string str);
	//清除字符串左边的空格，遇到第一个非空格的字符则结束
	Cstring& trim_left();
	//清除字符串左边指定的字符，遇到第一个非指定的字符则结束
	Cstring& trim_left(const char ch);
	//清除字符串左边指定的所有字符，遇到第一个非指定的所有字符则结束
	Cstring& trim_left(const string str);
	//清除字符串右边的空格，遇到第一个非空格的字符则结束
	Cstring& trim_right();
	//清除字符串右边指定的字符，遇到第一个非指定的字符则结束
	Cstring& trim_right(const char ch);
	//清除字符串右边指定的所有字符，遇到第一个非指定的所有字符则结束
	Cstring& trim_right(const string str);

	//将数值类型的数据转换成字符串
	template<class T>
	void to_string(const T& t, int nFixed = 2)
	{
		stringstream ss;
		ss << fixed << setprecision(nFixed) << t;
		this->clear();
		this->append(ss.str());
	}

	//将字符串转换成数值型数据
	template<class T>
	T to_value(int nFixed = 2)
	{
		stringstream ss;
		ss << this->c_str();
		T t;
		ss >> fixed >> setprecision(nFixed) >> t;
		return t;
	}

private:
	void TcharToChar(const TCHAR * tchar, char * _char);
	void CharToTchar(const char * _char, TCHAR * tchar);

	CString m_str;
};

