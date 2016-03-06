// ProcessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeavenShadow.h"
#include "ProcessDlg.h"
#include "afxdialogex.h"
#include "ProcessFunc.h"

#include "HeavenShadowDlg.h"
#include "Common.h"

#include "ProcessViewDlg.h"
#include "InjectFunc.h"

#include <afxdb.h>             //ע��������ļ�����
#include <odbcinst.h>


CWnd* g_process = NULL;


extern HANDLE g_hDevice;
extern BOOL bIsChecking;

extern enum HS_ENUM_PROCVIEW_TYPE;


int ResizeX = 0;
int ResizeY = 0;

extern int dpix;
extern int dpiy;

//////////////////////////////////////////////////////////////////////////
//����

UINT sort_column;		//��¼�������
BOOL method = TRUE;		//��¼�ȽϷ���

//////////////////////////////////////////////////////////////////////////


// CProcessDlg �Ի���

IMPLEMENT_DYNAMIC(CProcessDlg, CDialog)

CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessDlg::IDD, pParent)
{
	m_wParent = pParent;
}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESSLIST, m_ProcessList);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(HS_PROCESSDIG_SEND_INSERT,HsProcessDlgSendInsert)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PROCESSLIST, &CProcessDlg::OnColumnclickListProcesslist)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROCESSLIST, &CProcessDlg::OnRclickListProcesslist)
	ON_COMMAND(ID_MENU_PROCESS_REFRESH, &CProcessDlg::OnMenuProcessRefresh)
	ON_COMMAND(ID_MENU_PROCESS_KILLPROCESS, &CProcessDlg::OnMenuProcessKillprocess)
	ON_COMMAND(ID_MENU_PROCESS_PROCESSTHREAD, &CProcessDlg::OnMenuProcessProcessthread)
	ON_COMMAND(ID_MENU_PROCESS_COPYINFO, &CProcessDlg::OnMenuProcessCopyinfo)
	ON_COMMAND(ID_MENU_PROCESS_ATTRIBUTE, &CProcessDlg::OnMenuProcessAttribute)
	ON_COMMAND(ID_MENU_PROCESS_LOCATIONFILE, &CProcessDlg::OnMenuProcessLocationfile)
	ON_COMMAND(ID_MENU_PROCESS_EXPORTTXT, &CProcessDlg::OnMenuProcessExporttxt)
	ON_COMMAND(ID_MENU_PROCESS_EXPORTEXCEL, &CProcessDlg::OnMenuProcessExportexcel)
	ON_COMMAND(ID_MENU_PROCESS_PROCESSPRIVILEGE, &CProcessDlg::OnMenuProcessProcessprivilege)
	ON_COMMAND(ID_MENU_PROCESS_INJECTDLL, &CProcessDlg::OnMenuProcessInjectdll)
	ON_COMMAND(ID_MENU_PROCESS_KILLMUST, &CProcessDlg::OnMenuProcessKillmust)
	ON_COMMAND(ID_MENU_PROCESS_DETAIL, &CProcessDlg::OnMenuProcessDetail)
	ON_COMMAND(ID_MENU_PROCESS_PROCESSHANDLE, &CProcessDlg::OnMenuProcessProcesshandle)
	ON_COMMAND(ID_MENU_PROCESS_MEMORY, &CProcessDlg::OnMenuProcessMemory)
	ON_COMMAND(ID_MENU_PROCESS_NEWRUN, &CProcessDlg::OnMenuProcessNewrun)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROCESSLIST, &CProcessDlg::OnDblclkListProcesslist)
	ON_COMMAND(ID_MENU_PROCESS_PROCESSWINDOW, &CProcessDlg::OnMenuProcessProcesswindow)
	ON_COMMAND(ID_MENU_PROCESS_SUSPEND, &CProcessDlg::OnMenuProcessSuspend)
	ON_COMMAND(ID_MENU_PROCESS_RECOVERY, &CProcessDlg::OnMenuProcessRecovery)
	ON_COMMAND(ID_MENU_PROCESS_PROCESSMODULE, &CProcessDlg::OnMenuProcessProcessmodule)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PROCESSLIST, &CProcessDlg::OnNMCustomdrawListProcesslist)
END_MESSAGE_MAP()


// CProcessDlg ��Ϣ�������


void CProcessDlg::OnBnClickedButtonTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(L"���̶Ի���",L"���̶Ի���");
}


void CProcessDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CRect   rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(255,255,255));
}


BOOL CProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	g_process = this;

	UINT uIconSize = 20;

	uIconSize *= (UINT)(dpix/96.0);

	m_TreeImageList.Create(uIconSize, uIconSize, ILC_COLOR32 | ILC_MASK, 2, 2);
	
	ListView_SetImageList(m_ProcessList.m_hWnd, m_TreeImageList.GetSafeHandle(), LVSIL_SMALL); 
	
	//m_ProcessList.SetImageList(&m_TreeImageList, LVSIL_SMALL);

	HsInitList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CProcessDlg::HsInitList(void)
{
	HsInitProcessList((CMyList*)&m_ProcessList);
	
	//m_ProcessList.SetSelectedColumn(1);
}

void CProcessDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������

	//////////////////////////////////////////////////////////////////////////
	
	if (bShow == TRUE)
	{
		m_ProcessList.MoveWindow(0,0,ResizeX,ResizeY);

		((CHeavenShadowDlg*)m_wParent)->m_bNowWindow = HS_DIALOG_PROCESS;

		((CHeavenShadowDlg*)m_wParent)->m_btnProc.EnableWindow(FALSE);

		HsLoadProcessList();

		HsSendStatusTip(L"����");

		m_ProcessList.SetFocus();
	}
}



void CProcessDlg::OnColumnclickListProcesslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int nItemCount;
	
	nItemCount = m_ProcessList.GetItemCount();

	sort_column=pNMLV->iSubItem;//�������

	m_ProcessList.SetSelectedColumn(sort_column);

	for(int i=0;i<nItemCount;i++) m_ProcessList.SetItemData(i,i);//ÿ�еıȽϹؼ��֣��˴�Ϊ����ţ�������кţ�����������Ϊ���� �ȽϺ����ĵ�һ��������

	m_ProcessList.SortItems(HsProcessListCompareProc,(DWORD_PTR)&m_ProcessList);//���� �ڶ��������ǱȽϺ����ĵ���������

	if (method)
	{
		method = FALSE;
	}
	else
	{
		method = TRUE;
	}

	for (int i = 0; i < nItemCount; i++)
	{
		if (_wcsnicmp(
			m_ProcessList.GetItemText(i,HS_PROCESS_COLUMN_COMPANY),
			L"Microsoft Corporation",
			wcslen(L"Microsoft Corporation"))==0
			)
		{
			m_ProcessList.SetItemData(i,1);
		}
	}

	*pResult = 0;
}


void CProcessDlg::HsLoadProcessList(void) //�����б�
{

	if (bIsChecking == FALSE)
	{
		bIsChecking = TRUE;

		while(m_TreeImageList.Remove(0));

		m_ProcessList.DeleteAllItems();

		m_ProcessList.SetSelectedColumn(-1);

		HsSendStatusDetail(L"�����б����ڼ��ء�");
		
		CloseHandle(
 			CreateThread(NULL,0, 
 			(LPTHREAD_START_ROUTINE)HsQueryProcessFunction,&m_ProcessList, 0,NULL)
			);
	}
}







void CProcessDlg::OnRclickListProcesslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CMenu	popup;
	popup.LoadMenu(IDR_MENU_PROCESS);				//���ز˵���Դ
	CMenu*	pM = popup.GetSubMenu(0);				//��ò˵�������
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();
	if (m_ProcessList.GetSelectedCount() == 0)		//���û��ѡ��
	{ 
		for (int i = 0;i<count;i++)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
		}

		pM->EnableMenuItem(ID_MENU_PROCESS_REFRESH, MF_BYCOMMAND | MF_ENABLED);
	}

	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

	*pResult = 0;
}


void CProcessDlg::OnMenuProcessRefresh()
{
	// TODO: �ڴ���������������
	HsLoadProcessList();
}





void CProcessDlg::OnMenuProcessKillprocess()
{
	// TODO: �ڴ���������������

	if (bIsChecking)
	{
		return;
	}

	if (MessageBox(L"ȷ��Ҫ�����ý�����",L"��Ӱ��ʿ",MB_YESNO) == IDYES)
	{
		bIsChecking = TRUE;

		POSITION pos = m_ProcessList.GetFirstSelectedItemPosition();

		while (pos)
		{
			int nItem = m_ProcessList.GetNextSelectedItem(pos);

			DWORD_PTR ulPid = _ttoi(m_ProcessList.GetItemText(nItem,HS_PROCESS_COLUMN_PID).GetBuffer());

			if (ulPid == 0 || ulPid == 4)
			{
				MessageBox(L"���̹ر�ʧ�ܡ�",L"��Ӱ��ʿ",0);
				return;
			}

			HsDebugPrivilege(SE_DEBUG_NAME, TRUE);

			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_VM_OPERATION, TRUE, (DWORD)ulPid);

			BOOL bIsSuccess = TerminateProcess(hProcess,0);

			HsDebugPrivilege(SE_DEBUG_NAME, FALSE);

			if (bIsSuccess == FALSE)
			{
				MessageBox(L"�رս���ʧ�ܡ�",L"��Ӱ��ʿ",0);
			}
			else
			{
				m_ProcessList.DeleteItem(nItem);
			}

			CloseHandle(hProcess);
		}
		
		bIsChecking = FALSE;
	}

}



// void CProcessDlg::HsQuaryProcessThread(ULONG_PTR ulPid)
// {
// 	DWORD dwReturnSize = 0;
// 	DWORD dwRet = 0;
// 	//����IO ������
// 
// 
// 
// 	dwRet = DeviceIoControl(g_hDevice,HS_IOCTL(HS_IOCTL_PROC_PROCESSTHREAD),
// 		(PVOID)&ulPid,
// 		sizeof(ULONG_PTR),
// 		NULL,
// 		0,
// 		&dwReturnSize,
// 		NULL);
// 
// 	if (dwRet==0)
// 	{
// 		WCHAR *temp = L"���ں˲�ͨ���쳣��";
// 		::SendMessageW(m_wParent->m_hWnd,HS_MESSAGE_STATUSDETAIL,NULL,(LPARAM)temp);
// 
// 		return;
// 	}
// }



BOOL CProcessDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN ||pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_F5)
	{
		OnMenuProcessRefresh();
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CProcessDlg::OnMenuProcessCopyinfo()
{
	// TODO: �ڴ���������������
	POSITION pos = m_ProcessList.GetFirstSelectedItemPosition();

	while(pos)
	{
		int iItem = m_ProcessList.GetNextSelectedItem(pos);

		CStringA(csProcInfo);
		csProcInfo = "ӳ������: ";
		csProcInfo += m_ProcessList.GetItemText(iItem,HS_PROCESS_COLUMN_NAME);
		csProcInfo += "  ����ID: ";
		csProcInfo += m_ProcessList.GetItemText(iItem,HS_PROCESS_COLUMN_PID);
		csProcInfo += "  ������ID: ";
		csProcInfo += m_ProcessList.GetItemText(iItem,HS_PROCESS_COLUMN_PPID);
		csProcInfo += "  ӳ��·��: ";
		csProcInfo += m_ProcessList.GetItemText(iItem,HS_PROCESS_COLUMN_PATH);
		csProcInfo += "  EPROCESS: ";
		csProcInfo += m_ProcessList.GetItemText(iItem,HS_PROCESS_COLUMN_EPROCESS);
		csProcInfo += "  Ӧ�ò����: ";
		csProcInfo += m_ProcessList.GetItemText(iItem,HS_PROCESS_COLUMN_USERACCESS);
		csProcInfo += "  �ļ�����: ";
		if (m_ProcessList.GetItemText(iItem,HS_PROCESS_COLUMN_COMPANY).GetLength()>0)
		{
			csProcInfo += m_ProcessList.GetItemText(iItem,HS_PROCESS_COLUMN_COMPANY);
		}

		if(::OpenClipboard(NULL))
		{
			HGLOBAL hmem=GlobalAlloc(GHND,csProcInfo.GetLength()+1);
			char *pmem=(char*)GlobalLock(hmem);

			EmptyClipboard();
			memcpy(pmem,csProcInfo.GetBuffer(),csProcInfo.GetLength()+1);
			SetClipboardData(CF_TEXT,hmem);
			CloseClipboard();
			GlobalFree(hmem); 
		}
	}
}



void CProcessDlg::OnMenuProcessAttribute()
{
	// TODO: �ڴ���������������

	POSITION pos = m_ProcessList.GetFirstSelectedItemPosition();

	while(pos)
	{
		int iItem = m_ProcessList.GetNextSelectedItem(pos);

		CString csFilePath = m_ProcessList.GetItemText(iItem,HS_PROCESS_COLUMN_PATH);

		HsCheckAttribute(csFilePath);
	}
}


void CProcessDlg::OnMenuProcessLocationfile()
{
	// TODO: �ڴ���������������
	POSITION pos = m_ProcessList.GetFirstSelectedItemPosition();

	while(pos)
	{
		int iItem = m_ProcessList.GetNextSelectedItem(pos);

		CString csFilePath = m_ProcessList.GetItemText(iItem,HS_PROCESS_COLUMN_PATH);

		HsLocationExplorer(csFilePath);
	}
}


void CProcessDlg::OnMenuProcessExporttxt()
{
	// TODO: �ڴ���������������
	
	if (m_ProcessList.GetItemCount () > 0) 
	{ 
		CString strFile; 

		if (!HsGetDefaultTextFileName(strFile))
			return;

		CFile File;

		TRY
		{
			if (File.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, NULL))
			{
				int i = 0;
				LVCOLUMN columnData;
				CString  columnName;
				int      columnNum = 0;
				CString  strCloumn;
				CHAR  szColumn[0x1000] = {0};
				WCHAR wzColumn[0x1000] = {0};

				columnData.mask = LVCF_TEXT;
				columnData.cchTextMax = 100;
				columnData.pszText = columnName.GetBuffer(100);

				for(i = 0;m_ProcessList.GetColumn(i,&columnData); i++)
				{
					strCloumn = strCloumn + columnData.pszText + L"  |  ";
				}

				strCloumn += "\r\n";
				ULONG  nLenTemp = strCloumn.GetLength();
				wcsncpy_s(wzColumn,0x1000,strCloumn.GetBuffer(),nLenTemp);
				strCloumn.ReleaseBuffer();
				WideCharToMultiByte(CP_ACP,0,wzColumn,-1,szColumn,0x1000,NULL,NULL);
				File.Write(szColumn,(UINT)strlen(szColumn));  

				columnName.ReleaseBuffer();
				columnNum = i;

				//������ȡ��ShowList �ı���


				for (int nItemIndex = 0; nItemIndex <m_ProcessList.GetItemCount(); nItemIndex++)
				{
					CHAR  szColumn[0x1000]  = {0};
					WCHAR wzColumn[0x1000] = {0};
					CString strItem;

					for( i = 0; i < columnNum; i++)
					{
						strItem = strItem + m_ProcessList.GetItemText(nItemIndex,i) + L"  |  ";
					}

					strItem += "\r\n";
					nLenTemp = strItem.GetLength();
					wcsncpy_s(wzColumn,0x1000,strItem.GetBuffer(),nLenTemp);
					strItem.ReleaseBuffer();
					WideCharToMultiByte(CP_ACP,0,wzColumn,-1,szColumn,0x1000,NULL,NULL);
					File.Write(szColumn, (UINT)strlen(szColumn));  
				}

				File.Close();
			}
		}
		CATCH_ALL(e)
		{
			File.Abort();  
		}
		END_CATCH_ALL


		if ( PathFileExists(strFile))
		{
			ShellExecuteW(NULL, L"open", strFile, NULL, NULL, SW_SHOW);
		}
		else
		{
			::MessageBox(NULL, L"�������ı��ļ�ʧ�ܡ�", L"��Ӱ��ʿ", MB_OK | MB_ICONERROR);
		}
	}
}


void CProcessDlg::OnMenuProcessExportexcel()
{
// 	// TODO: �ڴ���������������
// 
// 	BOOL bOk = FALSE;
// 
// 	CString strEnd = L"ö�����";
// 
// 	if (m_ProcessList.GetItemCount () > 0) 
// 	{ 
// 		CDatabase DataBase;
// 		CString   strDriver;
// 		CString   strExcelFile; 
// 		CString   strSql;
// 		CString   TableName = L"HeavenShadow";
// 		strDriver = HsGetExcelDriver();
// 		if (strDriver.IsEmpty())
// 		{
// 			::MessageBox(NULL, L"û�а�װExcel!\n���Ȱ�װExcel�������ʹ�õ�������!", L"����", MB_OK | MB_ICONERROR);
// 			return;
// 		}
//
// 		if (!HsGetDefaultExcelFileName(strExcelFile))
// 		{
// 			return;
// 		}
// 
// 		strSql.Format(L"DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", strDriver, strExcelFile, strExcelFile);
// 
// 		if(DataBase.OpenEx(strSql, CDatabase::noOdbcDialog))
// 		{
// 			int i;
// 			LVCOLUMN columnData;
// 			CString columnName;
// 			int columnNum = 0;
// 			CString strH;
// 			CString strV;
// 
// 			strSql = L"";
// 			strH = L"";
// 			columnData.mask = LVCF_TEXT;
// 			columnData.cchTextMax = 100;
// 			columnData.pszText = columnName.GetBuffer(100);
// 
// 			for(i = 0; m_ProcessList.GetColumn(i, &columnData); i++)
// 			{
// 				if (i != 0)
// 				{
// 					strSql = strSql + L", " ;
// 					strH = strH + L", " ;
// 				}
// 
// 				strSql = strSql + L" " + L"[" + columnData.pszText + L"]" + L" TEXT";
// 				strH = strH + L" " + L"[" + columnData.pszText + L"]" + L" ";
// 			}
// 
// 			columnName.ReleaseBuffer();
// 			columnNum = i;
// 
// 			strSql = L"CREATE TABLE " +TableName + L" ( " + strSql +   L" ) ";
// 			DataBase.ExecuteSQL(strSql);
// 
// 			for (int nItemIndex = 0; nItemIndex < m_ProcessList.GetItemCount(); nItemIndex++)
// 			{
// 				strV = L"";
// 				for( i = 0; i < columnNum; i++)
// 				{
// 					if (i != 0)
// 					{
// 						strV = strV + L", " ;
// 					}
// 
// 					strV = strV + L" '" + m_ProcessList.GetItemText(nItemIndex, i) + L"' ";
// 				}
// 
// 				strSql = L"INSERT INTO "+ TableName + L" ("+ strH + L")" + L" VALUES("+ strV + L")";
// 				DataBase.ExecuteSQL(strSql);
// 			}
// 
// 			strV = L"";
// 			for( i = 0; i < columnNum; i++)
// 			{
// 				if (i != 0)
// 				{
// 					strV = strV + L", " ;
// 				}
// 
// 				strV = strV + L" '" + L" " + L"' ";
// 			}
// 
// 			strSql = L"INSERT INTO "+ TableName + L" ("+ strH + L")" + L" VALUES("+ strV + L")";
// 			DataBase.ExecuteSQL(strSql);
// 
// 			strV = L" ";
// 			strV = strV + L" '" + strEnd + L"' ";
// 
// 			for( i = 1; i < columnNum; i++)
// 			{
// 				strV = strV + L", " ;
// 				strV = strV + L" '" + L" " + L"' ";
// 			}
// 
// 			strSql = L"INSERT INTO "+ TableName + L" ("+ strH + L")" + L" VALUES("+ strV + L")";
// 			DataBase.ExecuteSQL(strSql);
// 
// 			bOk = TRUE;
// 		}      
// 
// 		DataBase.Close();
// 
// 		if ( bOk && PathFileExists(strExcelFile) )
// 		{
// 			ShellExecuteW(NULL,L"open",strExcelFile,NULL,NULL,SW_SHOW);
// 		}
// 		else
// 		{
// 			::MessageBox(NULL,L"������Excel�ļ�ʧ�ܡ�",L"��Ӱ��ʿ",MB_OK | MB_ICONERROR);
// 		}
// 	}
}






//����ȽϺ���
static int CALLBACK HsProcessListCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{

	//�Ӳ�������ȡ����Ƚ�lc����������

	int row1=(int) lParam1;
	int row2=(int) lParam2;

	CListCtrl*lc=(CListCtrl*)lParamSort;

	CString lp1=lc->GetItemText(row1,sort_column);
	CString lp2=lc->GetItemText(row2,sort_column);


	//�Ƚϣ��Բ�ͬ���У���ͬ�Ƚϣ�ע���¼ǰһ����������һ��Ҫ�෴����

	if (sort_column == HS_PROCESS_COLUMN_PID ||
		sort_column == HS_PROCESS_COLUMN_PPID)
	{
		// int�ͱȽ�
		if (method)
			return _ttoi(lp1)-_ttoi(lp2);
		else
			return _ttoi(lp2)-_ttoi(lp1);
	}
	else if (sort_column == HS_PROCESS_COLUMN_EPROCESS)
	{
		if (method)
		{
			ULONG_PTR nlp1 = 0, nlp2 = 0;

			lp1 = lp1.GetBuffer()+2;
			lp2 = lp2.GetBuffer()+2;

			swscanf_s(lp1.GetBuffer(),L"%P",&nlp1);
			swscanf_s(lp2.GetBuffer(),L"%P",&nlp2);
			return (int)(nlp1 - nlp2);
		}
		else
		{
			int nlp1 = 0, nlp2 = 0;
			lp1 = lp1.GetBuffer()+2;
			lp2 = lp2.GetBuffer()+2;
			swscanf_s(lp1.GetBuffer(),L"%X",&nlp1);
			swscanf_s(lp2.GetBuffer(),L"%X",&nlp2);
			return nlp2 - nlp1;
		}
	}
	else
	{
		// �����ͱȽ�
		if(method)
			return lp1.CompareNoCase(lp2);
		else
			return lp2.CompareNoCase(lp1);
	}

	return 0;
}


void CProcessDlg::OnMenuProcessInjectdll()
{
	// TODO: �ڴ���������������

	CloseHandle(
		CreateThread(NULL,0, 
		(LPTHREAD_START_ROUTINE)HsRemoteThreadInjectDll,&m_ProcessList, 0,NULL)
		);

}


void CProcessDlg::OnMenuProcessKillmust()
{
	// TODO: �ڴ���������������

	if (bIsChecking)
	{
		return;
	}

	if (MessageBox(L"ǿ�ƹرս��̵Ĳ����з��գ������������\r\nȷ��Ҫ�����ý�����",L"��Ӱ��ʿ",MB_YESNO) == IDNO)
	{
		return;
	}

// 	CreateThread(NULL,0, 
// 		(LPTHREAD_START_ROUTINE)HsKillProcessByForce,(CMyList*)&m_ProcessList, 0,NULL);

	bIsChecking = TRUE;

	HsKillProcessByForce((CMyList*)&m_ProcessList);

	bIsChecking = FALSE;

}






void CProcessDlg::OnMenuProcessProcessthread()
{
	// TODO: �ڴ���������������

	//HsQuaryProcessThread(ulPid);

	HsOpenProcessViewDlg(HS_PROCVIEW_TYPE_THREAD);

}

void CProcessDlg::OnMenuProcessProcessprivilege()
{
	// TODO: �ڴ���������������
	HsOpenProcessViewDlg(HS_PROCVIEW_TYPE_PRIVILEGE);
}


void CProcessDlg::OnMenuProcessDetail()
{
	// TODO: �ڴ���������������
	HsOpenProcessViewDlg(HS_PROCVIEW_TYPE_DETAIL);
}


void CProcessDlg::HsOpenProcessViewDlg(int nViewType)
{
	POSITION pos = m_ProcessList.GetFirstSelectedItemPosition();

	while(pos)
	{
		int nItem = m_ProcessList.GetNextSelectedItem(pos);

		ULONG_PTR ulPid = _ttoi(m_ProcessList.GetItemText(nItem,HS_PROCESS_COLUMN_PID).GetBuffer());
		ULONG_PTR ulPPid = _ttoi(m_ProcessList.GetItemText(nItem,HS_PROCESS_COLUMN_PPID).GetBuffer());

		CString ProcessEProcess = m_ProcessList.GetItemText(nItem,HS_PROCESS_COLUMN_EPROCESS);

		ProcessEProcess = ProcessEProcess.GetBuffer()+2;

		ULONG_PTR ulEProcess = 0;

		swscanf_s(ProcessEProcess.GetBuffer(),L"%P",&ulEProcess);


		HSPROCESSINFO hsProcInfo = {0};

		hsProcInfo.Pid = ulPid;
		hsProcInfo.PPid = ulPPid;
		hsProcInfo.Eprocess = ulEProcess;

		if (_wcsnicmp(m_ProcessList.GetItemText(nItem,HS_PROCESS_COLUMN_COMPANY).GetBuffer(),L"�ܾ�",wcslen(L"�ܾ�")) == 0)
		{
			hsProcInfo.UserAccess = FALSE;
		}
		else
		{
			hsProcInfo.UserAccess = TRUE;
		}

		StringCchCopyW(hsProcInfo.CompanyName,
			m_ProcessList.GetItemText(nItem,HS_PROCESS_COLUMN_COMPANY).GetLength()+1,
			m_ProcessList.GetItemText(nItem,HS_PROCESS_COLUMN_COMPANY).GetBuffer());
		StringCchCopyW(hsProcInfo.Name,
			m_ProcessList.GetItemText(nItem,HS_PROCESS_COLUMN_NAME).GetLength()+1,
			m_ProcessList.GetItemText(nItem,HS_PROCESS_COLUMN_NAME).GetBuffer());
		StringCchCopyW(hsProcInfo.Path,
			m_ProcessList.GetItemText(nItem,HS_PROCESS_COLUMN_PATH).GetLength()+1,
			m_ProcessList.GetItemText(nItem,HS_PROCESS_COLUMN_PATH).GetBuffer());

		CProcessViewDlg* dlg = new CProcessViewDlg(nViewType,&hsProcInfo,this);
		dlg->DoModal();
	}
}



LRESULT CProcessDlg::HsProcessDlgSendInsert(WPARAM wParam, LPARAM lParam)
{
	PHSPROCESSINFO* hsProcItem = (PHSPROCESSINFO*)lParam;

	CString Name = NULL;
	CString Pid = NULL;
	CString PPid = NULL;
	CString Path = NULL;
	CString EProcess = NULL;
	CString UserAccess = NULL;
	CString CompanyName = NULL;

	ULONG ulItem = m_ProcessList.GetItemCount();

	WCHAR tempdir[100];
	GetEnvironmentVariableW(L"windir",tempdir,100);


	Name = (*hsProcItem)->Name;
	Pid.Format(L"%d",(*hsProcItem)->Pid);

	if ((*hsProcItem)->PPid == 0xffffffff)
	{
		PPid = L"-";
	}
	else
	{
		PPid.Format(L"%d",(*hsProcItem)->PPid);
	}

	Path = (*hsProcItem)->Path;

	EProcess.Format(L"0x%p",(*hsProcItem)->Eprocess);

	if ((*hsProcItem)->UserAccess == TRUE)
	{
		UserAccess = L"����";
	}
	else
	{
		UserAccess = L"�ܾ�";
	}

	CompanyName = (*hsProcItem)->CompanyName;

	AddProcessFileIcon(Path.GetBuffer());

	m_ProcessList.InsertItem(ulItem,Name,ulItem);
	m_ProcessList.SetItemText(ulItem,HS_PROCESS_COLUMN_PID,Pid);
	m_ProcessList.SetItemText(ulItem,HS_PROCESS_COLUMN_PPID,PPid);
	m_ProcessList.SetItemText(ulItem,HS_PROCESS_COLUMN_PATH,Path);
	m_ProcessList.SetItemText(ulItem,HS_PROCESS_COLUMN_EPROCESS,EProcess);
	m_ProcessList.SetItemText(ulItem,HS_PROCESS_COLUMN_USERACCESS,UserAccess);
	m_ProcessList.SetItemText(ulItem,HS_PROCESS_COLUMN_COMPANY,CompanyName);

	if (_wcsnicmp(CompanyName,L"Microsoft Corporation", wcslen(L"Microsoft Corporation"))==0)
	{
		m_ProcessList.SetItemData(ulItem,1);
	}


	CToolTipCtrl ToolTipCtrl;

	ToolTipCtrl.Create(this);
	m_ProcessList.SetToolTips(&ToolTipCtrl);


	return TRUE;
}



void CProcessDlg::OnMenuProcessProcesshandle()
{
	// TODO: �ڴ���������������
	HsOpenProcessViewDlg(HS_PROCVIEW_TYPE_HANDLE);
}

void CProcessDlg::OnMenuProcessProcesswindow()
{
	// TODO: �ڴ���������������
	HsOpenProcessViewDlg(HS_PROCVIEW_TYPE_WINDOW);
}


void CProcessDlg::OnMenuProcessProcessmodule()
{
	// TODO: �ڴ���������������
	HsOpenProcessViewDlg(HS_PROCVIEW_TYPE_MODULE);
}

void CProcessDlg::OnMenuProcessMemory()
{
	// TODO: �ڴ���������������

	HsOpenProcessViewDlg(HS_PROCVIEW_TYPE_MEMORY);
}


void CProcessDlg::OnMenuProcessNewrun()
{
	// TODO: �ڴ���������������

	WCHAR wzFileFilter[] = L"Ӧ�ó��� (*.exe)\0*.exe\0";
	WCHAR wzFileChoose[] = L"���ļ�";


	CFileDialog FileDlg(TRUE);
	FileDlg.m_ofn.lpstrTitle  = wzFileChoose;
	FileDlg.m_ofn.lpstrFilter = wzFileFilter;

	if (IDOK != FileDlg.DoModal())
	{
		return;
	}

	CString ExePath = FileDlg.GetPathName();

	ShellExecuteW(NULL, L"open", ExePath, L"", L"", SW_SHOW);
}



void CProcessDlg::AddProcessFileIcon(WCHAR* ProcessPath)
{
	SHFILEINFO shInfo;
	memset(&shInfo, 0, sizeof(shInfo));
	SHGetFileInfo(ProcessPath, FILE_ATTRIBUTE_NORMAL, 
		&shInfo, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);

	HICON  hIcon = shInfo.hIcon;

	m_TreeImageList.Add(hIcon);
}

void CProcessDlg::OnDblclkListProcesslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnMenuProcessDetail();

	*pResult = 0;
}





void CProcessDlg::OnMenuProcessSuspend()
{
	// TODO: �ڴ���������������
}


void CProcessDlg::OnMenuProcessRecovery()
{
	// TODO: �ڴ���������������
}




void CProcessDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	ResizeX = cx;
	ResizeY = cy;
}


void CProcessDlg::OnNMCustomdrawListProcesslist(NMHDR *pNMHDR, LRESULT *pResult)
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

		bHooked = (int)m_ProcessList.GetItemData(nItem); 		
		if (bHooked == 1)
		{
			clrNewTextColor = RGB( 0, 0, 255 );
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;

		*pResult = CDRF_DODEFAULT;
	}

}
