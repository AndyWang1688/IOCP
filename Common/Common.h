#pragma once
#include "LeeDateTime.h"

//K线的单根bar数据结构体
struct BarData {
	CLeeDateTime time;//Bar的时间
	double open;//Bar的开盘价
	double high;//Bar的最高价
	double low;//Bar的最低价
	double close;//Bar的收盘价
	BarData(const CLeeDateTime& t = CLeeDateTime(), double o = 0.0, double h = 0.0,
		double l = 0.0, double c = 0.0)
		:time(t), open(o), high(h), low(l), close(c) { }//构造函数
};

//交易信号结构体
struct TradeSignal {
	string ContractID;//合约代码
	/*
	order_direction委托方向:
	"Buy" - 多开
	"Sellshort" - 空开
	"Sell" - 平多
	"Buytocover" - 平空
	*/
	string order_direction;
	int order_num;//委托数量
	double order_price;//委托价格
	TradeSignal(const string& contract = "", const string& direction = "",
		int num = 0, double price = 0.0)
		:ContractID(contract), order_direction(direction),
		order_num(num), order_price(price) { }//构造函数
};


//控制自动上锁与解锁的类(同一个线程内锁无效)
class CLock {
public:
	CLock(CRITICAL_SECTION* p) : p_cs(p) {
		EnterCriticalSection(p_cs);//上锁(进入临界池)
	}
	~CLock() {
		LeaveCriticalSection(p_cs);//解锁(离开临界池) 
	}
	CLock(const CLock&) = delete;//禁止拷贝构造
	CLock& operator= (const CLock&) = delete;//禁止赋值
private:
	CRITICAL_SECTION* p_cs;//临界池
};

//获取合约中的品种名.如参数为string:"CF801",则返回string:"CF"
inline const string GetCommodityFromContract(const string& contract) {
	size_t index = 0;
	for (; index != contract.size(); ++index) {
		if (isdigit(contract[index])){//index定位到第一个为数字(0~9)的字母索引
			break;
		}
	}
	return contract.substr(0, index);//品种名
}

//获取合约中的年份和月份.如参数为string:"CF801",则返回string:"801"
inline const string GetYearMonthFromContract(const string& contract) {
	size_t index = 0;
	for (; index != contract.size(); ++index) {
		if (isdigit(contract[index])) {//index定位到第一个为数字(0~9)的字母索引
			break;
		}
	}
	return contract.substr(index);
}
