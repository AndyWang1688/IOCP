#include "stdafx.h"
#include "XJWinsockExApi.h"


CXJWinsockExApi::CXJWinsockExApi()
{
}

CXJWinsockExApi::~CXJWinsockExApi()
{
}

bool CXJWinsockExApi::LoadAllFun(SOCKET sSocket)
{
	bool bCreateSocket = false;
	if (INVALID_SOCKET == sSocket)
	{
		if (INVALID_SOCKET == (sSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED)))
		{
			return false;
		}
		bCreateSocket = true;
	}

	bool bRet = false;
	bRet = LoadAcceptExFun(sSocket)
		&& LoadConnectExFun(sSocket)
		&& LoadDisconnectExFun(sSocket)
		&& LoadGetAcceptExSockaddrsFun(sSocket)
		&& LoadTransmitfileFun(sSocket)
		&& LoadTransmitPacketsFun(sSocket)
		&& LoadWSARecvMsgFun(sSocket);
#if(_WIN32_WINNT >= 0x0600)
	bRet = bRet && LoadWSASendMsgFun(sSocket);
#endif
	if (bCreateSocket)
	{
		closesocket(sSocket);
	}
	
	return bRet;
}

bool CXJWinsockExApi::LoadAllFun(int nAf, int nType, int nProtocol)
{
	SOCKET sSocket = INVALID_SOCKET;
	if (INVALID_SOCKET == (sSocket = WSASocket(nAf, nType, nProtocol, NULL, 0, WSA_FLAG_OVERLAPPED)))
	{
		return false;
	}

	bool bRet = false;
	bRet = LoadAcceptExFun(sSocket)
		&& LoadConnectExFun(sSocket)
		&& LoadDisconnectExFun(sSocket)
		&& LoadGetAcceptExSockaddrsFun(sSocket)
		&& LoadTransmitfileFun(sSocket)
		&& LoadTransmitPacketsFun(sSocket)
		&& LoadWSARecvMsgFun(sSocket);
#if(_WIN32_WINNT >= 0x0600)
	bRet = bRet && LoadWSASendMsgFun(sSocket);
#endif

	closesocket(sSocket);
	return bRet;
}

bool CXJWinsockExApi::LoadWSAFun(SOCKET& sSocket, GUID&funGuid, void*&pFun)
{
	DWORD dwBytes = 0;
	BOOL bRet = TRUE;
	pFun = NULL;

	if (SOCKET_ERROR == ::WSAIoctl(sSocket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&funGuid,
		sizeof(funGuid),
		&pFun,
		sizeof(pFun),
		&dwBytes,
		NULL,
		NULL))
	{
		pFun = NULL;
	}
	return NULL != pFun;
}

bool CXJWinsockExApi::LoadAcceptExFun(SOCKET& sSocket)
{
	GUID funGuid = WSAID_ACCEPTEX;
	return LoadWSAFun(sSocket, funGuid, (void*&)m_pfnAcceptEx);
}

bool CXJWinsockExApi::LoadConnectExFun(SOCKET& sSocket)
{
	GUID funGuid = WSAID_CONNECTEX;
	return LoadWSAFun(sSocket, funGuid, (void*&)m_pfnConnectEx);
}

bool CXJWinsockExApi::LoadDisconnectExFun(SOCKET& sSocket)
{
	GUID funGuid = WSAID_DISCONNECTEX;
	return LoadWSAFun(sSocket, funGuid, (void*&)m_pfnDisconnectEx);
}

bool CXJWinsockExApi::LoadGetAcceptExSockaddrsFun(SOCKET& sSocket)
{
	GUID funGuid = WSAID_GETACCEPTEXSOCKADDRS;
	return LoadWSAFun(sSocket, funGuid, (void*&)m_pfnGetAcceptExSockaddrs);
}

bool CXJWinsockExApi::LoadTransmitfileFun(SOCKET& sSocket)
{
	GUID funGuid = WSAID_TRANSMITFILE;
	return LoadWSAFun(sSocket, funGuid, (void*&)m_pfnTransmitfile);
}

bool CXJWinsockExApi::LoadTransmitPacketsFun(SOCKET& sSocket)
{
	GUID funGuid = WSAID_TRANSMITPACKETS;
	return LoadWSAFun(sSocket, funGuid, (void*&)m_pfnTransmitPackets);
}

bool CXJWinsockExApi::LoadWSARecvMsgFun(SOCKET& sSocket)
{
	GUID funGuid = WSAID_WSARECVMSG;
	return LoadWSAFun(sSocket, funGuid, (void*&)m_pfnWSARecvMsg);
}

#if(_WIN32_WINNT >= 0x0600)
bool CXJWinsockExApi::LoadWSASendMsgFun(SOCKET& sSocket)
{
	GUID funGuid = WSAID_WSASENDMSG;
	return LoadWSAFun(sSocket, funGuid, (void*&)m_pfnWSASendMsg);
}

#endif