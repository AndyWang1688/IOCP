#pragma once
#include "IXJUnknown.h"

// {93CA4F87-9590-43AA-BA14-00383E2A032E}
static const GUID MID_XJNetworkServer =
{ 0x93ca4f87, 0x9590, 0x43aa,{ 0xba, 0x14, 0x0, 0x38, 0x3e, 0x2a, 0x3, 0x2e } };

// {EAAEB736-2CFF-4F65-8C7C-DBFDE1EFE9FE}
static const GUID IID_IXJServer =
{ 0xeaaeb736, 0x2cff, 0x4f65,{ 0x8c, 0x7c, 0xdb, 0xfd, 0xe1, 0xef, 0xe9, 0xfe } };



class DLL_PORT IXJServer :public IXJUnknown
{
public:
	virtual bool InitServer(int nThreadMinimum, int nThreadMaximum, int nNetBuffSize) = 0;
	virtual void Run() = 0;
	virtual bool Exit(bool bCancelPendingCallbacks) = 0;
};
