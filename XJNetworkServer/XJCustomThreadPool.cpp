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
	//�����̳߳�
	m_pThreadpool = ::CreateThreadpool(NULL);
	if (NULL == m_pThreadpool)
	{
		return false;
	}

	//�����̳߳������߳���������߳���
	::SetThreadpoolThreadMinimum(m_pThreadpool, nThreadMinimum);
	::SetThreadpoolThreadMaximum(m_pThreadpool, nThreadMaximum);

	//��ʼ���̳߳ػ�������
	::InitializeThreadpoolEnvironment(&m_Environ);
	//���̳߳ػ�������
	::SetThreadpoolCallbackPool(&m_Environ, m_pThreadpool);
	//�����̳߳ظ���ش����߳�
	::SetThreadpoolCallbackRunsLong(&m_Environ);

	//�����̳߳�������
	m_pThreadpoolCleanupGroup = ::CreateThreadpoolCleanupGroup();
	if (NULL == m_pThreadpoolCleanupGroup)
	{
		return false;
	}
	//���̳߳�������
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
	//���������ύ����δ����Ĺ�����ֱ��ȡ��,�����������е�ǰ�������еĹ��������֮�󷵻أ�bCancelPendingCallbacksΪtrueʱ������������ص�
	::CloseThreadpoolCleanupGroupMembers(m_pThreadpoolCleanupGroup, bCancelPendingCallbacks, NULL);
	//�����̳߳ػ�������
	::DestroyThreadpoolEnvironment(&m_Environ);
	//�ر��̳߳�
	::CloseThreadpool(m_pThreadpool);
	return true;
}

void __stdcall CXJCustomThreadPool::ThreadPoolCleanupGroupCallback(void *pContext, void *pCleanupContext)
{

}
