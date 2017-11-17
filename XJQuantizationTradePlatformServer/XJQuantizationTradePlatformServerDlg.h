
// XJQuantizationTradePlatformServerDlg.h : 头文件
//

#pragma once
#include "../Common/IXJServer.h"

// CXJQuantizationTradePlatformServerDlg 对话框
class CXJQuantizationTradePlatformServerDlg : public CDialogEx
{
// 构造
public:
	CXJQuantizationTradePlatformServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XJQUANTIZATIONTRADEPLATFORMSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
private:
	IXJServer *m_pIXJServer;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
