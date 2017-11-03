
// DaemonProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DaemonProgressAPP.h"
#include "DaemonProgressDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDaemonProgressDlg 对话框



CDaemonProgressDlg::CDaemonProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DAEMONPROGRESSAPP_DIALOG, pParent)
	, processPath_(_T(""))
	, processPara_(_T(""))
	, ipAddr_(_T(""))
	, port_(0)
	, isMaster_(FALSE)
	, log_(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDaemonProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILEADDR, processPath_);
	DDX_Text(pDX, IDC_PARAMETER, processPara_);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, ipAddr_);
	DDX_Text(pDX, IDC_PORT, port_);
	DDX_Control(pDX, IDC_START, startDaemon_);
	DDX_Control(pDX, IDC_STOP, stopDaemon_);
	DDX_Radio(pDX, IDC_RADIO_MASTER, isMaster_);
	DDX_Control(pDX, IDC_EXIT, exit_);
	DDX_Text(pDX, IDC_LOG, log_);
}

BEGIN_MESSAGE_MAP(CDaemonProgressDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXIT, &CDaemonProgressDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_RADIO_CLUSTER, &CDaemonProgressDlg::OnBnClickedRadioCluster)
	ON_BN_CLICKED(IDC_RADIO_MASTER, &CDaemonProgressDlg::OnBnClickedRadioMaster)
	ON_BN_CLICKED(IDC_START, &CDaemonProgressDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CDaemonProgressDlg::OnBnClickedStop)
END_MESSAGE_MAP()


// CDaemonProgressDlg 消息处理程序

BOOL CDaemonProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	if (!daemonAPI_.InitAPI())
	{
		MessageBox("获取dll失败！", NULL, MB_OK | MB_ICONERROR);
		SendMessage(WM_CLOSE);
	}
	

	((CButton*)GetDlgItem(IDC_RADIO_MASTER))->SetCheck(1);
	stopDaemon_.EnableWindow(FALSE);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDaemonProgressDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDaemonProgressDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDaemonProgressDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedStop();
	daemonAPI_.ReleaseAPI();
	SendMessage(WM_CLOSE);
}


void CDaemonProgressDlg::OnBnClickedRadioCluster()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	SetDlgItemText(IDC_PCNAME, "主计算机地址");
}


void CDaemonProgressDlg::OnBnClickedRadioMaster()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	SetDlgItemText(IDC_PCNAME, "备用计算机地址");
}

BOOL CDaemonProgressDlg::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return	TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return	TRUE;
	}
	else
	{
		return	CDialog::PreTranslateMessage(pMsg);
	}
}


void CDaemonProgressDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	stopDaemon_.EnableWindow(TRUE);
	startDaemon_.EnableWindow(FALSE);
	log_.Insert(0, "开始进程守护！\r\n");
	UpdateData(FALSE);
	HeartBeatConfig cfg;
	cfg.ip = ipAddr_;
	cfg.isMaster = isMaster_;
	cfg.port = port_;
	if (daemonAPI_.Daemon(std::string(processPath_), processPara_.GetBuffer(), cfg))
	{
		log_.Insert(0, "开始进程守护！\r\n");
	}
	else
	{
		log_.Insert(0, "开启守护失败！\r\n");
	}
}


void CDaemonProgressDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	stopDaemon_.EnableWindow(FALSE);
	startDaemon_.EnableWindow(TRUE);
	log_.Insert(0, "结束进程守护!\r\n");
	UpdateData(FALSE);
	daemonAPI_.Stop();
}
