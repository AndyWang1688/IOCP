#pragma once
#include "XJSocket.h"

class CXJClientSocket :	public CXJSocket
{
public:
	CXJClientSocket(SOCKET skSocket, SOCKET& skListen);
	~CXJClientSocket();

	PER_SOCKET_CONTEXT *GetSocketContext();
	void AddIOContext(PER_IO_CONTEXT *pIOContext);

	void SetEffective(bool bEffective);
	bool IsEffective();
	int GetHeadPackTimes();
	void IncreaseHeadPackTimes();

	bool PostRecv(PER_IO_CONTEXT *pIOContext);
	bool OnRecv(PER_IO_CONTEXT *pIOContext);
	bool PostSend(PER_IO_CONTEXT *pIOContext);
	bool OnSend(PER_IO_CONTEXT *pIOContext);
	bool PostDisconnect(PER_IO_CONTEXT *pIOContext);
	bool OnDisconnect(PER_IO_CONTEXT *pIOContext);

private:
	bool ReAcceptEx(PER_IO_CONTEXT *pIOContext);

private:
	SOCKET &m_skListen;
	CAtlMap<int, PER_IO_CONTEXT*> m_IOContextMap;
	bool m_bEffective;
	int m_nHeadPackTimes;
};

