#pragma once

typedef void (__stdcall *PFN_THREADPOOL_CALLBACK)(HANDLE , void *);

class CXJCustomThreadPool
{
public:
	CXJCustomThreadPool();
	~CXJCustomThreadPool();

	bool InitThreadPool(int nThreadMinimum, int nThreadMaximum);	
	static bool SubmitThreadPoolCallback(PFN_THREADPOOL_CALLBACK pfnThreadPoolCallback, void *pParam);
	bool CleanThreadPool(bool bCancelPendingCallbacks);

private:
	static void __stdcall ThreadPoolCleanupGroupCallback(void *pContext, void *pCleanupContext);

private:
	//�̳߳ض���
	PTP_POOL m_pThreadpool;
	//�̳߳ػ���
	static TP_CALLBACK_ENVIRON m_Environ;
	//�̳߳�������
	PTP_CLEANUP_GROUP m_pThreadpoolCleanupGroup;
};

