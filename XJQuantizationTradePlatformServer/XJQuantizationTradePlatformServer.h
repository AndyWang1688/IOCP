
// XJQuantizationTradePlatformServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CXJQuantizationTradePlatformServerApp: 
// �йش����ʵ�֣������ XJQuantizationTradePlatformServer.cpp
//

class CXJQuantizationTradePlatformServerApp : public CWinApp
{
public:
	CXJQuantizationTradePlatformServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CXJQuantizationTradePlatformServerApp theApp;