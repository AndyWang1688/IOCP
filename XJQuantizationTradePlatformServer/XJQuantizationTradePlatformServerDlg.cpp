
// XJQuantizationTradePlatformServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XJQuantizationTradePlatformServer.h"
#include "XJQuantizationTradePlatformServerDlg.h"
#include "afxdialogex.h"
#include "../Common/XJLoadDll.h"
#pragma comment(lib, "XJLoadLibraryLib.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CXJQuantizationTradePlatformServerDlg �Ի���



CXJQuantizationTradePlatformServerDlg::CXJQuantizationTradePlatformServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_XJQUANTIZATIONTRADEPLATFORMSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXJQuantizationTradePlatformServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CXJQuantizationTradePlatformServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNSTART, &CXJQuantizationTradePlatformServerDlg::OnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTNSTOP, &CXJQuantizationTradePlatformServerDlg::OnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTNSET, &CXJQuantizationTradePlatformServerDlg::OnClickedBtnSet)
END_MESSAGE_MAP()


// CXJQuantizationTradePlatformServerDlg ��Ϣ�������

BOOL CXJQuantizationTradePlatformServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_pIXJServer = NULL;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CXJQuantizationTradePlatformServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CXJQuantizationTradePlatformServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CXJQuantizationTradePlatformServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CXJQuantizationTradePlatformServerDlg::OnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IXJUnknown *pXJUnknown = NULL;
	InitLoadLibrary();
	pXJUnknown = CreateComObject("XJNetworkServer", MID_XJNetworkServer, IID_IXJServer);
	if (NULL != pXJUnknown)
	{
		pXJUnknown->QueryInterface(IID_IXJServer, (void**)&m_pIXJServer);
		if (NULL != m_pIXJServer)
		{
			if (m_pIXJServer->InitServer(2, 10, 1024))
			{
				m_pIXJServer->Run();
			}
		}
	}
}


void CXJQuantizationTradePlatformServerDlg::OnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (NULL == m_pIXJServer)
	{
		return;
	}
	m_pIXJServer->Exit(true);
	m_pIXJServer->Release();
	UnLoadLibrary(MID_XJNetworkServer);
	m_pIXJServer = NULL;
}


void CXJQuantizationTradePlatformServerDlg::OnClickedBtnSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxMessageBox(L"OnClickedBtnSet", MB_OK);
}
