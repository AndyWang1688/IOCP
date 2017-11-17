#include "LeeDateTime.h"

CLeeDateTime::CLeeDateTime(const struct tm& s) noexcept: CLeeDateTime(
	s.tm_year + 1900, s.tm_mon + 1, s.tm_mday, s.tm_hour, s.tm_min, s.tm_sec)
{
	//在1970/1/1之前的struct tm日期, 通过mktime(&s)转换为time_t时, 得到-1,
	//故不能使用mktime(&s)得到time_t再通过time_t构造
}

CLeeDateTime::CLeeDateTime(int year, int month, int day, 
	int hour /*= 0*/, int minute /*= 0*/, int second /*= 0*/) noexcept
	: CLeeDateTime(SYSTEMTIME{
	static_cast<WORD>(year), static_cast<WORD>(month), 0, static_cast<WORD>(day),
	static_cast<WORD>(hour), static_cast<WORD>(minute), static_cast<WORD>(second) , 0 })
{
}

CLeeDateTime::CLeeDateTime(const SYSTEMTIME& st) noexcept
{
	//st.wMilliseconds = 0;//毫秒设为0
	FILETIME fileTime1 = { 0 };//Contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC时间).
	SystemTimeToFileTime(&st, &fileTime1);
	long long ll = 0;
	memcpy(&ll, &fileTime1, 8);
	/*long long ll =
		((long long)fileTime1.dwHighDateTime << 32) | fileTime1.dwLowDateTime;*///比较慢
	m_dt = ll / (10.0 * 1000.0 * 1000.0 * 60.0 * 60.0 * 24.0) - 109205.0;//从1601/01/01到1899/12/30有109205天
}

CLeeDateTime::CLeeDateTime(const string& str) noexcept
{
	CLeeDateTime temp_datetime;
	temp_datetime.ParseDateTime(str);
	m_dt = temp_datetime.m_dt;
}

std::string CLeeDateTime::Format(const string& format /*= "%Y-%m-%d %H:%M:%S"*/) const
{
	long long ll = static_cast<long long>((m_dt + CLeeDateTime::HALF_SECOND + 109205.0)
		* 24.0 * 3600.0 * 1000.0 * 1000.0 * 10.0);//从1601/01/01到1899/12/30有109205天
	FILETIME fileTime1 = { 0 };
	memcpy(&fileTime1, &ll, 8);
	SYSTEMTIME sysTime1 = { 0 };
	FileTimeToSystemTime(&fileTime1, &sysTime1);

	struct tm newtime{ static_cast<int>(sysTime1.wSecond), 
		static_cast<int>(sysTime1.wMinute), 
		static_cast<int>(sysTime1.wHour),
		static_cast<int>(sysTime1.wDay), 
		static_cast<int>(sysTime1.wMonth - 1),
		static_cast<int>(sysTime1.wYear - 1900), 
		static_cast<int>(sysTime1.wDayOfWeek), 
		0,
		0 };//tm_yday和tm_isdst没有数据
	char tmpbuf[128] = { 0 };
	strftime(tmpbuf, 128, format.c_str(), &newtime);
	return string(tmpbuf);
}

void CLeeDateTime::SetDateTime(int year, int month, int day, int hour /*= 0*/, int minute /*= 0*/, int second /*= 0*/) noexcept
{
	*this = CLeeDateTime(year, month, day, hour, minute, second);
}

void CLeeDateTime::ParseDateTime(const string& str) noexcept
{
	istringstream iss(str);
	SYSTEMTIME sysTime1 = { 0 };
	char symbol = 0;//用于读取间隔符号的字符
	iss >> sysTime1.wYear >> symbol;//先读取年份和第一个分隔符
	if (symbol == ':') {//如果字符串中没有日期,只有时间
		sysTime1.wYear = 1899;
		sysTime1.wMonth = 12;
		sysTime1.wDay = 30;
		iss.clear();
		iss.str(str);
		iss >> sysTime1.wHour >> symbol
			>> sysTime1.wMinute >> symbol
			>> sysTime1.wSecond;
	}
	else {
		iss >> sysTime1.wMonth >> symbol
			>> sysTime1.wDay
			>> sysTime1.wHour >> symbol
			>> sysTime1.wMinute >> symbol
			>> sysTime1.wSecond;
	}
	*this = CLeeDateTime(sysTime1);
}

int CLeeDateTime::GetYear() const
{
	return Get_SYSTEMTIME().wYear;
}

int CLeeDateTime::GetMonth() const
{
	return Get_SYSTEMTIME().wMonth;
}

int CLeeDateTime::GetDay() const
{
	return Get_SYSTEMTIME().wDay;
}

int CLeeDateTime::GetHour() const
{
	return Get_SYSTEMTIME().wHour;
}

int CLeeDateTime::GetMinute() const
{
	return Get_SYSTEMTIME().wMinute;
}

int CLeeDateTime::GetSecond() const
{
	return Get_SYSTEMTIME().wSecond;
}

int CLeeDateTime::GetDayOfWeek() const
{
	return Get_SYSTEMTIME().wDayOfWeek;
}

SYSTEMTIME CLeeDateTime::Get_SYSTEMTIME() const
{
	long long ll = static_cast<long long>((m_dt + 109205.0)
		* 24.0 * 60.0 * 60.0 * 1000.0 * 1000.0 * 10.0);//从1601/01/01到1899/12/30有109205天
	FILETIME fileTime1 = { 0 };
	memcpy(&fileTime1, &ll, 8);
	//fileTime1.dwHighDateTime = static_cast<DWORD>(ll >> 32);
	//fileTime1.dwLowDateTime = static_cast<DWORD>(ll | 0x00000000FFFFFFFF);//不准确
	SYSTEMTIME sysTime1 = { 0 };
	FileTimeToSystemTime(&fileTime1, &sysTime1);

	return sysTime1;
}

struct tm CLeeDateTime::Get_tm() const
{
	SYSTEMTIME sysTime1 = Get_SYSTEMTIME();

	struct tm newtime { static_cast<int>(sysTime1.wSecond),
		static_cast<int>(sysTime1.wMinute),
		static_cast<int>(sysTime1.wHour),
		static_cast<int>(sysTime1.wDay),
		static_cast<int>(sysTime1.wMonth - 1),
		static_cast<int>(sysTime1.wYear - 1900),
		static_cast<int>(sysTime1.wDayOfWeek),
		0,
		0 };//tm_yday和tm_isdst没有数据
	return newtime;
}

time_t CLeeDateTime::Get_time_t() const
{
	return double_to_time_t(m_dt);
}

CLeeDateTime CLeeDateTime::operator+(const CLeeDateTimeSpan& span) const noexcept
{
	return CLeeDateTime(m_dt + span.m_span);
}

CLeeDateTime& CLeeDateTime::operator+=(const CLeeDateTimeSpan& span) noexcept
{
	m_dt += span.m_span;
	return *this;
}

CLeeDateTime CLeeDateTime::operator-(const CLeeDateTimeSpan& span) const noexcept
{
	return CLeeDateTime(m_dt - span.m_span);
}

CLeeDateTime& CLeeDateTime::operator-=(const CLeeDateTimeSpan& span) noexcept
{
	m_dt -= span.m_span;
	return *this;
}

CLeeDateTimeSpan CLeeDateTime::operator-(const CLeeDateTime& rhs) const noexcept
{
	return CLeeDateTimeSpan(m_dt - rhs.m_dt);
}

CLeeDateTime CLeeDateTime::GetCurrentTime(){
	SYSTEMTIME sysTime1 = { 0 };
	GetLocalTime(&sysTime1);
	return CLeeDateTime(sysTime1);
}

int CLeeDateTime::GetTimeZone() {
	static time_t tt0 = 0;
	static struct tm tm_test = { 0 };
	static bool bFirst = true;
	if (bFirst) {
		localtime_s(&tm_test, &tt0);
		bFirst = false;
	}
	return tm_test.tm_hour;
}

bool CLeeDateTime::IsLeapYear(int year)
{
	return (year % 4 == 0 && ((year % 400 == 0) || (year % 100 != 0)));
}

int CLeeDateTime::GetDays(int year, int month)
{
	int nDays = 0;
	switch (month) 
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			nDays = 31;
			break;
		case 2:
			nDays = (1 == IsLeapYear(year)) ? 29 : 28;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			nDays = 30;
			break;
		default:
			break;
	}

	return nDays;
}

bool CLeeDateTime::IsLeapYear() const
{
	int year = GetYear();
	return CLeeDateTime::IsLeapYear(year);
}

CLeeDateTime& CLeeDateTime::operator=(const CLeeDateTime& rhs)
{
	if (this == &rhs) {
		return *this;
	}
	m_dt = rhs.m_dt;
	return *this;
}

bool CLeeDateTime::operator>(const CLeeDateTime& rhs) const
{
	return m_dt - rhs.m_dt >= CLeeDateTime::HALF_SECOND;
}

bool CLeeDateTime::operator<(const CLeeDateTime& rhs) const
{
	return rhs.m_dt - m_dt >= CLeeDateTime::HALF_SECOND;
}

bool CLeeDateTime::operator>=(const CLeeDateTime& rhs) const
{
	return !(*this < rhs);
}

bool CLeeDateTime::operator<=(const CLeeDateTime& rhs) const
{
	return !(*this > rhs);
}

bool CLeeDateTime::operator==(const CLeeDateTime& rhs) const
{
	
	return m_dt - rhs.m_dt < CLeeDateTime::HALF_SECOND &&
		rhs.m_dt - m_dt < CLeeDateTime::HALF_SECOND;
}

bool CLeeDateTime::operator!=(const CLeeDateTime& rhs) const
{
	return !(*this == rhs);
}

double CLeeDateTime::time_t_to_double(time_t t) {
	return t / (24.0 * 60.0 * 60.0) + 25569.0 + GetTimeZone() / 24.0;//1899/12/30到1970/1/1之间有25569天
}

time_t CLeeDateTime::double_to_time_t(double d) {
	return static_cast<time_t>((d - 25569) * (24.0 * 60.0 * 60.0)
		- GetTimeZone() * (60.0 * 60.0) + 0.5);//+0.5是为了四舍五入,例如4.2秒+0.5,为4.7秒,转换为整数仍为4秒, 5.9秒+0.5,为6.4秒, 转换为整数仍为6秒.
}
