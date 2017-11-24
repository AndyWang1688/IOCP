#include "stdafx.h"
#include "XJListenSocket.h"


CXJListenSocket::CXJListenSocket(long nSocketPoolSize, HANDLE hCompletion) :m_nSocketPoolSize(nSocketPoolSize), m_hCompletion(hCompletion)
{
	m_nEffectiveSocketCount = 0;
	m_bRunning = false;
	m_hListenEvent = WSA_INVALID_EVENT;
}

CXJListenSocket::~CXJListenSocket()
{
	if (WSA_INVALID_EVENT != m_hListenEvent)
	{
		WSACloseEvent(m_hListenEvent);
		m_hListenEvent = WSA_INVALID_EVENT;
	}
}

CXJListenSocket* CXJListenSocket::CreateObject(long nSocketPoolSize, HANDLE hCompletion)
{
	static CXJListenSocket obj(nSocketPoolSize, hCompletion);
	return &obj;
}

bool CXJListenSocket::Startup()
{
	WSADATA wsd;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsd))
	{
		return false;
	}

	return m_WinsockExApi.LoadAllFun();
}

bool CXJListenSocket::Cleanup()
{
	closesocket(m_skContext.m_skSocket);

	CleanSocketPool();

	WSACleanup();
	return true;
}

bool CXJListenSocket::Listen()
{
	m_skContext.m_skSocket = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_skContext.m_skSocket)
	{
		return false;
	}

	SOCKADDR_IN saServer;
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = htonl(INADDR_ANY);
	saServer.sin_port = htons(5000);

	if (SOCKET_ERROR == bind(m_skContext.m_skSocket, (const sockaddr*)&saServer, sizeof(SOCKADDR_IN)))
	{
		return false;
	}

	if (SOCKET_ERROR == listen(m_skContext.m_skSocket, SOMAXCONN))
	{
		return false;
	}

	BindIOCP(m_skContext.m_skSocket, m_hCompletion);

	return true;
}

bool CXJListenSocket::CreateSocketPool()
{
	m_nEffectiveSocketCount = m_nSocketPoolSize;
	for (int i = 0; i < m_nSocketPoolSize; i++)
	{
		PostAccept();
	}

	//启动监控SOCKET池线程
	m_bRunning = true;
	if (WSA_INVALID_EVENT == (m_hListenEvent = WSACreateEvent()))
	{	
		int nErr = GetLastError();
		return false;
	}
	if (SOCKET_ERROR == WSAEventSelect(m_skContext.m_skSocket, m_hListenEvent, FD_ACCEPT))
	{
		int nErr = GetLastError();
		return false;
	}
	HANDLE hSocketPoolThread = (HANDLE)_beginthreadex(NULL, 0, SocketPoolMonitor, this, 0, NULL);
	CloseHandle(hSocketPoolThread);
	HANDLE hHeartPackThread = (HANDLE)_beginthreadex(NULL, 0, DetectHeartPackProc, this, 0, NULL);
	CloseHandle(hHeartPackThread);
	return true;
}

unsigned _stdcall CXJListenSocket::SocketPoolMonitor(void *pParam)
{
	CXJListenSocket *pThis = (CXJListenSocket*)pParam;
	while (true)
	{
		int nRet = WSAWaitForMultipleEvents(1, &(pThis->m_hListenEvent), FALSE, WSA_INFINITE, FALSE);
		if (0 == nRet)
		{
			if (pThis->m_bRunning)
			{
				WSANETWORKEVENTS event;
				::WSAEnumNetworkEvents(pThis->m_skContext.m_skSocket, pThis->m_hListenEvent, &event);
				if (event.lNetworkEvents & FD_ACCEPT)                // 处理FD_ACCEPT通知消息  
				{
					if (event.iErrorCode[FD_ACCEPT_BIT] == 0)
					{
						for (int i = 0; i < _PER_SOCKETPOOL_INCREASE_SIZE; i++)
						{
							pThis->PostAccept();
						}
					}
				}
			}			
			else
			{
				break;
			}
		}
	}

	return 0;
}

unsigned _stdcall CXJListenSocket::DetectHeartPackProc(void *pParam)
{
	CXJListenSocket *pThis = (CXJListenSocket*)pParam;
	while (pThis->m_bRunning)
	{
		CAtlMap<SOCKET, CXJClientSocket*>::CPair *pair = NULL;
		POSITION ps = pThis->m_ClientSocketMap.GetStartPosition();
		while (NULL != ps)
		{
			pair = pThis->m_ClientSocketMap.GetNext(ps);
			if (NULL != pair)
			{
				if (pair->m_value->IsEffective())
				{
					if (5 == pair->m_value->GetHeadPackTimes())
					{
						pair->m_value->PostDisconnect(NULL);
					}
					else
					{
						pair->m_value->IncreaseHeadPackTimes();
					}
				}
			}
		}
	}

	return 0;
}

bool CXJListenSocket::CleanSocketPool()
{
	m_bRunning = false;
	WSASetEvent(m_hListenEvent);

	CAtlMap<SOCKET, CXJClientSocket*>::CPair *pair = NULL;
	POSITION ps = m_ClientSocketMap.GetStartPosition();
	while (NULL != ps)
	{
		pair = m_ClientSocketMap.GetNext(ps);
		if (NULL != pair)
		{
			delete pair->m_value;
			pair->m_value = NULL;
		}
	}
	m_ClientSocketMap.RemoveAll();
	return true;
}

void CXJListenSocket::IncrementEffectiveSocketCount()
{
	m_nEffectiveSocketCount = InterlockedIncrement(&m_nEffectiveSocketCount);
}

void CXJListenSocket::DecrementEffectiveSocketCount()
{
	m_nEffectiveSocketCount = InterlockedDecrement(&m_nEffectiveSocketCount);
}

bool CXJListenSocket::PostAccept()
{
	SOCKET sSocket = INVALID_SOCKET;
	sSocket = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET != sSocket)
	{
		//重叠结构，投递连接成功后与客户端套接字绑定，在客户端套接字实例内释放资源
		PER_IO_CONTEXT *pIOContext = NULL;
		pIOContext = new PER_IO_CONTEXT();
		if (NULL != pIOContext)
		{
			CXJClientSocket *pClientSocket = NULL;
			pClientSocket = new CXJClientSocket(sSocket, m_skContext.m_skSocket);
			if (NULL != pClientSocket)
			{
				PER_SOCKET_CONTEXT *pskContext = pClientSocket->GetSocketContext();
				pIOContext->m_skSocket = sSocket;
				pIOContext->Reset();
				pIOContext->m_nNetworkEvent = FD_ACCEPT;
				if (FALSE == m_WinsockExApi.AcceptEx(m_skContext.m_skSocket, pIOContext->m_skSocket, pskContext->m_szAcceptBuff, 0,
					(sizeof(SOCKADDR_IN) + 16), (sizeof(SOCKADDR_IN) + 16), &(pIOContext->m_dwTransByte), &(pIOContext->m_OL)))
				{
					int iError = WSAGetLastError();
					if (ERROR_IO_PENDING != iError
						&& WSAECONNRESET != iError)
					{
						if (INVALID_SOCKET != pIOContext->m_skSocket)
						{
							::closesocket(pIOContext->m_skSocket);
							pIOContext->m_skSocket = INVALID_SOCKET;
						}
						delete pIOContext;
						pIOContext = NULL;
						delete pClientSocket;
						pClientSocket = NULL;
						return false;
					}

					pClientSocket->AddIOContext(pIOContext);
					m_ClientSocketMap[sSocket] = pClientSocket;
				}

				return true;
			}
		}
	}
	return true;
}

bool CXJListenSocket::OnAccept(PER_IO_CONTEXT *pIOContext)
{
	if (NULL == pIOContext)
		return false;

	CXJClientSocket* pClientSocket = NULL;
	m_ClientSocketMap.Lookup(pIOContext->m_skSocket, pClientSocket);
	if (NULL == pClientSocket)
	{
		return false;
	}

	PER_SOCKET_CONTEXT *pskContext = pClientSocket->GetSocketContext();
	int nLocalAddrLen = sizeof(SOCKADDR_IN);
	int nRemoteAddrLen = sizeof(SOCKADDR_IN);
	m_WinsockExApi.GetAcceptExSockaddrs(pskContext->m_szAcceptBuff, 0,	(sizeof(SOCKADDR_IN) + 16), (sizeof(SOCKADDR_IN) + 16),
		(sockaddr**)(&(pskContext->m_psaLocal)), &nLocalAddrLen, (sockaddr**)(&(pskContext->m_psaRemote)), &nRemoteAddrLen);
	
	pClientSocket->BindIOCP(pskContext->m_skSocket, m_hCompletion);

	int nBufLen = 0;
	//关闭套接字上的发送缓冲，这样可以提高性能
	if (0 != ::setsockopt(pskContext->m_skSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&nBufLen, sizeof(int)))
	{
		return false;
	}
	//这个类模型中不要关闭接收缓冲比较好
	/*nBufLen = 0;
	if (0 != ::setsockopt(pskContext->m_skSocket,SOL_SOCKET,SO_RCVBUF,(const char*)&nBufLen,sizeof(int)))
	{
		return false;
	}*/

	//强制发送延时算法关闭,直接发送到网络上去
	BOOL bNODELOY = TRUE;
	if(0 != ::setsockopt(pskContext->m_skSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&bNODELOY, sizeof(BOOL)))
	{
		return false;
	}

	//设置客户端套接字和监听套接字一样的属性
	if (0 != ::setsockopt(pskContext->m_skSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&(m_skContext.m_skSocket), sizeof(SOCKET)))
	{
		return false;
	}

	//连接成功后设置客户端套接字为有效的
	pClientSocket->SetEffective(true);
	return pClientSocket->PostRecv(pIOContext);
}
