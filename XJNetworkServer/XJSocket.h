#pragma once
#include "XJWinsockExApi.h"
#include "XJIOPacket.h"

extern long IO_CONTEXT_MARK;

typedef struct _PER_IO_CONTEXT
{
	_PER_IO_CONTEXT() { memset(this, 0, sizeof(_PER_IO_CONTEXT)); m_skSocket = INVALID_SOCKET; m_nIndex = InterlockedIncrement(&IO_CONTEXT_MARK); };
	void Reset() { memset(&m_OL, 0, sizeof(OVERLAPPED)); m_nNetworkEvent = 0; m_dwTransByte = 0;  m_dwFlag = 0; m_IOPacket.RemoveAll(); };
	OVERLAPPED m_OL;
	long m_nIndex;
	SOCKET m_skSocket;
	int m_nNetworkEvent;
	DWORD m_dwTransByte;
	DWORD m_dwFlag;
	CXJIOPacket m_IOPacket;
}PER_IO_CONTEXT, *PPER_IO_CONTEXT;

typedef struct _PER_SOCKET_CONTEXT
{
	_PER_SOCKET_CONTEXT() { memset(this, 0, sizeof(_PER_SOCKET_CONTEXT)); m_skSocket = INVALID_SOCKET; }
	SOCKET m_skSocket;
	char m_szAcceptBuff[2 * (sizeof(SOCKADDR_IN) + 16)];
	SOCKADDR_IN *m_psaLocal;
	SOCKADDR_IN *m_psaRemote;
}PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;

class CXJSocket
{
public:
	CXJSocket();
	virtual ~CXJSocket();

	SOCKET Socket(int nAf, int nType, int nProtocol);
	void BindIOCP(SOCKET &sSocket, HANDLE hIOCP);
	int CloseSocket(SOCKET &sSocket);
	
protected:
	PER_SOCKET_CONTEXT m_skContext;
	static CXJWinsockExApi m_WinsockExApi;
};

