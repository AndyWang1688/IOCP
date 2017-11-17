#pragma once
#include "../Common/IXJServer.h"
#include "XJCompletionPort.h"

class CXJServer :public IXJServer
{
public:
	CXJServer();
	~CXJServer();

	bool InitServer(int nThreadMinimum, int nThreadMaximum, int nNetBuffSize);
	void Run();
	bool Exit(bool bCancelPendingCallbacks);

	void AddRef();
	void Release();
	void QueryInterface(GUID GIID, void **pInterface);

private:
	CXJCompletionPort XJCompltion;
	int m_nRef;
};

