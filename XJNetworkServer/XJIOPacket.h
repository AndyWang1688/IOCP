#pragma once
#include <atlcoll.h>

class CXJIOPacket
{
public:
	CXJIOPacket();
	~CXJIOPacket();

	void Add(int nLen, char *pBuff)
	{
		WSABUF wsb;
		wsb.len = nLen;
		wsb.buf = pBuff;
		m_WsaBufArray.Add(wsb);
	}

	void InsertAt(int nIndex, int nLen, char *pBuff)
	{
		WSABUF wsb;
		wsb.len = nLen;
		wsb.buf = pBuff;
		m_WsaBufArray.InsertAt(nIndex, wsb);
	}

	template<class T>
	void Add(T &data)
	{
		WSABUF wsb;
		wsb.len = sizeof(T);
		wsb.buf = (char*)&data;
		m_WsaBufArray.Add(wsb);
	}

	template<class T>
	void Insert(int nIndex, T&data)
	{
		WSABUF wsb;
		wsb.len = sizeof(T);
		wsb.buf = (char*)&data;
		m_WsaBufArray.InsertAt(nIndex, wsb);
	}

	operator LPWSABUF()
	{
		return m_WsaBufArray.GetData();
	}

	int GetCount()
	{
		return m_WsaBufArray.GetCount();
	}

	WSABUF& operator[](int nIndex)
	{
		return m_WsaBufArray[nIndex];
	}

	const WSABUF& operator[](int nIndex) const
	{
		return m_WsaBufArray[nIndex];
	}

	void RemoveAt(int nIndex)
	{
		m_WsaBufArray.RemoveAt(nIndex);
	}

	void RemoveAll()
	{
		m_WsaBufArray.RemoveAll();
	}

private:
	CAtlArray<WSABUF> m_WsaBufArray;
};

