
// InlineHookSSSDTӦ�ó���Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "InlineHookSSSDTӦ�ó���.h"
#include "InlineHookSSSDTӦ�ó���Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
HANDLE  g_hDevice = NULL;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CInlineHookSSSDTӦ�ó���Dlg �Ի���




CInlineHookSSSDTӦ�ó���Dlg::CInlineHookSSSDTӦ�ó���Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInlineHookSSSDTӦ�ó���Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bHook = FALSE;
}

void CInlineHookSSSDTӦ�ó���Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInlineHookSSSDTӦ�ó���Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PROTECT, &CInlineHookSSSDTӦ�ó���Dlg::OnBnClickedButtonProtect)
	ON_BN_CLICKED(IDC_BUTTON_UNPROTECT, &CInlineHookSSSDTӦ�ó���Dlg::OnBnClickedButtonUnprotect)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CInlineHookSSSDTӦ�ó���Dlg ��Ϣ�������

BOOL CInlineHookSSSDTӦ�ó���Dlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CInlineHookSSSDTӦ�ó���Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CInlineHookSSSDTӦ�ó���Dlg::OnPaint()
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
HCURSOR CInlineHookSSSDTӦ�ó���Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CInlineHookSSSDTӦ�ó���Dlg::OnBnClickedButtonProtect()
{
	if (m_bHook==FALSE)
	{
		g_hDevice = OpenDevice(L"\\\\.\\InlineHookSSSDTLink");

		if (g_hDevice==(HANDLE)-1)
		{
			MessageBox(L"���豸ʧ��");

			return;
		}

		if(SendIoControlCode(INITILIZE)==FALSE)
		{


			CloseHandle(g_hDevice);
			return;
		}


		if(SendIoControlCode(HOOK)==FALSE)
		{


			CloseHandle(g_hDevice);
			return;
		}

		CloseHandle(g_hDevice);

		m_bHook = TRUE;
	}
}


void CInlineHookSSSDTӦ�ó���Dlg::OnBnClickedButtonUnprotect()
{
	if (m_bHook==TRUE)
	{
		g_hDevice = OpenDevice(L"\\\\.\\InlineHookSSSDTLink");

		if (g_hDevice==(HANDLE)-1)
		{
			MessageBox(L"���豸ʧ��");

			return;
		}

		if(SendIoControlCode(UNHOOK)==FALSE)
		{

			CloseHandle(g_hDevice);
			return;
		}

		CloseHandle(g_hDevice);


		m_bHook = FALSE;
	}

}


void CInlineHookSSSDTӦ�ó���Dlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	OnBnClickedButtonUnprotect();

	CDialogEx::OnClose();
}

BOOL CInlineHookSSSDTӦ�ó���Dlg::SendIoControlCode(ULONG_PTR ulControlCode)
{
	BOOL bRet = FALSE;
	DWORD ulReturnSize = 0;

	if (ulControlCode==INITILIZE)
	{
		bRet = DeviceIoControl(g_hDevice,IOCTL_GET_SSSDTSERVERICE,
			NULL,
			NULL,
			NULL,
			NULL,
			&ulReturnSize,
			NULL);

		if (bRet==FALSE)
		{
			return FALSE;
		}


	}

	if (ulControlCode==HOOK)
	{

		bRet = DeviceIoControl(g_hDevice,IOCTL_INLINEHOOK_SSSDT,
			NULL,
			NULL,
			NULL,
			NULL,
			&ulReturnSize,
			NULL);

		if (bRet==FALSE)
		{
			return FALSE;
		}
	}


	if (ulControlCode==UNHOOK)
	{

		bRet = DeviceIoControl(g_hDevice,IOCTL_INLINEUNHOOK_SSSDT,
			NULL,
			NULL,
			NULL,
			NULL,
			&ulReturnSize,
			NULL);

		if (bRet==FALSE)
		{
			return FALSE;
		}
	}



	return TRUE;
}
