
// DaemonProgressDlg.h : ͷ�ļ�
//

#pragma once
#include "DaemonToolAPI.h"
#include "afxbutton.h"

// CDaemonProgressDlg �Ի���
class CDaemonProgressDlg : public CDialogEx
{
// ����
public:
	CDaemonProgressDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAEMONPROGRESSAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
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
