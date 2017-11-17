#include "stdafx.h"
#include "XJListenSocket.h"


CXJListenSocket::CXJListenSocket(long nSocketPoolSize, HANDLE hCompletion) :m_nSocketPoolSize(nSocketPoolSize), m_hCompletion(hCompletion)
{
	m_nEffectiveSocketCount = 0;
	bRunning = false;
}

CXJListenSocket::~CXJListenSocket()
{
	
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
		SOCKET sSocket = INVALID_SOCKET;
		sSocket = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET != sSocket)
		{
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
					BindIOCP(sSocket, m_hCompletion);
					if (PostAccept(pskContext, pIOContext))
					{
						pClientSocket->AddIOContext(pIOContext);
						m_ClientSocketMap[sSocket] = pClientSocket;
					}
					else
					{
						delete pIOContext;
						pIOContext = NULL;
						delete pClientSocket;
						pClientSocket = NULL;
					}
				}				
			}
		}
	}

	//启动监控SOCKET池线程
	bRunning = true;
	HANDLE hSocketPoolThread = (HANDLE)_beginthreadex(NULL, 0, SocketPoolMonitor, this, 0, NULL);
	return true;
}

unsigned _stdcall CXJListenSocket::SocketPoolMonitor(void *pParam)
{
	CXJListenSocket *pThis = (CXJListenSocket*)pParam;
	while (pThis->bRunning)
	{
		if (0 == pThis->m_nEffectiveSocketCount)
		{

		}
	}

	return 0;
}

bool CXJListenSocket::CleanSocketPool()
{
	bRunning = false;

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
				BindIOCP(sSocket, m_hCompletion);

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
				}

				pClientSocket->AddIOContext(pIOContext);
				m_ClientSocketMap[sSocket] = pClientSocket;
				return true;
			}
		}
	}
}

bool CXJListenSocket::PostAccept(PER_SOCKET_CONTEXT *pskContext, PER_IO_CONTEXT *pIOContext)
{
	if ((NULL == pskContext) && (NULL == pIOContext))
	{
		return false;
	}

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
			return false;
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
		(sockaddr**)(&(pskContext->m_psaLocal)), &nLocalAddrLen, (sockaddr**)(&(pskContext->m_psaLocal)), &nRemoteAddrLen);

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
	pClientSocket->SetEffective();
	return pClientSocket->PostRecv(pIOContext);
}
