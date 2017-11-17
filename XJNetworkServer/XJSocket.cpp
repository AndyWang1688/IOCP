#include "stdafx.h"
#include "XJSocket.h"

extern long IO_CONTEXT_MARK = 0;

CXJWinsockExApi CXJSocket::m_WinsockExApi;

CXJSocket::CXJSocket()
{
	
}


CXJSocket::~CXJSocket()
{
	
}

SOCKET CXJSocket::Socket(int nAf, int nType, int nProtocol)
{
	return WSASocket(nAf, nType, nProtocol, NULL, 0, WSA_FLAG_OVERLAPPED);
}

void CXJSocket::BindIOCP(SOCKET &sSocket, HANDLE hIOCP)
{
	if (INVALID_SOCKET != sSocket)
	{
		::CreateIoCompletionPort((HANDLE)sSocket, hIOCP, (ULONG_PTR)this, 0);
	}
}

int CXJSocket::CloseSocket(SOCKET &sSocket)
{
	if (INVALID_SOCKET != sSocket)
	{
		shutdown(sSocket, SB_BOTH);
		return closesocket(sSocket);
	}
	return -1;
}

