#include "stdafx.h"
#include "XJServer.h"


CXJServer::CXJServer()
{
	m_nRef = 0;
}


CXJServer::~CXJServer()
{
}

bool CXJServer::InitServer(int nThreadMinimum, int nThreadMaximum, int nNetBuffSize)
{
	XJCompltion.Init(nThreadMinimum, nThreadMaximum, nNetBuffSize);
	return true;
}

void CXJServer::Run()
{
	XJCompltion.Run();
}

bool CXJServer::Exit(bool bCancelPendingCallbacks)
{
	XJCompltion.Exit(bCancelPendingCallbacks);
	return true;
}


void CXJServer::AddRef()
{
	m_nRef++;
}

void CXJServer::Release()
{
	if (0 == m_nRef)
	{
		delete this;
		return;
	}
	m_nRef--;
}

void CXJServer::QueryInterface(GUID GIID, void **pInterface)
{
	if (IID_IXJServer == GIID)
	{
		*pInterface = this;
	}
	else
	{
		*pInterface = NULL;
	}
}
