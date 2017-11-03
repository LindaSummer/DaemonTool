
// DaemonProgressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DaemonProgressAPP.h"
#include "DaemonProgressDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDaemonProgressDlg �Ի���



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


// CDaemonProgressDlg ��Ϣ�������

BOOL CDaemonProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if (!daemonAPI_.InitAPI())
	{
		MessageBox("��ȡdllʧ�ܣ�", NULL, MB_OK | MB_ICONERROR);
		SendMessage(WM_CLOSE);
	}
	

	((CButton*)GetDlgItem(IDC_RADIO_MASTER))->SetCheck(1);
	stopDaemon_.EnableWindow(FALSE);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDaemonProgressDlg::OnPaint()
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
HCURSOR CDaemonProgressDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDaemonProgressDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedStop();
	daemonAPI_.ReleaseAPI();
	SendMessage(WM_CLOSE);
}


void CDaemonProgressDlg::OnBnClickedRadioCluster()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	SetDlgItemText(IDC_PCNAME, "���������ַ");
}


void CDaemonProgressDlg::OnBnClickedRadioMaster()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	SetDlgItemText(IDC_PCNAME, "���ü������ַ");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	stopDaemon_.EnableWindow(TRUE);
	startDaemon_.EnableWindow(FALSE);
	log_.Insert(0, "��ʼ�����ػ���\r\n");
	UpdateData(FALSE);
	HeartBeatConfig cfg;
	cfg.ip = ipAddr_;
	cfg.isMaster = isMaster_;
	cfg.port = port_;
	if (daemonAPI_.Daemon(std::string(processPath_), processPara_.GetBuffer(), cfg))
	{
		log_.Insert(0, "��ʼ�����ػ���\r\n");
	}
	else
	{
		log_.Insert(0, "�����ػ�ʧ�ܣ�\r\n");
	}
}


void CDaemonProgressDlg::OnBnClickedStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	stopDaemon_.EnableWindow(FALSE);
	startDaemon_.EnableWindow(TRUE);
	log_.Insert(0, "���������ػ�!\r\n");
	UpdateData(FALSE);
	daemonAPI_.Stop();
}
