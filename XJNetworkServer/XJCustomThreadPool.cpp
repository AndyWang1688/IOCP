#include "stdafx.h"
#include "XJCustomThreadPool.h"

TP_CALLBACK_ENVIRON CXJCustomThreadPool::m_Environ;

CXJCustomThreadPool::CXJCustomThreadPool()
{
	m_pThreadpool = NULL;
	m_pThreadpoolCleanupGroup = NULL;
}


CXJCustomThreadPool::~CXJCustomThreadPool()
{
}

bool CXJCustomThreadPool::InitThreadPool(int nThreadMinimum, int nThreadMaximum)
{
	//创建线程池
	m_pThreadpool = ::CreateThreadpool(NULL);
	if (NULL == m_pThreadpool)
	{
		return false;
	}

	//设置线程池最少线程数和最大线程数
	::SetThreadpoolThreadMinimum(m_pThreadpool, nThreadMinimum);
	::SetThreadpoolThreadMaximum(m_pThreadpool, nThreadMaximum);

	//初始化线程池环境变量
	::InitializeThreadpoolEnvironment(&m_Environ);
	//绑定线程池环境变量
	::SetThreadpoolCallbackPool(&m_Environ, m_pThreadpool);
	//设置线程池更快地创建线程
	::SetThreadpoolCallbackRunsLong(&m_Environ);

	//创建线程池清理组
	m_pThreadpoolCleanupGroup = ::CreateThreadpoolCleanupGroup();
	if (NULL == m_pThreadpoolCleanupGroup)
	{
		return false;
	}
	//绑定线程池清理组
	::SetThreadpoolCallbackCleanupGroup(&m_Environ, m_pThreadpoolCleanupGroup, ThreadPoolCleanupGroupCallback);
	return true;
}

bool CXJCustomThreadPool::SubmitThreadPoolCallback(PFN_THREADPOOL_CALLBACK pfnThreadPoolCallback, void *pParam)
{
	BOOL bRet = ::TrySubmitThreadpoolCallback((PTP_SIMPLE_CALLBACK)pfnThreadPoolCallback, pParam, &m_Environ);
	return bRet;
}

bool CXJCustomThreadPool::CleanThreadPool(bool bCancelPendingCallbacks)
{
	//将所有已提交但尚未处理的工作项直接取消,函数会在所有当前正在运行的工作项完成之后返回，bCancelPendingCallbacks为true时则会调用清理组回调
	::CloseThreadpoolCleanupGroupMembers(m_pThreadpoolCleanupGroup, bCancelPendingCallbacks, NULL);
	//清理线程池环境变量
	::DestroyThreadpoolEnvironment(&m_Environ);
	//关闭线程池
	::CloseThreadpool(m_pThreadpool);
	return true;
}

void __stdcall CXJCustomThreadPool::ThreadPoolCleanupGroupCallback(void *pContext, void *pCleanupContext)
{

}
