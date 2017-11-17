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
	//线程池对象
	PTP_POOL m_pThreadpool;
	//线程池环境
	static TP_CALLBACK_ENVIRON m_Environ;
	//线程池清理组
	PTP_CLEANUP_GROUP m_pThreadpoolCleanupGroup;
};

