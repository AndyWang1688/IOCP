#pragma once
//#include <Winsock.h> //确定使用Winsock1时 还原这个 而注释下一行
#include <Winsock2.h>
#include <MSWSOCK.h> 	

//#pragma comment( lib, "WSOCK32.lib" ) //确定使用Winsock1时 还原这个 而注释下一行
#pragma comment( lib, "Ws2_32.lib" )
#pragma comment( lib, "Mswsock.lib" )

class CXJWinsockExApi
{
public:
	CXJWinsockExApi();
	~CXJWinsockExApi();

public:
	bool LoadAllFun(SOCKET sSocket = INVALID_SOCKET);
	bool LoadAllFun(int nAf, int nType, int nProtocol);

public:
	inline BOOL AcceptEx(
		SOCKET sListenSocket,
		SOCKET sAcceptSocket,
		PVOID lpOutputBuffer,
		DWORD dwReceiveDataLength,
		DWORD dwLocalAddressLength,
		DWORD dwRemoteAddressLength,
		LPDWORD lpdwBytesReceived,
		LPOVERLAPPED lpOverlapped
	)
	{
		return m_pfnAcceptEx(sListenSocket,
			sAcceptSocket,
			lpOutputBuffer,
			dwReceiveDataLength,
			dwLocalAddressLength,
			dwRemoteAddressLength,
			lpdwBytesReceived,
			lpOverlapped);
	}
	

	inline BOOL ConnectEx(
		SOCKET s,
		const struct sockaddr FAR *name,
		int namelen,
		PVOID lpSendBuffer,
		DWORD dwSendDataLength,
		LPDWORD lpdwBytesSent,
		LPOVERLAPPED lpOverlapped
	) 
	{
		return m_pfnConnectEx(s,
			name,
			namelen,
			lpSendBuffer,
			dwSendDataLength,
			lpdwBytesSent,
			lpOverlapped);
	}

	inline BOOL DisconnectEx(
		SOCKET s,
		LPOVERLAPPED lpOverlapped,
		DWORD  dwFlags,
		DWORD  dwReserved
	)
	{
		return m_pfnDisconnectEx(s,
			lpOverlapped,
			dwFlags,
			dwReserved);
	}

	inline void GetAcceptExSockaddrs(
		PVOID lpOutputBuffer,
		DWORD dwReceiveDataLength,
		DWORD dwLocalAddressLength,
		DWORD dwRemoteAddressLength,
		sockaddr **LocalSockaddr,
		LPINT LocalSockaddrLength,
		sockaddr **RemoteSockaddr,
		LPINT RemoteSockaddrLength
	)
	{
		m_pfnGetAcceptExSockaddrs(lpOutputBuffer,
			dwReceiveDataLength,
			dwLocalAddressLength,
			dwRemoteAddressLength,
			LocalSockaddr,
			LocalSockaddrLength,
			RemoteSockaddr,
			RemoteSockaddrLength);
	}

	inline BOOL TransmitFile(
		SOCKET hSocket,
		HANDLE hFile,
		DWORD nNumberOfBytesToWrite,
		DWORD nNumberOfBytesPerSend,
		LPOVERLAPPED lpOverlapped,
		LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers,
		DWORD dwReserved
	)
	{
		return m_pfnTransmitfile(hSocket,
			hFile,
			nNumberOfBytesToWrite,
			nNumberOfBytesPerSend,
			lpOverlapped,
			lpTransmitBuffers,
			dwReserved);
	}

	inline BOOL TransmitPackets(
		SOCKET hSocket,
		LPTRANSMIT_PACKETS_ELEMENT lpPacketArray,
		DWORD nElementCount,
		DWORD nSendSize,
		LPOVERLAPPED lpOverlapped,
		DWORD dwFlags
	)
	{
		return m_pfnTransmitPackets(hSocket,
			lpPacketArray,
			nElementCount,
			nSendSize,
			lpOverlapped,
			dwFlags);
	}

	inline INT WSARecvMsg(
		SOCKET s,
		LPWSAMSG lpMsg,
		LPDWORD lpdwNumberOfBytesRecvd,
		LPWSAOVERLAPPED lpOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
	)
	{
		return m_pfnWSARecvMsg(s,
			lpMsg,
			lpdwNumberOfBytesRecvd,
			lpOverlapped,
			lpCompletionRoutine);
	}

#if(_WIN32_WINNT >= 0x0600)
	inline INT WSASendMsg(
		SOCKET s,
		LPWSAMSG lpMsg,
		DWORD dwFlags,
		LPDWORD lpNumberOfBytesSent,
		LPWSAOVERLAPPED lpOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
	)
	{
		return m_pfnWSASendMsg(s,
			lpMsg,
			dwFlags,
			lpNumberOfBytesSent,
			lpOverlapped,
			lpCompletionRoutine);
	}
#endif

private:
	bool LoadWSAFun(SOCKET& sSocket, GUID&funGuid, void*&pFun);
	bool LoadAcceptExFun(SOCKET& sSocket);
	bool LoadConnectExFun(SOCKET& sSocket);
	bool LoadDisconnectExFun(SOCKET& sSocket);
	bool LoadGetAcceptExSockaddrsFun(SOCKET& sSocket);
	bool LoadTransmitfileFun(SOCKET& sSocket);
	bool LoadTransmitPacketsFun(SOCKET& sSocket);
	bool LoadWSARecvMsgFun(SOCKET& sSocket);
#if(_WIN32_WINNT >= 0x0600)
	bool LoadWSASendMsgFun(SOCKET& sSocket);
#endif

private:
	LPFN_ACCEPTEX m_pfnAcceptEx;
	LPFN_CONNECTEX m_pfnConnectEx;
	LPFN_DISCONNECTEX m_pfnDisconnectEx;
	LPFN_GETACCEPTEXSOCKADDRS m_pfnGetAcceptExSockaddrs;
	LPFN_TRANSMITFILE m_pfnTransmitfile;
	LPFN_TRANSMITPACKETS m_pfnTransmitPackets;
	LPFN_WSARECVMSG m_pfnWSARecvMsg;
#if(_WIN32_WINNT >= 0x0600)
	LPFN_WSASENDMSG m_pfnWSASendMsg;
#endif

	/*
	WSAID_ACCEPTEX
	WSAID_CONNECTEX
	WSAID_DISCONNECTEX
	WSAID_GETACCEPTEXSOCKADDRS
	WSAID_TRANSMITFILE
	WSAID_TRANSMITPACKETS
	WSAID_WSARECVMSG
	WSAID_WSASENDMSG 
	*/
};

