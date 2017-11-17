
// XJQuantizationTradePlatformServerDlg.h : ͷ�ļ�
//

#pragma once
#include "../Common/IXJServer.h"

// CXJQuantizationTradePlatformServerDlg �Ի���
class CXJQuantizationTradePlatformServerDlg : public CDialogEx
{
// ����
public:
	CXJQuantizationTradePlatformServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XJQUANTIZATIONTRADEPLATFORMSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
private:
	IXJServer *m_pIXJServer;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedBtnStart();
	afx_msg void OnClickedBtnStop();
	afx_msg void OnClickedBtnSet();
};
