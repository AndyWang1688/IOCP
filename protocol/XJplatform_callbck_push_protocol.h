#pragma once


#include <string>
using namespace std;

//推送平台数据响应代码
enum XJ_PUSH_PLATFORMDATA_RSP
{
	RSP_NULL = 0,
	//推送提示信息,对应 XJRspShowTipMessage 结构体
	RSP_SHOW_TIPMESSAGE,
	//推送合约信息,对应 XJRspContractInfo 结构体
	RSP_QRY_CONTRACTINFO,
	//账号登入,对应 XJRspLoginInfo 结构体
	RSP_USER_RLOGIN,
	//账号登出,对应 XJRspLogoutInfo 结构体
	RSP_USER_LOGOUT,
	//推送资金账号,对应 XJRspFundAccountInfo 结构体
	RSP_QRY_FUNDACCOUNT,
	//推送资金账号策略,对应 XJRspAccountStrategyInfo 结构体
	RSP_FUNDACCOUNT_STRATEGY,
	//资金账号持仓,对应 XJRspAccountPosition 结构体
	RSP_FUNDACCOUNT_POSITION,
	//委托变更,对应 XJRspOrderInfo 结构体
	RSP_CHANGE_ORDER,
	//推送成交信息,对应 XJRspTradeInfo 结构体
	RSP_QRY_TRADEINFO
};

//产品类型
enum PRODUCT_TYPE
{
	//期货
	PDT_FUTURES = '1',
	//期权
	PDT_OPTION = '2',
	//组合
	PDT_COMBINATION = '3',
};

//买卖方向类型
enum SIDE_TYPE
{
	//买
	SIZE_BUY = '0',
	//卖
	SIZD_SELL = '1',
};

//开平方向
enum POSITION_EFFECT_TYPE
{
	//开仓
	PF_OPEN = '0',
	//平仓
	PF_CLOSE = '1',
	//平今
	PF_CLOSE_TODAY = '3',
	//平昨
	PF_CLOSE_YESTERDAY = '4',
};

//持仓方向
enum POSITION_DIRECTOIN_TYPE
{
	//净
	PD_NET = '1',
	//多头
	PD_LONG = '2',
	//空头
	PD_SHORT = '3'
};

//委托状态
enum ORDER_STATE_TYPE
{
	//完全成交
	OS_FINISHED = '0',
	//撤单
	OS_CANCELED = '1',
	//排队
	OS_QUEUED = '2',
	//已挂起
	OS_SUPPENDED = '3',
	//部分成交
	OS_PARTFINISHED = '4',
};

//平台类型
enum PLATFORM_TYPE
{
	PF_NULL = 0,
	//CTP
	PF_CTP,
	//易盛
	PF_Esunny,
	//天富宝
	PF_TFB
};

//消息头
struct XJRspMspHead
{
	virtual ~XJRspMspHead() {}
	//平台类型
	PLATFORM_TYPE pfType;
	//错误码
	int nErr;
	//提示信息
	string strMsg;
};

//提示信息
struct XJRspShowTipMessage : XJRspMspHead
{
	//消息头，提示信息在消息头szMsg中
};

//合约信息
struct XJRspContractInfo : XJRspMspHead
{
	//合约代码
	string strContractNo;
	//合约名称
	string strContractName;
	//合约类型
	char chContractType;
	//交易所编号
	string strExchangeNo;
	//产品代码
	string strProductID;
	//产品类型
	char chProductType;
	//上市日
	string strOpenDate;
	//到期日
	string strExpireDate;
	//执行价
	double dStrikePrice;
};

//用户登入返回信息
struct XJRspLoginInfo : XJRspMspHead
{
	//用户名
	string strUserName;
	//当前交易日期 
	string strTradingDate;
	//登入时间
	string strLoginTime;
	//经纪公司代码
	string strBrokerID;
	//交易平台名称
	string strSystemName;
};

//用户登出返回信息
struct XJRspLogoutInfo : XJRspMspHead
{
	//用户名
	string strUserName;
	//登出原因
	string strLogoutReason;
};

//资金账号返回信息
struct XJRspFundAccountInfo : XJRspMspHead
{
	//资金账号
	string strAccountId;
	//币种
	string strCurrency;
	//可用资金
	double dAvailable;
	//昨日结存	
	double PreBalance;
	//平仓盈亏
	double CloseProfit;
	//持仓盈亏
	double PositionProfit;
	//手续费
	double Commission;
	//持仓保证金
	double PositionMargin;
	//冻结资金
	double FrozenCash;
	//特殊产品占用保证金
	double SpecProductMargin;
	//特殊产品冻结保证金
	double SpecProductFrozenMargin;
	//特殊产品手续费
	double SpecProductCommission;
	//特殊产品冻结手续费
	double SpecProductFrozenCommission;
	//特殊产品持仓盈亏
	double SpecProductPositionProfit;
	//特殊产品平仓盈亏
	double SpecProductCloseProfit;
};

//策略参数
struct XJStrategyParam
{
	//商品名称
	string strCommodityName;
	//MABrate
	int nMABrate;
	//MABLen
	int nMABLen;
	//initialcp
	int nInitialCP;
	//finalper
	float fFinalPer;
	//perm
	float fPerm;
	//MaxRangeRate
	float fMaxRangeRate;
	//PrevBars
	int nPrevBars;
	//monthhour
	int nMonthHour;
	//ContractUnit
	int nContractUnit;
	//MarginRatio
	float fMarginRatio;
	//minp
	float fMinp;
};

//资金账号策略信息
struct XJRspAccountStrategyInfo : XJRspMspHead
{
	//资金账号
	string strAccountId;
	//策略名
	string strStrategyName;
	//商品名称
	string strCommodityName;
	//理论持仓
	int nTheoreticalPosition;
	//实际持仓
	int nPracticalPosition;
	//最近一次调用时间
	string strLastInvokeTime;
	//策略参数
	//XJStrategyParam StrategyParam;
};

//资金账号持仓
struct XJRspAccountPosition : XJRspMspHead
{
	//资金账号
	string strAccountId;
	//商品编号
	string strCommodityId;
	//商品名称
	string strCommodityName;
	//商品类型
	char chCommodityType;
	//合约
	string strContractNo;
	//买卖
	char chOrderSide;
	//总持仓
	int nPositions;
	//开仓均价
	double dOpenAveragePrice;
	//平仓盈亏
	double dCloseProfit;
	//浮动盈亏
	double dFloatProfit;
	//保证金
	double dMargin;
};

//委托变更
struct XJRspOrderInfo : XJRspMspHead
{
	//资金账号
	string strAccountId;
	//交易所编号
	string strExchangeId;
	//委托编号
	string strOrderNo;
	//委托时间
	string strInsertDataTime;
	//商品编号
	string strCommodityId;
	//商品名称
	string strCommodityName;
	//商品类型
	char chCommodityType;
	//合约
	string strContrctNo;
	//买卖
	char chOrderSide;
	//平开标志
	char chOffset;
	//委托价格
	double dOrderPrice;
	//委托数量
	int nOrderVolume;
	//已成交量
	int nnMatchVolume;
	//委托状态
	char chOrderstate;
};

//成交信息
struct XJRspTradeInfo : XJRspMspHead
{
	//资金账号
	string strAccountId;
	//委托编号
	string strOrderNo;
	//成交时间
	string strTradeDataTime;
	//合约
	string strContrctNo;
	//买卖
	char chOrderSide;
	//平开标志
	char chOffset;
	//成交价格
	double dMatchPrice;
	//成交数量
	int nMatchVolume;
	//手续费
	double dFeeValue;
};

typedef int(*PFNPLATFORMDATAPROC)(int, XJRspMspHead*);
