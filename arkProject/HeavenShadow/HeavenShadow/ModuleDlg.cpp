// ModuleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeavenShadow.h"
#include "ModuleDlg.h"
#include "afxdialogex.h"

#include "HeavenShadowDlg.h"
#include "Common.h"

#include "ModuleFunc.h"

// CModuleDlg �Ի���

extern int ResizeX;
extern int ResizeY;

extern int dpix;
extern int dpiy;

//////////////////////////////////////////////////////////////////////////
//����

UINT module_sort_column;		//��¼�������
BOOL module_method = TRUE;		//��¼�ȽϷ���

//////////////////////////////////////////////////////////////////////////

extern CImageList m_TreeModuleImageList;

extern BOOL bIsChecking;

IMPLEMENT_DYNAMIC(CModuleDlg, CDialog)

CModuleDlg::CModuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModuleDlg::IDD, pParent)
{
	m_wParent = pParent;
}

CModuleDlg::~CModuleDlg()
{
}

void CModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODULELIST, m_ModuleList);
}


BEGIN_MESSAGE_MAP(CModuleDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_MODULELIST, &CModuleDlg::OnNMCustomdrawListModulelist)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_MODULELIST, &CModuleDlg::OnLvnColumnclickListModulelist)
	ON_COMMAND(ID_MENU_MODULE_REMOVEMODULE, &CModuleDlg::OnMenuModuleRemovemodule)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MODULELIST, &CModuleDlg::OnRclickListModulelist)
	ON_COMMAND(ID_MENU_MODULE_REFRESH, &CModuleDlg::OnMenuModuleRefresh)
END_MESSAGE_MAP()


// CModuleDlg ��Ϣ�������


void CModuleDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CRect   rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(255,255,255));
}


void CModuleDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������

	if (bShow == TRUE)
	{
		m_ModuleList.MoveWindow(0,0,ResizeX,ResizeY);

		((CHeavenShadowDlg*)m_wParent)->m_bNowWindow = HS_DIALOG_MODULE;

		((CHeavenShadowDlg*)m_wParent)->m_btnModu.EnableWindow(FALSE);

		HsLoadModuleList();

		HsSendStatusTip(L"����ģ��");

		m_ModuleList.SetFocus();
	}
}


BOOL CModuleDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN ||pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CModuleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	HsInitModuleList((CMyList*)&m_ModuleList);

	UINT uIconSize = 20;

	uIconSize *= (UINT)(dpix/96.0);

	m_TreeModuleImageList.Create(uIconSize, uIconSize, ILC_COLOR32 | ILC_MASK, 2, 2);

	ListView_SetImageList(m_ModuleList.m_hWnd, m_TreeModuleImageList.GetSafeHandle(), LVSIL_SMALL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CModuleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	ResizeX = cx;
	ResizeY = cy;
}


void CModuleDlg::HsLoadModuleList(void)
{
	if (bIsChecking == TRUE)
	{
		return;
	}

	bIsChecking = TRUE;

	m_ModuleList.DeleteAllItems();

	m_ModuleList.SetSelectedColumn(-1);

	HsSendStatusDetail(L"�����б����ڼ��ء�");

	CloseHandle(
		CreateThread(NULL,0, 
		(LPTHREAD_START_ROUTINE)HsQueryModuleFunction,&m_ModuleList, 0,NULL)
		);
}




void CModuleDlg::OnNMCustomdrawListModulelist(NMHDR *pNMHDR, LRESULT *pResult)
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

		bHooked = (int)m_ModuleList.GetItemData(nItem); 		
		if (bHooked == 1)
		{
			clrNewTextColor = RGB( 0, 0, 255 );
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;

		*pResult = CDRF_DODEFAULT;
	}
}


void CModuleDlg::OnLvnColumnclickListModulelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int nItemCount;

	nItemCount = m_ModuleList.GetItemCount();

	module_sort_column=pNMLV->iSubItem;//�������

	m_ModuleList.SetSelectedColumn(module_sort_column);

	for(int i=0;i<nItemCount;i++) m_ModuleList.SetItemData(i,i);//ÿ�еıȽϹؼ��֣��˴�Ϊ����ţ�������кţ�����������Ϊ���� �ȽϺ����ĵ�һ��������

	m_ModuleList.SortItems(HsModuleListCompareProc,(DWORD_PTR)&m_ModuleList);//���� �ڶ��������ǱȽϺ����ĵ���������

	if (module_method)
	{
		module_method = FALSE;
	}
	else
	{
		module_method = TRUE;
	}

	for (int i = 0; i < nItemCount; i++)
	{
		if (_wcsnicmp(
			m_ModuleList.GetItemText(i,8),
			L"Microsoft Corporation",
			wcslen(L"Microsoft Corporation"))==0
			)
		{
			m_ModuleList.SetItemData(i,1);
		}
	}

	*pResult = 0;
}




//////////////////////////////////////////////////////////////////////////

//����ȽϺ���
static int CALLBACK HsModuleListCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{

	//�Ӳ�������ȡ����Ƚ�lc����������

	int row1=(int) lParam1;
	int row2=(int) lParam2;

	CListCtrl*lc=(CListCtrl*)lParamSort;

	CString lp1=lc->GetItemText(row1,module_sort_column);
	CString lp2=lc->GetItemText(row2,module_sort_column);


	//�Ƚϣ��Բ�ͬ���У���ͬ�Ƚϣ�ע���¼ǰһ����������һ��Ҫ�෴����

	if (module_sort_column == 7)
	{
		// int�ͱȽ�
		if (module_method)
			return _ttoi(lp1)-_ttoi(lp2);
		else
			return _ttoi(lp2)-_ttoi(lp1);
	}
	else if (module_sort_column == 1 ||module_sort_column == 2 ||module_sort_column == 3 ||module_sort_column == 6)
	{
		ULONG_PTR nlp1 = 0, nlp2 = 0;

		if (lp1.GetLength()>1)
		{
			lp1 = lp1.GetBuffer()+2;
			swscanf_s(lp1.GetBuffer(),L"%P",&nlp1);
		}
		else
		{
			nlp1 = 0;
		}

		if (lp2.GetLength()>1)
		{
			lp2 = lp2.GetBuffer()+2;
			swscanf_s(lp2.GetBuffer(),L"%P",&nlp2);
		}
		else
		{
			nlp2 = 0;
		}

		if (module_method)
		{
			return (int)(nlp1 - nlp2);
		}
		else
		{
			return (int)(nlp2 - nlp1);
		}
	}
	else
	{
		// �����ͱȽ�
		if(module_method)
			return lp1.CompareNoCase(lp2);
		else
			return lp2.CompareNoCase(lp1);
	}

	return 0;
}


void CModuleDlg::OnMenuModuleRemovemodule()
{
	// TODO: �ڴ���������������

	BOOL bRet = MessageBox(L"ж��ģ����������ܻ����ϵͳ�쳣�������\r\n����ȷ�Ϻ������",L"��Ӱ��ʿ",MB_ICONWARNING | MB_OKCANCEL);

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
 		(LPTHREAD_START_ROUTINE)HsRemoveDriverModule,&m_ModuleList, 0,NULL)
 		);
	//HsRemoveDriverModule(&m_ListSystemCtrl);
}


void CModuleDlg::OnRclickListModulelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CMenu	popup;
	CPoint	p;

	popup.LoadMenu(IDR_MENU_MODULE);				//���ز˵���Դ
	CMenu*	pM = popup.GetSubMenu(0);				//��ò˵�������

	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();
	if (m_ModuleList.GetSelectedCount() == 0)		//���û��ѡ��
	{ 
		for (int i = 0;i<count;i++)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
		}
	}

	int Index = m_ModuleList.GetSelectionMark();

	if (Index>=0)
	{
		if (_wcsicmp(L"-",m_ModuleList.GetItemText(Index,3))==0)
		{
			pM->EnableMenuItem(ID_MENU_MODULE_REMOVEMODULE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
	}

	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

	*pResult = 0;
}


void CModuleDlg::OnMenuModuleRefresh()
{
	// TODO: �ڴ���������������

	HsLoadModuleList();
}
