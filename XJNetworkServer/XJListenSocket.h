#pragma once
#include "XJSocket.h"
#include "XJClientSocket.h"

class CXJListenSocket :	public CXJSocket
{
	CXJListenSocket(long nSocketPoolSize, HANDLE hCompletion);
public:
	~CXJListenSocket();

	static CXJListenSocket* CreateObject(long nSocketPoolSize, HANDLE hCompletion);

	bool Startup();
	bool Cleanup();
	bool Listen();

	bool CreateSocketPool();
	bool CleanSocketPool();
	void IncrementEffectiveSocketCount();
	void DecrementEffectiveSocketCount();

	bool PostAccept();
	bool PostAccept(PER_SOCKET_CONTEXT *pskContext, PER_IO_CONTEXT *pIOContext);
	bool OnAccept(PER_IO_CONTEXT *pIOContext);

private:
	//SOCKET³Ø¼à¿ØÏß³Ì
	static unsigned _stdcall SocketPoolMonitor(void *pParam);

private:	
	HANDLE m_hCompletion;

	bool bRunning;
	long m_nSocketPoolSize;
	long m_nEffectiveSocketCount;
	CAtlMap<SOCKET, CXJClientSocket*> m_ClientSocketMap;
};

