#include "stdafx.h"
#include "XJCompletionPort.h"


CXJCompletionPort::CXJCompletionPort()
{
	m_hCompletionPort = INVALID_HANDLE_VALUE;
	m_pXJCustomThreadpool = NULL;

	m_nThreadMinimum = 0;
	m_nThreadMaximum = 0;
	m_nNetBuffSize = 0;
}

CXJCompletionPort::~CXJCompletionPort()
{
	if (NULL != m_pXJCustomThreadpool)
	{
		delete m_pXJCustomThreadpool;
	}
}

bool CXJCompletionPort::Init(int nThreadMinimum, int nThreadMaximum, int nNetBuffSize)
{
	m_nThreadMinimum = nThreadMinimum;
	m_nThreadMaximum = nThreadMaximum;
	m_nNetBuffSize = nNetBuffSize;

	return true;
}

bool CXJCompletionPort::Run()
{
	//创建一个线程池对象
	m_pXJCustomThreadpool = new CXJCustomThreadPool();
	if (NULL != m_pXJCustomThreadpool)
	{
		//初始化线程池
		if (false == m_pXJCustomThreadpool->InitThreadPool(m_nThreadMinimum, m_nThreadMaximum))
		{
			delete m_pXJCustomThreadpool;
			m_pXJCustomThreadpool = NULL;
			return false;
		}
	}

	//创建一个完成端口
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (NULL == m_hCompletionPort)
	{
		delete m_pXJCustomThreadpool;
		m_pXJCustomThreadpool = NULL;
		return false;
	}

	//提交完成端口工作线程
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	m_nNumberOfConcurrentThreads = systemInfo.dwNumberOfProcessors * 2;
	for (int i = 0; i < m_nNumberOfConcurrentThreads; i++)
	{
		m_pXJCustomThreadpool->SubmitThreadPoolCallback(CompletionPortOperationDataCallback, this);
	}

	//创建监听套接字
	m_pXJListenSocket = CXJListenSocket::CreateObject(1000, m_hCompletionPort);
	if (m_pXJListenSocket->Startup())
	{
		m_pXJListenSocket->Listen();
		m_pXJListenSocket->CreateSocketPool();
	}

	return true;
}

bool CXJCompletionPort::Exit(bool bCancelPendingCallbacks)
{	
	for (int i = 0; i < m_nNumberOfConcurrentThreads; i++)
	{
		PostQueuedCompletionStatus(m_hCompletionPort, -1, NULL, NULL);
	}
	CloseHandle(m_hCompletionPort);
	m_pXJListenSocket->Cleanup();
	return m_pXJCustomThreadpool->CleanThreadPool(bCancelPendingCallbacks);
}

void __stdcall CXJCompletionPort::CompletionPortOperationDataCallback(HANDLE hInstance, void *pParam)
{
	CXJCompletionPort *pThis = (CXJCompletionPort *)pParam;
	DWORD dwTransferredByte = 0;
	BOOL bRet = FALSE;
	void* pCompletionKey = NULL;

	while (true)
	{
		dwTransferredByte = 0;
		bRet = FALSE;
		pCompletionKey = NULL;
		PER_IO_CONTEXT *pIOContext;
		bRet = ::GetQueuedCompletionStatus(pThis->m_hCompletionPort, &dwTransferredByte, 
			(PULONG_PTR)&pCompletionKey, (LPOVERLAPPED*)(&pIOContext), INFINITE);
	
		if (!bRet)
		{
			int nErr = WSAGetLastError();
			if (ERROR_INVALID_HANDLE == nErr)
			{
				break;
			}
			else if (ERROR_NETNAME_DELETED == nErr)
			{
				//客户端直接退出(没有调用closesocket()函数)
				//closesocket(pXJOverlapped->m_sSocket);
			}
			continue;
		}

		//if (bRet && (0 == dwTransferredByte) && ((IO_OPER_TYPE::IO_RECV == pXJOverlapped->m_ioOperType) || (IO_OPER_TYPE::IO_SEND == pXJOverlapped->m_ioOperType)))
		//{
		//	//客户端调用closesocket()关闭连接
		//	closesocket(pXJOverlapped->m_sSocket);
		//	continue;
		//}

		if ((-1 == dwTransferredByte) && (NULL == pCompletionKey))
		{
			//服务器关闭
			break;
		}
		
		//if (IO_OPER_TYPE::IO_ACCEPT == pXJOverlapped->m_ioOperType)
		//{//连接请求
		//	((CXJAccept*)pCompletionKey)->DoAccept();
		//}
		//else if (IO_OPER_TYPE::IO_RECV == pXJOverlapped->m_ioOperType)
		//{//接收请求
		//	((CXJClient*)pCompletionKey)->DoRecv();
		//}
		//else if (IO_OPER_TYPE::IO_SEND == pXJOverlapped->m_ioOperType)
		//{//发送请求

		//}
	}
	
}
