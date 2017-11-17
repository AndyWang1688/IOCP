#ifndef __XJTRADEPLATFORM_NETWORK_PROTOCOL_H__
#define __XJTRADEPLATFORM_NETWORK_PROTOCOL_H__
#include "../Json/json.h"
#include <string>
using namespace std;

struct XJMsgHead
{
   int nEvent = 0;
   int nPlatformType = 0;
   int nErr = 0;
   string strMsg;

   virtual void to_object(string strJsonData)
   {
       Json::Reader rd;
       Json::Value root;

       rd.parse(strJsonData, root);
       nEvent = root["nEvent"].asInt();
       nPlatformType = root["nPlatformType"].asInt();
       nErr = root["nErr"].asInt();
       strMsg = root["strMsg"].asString();
   }

   virtual string to_string()
   {
       Json::FastWriter fw;
       Json::Value root;

       root["nEvent"] = nEvent;
       root["nPlatformType"] = nPlatformType;
       root["nErr"] = nErr;
       root["strMsg"] = strMsg;
       return fw.write(root);
   }
};

struct XJReqUserLogin : public XJMsgHead
{
   string strAccountId;
   string strPassword;
   bool b_t = false;
   bool b_n = false;
   bool bSimulate = false;

   virtual void to_object(string strJsonData)
   {
       Json::Reader rd;
       Json::Value root;

       rd.parse(strJsonData, root);
       nEvent = root["nEvent"].asInt();
       nPlatformType = root["nPlatformType"].asInt();
       nErr = root["nErr"].asInt();
       strMsg = root["strMsg"].asString();
       strAccountId = root["strAccountId"].asString();
       strPassword = root["strPassword"].asString();
       b_t = root["b_t"].asBool();
       b_n = root["b_n"].asBool();
       bSimulate = root["bSimulate"].asBool();
   }

   virtual string to_string()
   {
       Json::FastWriter fw;
       Json::Value root;

       root["nEvent"] = nEvent;
       root["nPlatformType"] = nPlatformType;
       root["nErr"] = nErr;
       root["strMsg"] = strMsg;
       root["strAccountId"] = strAccountId;
       root["strPassword"] = strPassword;
       root["b_t"] = b_t;
       root["b_n"] = b_n;
       root["bSimulate"] = bSimulate;
       return fw.write(root);
   }
};

struct XJRspUserLogin : public XJMsgHead
{
   string strAccountId;
   double dLoginDateTime = 0.0;
   string strBrokerID;
   string strSystemName;

   virtual void to_object(string strJsonData)
   {
       Json::Reader rd;
       Json::Value root;

       rd.parse(strJsonData, root);
       nEvent = root["nEvent"].asInt();
       nPlatformType = root["nPlatformType"].asInt();
       nErr = root["nErr"].asInt();
       strMsg = root["strMsg"].asString();
       strAccountId = root["strAccountId"].asString();
       dLoginDateTime = root["dLoginDateTime"].asDouble();
       strBrokerID = root["strBrokerID"].asString();
       strSystemName = root["strSystemName"].asString();
   }

   virtual string to_string()
   {
       Json::FastWriter fw;
       Json::Value root;

       root["nEvent"] = nEvent;
       root["nPlatformType"] = nPlatformType;
       root["nErr"] = nErr;
       root["strMsg"] = strMsg;
       root["strAccountId"] = strAccountId;
       root["dLoginDateTime"] = dLoginDateTime;
       root["strBrokerID"] = strBrokerID;
       root["strSystemName"] = strSystemName;
       return fw.write(root);
   }
};

struct XJReqUserLogout : public XJMsgHead
{
   string strAccountId;

   virtual void to_object(string strJsonData)
   {
       Json::Reader rd;
       Json::Value root;

       rd.parse(strJsonData, root);
       nEvent = root["nEvent"].asInt();
       nPlatformType = root["nPlatformType"].asInt();
       nErr = root["nErr"].asInt();
       strMsg = root["strMsg"].asString();
       strAccountId = root["strAccountId"].asString();
   }

   virtual string to_string()
   {
       Json::FastWriter fw;
       Json::Value root;

       root["nEvent"] = nEvent;
       root["nPlatformType"] = nPlatformType;
       root["nErr"] = nErr;
       root["strMsg"] = strMsg;
       root["strAccountId"] = strAccountId;
       return fw.write(root);
   }
};

struct XJRspUserLogout : public XJMsgHead
{
   string strAccountId;
   string strLogoutReason;

   virtual void to_object(string strJsonData)
   {
       Json::Reader rd;
       Json::Value root;

       rd.parse(strJsonData, root);
       nEvent = root["nEvent"].asInt();
       nPlatformType = root["nPlatformType"].asInt();
       nErr = root["nErr"].asInt();
       strMsg = root["strMsg"].asString();
       strAccountId = root["strAccountId"].asString();
       strLogoutReason = root["strLogoutReason"].asString();
   }

   virtual string to_string()
   {
       Json::FastWriter fw;
       Json::Value root;

       root["nEvent"] = nEvent;
       root["nPlatformType"] = nPlatformType;
       root["nErr"] = nErr;
       root["strMsg"] = strMsg;
       root["strAccountId"] = strAccountId;
       root["strLogoutReason"] = strLogoutReason;
       return fw.write(root);
   }
};

struct XJReqAccountInfo : public XJMsgHead
{
   string strAccountId;

   virtual void to_object(string strJsonData)
   {
       Json::Reader rd;
       Json::Value root;

       rd.parse(strJsonData, root);
       nEvent = root["nEvent"].asInt();
       nPlatformType = root["nPlatformType"].asInt();
       nErr = root["nErr"].asInt();
       strMsg = root["strMsg"].asString();
       strAccountId = root["strAccountId"].asString();
   }

   virtual string to_string()
   {
       Json::FastWriter fw;
       Json::Value root;

       root["nEvent"] = nEvent;
       root["nPlatformType"] = nPlatformType;
       root["nErr"] = nErr;
       root["strMsg"] = strMsg;
       root["strAccountId"] = strAccountId;
       return fw.write(root);
   }
};

struct XJRspAccountInfo : public XJMsgHead
{
   string strAccountId;
   string strCurrency;
   double dAvailable = 0.0;
   double dCloseProfit = 0.0;
   double dPositionProfit = 0.0;
   double dCommission = 0.0;
   double dPositionMargin = 0.0;
   double dFrozenCash = 0.0;
   double dSpecProductMargin = 0.0;
   double dSpecProductFrozenMargin = 0.0;
   double dSpecProductCommission = 0.0;
   double dSpecProductFrozenCommission = 0.0;
   double dSpecProductPositionProfit = 0.0;
   double dSpecProductCloseProfit = 0.0;

   virtual void to_object(string strJsonData)
   {
       Json::Reader rd;
       Json::Value root;

       rd.parse(strJsonData, root);
       nEvent = root["nEvent"].asInt();
       nPlatformType = root["nPlatformType"].asInt();
       nErr = root["nErr"].asInt();
       strMsg = root["strMsg"].asString();
       strAccountId = root["strAccountId"].asString();
       strCurrency = root["strCurrency"].asString();
       dAvailable = root["dAvailable"].asDouble();
       dCloseProfit = root["dCloseProfit"].asDouble();
       dPositionProfit = root["dPositionProfit"].asDouble();
       dCommission = root["dCommission"].asDouble();
       dPositionMargin = root["dPositionMargin"].asDouble();
       dFrozenCash = root["dFrozenCash"].asDouble();
       dSpecProductMargin = root["dSpecProductMargin"].asDouble();
       dSpecProductFrozenMargin = root["dSpecProductFrozenMargin"].asDouble();
       dSpecProductCommission = root["dSpecProductCommission"].asDouble();
       dSpecProductFrozenCommission = root["dSpecProductFrozenCommission"].asDouble();
       dSpecProductPositionProfit = root["dSpecProductPositionProfit"].asDouble();
       dSpecProductCloseProfit = root["dSpecProductCloseProfit"].asDouble();
   }

   virtual string to_string()
   {
       Json::FastWriter fw;
       Json::Value root;

       root["nEvent"] = nEvent;
       root["nPlatformType"] = nPlatformType;
       root["nErr"] = nErr;
       root["strMsg"] = strMsg;
       root["strAccountId"] = strAccountId;
       root["strCurrency"] = strCurrency;
       root["dAvailable"] = dAvailable;
       root["dCloseProfit"] = dCloseProfit;
       root["dPositionProfit"] = dPositionProfit;
       root["dCommission"] = dCommission;
       root["dPositionMargin"] = dPositionMargin;
       root["dFrozenCash"] = dFrozenCash;
       root["dSpecProductMargin"] = dSpecProductMargin;
       root["dSpecProductFrozenMargin"] = dSpecProductFrozenMargin;
       root["dSpecProductCommission"] = dSpecProductCommission;
       root["dSpecProductFrozenCommission"] = dSpecProductFrozenCommission;
       root["dSpecProductPositionProfit"] = dSpecProductPositionProfit;
       root["dSpecProductCloseProfit"] = dSpecProductCloseProfit;
       return fw.write(root);
   }
};

#endif