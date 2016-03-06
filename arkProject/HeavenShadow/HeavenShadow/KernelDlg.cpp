// KernelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeavenShadow.h"
#include "KernelDlg.h"
#include "afxdialogex.h"

#include "SSDTFunc.h"
#include "SSSDTFunc.h"
#include "SysThread.h"

#include "KernelFunc.h"

#include "HeavenShadowDlg.h"

// CKernelDlg �Ի���


enum HS_KERNEL_LIST
{
	HS_KERNEL_SSDT,
	HS_KERNEL_SSSDT,
	HS_KERNEL_KRNLFUNC,
	HS_KERNEL_KRNLIAT,
	HS_KERNEL_KRNLEAT,
};

extern int dpix;
extern int dpiy;

extern BOOL bIsChecking;

BOOL bNowKernelSel = 255;


IMPLEMENT_DYNAMIC(CKernelDlg, CDialog)

CKernelDlg::CKernelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKernelDlg::IDD, pParent)
{
	m_wParent = pParent;
}

CKernelDlg::~CKernelDlg()
{
}

void CKernelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_KERNEL, m_ListKernel);
	DDX_Control(pDX, IDC_LIST_KERNEL_CTRL, m_ListKernelCtrl);
	DDX_Control(pDX, IDC_LIST_KRNLNAME, m_KernelNameList);
}


BEGIN_MESSAGE_MAP(CKernelDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_LIST_KERNEL, &CKernelDlg::OnSelchangeListKernel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_KERNEL_CTRL, &CKernelDlg::OnNMCustomdrawListKernelCtrl)
	ON_COMMAND(ID_MENU_SSDT_REFRESH, &CKernelDlg::OnMenuSsdtRefresh)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_KERNEL_CTRL, &CKernelDlg::OnNMRClickListKernelCtrl)
	ON_COMMAND(ID_MENU_SSDT_RESUME, &CKernelDlg::OnMenuSsdtResume)
	ON_LBN_SELCHANGE(IDC_LIST_KRNLNAME, &CKernelDlg::OnLbnSelchangeListKrnlname)
END_MESSAGE_MAP()


// CKernelDlg ��Ϣ�������


BOOL CKernelDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN ||pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CKernelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitKernelList();

	UINT uIconSize = 20;

	uIconSize *= (UINT)(dpix/96.0);

	m_TreeKernelImageList.Create(1, uIconSize, ILC_COLOR32 | ILC_MASK, 2, 2);

	ListView_SetImageList(m_ListKernelCtrl.m_hWnd, m_TreeKernelImageList.GetSafeHandle(), LVSIL_SMALL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CKernelDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
 
 	CRect   rect;
 	GetClientRect(rect);
 	dc.FillSolidRect(rect,RGB(255,255,255));

	CRect KernelListRect;
	CRect KernelNameRect;
	CRect KernelCtrlRect;

	//m_ListKernel.GetClientRect(KernelListRect);
	m_ListKernel.GetWindowRect(&KernelListRect);
	ClientToScreen(&rect);
	KernelListRect.left -= rect.left;
	KernelListRect.right -= rect.left;
	KernelListRect.top -= rect.top;
	KernelListRect.bottom -= rect.top;
	KernelListRect.bottom = rect.Height() - 2;
	//m_ListKernel.MoveWindow(&KernelListRect);

	CPoint startPoint;
	startPoint.x = (LONG)(KernelListRect.right)+2;
	startPoint.y = -1;
	CPoint endPoint;
	endPoint.x = (LONG)(KernelListRect.right)+2;
	endPoint.y = rect.Height()+2;

	KernelCtrlRect.left = startPoint.x+1;
	KernelCtrlRect.right = rect.Width();
	KernelCtrlRect.top = 0;
	KernelCtrlRect.bottom = rect.Height();
	m_ListKernelCtrl.MoveWindow(KernelCtrlRect);

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

	

	if (m_KernelNameList.IsWindowVisible())
	{
		KernelListRect.bottom = rect.Height()/2;

		startPoint.x = -1;
		startPoint.y = (LONG)(KernelListRect.bottom) + 2;
		endPoint.x = (LONG)(KernelListRect.right) + 2;
		endPoint.y = (LONG)(KernelListRect.bottom) + 2;

		m_KernelNameList.MoveWindow(
			KernelListRect.left,
			KernelListRect.bottom+5,
			KernelListRect.Width(),
			rect.Height()-KernelListRect.bottom-7
			);

		CPen pen2(PS_SOLID,1,m_Color);	//����һ����������󣬹���ʱ���û�������
		aDC.SelectObject(&pen2);
		aDC.MoveTo(startPoint);
		aDC.LineTo(endPoint);
	}

	m_ListKernel.MoveWindow(&KernelListRect);

}


void CKernelDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������

	if (bShow == TRUE)
	{
		((CHeavenShadowDlg*)m_wParent)->m_bNowWindow = HS_DIALOG_KERNEL;

		((CHeavenShadowDlg*)m_wParent)->m_btnKrnl.EnableWindow(FALSE);

		HsSendStatusDetail(L"����ϵͳ�ں���Ϣ��");
		HsSendStatusTip(L"�ں�");

		m_ListKernel.SetCurSel(HS_KERNEL_SSDT);
		
		bNowKernelSel = 255;

		OnSelchangeListKernel();

		m_ListKernelCtrl.SetFocus();

	}
}


void CKernelDlg::InitKernelList(void)
{
	m_ListKernel.AddString(L"SSDT");
	m_ListKernel.InsertString(HS_KERNEL_SSSDT,L"ShadowSSDT");
	m_ListKernel.InsertString(HS_KERNEL_KRNLFUNC,L"�ں˺���");
 	m_ListKernel.InsertString(HS_KERNEL_KRNLIAT,L"�ں˵����");
 	m_ListKernel.InsertString(HS_KERNEL_KRNLEAT,L"�ں˵�����");

	m_ListKernel.SetItemHeight(-1,(UINT)(16*(dpiy/96.0)));
	m_KernelNameList.SetItemHeight(-1,(UINT)(16*(dpiy/96.0)));
}


void CKernelDlg::OnSelchangeListKernel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int nCurSel = m_ListKernel.GetCurSel();

	switch(nCurSel)
	{
	case HS_KERNEL_SSDT:
		{
			if (bIsChecking == TRUE || bNowKernelSel == HS_KERNEL_SSDT)	//
			{
				m_ListKernel.SetCurSel(bNowKernelSel);
				m_ListKernelCtrl.SetFocus();
				return;
			}

			bNowKernelSel = nCurSel;
			bIsChecking = TRUE;

			m_KernelNameList.ShowWindow(FALSE);

			HsInitSSDTList(&m_ListKernelCtrl);

			CloseHandle(
				CreateThread(NULL,0, 
				(LPTHREAD_START_ROUTINE)HsLoadSSDTList,&m_ListKernelCtrl, 0,NULL)
				);
			//HsLoadSSDTList(&m_ListKernelCtrl);
			break;
		}
	case HS_KERNEL_SSSDT:
		{
			if (bIsChecking == TRUE || bNowKernelSel == HS_KERNEL_SSSDT)	//
			{
				m_ListKernel.SetCurSel(bNowKernelSel);
				m_ListKernelCtrl.SetFocus();
				return;
			}

			bNowKernelSel = nCurSel;
			bIsChecking = TRUE;

			m_KernelNameList.ShowWindow(FALSE);

			HsInitSSSDTList(&m_ListKernelCtrl);

			CloseHandle(
				CreateThread(NULL,0, 
				(LPTHREAD_START_ROUTINE)HsLoadSSSDTList,&m_ListKernelCtrl, 0,NULL)
				);
			//HsLoadSSSDTList(&m_ListKernelCtrl);
			break;
		}

	case HS_KERNEL_KRNLFUNC:
		{
			if (bIsChecking == TRUE || bNowKernelSel == HS_KERNEL_KRNLFUNC)	//
			{
				m_ListKernel.SetCurSel(bNowKernelSel);
				m_ListKernelCtrl.SetFocus();
				Sleep(50);
				return;
			}

			bNowKernelSel = nCurSel;

			m_KernelNameList.ShowWindow(TRUE);

			HsSendStatusDetail(L"�ں˺������ڼ���...");

			HsInitKernelFuncList(&m_KernelNameList, &m_ListKernelCtrl);

			m_KernelNameList.SetCurSel(0);

			OnLbnSelchangeListKrnlname();

			break;
		}

	case HS_KERNEL_KRNLIAT:
		{
			if (bIsChecking == TRUE || bNowKernelSel == HS_KERNEL_KRNLIAT)	//
			{
				m_ListKernel.SetCurSel(bNowKernelSel);
				m_ListKernelCtrl.SetFocus();
				Sleep(50);
				return;
			}

			bNowKernelSel = nCurSel;

			m_KernelNameList.ShowWindow(TRUE);

			HsSendStatusDetail(L"�ں˵�������ڼ���...");

			HsInitKernelFileList(&m_KernelNameList, &m_ListKernelCtrl);

// 			m_KernelNameList.SetCurSel(0);
// 
// 			OnLbnSelchangeListKrnlname();

			break;
		}
	case HS_KERNEL_KRNLEAT:
		{
			if (bIsChecking == TRUE || bNowKernelSel == HS_KERNEL_KRNLEAT)	//
			{
				m_ListKernel.SetCurSel(bNowKernelSel);
				m_ListKernelCtrl.SetFocus();
				Sleep(50);
				return;
			}

			bNowKernelSel = nCurSel;

			m_KernelNameList.ShowWindow(TRUE);

			HsSendStatusDetail(L"�ں˵�������ڼ���...");

			HsInitKernelFileList(&m_KernelNameList, &m_ListKernelCtrl);

// 			m_KernelNameList.SetCurSel(0);
// 
// 			OnLbnSelchangeListKrnlname();

			break;
		}
	default:
		{

		}
	}

	m_ListKernelCtrl.SetFocus();
}


void CKernelDlg::OnNMCustomdrawListKernelCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = CDRF_DODEFAULT;

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF clrNewTextColor, clrNewBkColor;
		int bHooked = 0;
		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );

		clrNewTextColor = RGB( 0, 0, 0 );
		clrNewBkColor = RGB( 255, 255, 255 );

		bHooked = (int)m_ListKernelCtrl.GetItemData(nItem); 		
		if (bHooked == 1)
		{
			clrNewTextColor = RGB( 255, 0, 0 );
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;

		*pResult = CDRF_DODEFAULT;
	}

}




void CKernelDlg::OnNMRClickListKernelCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CMenu	popup;
	CPoint	p;

	switch(bNowKernelSel)
	{
	case HS_KERNEL_SSDT:
		{
			popup.LoadMenu(IDR_MENU_KERNEL_SSDT);			//���ز˵���Դ
			CMenu*	pM = popup.GetSubMenu(0);				//��ò˵�������

			GetCursorPos(&p);
			int	count = pM->GetMenuItemCount();
			if (m_ListKernelCtrl.GetSelectedCount() == 0)		//���û��ѡ��
			{ 
				for (int i = 0;i<count;i++)
				{
					pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
				}
			}

			POSITION pos = m_ListKernelCtrl.GetFirstSelectedItemPosition();

			while (pos)
			{
				int nItem = m_ListKernelCtrl.GetNextSelectedItem(pos);

				if (_wcsnicmp(L"����",m_ListKernelCtrl.GetItemText(nItem,5),wcslen(L"����"))==0)
				{
					pM->EnableMenuItem(ID_MENU_SSDT_RESUME, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
				}
			}

			pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
			break;
		}
	case HS_KERNEL_SSSDT:
		{
			popup.LoadMenu(IDR_MENU_KERNEL_SSDT);			//���ز˵���Դ
			CMenu*	pM = popup.GetSubMenu(0);				//��ò˵�������

			GetCursorPos(&p);
			int	count = pM->GetMenuItemCount();
			if (m_ListKernelCtrl.GetSelectedCount() == 0)		//���û��ѡ��
			{ 
				for (int i = 0;i<count;i++)
				{
					pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
				}

			}

			POSITION pos = m_ListKernelCtrl.GetFirstSelectedItemPosition();

			while (pos)
			{
				int nItem = m_ListKernelCtrl.GetNextSelectedItem(pos);

				if (_wcsnicmp(L"����",m_ListKernelCtrl.GetItemText(nItem,5),wcslen(L"����"))==0)
				{
					pM->EnableMenuItem(ID_MENU_SSDT_RESUME, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
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



//////////////////////////////////////////////////////////////////////////


void CKernelDlg::OnMenuSsdtRefresh()
{
	// TODO: �ڴ���������������

	switch(bNowKernelSel)
	{
	case HS_KERNEL_SSDT:
		{
			bNowKernelSel = 255;

			m_ListKernel.SetCurSel(HS_KERNEL_SSDT);

			OnSelchangeListKernel();
			break;
		}
	case HS_KERNEL_SSSDT:
		{
			bNowKernelSel = 255;

			m_ListKernel.SetCurSel(HS_KERNEL_SSSDT);

			OnSelchangeListKernel();
			break;
		}
	}


}

void CKernelDlg::OnMenuSsdtResume()
{
	// TODO: �ڴ���������������

	switch(bNowKernelSel)
	{
	case HS_KERNEL_SSDT:
		{
			HsResumeSSDTHook(&m_ListKernelCtrl);
			break;
		}
	case HS_KERNEL_SSSDT:
		{
			break;
		}
	}
}


void CKernelDlg::OnLbnSelchangeListKrnlname()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	switch(bNowKernelSel)
	{
	case HS_KERNEL_KRNLFUNC:
		{
			SelchangeListKrnlFunc(&m_KernelNameList, &m_ListKernelCtrl);
			break;
		}
	case HS_KERNEL_KRNLIAT:
		{
			SelchangeListKrnlIAT(&m_KernelNameList, &m_ListKernelCtrl);
			break;
		}
	case HS_KERNEL_KRNLEAT:
		{
			SelchangeListKrnlEAT(&m_KernelNameList, &m_ListKernelCtrl);
			break;
		}
	default:
		{

		}
	}

	m_ListKernelCtrl.SetFocus();
}
