// XJNetworkServer.h : XJNetworkServer DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CXJNetworkServerApp
// �йش���ʵ�ֵ���Ϣ������� XJNetworkServer.cpp
//

class CXJNetworkServerApp : public CWinApp
{
public:
	CXJNetworkServerApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
