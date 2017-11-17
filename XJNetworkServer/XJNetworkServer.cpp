// XJNetworkServer.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "XJNetworkServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "XJServer.h"

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CXJNetworkServerApp

BEGIN_MESSAGE_MAP(CXJNetworkServerApp, CWinApp)
END_MESSAGE_MAP()


// CXJNetworkServerApp ����

CXJNetworkServerApp::CXJNetworkServerApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CXJNetworkServerApp ����

CXJNetworkServerApp theApp;


// CXJNetworkServerApp ��ʼ��

BOOL CXJNetworkServerApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" DLL_PORT IXJUnknown* CreateXJObject(GUID MID, GUID IID)
{
	IXJUnknown* pIXJUnknown = NULL;
	if (MID_XJNetworkServer == MID)
	{
		if (IID_IXJServer == IID)
		{
			pIXJUnknown = new CXJServer();
		}		
	}

	return pIXJUnknown;
}
