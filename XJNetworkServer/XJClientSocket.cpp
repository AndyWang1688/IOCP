#include "stdafx.h"
#include "XJClientSocket.h"


CXJClientSocket::CXJClientSocket(SOCKET skSocket, SOCKET skListen) : m_skListen(skListen)
{
	m_skContext.m_skSocket = skSocket;
	m_bEffective = false;
}


CXJClientSocket::~CXJClientSocket()
{
	closesocket(m_skContext.m_skSocket);

	CAtlMap<int, PER_IO_CONTEXT*>::CPair *pair = NULL;
	POSITION ps = m_IOContextMap.GetStartPosition();
	while (NULL != ps)
	{
		pair = m_IOContextMap.GetNext(ps);
		if (NULL != pair)
		{
			delete pair->m_value;
			pair->m_value = NULL;
		}
	}
	m_IOContextMap.RemoveAll();
}

PER_SOCKET_CONTEXT *CXJClientSocket::GetSocketContext()
{
	return &m_skContext;
}

void CXJClientSocket::AddIOContext(PER_IO_CONTEXT *pIOContext)
{
	if (NULL != pIOContext)
	{
		m_IOContextMap[pIOContext->m_nIndex] = pIOContext;
	}
}

void CXJClientSocket::SetEffective()
{
	m_bEffective = true;
}

bool CXJClientSocket::IsEffective()
{
	return m_bEffective;
}

bool CXJClientSocket::PostRecv(PER_IO_CONTEXT *pIOContext)
{
	if (NULL == pIOContext)
	{
		return false;
	}

	pIOContext->Reset();
	pIOContext->m_nNetworkEvent = FD_READ;
	//Ͷ��һ�������������󣬴�ʱ�������ݻ�����Ϊ�գ��������õ���������֪ͨʱ׼�����ݻ�����
	WSABUF wsb = { 0, NULL };
	if (SOCKET_ERROR == WSARecv(m_skContext.m_skSocket, &wsb, 1, 
		&(pIOContext->m_dwTransByte), &(pIOContext->m_dwFlag), &(pIOContext->m_OL), NULL))
	{
		int nErr = GetLastError();
		if (WSA_IO_PENDING != nErr)
		{
			return false;
		}
	}
	return true;
}

bool CXJClientSocket::OnRecv(PER_IO_CONTEXT *pIOContext)
{
	if (NULL == pIOContext)
	{
		return false;
	}

	//�ȴ���������֪ͨ����ʱLPOVERLAPPED����ΪNULL������ʾ�������������ݣ���ʱ��Ҫ׼���������ݵĻ���
	char szBuff[1024] = { 0 };
	pIOContext->m_IOPacket.Add(1024, szBuff);
	if (SOCKET_ERROR == WSARecv(m_skContext.m_skSocket, pIOContext->m_IOPacket, pIOContext->m_IOPacket.GetCount(), 
		&(pIOContext->m_dwTransByte), &(pIOContext->m_dwFlag), NULL, NULL))
	{
		int nErr = GetLastError();
		if (WSAEWOULDBLOCK != nErr)
		{
			return false;
		}
	}

	return true;
}

bool CXJClientSocket::PostSend(PER_IO_CONTEXT *pIOContext)
{
	if (NULL == pIOContext)
	{
		return false;
	}

	pIOContext->m_nNetworkEvent = FD_WRITE;
	if (SOCKET_ERROR == WSASend(m_skContext.m_skSocket, pIOContext->m_IOPacket, pIOContext->m_IOPacket.GetCount(),
		&(pIOContext->m_dwTransByte), 0, (LPOVERLAPPED)(&(pIOContext->m_OL)), NULL))
	{
		int nErr = WSAGetLastError();
		if (nErr != WSA_IO_PENDING)
		{
			return false;
		}
	}

	AddIOContext(pIOContext);
	return true;
}

bool CXJClientSocket::OnSend(PER_IO_CONTEXT *pIOContext)
{
	return true;
}

bool CXJClientSocket::PostDisconnect(PER_IO_CONTEXT *pIOContext)
{
	pIOContext->m_nNetworkEvent = FD_CLOSE;
	shutdown(m_skContext.m_skSocket, SD_BOTH);
	//ע������ط�������DisconnectEx������һ���������Զ�SOCKET�����˻��գ�
	//���ն����ǹرպ��ٴ����������˷�����������
	m_WinsockExApi.DisconnectEx(m_skContext.m_skSocket, &(pIOContext->m_OL), TF_REUSE_SOCKET, 0);
	return true;
}

bool CXJClientSocket::OnDisconnect(PER_IO_CONTEXT *pIOContext)
{
	m_bEffective = false;
	return ReAcceptEx(pIOContext);
}

bool CXJClientSocket::ReAcceptEx(PER_IO_CONTEXT *pIOContext)
{
	pIOContext->Reset();
	pIOContext->m_nNetworkEvent = FD_ACCEPT;
	if (FALSE == m_WinsockExApi.AcceptEx(m_skListen, m_skContext.m_skSocket, m_skContext.m_szAcceptBuff, 0,
		(sizeof(SOCKADDR_IN) + 16), (sizeof(SOCKADDR_IN) + 16), &(pIOContext->m_dwTransByte), &(pIOContext->m_OL)))
	{
		int iError = WSAGetLastError();
		if (ERROR_IO_PENDING != iError
			&& WSAECONNRESET != iError)
		{
			if (INVALID_SOCKET != pIOContext->m_skSocket)
			{
				::closesocket(m_skContext.m_skSocket);
				m_skContext.m_skSocket = INVALID_SOCKET;
			}
			return false;
		}
	}
	return true;
}
