#pragma once
#include <ctime>
#include <sstream>
#include <windows.h>
using namespace std;

class CLeeDateTime;
class CLeeDateTimeSpan {
	friend class CLeeDateTime;
public:
	CLeeDateTimeSpan()noexcept : m_span(0.0){ }
	CLeeDateTimeSpan(int days, int hours, int minutes, int seconds) noexcept
		: m_span(days + hours / 24.0 + minutes / 60.0 / 24.0 + seconds / 3600.0 / 24.0){ }
	CLeeDateTimeSpan(double span) noexcept: m_span(span){ }
	CLeeDateTimeSpan& operator=(const CLeeDateTimeSpan&rhs) noexcept {
		if (this == &rhs) {
			return *this;
		}
		m_span = rhs.m_span;
		return *this;
	}
	operator double() const noexcept { return m_span; }//类型转换函数,转换为double
	void SetDateTimeSpan(int days, int hours, int minutes, int seconds) noexcept {
		m_span = days + hours / 24.0 + minutes / 60.0 / 24.0 + seconds / 3600.0 / 24.0;
	}
	bool operator==(const CLeeDateTimeSpan& rhs) const {
		return m_span - rhs.m_span < HALF_SECOND &&
			rhs.m_span - m_span < HALF_SECOND;
	}
	bool operator!=(const CLeeDateTimeSpan& rhs) const { return !(*this == rhs); }
	bool operator>(const CLeeDateTimeSpan& rhs) const {
		return m_span - rhs.m_span >= HALF_SECOND;
	}
	bool operator<(const CLeeDateTimeSpan& rhs) const {
		return  rhs.m_span - m_span >= HALF_SECOND;
	}
	bool operator>=(const CLeeDateTimeSpan& rhs) const {
		return  !(*this < rhs);
	}
	bool operator<=(const CLeeDateTimeSpan& rhs) const {
		return  !(*this > rhs);
	}
	CLeeDateTimeSpan operator+(const CLeeDateTimeSpan& rhs) const {
		return CLeeDateTimeSpan(m_span + rhs.m_span);
	}
	CLeeDateTimeSpan operator-(const CLeeDateTimeSpan& rhs) const {
		return CLeeDateTimeSpan(m_span - rhs.m_span);
	}
	CLeeDateTimeSpan& operator+=(const CLeeDateTimeSpan& rhs) {
		m_span += rhs.m_span;
		return *this;
	}
	CLeeDateTimeSpan& operator-=(const CLeeDateTimeSpan& rhs) {
		m_span -= rhs.m_span;
		return *this;
	}
	double GetTotalDays() const noexcept { return m_span; }//得到总天数
	double GetTotalHours() const noexcept { return m_span * 24.0; }//得到总小时数
	double GetTotalMinutes() const noexcept { return m_span * (24.0 * 60.0); }//得到总分钟数
	double GetTotalSeconds() const noexcept { return m_span * (24.0 * 60.0 * 60.0); }//得到总秒数
private:
	double m_span;
	constexpr static double HALF_SECOND = 0.5 / (24.0 * 60.0 * 60.0);//半秒
};

//日期时间类,能表示1601/01/01以来的日期
class CLeeDateTime {
public:
	CLeeDateTime() : m_dt(0.0) { }
	CLeeDateTime(double d) : m_dt(d) { }
	CLeeDateTime(time_t t) : m_dt(time_t_to_double(t)) { }
	CLeeDateTime(const struct tm& s) noexcept;
	CLeeDateTime(const SYSTEMTIME& st) noexcept;//会将毫秒数也引入
	CLeeDateTime(int year, int month, int day, 
		int hour = 0, int minute = 0, int second = 0) noexcept;
	CLeeDateTime(const string& str) noexcept;
	CLeeDateTime(const CLeeDateTime& c) : m_dt(c.m_dt) { }//拷贝构造函数
	string Format(const string& format = "%Y-%m-%d %H:%M:%S") const;//格式化输出时间,格式请查阅strftime函数的格式
	void SetDateTime(int year, int month, int day,
		int hour = 0, int minute = 0, int second = 0) noexcept;//设置日期时间
	void ParseDateTime(const string& str) noexcept;//从字符串中解析日期时间
	int GetYear() const;
	int GetMonth() const;
	int GetDay() const;
	int GetHour() const;
	int GetMinute() const;
	int GetSecond() const;
	int GetDayOfWeek() const;//一周的第几天. 周日为0, 周六为6.
	SYSTEMTIME Get_SYSTEMTIME() const;
	struct tm Get_tm() const;
	time_t Get_time_t() const;
	bool IsLeapYear() const;//是否是闰年
	operator double() const { return m_dt; }//类型转换函数,转换为double
	CLeeDateTime& operator=(const CLeeDateTime& rhs);//赋值函数
	bool operator==(const CLeeDateTime& rhs) const;
	bool operator!=(const CLeeDateTime& rhs) const;
	bool operator>(const CLeeDateTime& rhs) const;
	bool operator<(const CLeeDateTime& rhs) const;
	bool operator>=(const CLeeDateTime& rhs) const;
	bool operator<=(const CLeeDateTime& rhs) const;

	CLeeDateTime operator+(const CLeeDateTimeSpan& span) const noexcept;
	CLeeDateTime& operator+=(const CLeeDateTimeSpan& span) noexcept;
	CLeeDateTime operator-(const CLeeDateTimeSpan& span) const noexcept;
	CLeeDateTime& operator-=(const CLeeDateTimeSpan& span) noexcept;

	CLeeDateTimeSpan operator-(const CLeeDateTime& rhs) const noexcept;//两个日期时间类相减

	static CLeeDateTime GetCurrentTime();//得到当前时间
	static bool IsLeapYear(int year);//某年是否是闰年
	static int GetDays(int year, int month);//得到某年某月的天数
	static int GetTimeZone();//得到当地时区						 
	static double time_t_to_double(time_t t);//将time_t类型时间转化为double类型
	static time_t double_to_time_t(double d);//将double类型时间转化为time_t类型
private:
	double m_dt;
	constexpr static double HALF_SECOND = 0.5 / 3600.0 / 24.0;//半秒
};
