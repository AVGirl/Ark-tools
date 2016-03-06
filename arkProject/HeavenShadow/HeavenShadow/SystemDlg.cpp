// SystemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeavenShadow.h"
#include "SystemDlg.h"
#include "afxdialogex.h"

#include "SysThread.h"
#include "IoTimerFunc.h"
#include "CallbackFunc.h"
#include "DpcTimerFunc.h"
#include "FilterDriverFunc.h"

#include "HeavenShadowDlg.h"

enum HS_SYSK_LIST
{
	HS_SYSK_CALLBACK,
	HS_SYSK_IOTIMER,
	HS_SYSK_DPCTIMER,
	HS_SYSK_FILTERDRIVER,
	HS_SYSK_SYSTHREAD,
};

extern int dpix;
extern int dpiy;

extern BOOL bIsChecking;

BOOL bNowSystemSel = 255;


// CSystemDlg �Ի���

IMPLEMENT_DYNAMIC(CSystemDlg, CDialog)

CSystemDlg::CSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemDlg::IDD, pParent)
{
	m_wParent = pParent;
}

CSystemDlg::~CSystemDlg()
{
}

void CSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SYSTEM, m_ListSystem);
	DDX_Control(pDX, IDC_LIST_SYSTEM_CTRL, m_ListSystemCtrl);
}


BEGIN_MESSAGE_MAP(CSystemDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_LIST_SYSTEM, &CSystemDlg::OnSelchangeListSystem)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SYSTEM_CTRL, &CSystemDlg::OnRclickListSystemCtrl)
	ON_COMMAND(ID_MENU_SYSK_IOTIMER_REFRESH, &CSystemDlg::OnMenuSyskIoTimerRefresh)
	ON_COMMAND(ID_MENU_SYSK_IOTIMER_OPER, &CSystemDlg::OnMenuSyskOperiotimer)
	ON_COMMAND(ID_MENU_SYSK_CALLBACK_REFRESH, &CSystemDlg::OnMenuSyskCallbackRefresh)
	ON_COMMAND(ID_MENU_SYSK_CALLBACK_REMOVE, &CSystemDlg::OnMenuSyskCallbackRemove)
	ON_COMMAND(ID_MENU_SYSK_IOTIMER_REMOVE, &CSystemDlg::OnMenuSyskRemoveiotimer)
	ON_COMMAND(ID_MENU_SYSK_DPCTIMER_REFRESH, &CSystemDlg::OnMenuSyskDpctimerRefresh)
	ON_COMMAND(ID_MENU_SYSK_DPCTIMER_REMOVE, &CSystemDlg::OnMenuSyskDpctimerRemove)
	ON_COMMAND(ID_MENU_SYSK_FILTER_REFRESH, &CSystemDlg::OnMenuSyskFilterRefresh)
	ON_COMMAND(ID_MENU_SYSK_FILTER_REMOVEFILTER, &CSystemDlg::OnMenuSyskFilterRemovefilter)
END_MESSAGE_MAP()


// CSystemDlg ��Ϣ�������


BOOL CSystemDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN ||pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CSystemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitSystemList();

	UINT uIconSize = 20;

	uIconSize *= (UINT)(dpix/96.0);

	m_TreeSystemImageList.Create(1, uIconSize, ILC_COLOR32 | ILC_MASK, 2, 2);

	ListView_SetImageList(m_ListSystemCtrl.m_hWnd, m_TreeSystemImageList.GetSafeHandle(), LVSIL_SMALL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSystemDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������

	if (bShow == TRUE)
	{
		((CHeavenShadowDlg*)m_wParent)->m_bNowWindow = HS_DIALOG_SYSTEM;

		((CHeavenShadowDlg*)m_wParent)->m_btnSys.EnableWindow(FALSE);

		HsSendStatusDetail(L"��ʾ����ϵͳ�ں������Ϣ��");
		HsSendStatusTip(L"�ں�");

		m_ListSystem.SetCurSel(0);

		bNowSystemSel = 255;

		OnSelchangeListSystem();

		m_ListSystemCtrl.SetFocus();
	}
}


void CSystemDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()


	CRect   rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(255,255,255));

	CRect SystemListRect;
	CRect SystemCtrlRect;

	//m_ListKernel.GetClientRect(KernelListRect);
	m_ListSystem.GetWindowRect(&SystemListRect);
	ClientToScreen(&rect);
	SystemListRect.left -= rect.left;
	SystemListRect.right -= rect.left;
	SystemListRect.top -= rect.top;
	SystemListRect.bottom -= rect.top;
	SystemListRect.bottom = rect.Height() - 2;
	//m_ListKernel.MoveWindow(&KernelListRect);

	CPoint startPoint;
	startPoint.x = (LONG)(SystemListRect.right)+2;
	startPoint.y = -1;
	CPoint endPoint;
	endPoint.x = (LONG)(SystemListRect.right)+2;
	endPoint.y = rect.Height()+2;

	SystemCtrlRect.left = startPoint.x+1;
	SystemCtrlRect.right = rect.Width();
	SystemCtrlRect.top = 0;
	SystemCtrlRect.bottom = rect.Height();
	m_ListSystemCtrl.MoveWindow(SystemCtrlRect);

	// 	CPoint startPoint;
	// 	startPoint.x = (UINT)(98*(dpix/96.0))+2;
	// 	startPoint.y = -1;
	// 	CPoint endPoint;
	// 	endPoint.x = (UINT)(98*(dpix/96.0))+2;
	// 	endPoint.y = rect.Height()+2;


	COLORREF m_Color(RGB(190,190,190));

	CClientDC aDC(this);			//CClientDC�Ĺ��캯����Ҫһ�����������������ָ���ͼ���ڵ�ָ�룬������thisָ��Ϳ�����
	CPen pen(PS_SOLID,1,m_Color);	//����һ����������󣬹���ʱ���û�������
	aDC.SelectObject(&pen);
	aDC.MoveTo(startPoint);
	aDC.LineTo(endPoint);
}




void CSystemDlg::InitSystemList(void)
{
	m_ListSystem.AddString(L"ϵͳ�ص�");
	m_ListSystem.InsertString(HS_SYSK_IOTIMER,L"IOPTimer");
	m_ListSystem.InsertString(HS_SYSK_DPCTIMER,L"DPCTimer");
	m_ListSystem.InsertString(HS_SYSK_FILTERDRIVER,L"��������");
	m_ListSystem.InsertString(HS_SYSK_SYSTHREAD,L"�ں��߳�");
	
	m_ListSystem.SetItemHeight(-1,(UINT)(16*(dpiy/96.0)));
}

void CSystemDlg::OnSelchangeListSystem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int nCurSel = m_ListSystem.GetCurSel();

	switch(nCurSel)
	{
	case HS_SYSK_CALLBACK:
		{
			if (bIsChecking == TRUE || bNowSystemSel == HS_SYSK_CALLBACK)	//
			{
				m_ListSystem.SetCurSel(bNowSystemSel);
				m_ListSystemCtrl.SetFocus();
				return;
			}

			bNowSystemSel = nCurSel;

			HsInitCallBackList(&m_ListSystemCtrl);

			CloseHandle(
				CreateThread(NULL,0, 
				(LPTHREAD_START_ROUTINE)HsLoadCallBackList,&m_ListSystemCtrl, 0,NULL)
				);
			//HsLoadCallBackList(&m_ListSystemCtrl);

			break;
		}
	case HS_SYSK_IOTIMER:
		{
			if (bIsChecking == TRUE || bNowSystemSel == HS_SYSK_IOTIMER)	//
			{
				m_ListSystem.SetCurSel(bNowSystemSel);
				m_ListSystemCtrl.SetFocus();
				return;
			}

			bNowSystemSel = nCurSel;

			HsInitIOTimerList(&m_ListSystemCtrl);

			CloseHandle(
				CreateThread(NULL,0, 
				(LPTHREAD_START_ROUTINE)HsLoadIOTimerList,&m_ListSystemCtrl, 0,NULL)
				);
			//HsLoadIOTimerList(&m_ListSystemCtrl);

			break;
		}
	case HS_SYSK_DPCTIMER:
		{
			if (bIsChecking == TRUE || bNowSystemSel == HS_SYSK_DPCTIMER)	//
			{
				m_ListSystem.SetCurSel(bNowSystemSel);
				m_ListSystemCtrl.SetFocus();
				return;
			}

			bNowSystemSel = nCurSel;

			HsInitDPCTimerList(&m_ListSystemCtrl);

			CloseHandle(
				CreateThread(NULL,0, 
				(LPTHREAD_START_ROUTINE)HsLoadDPCTimerList,&m_ListSystemCtrl, 0,NULL)
				);
			//HsLoadDPCTimerList(&m_ListSystemCtrl);

			break;
		}
	case HS_SYSK_FILTERDRIVER:
		{
			if (bIsChecking == TRUE || bNowSystemSel == HS_SYSK_FILTERDRIVER)	//
			{
				m_ListSystem.SetCurSel(bNowSystemSel);
				m_ListSystemCtrl.SetFocus();
				return;
			}

			bNowSystemSel = nCurSel;

			HsInitFilterDriverList(&m_ListSystemCtrl);

			CloseHandle(
				CreateThread(NULL,0, 
				(LPTHREAD_START_ROUTINE)HsLoadFilterDriverList,&m_ListSystemCtrl, 0,NULL)
				);
			//HsLoadFilterDriverList(&m_ListSystemCtrl);

			break;
		}
	case HS_SYSK_SYSTHREAD:
		{
			if (bIsChecking == TRUE || bNowSystemSel == HS_SYSK_SYSTHREAD)	//
			{
				m_ListSystem.SetCurSel(bNowSystemSel);
				m_ListSystemCtrl.SetFocus();
				return;
			}

			bNowSystemSel = nCurSel;

			HsInitSysThreadList(&m_ListSystemCtrl);

			CloseHandle(
				CreateThread(NULL,0, 
				(LPTHREAD_START_ROUTINE)HsLoadSysThreadList,&m_ListSystemCtrl, 0,NULL)
				);
			//HsLoadSysThreadList(&m_ListSystemCtrl);

			break;
		}
	default:
		{

		}
	}

	m_ListSystemCtrl.SetFocus();
}


void CSystemDlg::OnRclickListSystemCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CMenu	popup;
	CPoint	p;

	switch(bNowSystemSel)
	{
	case HS_SYSK_IOTIMER:
		{
			popup.LoadMenu(IDR_MENU_SYSK_IOTIMER);			//���ز˵���Դ
			CMenu*	pM = popup.GetSubMenu(0);				//��ò˵�������

			GetCursorPos(&p);
			int	count = pM->GetMenuItemCount();
			if (m_ListSystemCtrl.GetSelectedCount() == 0)		//���û��ѡ��
			{ 
				for (int i = 1;i<count;i++)
				{
					pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
				}
			}

			POSITION pos = m_ListSystemCtrl.GetFirstSelectedItemPosition();

			while (pos)
			{
				int nItem = m_ListSystemCtrl.GetNextSelectedItem(pos);

				if (_wcsnicmp(L"����",m_ListSystemCtrl.GetItemText(nItem,2),wcslen(L"����"))==0)
				{
					pM->ModifyMenuW(ID_MENU_SYSK_IOTIMER_OPER,MF_BYCOMMAND,ID_MENU_SYSK_IOTIMER_OPER,L"ֹͣ(&S)");
				}
				else if (_wcsnicmp(L"ֹͣ",m_ListSystemCtrl.GetItemText(nItem,2),wcslen(L"ֹͣ"))==0)
				{
					pM->ModifyMenuW(ID_MENU_SYSK_IOTIMER_OPER,MF_BYCOMMAND,ID_MENU_SYSK_IOTIMER_OPER,L"����(&S)");
				}
			}

			pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
			break;
		}
	case HS_SYSK_CALLBACK:
		{
			popup.LoadMenu(IDR_MENU_SYSK_CALLBACK);			//���ز˵���Դ
			CMenu*	pM = popup.GetSubMenu(0);				//��ò˵�������

			GetCursorPos(&p);
			int	count = pM->GetMenuItemCount();
			if (m_ListSystemCtrl.GetSelectedCount() == 0)		//���û��ѡ��
			{ 
				for (int i = 1;i<count;i++)
				{
					pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
				}
			}

			pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
			break;
		}
	case HS_SYSK_DPCTIMER:
		{
			popup.LoadMenu(IDR_MENU_SYSK_DPCTIMER);			//���ز˵���Դ
			CMenu*	pM = popup.GetSubMenu(0);				//��ò˵�������

			GetCursorPos(&p);
			int	count = pM->GetMenuItemCount();
			if (m_ListSystemCtrl.GetSelectedCount() == 0)		//���û��ѡ��
			{ 
				for (int i = 1;i<count;i++)
				{
					pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
				}
			}

			pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
			break;
		}
	case HS_SYSK_FILTERDRIVER:
		{
			popup.LoadMenu(IDR_MENU_SYSK_FILTERDRIVER);		//���ز˵���Դ
			CMenu*	pM = popup.GetSubMenu(0);				//��ò˵�������

			GetCursorPos(&p);
			int	count = pM->GetMenuItemCount();
			if (m_ListSystemCtrl.GetSelectedCount() == 0)		//���û��ѡ��
			{ 
				for (int i = 1;i<count;i++)
				{
					pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
				}
			}

			pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
			break;
		}
	default:
		{

		}
	}


	*pResult = 0;
}


void CSystemDlg::OnMenuSyskIoTimerRefresh()
{
	// TODO: �ڴ���������������
	m_ListSystem.SetCurSel(HS_SYSK_IOTIMER);
	bNowSystemSel = 255;
	OnSelchangeListSystem();
}


void CSystemDlg::OnMenuSyskOperiotimer()
{
	// TODO: �ڴ���������������
	if (bIsChecking)
	{
		return;
	}

	bIsChecking = TRUE;

	CloseHandle(
		CreateThread(NULL,0, 
		(LPTHREAD_START_ROUTINE)HsOperIOTimer,&m_ListSystemCtrl, 0,NULL)
		);
	//HsOperIOTimer(&m_ListSystemCtrl);
}


void CSystemDlg::OnMenuSyskCallbackRefresh()
{
	// TODO: �ڴ���������������
	m_ListSystem.SetCurSel(HS_SYSK_CALLBACK);
	bNowSystemSel = 255;
	OnSelchangeListSystem();
}


void CSystemDlg::OnMenuSyskCallbackRemove()
{
	// TODO: �ڴ���������������
	if (bIsChecking)
	{
		return;
	}

	bIsChecking = TRUE;

	CloseHandle(
		CreateThread(NULL,0, 
		(LPTHREAD_START_ROUTINE)HsRemoveCallBackItem,&m_ListSystemCtrl, 0,NULL)
		);
	//HsRemoveCallBackItem(&m_ListSystemCtrl);
}


void CSystemDlg::OnMenuSyskRemoveiotimer()
{
	// TODO: �ڴ���������������

	BOOL bRet = MessageBox(L"ɾ��IOTimer�������ܻ����ϵͳ�쳣�������\r\n����ȷ�Ϻ������",L"��Ӱ��ʿ",MB_ICONWARNING | MB_OKCANCEL);

	if (bRet == IDCANCEL)
	{
		return;
	}

	if (bIsChecking == TRUE)
	{
		return;
	}

	bIsChecking = TRUE;

	CloseHandle(
		CreateThread(NULL,0, 
		(LPTHREAD_START_ROUTINE)HsRemoveIOTimerItem,&m_ListSystemCtrl, 0,NULL)
		);
	//HsRemoveIOTimerItem(&m_ListSystemCtrl);
}


void CSystemDlg::OnMenuSyskDpctimerRefresh()
{
	// TODO: �ڴ���������������
	m_ListSystem.SetCurSel(HS_SYSK_DPCTIMER);
	bNowSystemSel = 255;
	OnSelchangeListSystem();
}


void CSystemDlg::OnMenuSyskDpctimerRemove()
{
	// TODO: �ڴ���������������
	BOOL bRet = MessageBox(L"ɾ��DPCTimer�������ܻ����ϵͳ�쳣�������\r\n����ȷ�Ϻ������",L"��Ӱ��ʿ",MB_ICONWARNING | MB_OKCANCEL);

	if (bRet == IDCANCEL)
	{
		return;
	}

	if (bIsChecking == TRUE)
	{
		return;
	}

	bIsChecking = TRUE;

	CloseHandle(
		CreateThread(NULL,0, 
		(LPTHREAD_START_ROUTINE)HsRemoveDPCTimerItem,&m_ListSystemCtrl, 0,NULL)
		);
	//HsRemoveDPCTimerItem(&m_ListSystemCtrl);
}


void CSystemDlg::OnMenuSyskFilterRefresh()
{
	// TODO: �ڴ���������������
	m_ListSystem.SetCurSel(HS_SYSK_FILTERDRIVER);
	bNowSystemSel = 255;
	OnSelchangeListSystem();
}


void CSystemDlg::OnMenuSyskFilterRemovefilter()
{
	// TODO: �ڴ���������������
	BOOL bRet = MessageBox(L"ж�ع��������������ܻ����ϵͳ�쳣�������\r\n����ȷ�Ϻ������",L"��Ӱ��ʿ",MB_ICONWARNING | MB_OKCANCEL);

	if (bRet == IDCANCEL)
	{
		return;
	}

	if (bIsChecking == TRUE)
	{
		return;
	}

	bIsChecking = TRUE;

	CloseHandle(
		CreateThread(NULL,0, 
		(LPTHREAD_START_ROUTINE)HsRemoveFilterDriverItem,&m_ListSystemCtrl, 0,NULL)
		);
	//HsRemoveFilterDriverItem(&m_ListSystemCtrl);
}
