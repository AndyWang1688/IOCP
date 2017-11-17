#pragma once
#include "XJCustomThreadPool.h"
#include "XJListenSocket.h"

class CXJCompletionPort
{
public:
	CXJCompletionPort();
	~CXJCompletionPort();

	bool Init(int nThreadMinimum, int nThreadMaximum, int nNetBuffSize);
	bool Run();
	bool Exit(bool bCancelPendingCallbacks);

private:
	static void __stdcall CompletionPortOperationDataCallback(HANDLE hInstance, void *pParam);

private:
	int m_nNumberOfConcurrentThreads;
	HANDLE m_hCompletionPort;
	CXJCustomThreadPool *m_pXJCustomThreadpool;
	//CXJListenSocket为单列模式，不需要手动释放
	CXJListenSocket *m_pXJListenSocket;

	int m_nThreadMinimum;
	int m_nThreadMaximum;
	int m_nNetBuffSize;
};

