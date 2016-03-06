
// HeavenShadow.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "HeavenShadow.h"
#include "HeavenShadowDlg.h"


//////////////////////////////////////////////////////////////////////////
#include "LoadSys.h"

#include "WzdSplash.h"

#define HS_ONLY_ONE_PROCESS L"HS_ONLY_ONE_PROCESS"

extern HANDLE g_hDevice;
BOOL bDriverIsOK = FALSE;
//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHeavenShadowApp

BEGIN_MESSAGE_MAP(CHeavenShadowApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHeavenShadowApp ����

CHeavenShadowApp::CHeavenShadowApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHeavenShadowApp ����

CHeavenShadowApp theApp;


// CHeavenShadowApp ��ʼ��

BOOL CHeavenShadowApp::InitInstance()
{
	//////////////////////////////////////////////////////////////////////////


	HANDLE hMutex = ::CreateMutexW(NULL,TRUE,HS_ONLY_ONE_PROCESS);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		::MessageBox(
			NULL,
			L"���Ѿ���������Ӱ��ʿ��",
			L"��Ӱ��ʿ",
			0
			);	//�����Ի���ȷ�ϲ������еڶ���ʵ����

		CloseHandle(hMutex);
		ExitProcess(0);
	}

	if (HsIs64BitWindows() == TRUE && sizeof(ULONG_PTR) == sizeof(ULONG32))
	{
		::MessageBox(
			NULL,
			L"����ʹ�� 64 λ�� Windows ����ϵͳ��������Ӱ��ʿ 32 λ��\r\n���ܻ���ɲ���Ԥ�ϵĺ��������ѡ����Ӱ��ʿ 64 λ�档",
			L"��Ӱ��ʿ",
			0
			);
		ExitProcess(0);
	}

	//////////////////��ʾSplash��2010-10-15///////////////////////////////////
	CWzdSplash wndSplash;                 //���������������ʵ��
	wndSplash.Create(IDB_BITMAP_WZDSPLASH);
	wndSplash.CenterWindow();
	wndSplash.UpdateWindow();          //send WM_PAINT


	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("��Ӱ��ʿ"));


	//////////////////////////////////////////////////////////////////////////

	// 	WCHAR *Temp = AfxGetApp()->m_lpCmdLine;
	// 
	// 	BOOL bIsHideWnd = FALSE;
	// 
	// 	if (wcslen(Temp))
	// 	{
	// 		WCHAR* p = Temp;
	// 
	// 		for (int i = 0; i< wcslen(Temp); i++)
	// 		{
	// 			if (_wcsnicmp(p,L"-HideWnd",wcslen(L"-HideWnd"))==0)
	// 			{
	// 				bIsHideWnd = TRUE;
	// 			}
	// 			p++;
	// 		}
	// 	}

	// 	CHeavenShadowDlg* dlg = new CHeavenShadowDlg();
	// 
	// 	m_pMainWnd = dlg;
	// 
	// 	INT_PTR nResponse = dlg->Create(IDD_HEAVENSHADOW_DIALOG);
	// 
	// 	dlg->ShowWindow(SW_HIDE);
	// 
	// 	return FALSE;


	//////////////////////////////////////////////////////////////////////////

	//��������
	//ExtractFile();
	//////////////////////////////////////////////////////////////////////////
	WCHAR wzSysPath[260] = {0};
	WCHAR *p;
	CString SysPath;
	HMODULE module = GetModuleHandle(0);
	GetModuleFileName(module,wzSysPath,sizeof(wzSysPath));
	p = wcsrchr(wzSysPath,L'\\');
	*p = 0;
	SysPath = wzSysPath;
	if (HsIs64BitWindows()) 
		SysPath += L"\\sys\\x64\\HeavenShadowDrv.sys";
	else 
		SysPath += L"\\sys\\x86\\HeavenShadowDrv.sys";

	HsLoadNTDriver(HS_DRIVER_NAME,SysPath.GetBuffer());
	//////////////////////////////////////////////////////////////////////////


	g_hDevice = CreateFileW(HS_LINK_NAME,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL );


	if (g_hDevice != INVALID_HANDLE_VALUE)
	{
		ULONG_PTR ulCurrentPid = GetCurrentProcessId();
		ULONG_PTR ulRetCode = 0;
		DWORD dwReturnSize = 0;

		BOOL dwRet = DeviceIoControl(g_hDevice,
			HS_IOCTL(HS_IOCTL_PROC_SENDSELFPID),
			&ulCurrentPid,
			sizeof(ULONG_PTR),
			&ulRetCode,
			sizeof(ULONG_PTR),
			&dwReturnSize,
			NULL);

		if (dwRet && ulRetCode)
		{
			bDriverIsOK = TRUE;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	
	



	CHeavenShadowDlg dlg;
	m_pMainWnd = &dlg;
	wndSplash.DestroyWindow();
	INT_PTR nResponse = dlg.DoModal();


	//////////////////////////////////////////////////////////////////////////

	CloseHandle(hMutex);

	CloseHandle(g_hDevice);

	HsUnloadNTDriver(HS_DRIVER_NAME);	//ж������
	
	//////////////////////////////////////////////////////////////////////////


	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
 
// 	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
// 	//  ����������Ӧ�ó������Ϣ�á�
 	return FALSE;
}

