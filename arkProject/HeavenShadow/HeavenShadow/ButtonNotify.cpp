#include "stdafx.h"
#include "HeavenShadow.h"
#include "HeavenShadowDlg.h"
#include "afxdialogex.h"


extern BOOL bIsChecking;
extern BOOL bDriverIsOK;

//////////////////////////////////////////////////////////////////////////

void CHeavenShadowDlg::OnClickedStaticProcess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!bIsChecking && bDriverIsOK)
	{
		HsEnableNowButton();
		HsSelectWindow(HS_DIALOG_PROCESS);
	}
}
void CHeavenShadowDlg::OnClickedStaticModule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!bIsChecking && bDriverIsOK)
	{
		HsEnableNowButton();
		HsSelectWindow(HS_DIALOG_MODULE);
	}
}




void CHeavenShadowDlg::OnClickedStaticService()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!bIsChecking && bDriverIsOK)
	{
		HsEnableNowButton();
		HsSelectWindow(HS_DIALOG_SERVICE);
	}
}





void CHeavenShadowDlg::OnEnableStaticProcess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns_proc = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_PROCESS),RT_GROUP_ICON);

	HBITMAP   hBmp_proc = ::LoadBitmap(hIns_proc, MAKEINTRESOURCE(IDB_BITMAP_PROCESS));

	m_btnProc.SetBitmap(hBmp_proc);
}


void CHeavenShadowDlg::OnDisableStaticProcess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����λͼ
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_PROCESS_NOW),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_PROCESS_NOW));

	m_btnProc.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnEnableStaticModule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_MODULE),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_MODULE));

	m_btnModu.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnDisableStaticModule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_MODULE_NOW),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_MODULE_NOW));

	m_btnModu.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnEnableStaticService()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_SERVICE),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_SERVICE));

	m_btnServ.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnDisableStaticService()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_SERVICE_NOW),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_SERVICE_NOW));

	m_btnServ.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnClickedStaticLogo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!bIsChecking && bDriverIsOK)
	{
		HsEnableNowButton();
		HsSelectWindow(HS_DIALOG_ABOUT);
	}

}

void CHeavenShadowDlg::OnEnableStaticLogo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_LOGOBAR),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_LOGOBAR));

	m_btnAbou.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnDisableStaticLogo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_LOGOBAR_NOW),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_LOGOBAR_NOW));

	m_btnAbou.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnClickedStaticFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!bIsChecking && bDriverIsOK)
	{
		HsEnableNowButton();
		HsSelectWindow(HS_DIALOG_FILE);
	}
}





void CHeavenShadowDlg::OnEnableStaticFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_FILE),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_FILE));

	m_btnFile.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnDisableStaticFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_FILE_NOW),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_FILE_NOW));

	m_btnFile.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnClickedStaticSystem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!bIsChecking && bDriverIsOK)
	{
		HsEnableNowButton();
		HsSelectWindow(HS_DIALOG_SYSTEM);
	}
}


void CHeavenShadowDlg::OnEnableStaticSystem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_SYSTEM),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_SYSTEM));

	m_btnSys.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnDisableStaticSystem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_SYSTEM_NOW),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_SYSTEM_NOW));

	m_btnSys.SetBitmap(hBmp);
}



void CHeavenShadowDlg::OnClickedStaticSetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!bIsChecking && bDriverIsOK)
	{
		HsEnableNowButton();
		HsSelectWindow(HS_DIALOG_SETTING);
	}
}


void CHeavenShadowDlg::OnEnableStaticSetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_SETTING),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_SETTING));

	m_btnSet.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnDisableStaticSetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_SETTING_NOW),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_SETTING_NOW));

	m_btnSet.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnClickedStaticTools()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!bIsChecking && bDriverIsOK)
	{
		HsEnableNowButton();
		HsSelectWindow(HS_DIALOG_TOOLS);
	}
}


void CHeavenShadowDlg::OnEnableStaticTools()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_TOOLS),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_TOOLS));

	m_btnTool.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnDisableStaticTools()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_TOOLS_NOW),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_TOOLS_NOW));

	m_btnTool.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnClickedStaticKernel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!bIsChecking && bDriverIsOK)
	{
		HsEnableNowButton();
		HsSelectWindow(HS_DIALOG_KERNEL);
	}
}


void CHeavenShadowDlg::OnEnableStaticKernel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_KERNEL),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_KERNEL));

	m_btnKrnl.SetBitmap(hBmp);
}


void CHeavenShadowDlg::OnDisableStaticKernel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_KERNEL_NOW),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_KERNEL_NOW));

	m_btnKrnl.SetBitmap(hBmp);
}