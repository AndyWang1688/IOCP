#pragma once
#include "XJplatform_callbck_push_protocol.h"
#include <string>
using namespace std;

#ifdef XJSERVICESLIB_EXPORTS
#define DLL_PORT __declspec(dllexport)
#else 
#define DLL_PORT __declspec(dllimport)
#endif

class DLL_PORT IPlatformApi
{
public:
	//继承时传入一个平台数据推送回调函数
	IPlatformApi(PFNPLATFORMDATAPROC pFn);
	virtual ~IPlatformApi() = 0;
	//初始化平台
	virtual void InitializePlatform() = 0;
	//登录一个交易账户
	virtual void Login(const string& userID, const string& password, bool b_t = false, bool b_n = false, bool bSimulate = false) = 0;
	//查询资金账号
	virtual void QueryTradeAccount(const string& userID) = 0;
	//登出一个交易账户
	virtual void Logout(const string& userID) = 0;
	//登出所有交易账户
	virtual void LogoutAll() = 0;
	//新建委托
	virtual void InsertOrder(const string& userID, const string& contract,
		char buy_or_sell, char open_or_cover, int order_num, double order_price = 0.0) = 0;
	//撤消委托
	virtual void CancelOrder(const string& userID, const string &strOrderNo, const string &ExchangeID) = 0;
	//查询持仓(汇总)
	virtual void QueryPostion(const string& userID, const string& contract = "") = 0;
	//查询持仓明细
	virtual void QueryPostionDetail(const string& userID, const string& contract = "") = 0;
	//查询成交信息,可指定起始日期，日期为空时默认为当日成交信息
	virtual void QueryTrade(const string& userID, const string& strStartDate = "", const string& strEndDate = "") = 0;
	//重启策略
	virtual void RestartStrategy(const string& userID, const string& strategyID) = 0;
	//关闭策略
	virtual void CloseStrategy(const string& userID, const string& strategyID) = 0;

protected:
	static PFNPLATFORMDATAPROC m_pPlatformDataProc;
};
