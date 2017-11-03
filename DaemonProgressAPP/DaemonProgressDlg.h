
// DaemonProgressDlg.h : 头文件
//

#pragma once
#include "DaemonToolAPI.h"
#include "afxbutton.h"

// CDaemonProgressDlg 对话框
class CDaemonProgressDlg : public CDialogEx
{
// 构造
public:
	CDaemonProgressDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAEMONPROGRESSAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()



private:
	DaemonToolAPI daemonAPI_;
public:
	CString processPath_;
	CString processPara_;
	CString ipAddr_;
	DWORD port_;
	CMFCButton startDaemon_;
	CMFCButton stopDaemon_;
	afx_msg void OnBnClickedClose();
	BOOL isMaster_;
	afx_msg void OnBnClickedRadioCluster();
	CMFCButton exit_;
	afx_msg void OnBnClickedRadioMaster();
	afx_msg BOOL PreTranslateMessage(MSG *);
	afx_msg void OnBnClickedStart();
	CString log_;
	afx_msg void OnBnClickedStop();
};
