// FileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeavenShadow.h"
#include "FileDlg.h"
#include "afxdialogex.h"
#include "HeavenShadowDlg.h"
#include "Common.h"

#include "FileFunc.h"


HICON m_hIcon;
CImageList m_pImageList_Large;
CImageList m_pImageList_Small;

WCHAR  DriveString[MAX_PATH] = {0};

// CFileDlg �Ի���

IMPLEMENT_DYNAMIC(CFileDlg, CDialog)

CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileDlg::IDD, pParent)
{
	HRESULT hresut=CoInitialize(NULL);

	SHFILEINFO	sfi;
	SHGetFileInfo
		(
		L"\\\\",                   //��㴫�������ֽ�
		FILE_ATTRIBUTE_NORMAL, 
		&sfi,
		sizeof(SHFILEINFO), 
		SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_PIDL
		);
	m_hIcon = sfi.hIcon;

	WCHAR Temp[100] = {0};

	HIMAGELIST hImageList;
	// ����ϵͳͼ���б�
	hImageList = (HIMAGELIST)SHGetFileInfoW
		(
		Temp,
		FILE_ATTRIBUTE_NORMAL,
		&sfi,
		sizeof(SHFILEINFO),
		SHGFI_LARGEICON | SHGFI_SYSICONINDEX | SHGFI_PIDL
		);
	m_pImageList_Large.Attach(hImageList);

	// ����ϵͳͼ���б�
	hImageList = (HIMAGELIST)SHGetFileInfoW
		(
		Temp,
		FILE_ATTRIBUTE_NORMAL,
		&sfi,
		sizeof(SHFILEINFO),
		SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_PIDL
		);
	m_pImageList_Small.Attach(hImageList);

	m_wParent = pParent;
}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILELIST, m_fileList);
	DDX_Control(pDX, IDC_COMBOBOX_FILEPATH, m_filePath);
	DDX_Control(pDX, IDC_TREE_FILEDIRECTORY, m_dirTree);
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_RCLICK, IDC_TREE_FILEDIRECTORY, &CFileDlg::OnRclickTreeFiledirectory)
END_MESSAGE_MAP()


// CFileDlg ��Ϣ�������


void CFileDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
 
 	CRect   rect;
 	GetClientRect(rect);
// 	dc.FillSolidRect(rect,RGB(255,255,255));

	CClientDC aDC(this); //CClientDC�Ĺ��캯����Ҫһ�����������������ָ���ͼ���ڵ�ָ�룬������thisָ��Ϳ�����

	CPoint startPoint;
	CPoint endPoint;

	startPoint.x = -1;
	startPoint.y = 38;
	endPoint.x = rect.Width()+1;
	endPoint.y = 38;

	COLORREF m_Color(RGB(160,160,255));

	CPen pen(PS_SOLID,1,m_Color); ////����һ����������󣬹���ʱ���û�������
	aDC.SelectObject(&pen);
	aDC.MoveTo(startPoint);
	aDC.LineTo(endPoint);

	startPoint.x = -1;
	startPoint.y = 40;
	endPoint.x = rect.Width()+1;
	endPoint.y = 40;

	COLORREF m_Color3(RGB(255,255,255));

	CPen pen3(PS_SOLID,2,m_Color3); ////����һ����������󣬹���ʱ���û�������
	aDC.SelectObject(&pen3);
	aDC.MoveTo(startPoint);
	aDC.LineTo(endPoint);



	startPoint.x = 218;
	startPoint.y = 38;
	endPoint.x = 218;
	endPoint.y = rect.Height()+2;


	COLORREF m_Color2(RGB(190,190,190));

	CPen pen2(PS_SOLID,1,m_Color2);
	aDC.SelectObject(&pen2);
	aDC.MoveTo(startPoint);
	aDC.LineTo(endPoint);
}


BOOL CFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//////////////////////////////////////////////////////////////////////////
	//��������
	if (!m_wndToolBar.Create(this, WS_CHILD |
		WS_VISIBLE | CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY, IDR_TOOLBAR_FILETOOLBAR) 
		||!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_FILETOOLBAR))
	{

		return -1;
	}
	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT);    //Fix for WinXP
	m_wndToolBar.LoadTrueColorToolBar
		(
		24,    //������ʹ����� 
		IDB_BITMAP_FILETOOLBAR,
		IDB_BITMAP_FILETOOLBAR,
		IDB_BITMAP_FILETOOLBAR
		);
	// ���������ť
	m_wndToolBar.AddDropDownButton(this, ID_BUTTON_FILE_VIEW, IDR_MENU_FILE_VIEW);

	m_wndToolBar.MoveWindow(0, 0, 300, 36);	//640
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//��������
	if (!m_wndToolBar_goto.Create(this, WS_CHILD |
		WS_VISIBLE | CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY, IDR_TOOLBAR_FILETOOLBARGOTO) 
		||!m_wndToolBar_goto.LoadToolBar(IDR_TOOLBAR_FILETOOLBARGOTO))
	{

		return -1;
	}
	m_wndToolBar_goto.ModifyStyle(0, TBSTYLE_FLAT);    //Fix for WinXP
	m_wndToolBar_goto.LoadTrueColorToolBar
		(
		24,    //������ʹ����� 
		IDB_BITMAP_FILETOOLBARGOTO,
		IDB_BITMAP_FILETOOLBARGOTO,
		IDB_BITMAP_FILETOOLBARGOTO
		);

	m_wndToolBar_goto.MoveWindow(829, 0, 300, 36);	//640
	//////////////////////////////////////////////////////////////////////////




	m_filePath.SendMessage(CB_SETITEMHEIGHT,-1,18); ///����Combobox�߶�



	

 	// Ϊ�б���ͼ����ImageList
 	m_fileList.SetImageList(&m_pImageList_Large, LVSIL_NORMAL);
 	m_fileList.SetImageList(&m_pImageList_Small, LVSIL_SMALL);



	m_dirTree.SetImageList(&m_pImageList_Small, LVSIL_NORMAL);


	CoInitialize(NULL);


	


	
	CloseHandle(CreateThread(NULL, 0, 
		(LPTHREAD_START_ROUTINE)HsLoadFileTreeList,this, 0, NULL));


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFileDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	if (bShow == TRUE)
	{
		((CHeavenShadowDlg*)m_wParent)->m_bNowWindow = HS_DIALOG_FILE;

		((CHeavenShadowDlg*)m_wParent)->m_btnFile.EnableWindow(FALSE);

		HsSendStatusDetail(L"������Ĵ����ļ���");
		HsSendStatusTip(L"�ļ�");

	}
}


BOOL CFileDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN ||pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


// ��ʼ���ļ������б�
void HsLoadFileTreeList(CFileDlg *cFleDlg)
{

	WCHAR* pDrive = NULL;

	HTREEITEM hRoot;     // ���ĸ��ڵ�ľ��   
	HTREEITEM hDriverItem; // �ɱ�ʾ��һ���̽ڵ�ľ��   
	HTREEITEM hFolderItem;  // �ɱ�ʾ��һĿ¼�ڵ�ľ��

	HIMAGELIST hImageListLarge = NULL;
	HIMAGELIST hImageListSmall = NULL;
	Shell_GetImageLists(&hImageListLarge, &hImageListSmall);
	ListView_SetImageList(cFleDlg->m_fileList.m_hWnd, hImageListLarge, LVSIL_NORMAL);
	ListView_SetImageList(cFleDlg->m_fileList.m_hWnd, hImageListSmall, LVSIL_SMALL);


	HsInitDriveList(&cFleDlg->m_fileList);


	cFleDlg->m_fileList.DeleteAllItems();
	cFleDlg->m_dirTree.DeleteAllItems();


	LPITEMIDLIST ppidl = {0};
	SHGetSpecialFolderLocation(NULL,CSIDL_DRIVES,&ppidl);
	//SHGetPathFromIDList(ppidl,TempPath);

	SHFILEINFO	sfi;

	SHGetFileInfoW
		(
		(LPCWSTR)ppidl,
		0, 
		&sfi,
		sizeof(SHFILEINFO), 
		SHGFI_SYSICONINDEX | SHGFI_PIDL 
		);

	hRoot = cFleDlg->m_dirTree.InsertItem(_T("�����"),sfi.iIcon,sfi.iIcon,0,0);

	GetLogicalDriveStrings(sizeof(DriveString), DriveString);


	pDrive = DriveString;

	CString DriveName;

	WCHAR	FileSystem[MAX_PATH];
	unsigned __int64	HDAmount = 0;
	unsigned __int64	HDFreeSpace = 0;
	unsigned long		AmntMB = 0; // �ܴ�С
	unsigned long		FreeMB = 0; // ʣ��ռ�


	cFleDlg->m_dirTree.Expand(hRoot,TVE_EXPAND);

	for (int i = 0; *pDrive != '\0'; i++, pDrive += lstrlen(pDrive) + 1)
	{
		

		// �õ����������Ϣ
		memset(FileSystem, 0, sizeof(FileSystem));
		// �õ��ļ�ϵͳ��Ϣ����С
		GetVolumeInformation(pDrive, NULL, 0, NULL, NULL, NULL, FileSystem, MAX_PATH);

		int	nFileSystemLen = lstrlen(FileSystem) + 1;
		if (GetDiskFreeSpaceEx(pDrive, (PULARGE_INTEGER)&HDFreeSpace, (PULARGE_INTEGER)&HDAmount, NULL))
		{	
			AmntMB = (ULONG)(HDAmount / 1024 / 1024);
			FreeMB = (ULONG)(HDFreeSpace / 1024 / 1024);
		}
		else
		{
			AmntMB = 0;
			FreeMB = 0;
		}

		//��ô������
		SHFILEINFO	sfi;
		SHGetFileInfo(pDrive, FILE_ATTRIBUTE_NORMAL, &sfi,sizeof(SHFILEINFO), SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES);

		DriveName = sfi.szTypeName;

		DriveName += L" (";
		DriveName += pDrive;

		DriveName.Delete(DriveName.GetLength()-1);

		DriveName += L")";

		

		int icon = GetIconIndex(pDrive,GetFileAttributes(pDrive));

		int	nItem = cFleDlg->m_fileList.InsertItem(i, DriveName, icon);		//�����б��
		cFleDlg->m_fileList.SetItemData(nItem, (DWORD_PTR)pDrive);

		hDriverItem = cFleDlg->m_dirTree.InsertItem(DriveName,icon,icon,hRoot,0);	//���������б�
		cFleDlg->m_dirTree.SetItemData(hDriverItem, (DWORD_PTR)pDrive);

		cFleDlg->m_dirTree.Expand(hRoot,TVE_EXPAND);

		cFleDlg->m_fileList.SetItemText(nItem, 1, sfi.szTypeName);	//����������

		CString	str;
		str.Format(L"%10.1f GB", (float)AmntMB / 1024);
		cFleDlg->m_fileList.SetItemText(nItem, 2, str);
		str.Format(L"%10.1f GB", (float)FreeMB / 1024);
		cFleDlg->m_fileList.SetItemText(nItem, 3, str);
	}
	// ���ñ��ص�ǰ·��

	cFleDlg->m_filePath.ResetContent();




	cFleDlg->m_filePath.SetWindowTextW(L"�����");

}





int GetIconIndex(LPCWSTR lpFileName, DWORD dwFileAttributes)
{
	SHFILEINFO	sfi;
	if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
		dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	else
		dwFileAttributes |= FILE_ATTRIBUTE_NORMAL;

	SHGetFileInfo
		(
		lpFileName,
		dwFileAttributes, 
		&sfi,
		sizeof(SHFILEINFO), 
		SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES
		);

	return sfi.iIcon;
}

















void CFileDlg::OnRclickTreeFiledirectory(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ѡ�е�ǰ�Ҽ�����Ľڵ�
	POINT pos ;
	if(!GetCursorPos(&pos))
		return ;
	m_dirTree.ScreenToClient(&pos);

	UINT uFlags;
	HTREEITEM hItem = m_dirTree.HitTest(pos, &uFlags);
	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		m_dirTree.Select(hItem, TVGN_CARET);
	}



// 	// ��ʾ�˵�
// 	CRect rect ;
// 	this->GetClientRect(&rect) ;
// 	ClientToScreen(&rect) ;
// 	m_dirTree.ClientToScreen(&pos);
// 
// 	int iCode= rect.PtInRect(pos) ;
// 
// 	CMenu m_Menu,*p_Menu=NULL ;
// 	if(iCode)
// 	{
// 		m_Menu.LoadMenu( IDR_MENU_REG) ; 
// 		p_Menu = (CMenu*) m_Menu.GetSubMenu(0);
// 		if( p_Menu != NULL)
// 			p_Menu->TrackPopupMenu( TPM_RIGHTBUTTON|TPM_LEFTALIGN, pos.x, pos.y, this);
// 		p_Menu = NULL;
// 		//mouse_event(MOUSEEVENTF_RIGHTUP, pos.x, pos.y, 0, NULL);
// 	}




	*pResult = 0;
}
