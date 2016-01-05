// RegFindDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RegisterManager.h"
#include "RegFindDlg.h"
#include "afxdialogex.h"
#include "RegisterManagerDlg.h"

// CRegFindDlg �Ի���

IMPLEMENT_DYNAMIC(CRegFindDlg, CDialog)

#define  WM_SEARCH_FINISH   WM_USER + 105
#define  WM_UPDATE_UI		WM_USER + 106


extern CWnd*  g_Father;

DWORD WINAPI SearchRegistryProc(PVOID lPParam)
{
	if (lPParam)
	{
		CRegFindDlg *Dlg = (CRegFindDlg*)lPParam;
		Dlg->SearchRegistry();
	}

	return 0;
}

CRegFindDlg::CRegFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegFindDlg::IDD, pParent)
	, m_strSearchInKey(_T(""))
	, m_nRadio(0)
	, m_strFindWhat(_T(""))
	, m_bKeys(TRUE)
	, m_bMachCase(FALSE)
	, m_bMachWholeString(FALSE)
	, m_bData(TRUE)
	, m_bValues(TRUE)
	, m_strSearchResult(_T(""))
{
	m_strFindWhatUpper = L"";
	m_nCnt = 0;
	m_bStop = FALSE;
	m_hThread = NULL;

}

CRegFindDlg::~CRegFindDlg()
{
}

void CRegFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT, m_strSearchInKey);
	DDX_Radio(pDX, IDC_RADIO_SEARCH_IN, m_nRadio);
	DDX_Text(pDX, IDC_EDIT_FIND_WHAT, m_strFindWhat);
	DDX_Check(pDX, IDC_CHECK_KEY, m_bKeys);
	DDX_Check(pDX, IDC_CHECK_MACH_CASE, m_bMachCase);
	DDX_Check(pDX, IDC_CHECK_MACH_WHOLE_STRING, m_bMachWholeString);
	DDX_Check(pDX, IDC_CHECK_DATA, m_bData);
	DDX_Check(pDX, IDC_CHECK_VALUES, m_bValues);
	DDX_Text(pDX, IDC_STATIC_FIND_RESULT, m_strSearchResult);
	DDX_Control(pDX, IDC_ANIMATE, m_ctlAnimate);
}


BEGIN_MESSAGE_MAP(CRegFindDlg, CDialog)
	
	ON_BN_CLICKED(IDC_BUTTON_START, &CRegFindDlg::OnBnClickedButtonStart)
	ON_MESSAGE(WM_UPDATE_UI, &CRegFindDlg::UpdateUI)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CRegFindDlg::OnBnClickedButtonStop)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CRegFindDlg::OnNMDblclkList)
END_MESSAGE_MAP()


// CRegFindDlg ��Ϣ�������
LRESULT CRegFindDlg::UpdateUI(WPARAM, LPARAM)
{
	UpdateData(FALSE);
	return 0;
}

BOOL CRegFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ctlAnimate.Open(IDR_AVI);//����AVI�ļ�
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,L"��", LVCFMT_LEFT, 300);
	m_List.InsertColumn(1,L"��ֵ", LVCFMT_LEFT, 80);
	m_List.InsertColumn(2,L"����", LVCFMT_LEFT, 120);


	m_strSearchInKey.IsEmpty() ? m_nRadio = 1 : m_nRadio = 0;     //ע��ؼ�����������
	GetDlgItem(IDC_EDIT)->EnableWindow(!m_nRadio);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}





void CRegFindDlg::OnBnClickedButtonStart()
{
	UpdateData(TRUE);
	m_List.DeleteAllItems();

	// �ж����������Ƿ��Ѿ���д
	if (m_strFindWhat.IsEmpty())
	{
		MessageBox(L"�����������",L"Shine", MB_OK | MB_ICONWARNING);
		return;
	}

	// �������ĳ����ֵ�в�ѯ
	if (m_nRadio == 0)
	{
		//	�����ж��Ƿ���д
		if (m_strSearchInKey.IsEmpty())
		{
			MessageBox(L"������ҷ�Χ", L"Shine", MB_OK | MB_ICONWARNING);
			return;
		}

		// �жϸ����Ƿ����
		HKEY hKey = GetRootKey(m_strSearchInKey);
		if (!hKey)
		{
			MessageBox(L"����������",L"Shine", MB_OK | MB_ICONERROR);
			return;
		}

		// �ж��ܷ�򿪸ü�
		CString strSubKey;
		if (m_strSearchInKey.Find('\\') != -1)
		{
			strSubKey = m_strSearchInKey.Right(m_strSearchInKey.GetLength() - m_strSearchInKey.Find('\\') - 1);
		}
		HKEY hKeyTemp = NULL;
		LONG ulRet = RegOpenKeyEx(hKey, strSubKey, 0, KEY_READ, &hKeyTemp);

		if (ERROR_SUCCESS != ulRet)
		{
			MessageBox(L"���ܴ򿪸ü�ֵ", L"Shine", MB_OK | MB_ICONERROR);
			return;
		}

		RegCloseKey(hKeyTemp);
	}

	// ����һ�ݴ�д��ĸ�Ĳ�������,������Һ��Դ�Сд,�Ϳ���ʹ�������
	m_strFindWhatUpper = m_strFindWhat;
	m_strFindWhatUpper.MakeUpper();

	// ͨ����ǰ��ļ�飬��ʼ����
	InitControl(FALSE);   //�����ؼ�

	if (m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	DWORD dwTid = 0;
	m_hThread = CreateThread(NULL, 0, SearchRegistryProc, this, 0, &dwTid);
}



HKEY CRegFindDlg::GetRootKey(CString strKey)
{
	HKEY hRet = NULL;

	if (strKey.IsEmpty())
	{
		return hRet;
	}

	CString strKeyRoot;
	if (strKey.Find('\\') != -1)
	{
		strKeyRoot = strKey.Left(strKey.Find('\\'));
	}
	else
	{
		strKeyRoot = strKey;
	}

	if (!strKeyRoot.CompareNoCase(L"HKEY_CLASSES_ROOT"))
	{
		hRet = HKEY_CLASSES_ROOT;
	}
	else if (!strKeyRoot.CompareNoCase(L"HKEY_CURRENT_USER"))
	{
		hRet = HKEY_CURRENT_USER;
	}
	else if (!strKeyRoot.CompareNoCase(L"HKEY_LOCAL_MACHINE"))
	{
		hRet = HKEY_LOCAL_MACHINE;
	}
	else if (!strKeyRoot.CompareNoCase(L"HKEY_USERS"))
	{
		hRet = HKEY_USERS;
	}
	else if (!strKeyRoot.CompareNoCase(L"HKEY_CURRENT_CONFIG"))
	{
		hRet = HKEY_CURRENT_CONFIG;
	}

	return hRet;
}


VOID CRegFindDlg::SearchRegistry()
{
	m_bStop = FALSE;
	m_nCnt = 0;
	m_strSearchResult = L"���ڲ�ѯ";
   	SendMessage(WM_UPDATE_UI);



	// ����ָ����
	if (m_nRadio == 0)
	{
		HKEY hKey = GetRootKey(m_strSearchInKey);
		CString strSubKey;
		if (m_strSearchInKey.Find('\\') != -1)
		{
			strSubKey = m_strSearchInKey.Right(m_strSearchInKey.GetLength() - m_strSearchInKey.Find('\\') - 1);
		}

		EnumKeys(hKey, strSubKey);
	}

	// ��������ע���
	else
	{
		m_ctlAnimate.Play(0,-1,-1);//���Ŷ���
		EnumKeys(HKEY_CLASSES_ROOT, NULL);
		EnumKeys(HKEY_CURRENT_USER, NULL);
		EnumKeys(HKEY_LOCAL_MACHINE, NULL);
		EnumKeys(HKEY_USERS, NULL);
		EnumKeys(HKEY_CURRENT_CONFIG, NULL);
		m_ctlAnimate.Stop();//ֹͣ����
	
	}

	InitControl(TRUE);
	m_strSearchResult.Format(L"����ѯ��%dƥ����", m_nCnt);

	SendMessage(WM_UPDATE_UI);

//	SendMessage(WM_SEARCH_FINISH);
}



void CRegFindDlg::EnumKeys(HKEY hRoot, CString strSubKey)
{
	if (!hRoot)
	{
		return;
	}

	strSubKey.TrimLeft('\\');
	HKEY hKeyTemp = NULL;
	LONG nRet = RegOpenKeyEx(hRoot, strSubKey, 0, KEY_READ, &hKeyTemp);
	if (nRet != ERROR_SUCCESS)
	{
		return;
	}

	DWORD dwSubKeys = 0, dwSubValues = 0;
	LONG ulRet = ::RegQueryInfoKey(hKeyTemp, NULL, NULL, NULL, &dwSubKeys, NULL, NULL, &dwSubValues, NULL, NULL, NULL, NULL);
	if (ulRet != ERROR_SUCCESS)
	{
		RegCloseKey(hKeyTemp);
		return;
	}

	// ���������  CheckBox
	if (m_bKeys)
	{
		for (DWORD dwIndex = 0; !m_bStop && dwIndex < dwSubKeys; dwIndex++)
		{
			DWORD dwLen = 1024;
			TCHAR szSubName[1024];
			memset(szSubName, 0, 1024 * sizeof(TCHAR));

			ulRet = RegEnumKey(hKeyTemp, dwIndex, szSubName, dwLen);
			if (ulRet == ERROR_SUCCESS)
			{ 
				CString strSubName = CString(szSubName);

				// ���ִ�Сд & ƥ��ȫ�ַ�
				if (m_bMachCase && m_bMachWholeString)
				{
					if (!strSubName.Compare(m_strFindWhat))
					{
						InsertKeys(hRoot, strSubKey, strSubName);
					}
				}

				// ���ִ�Сд,���ǲ�ȫ��ƥ��
				else if (m_bMachCase && !m_bMachWholeString)
				{
					if (-1 != strSubName.Find(m_strFindWhat))
					{
						InsertKeys(hRoot, strSubKey, strSubName);
					}
				}	

				// �����ִ�Сд & ȫ��ƥ��
				else if (!m_bMachCase && m_bMachWholeString)
				{
					if (!strSubName.CompareNoCase(m_strFindWhat))
					{
						InsertKeys(hRoot, strSubKey, strSubName);
					}
				}

				// �����ִ�Сд & ��ȫ��ƥ��
				else if (!m_bMachCase && !m_bMachWholeString)
				{
					CString szTemp = strSubName;
					szTemp.MakeUpper();

					if (szTemp.Find(m_strFindWhatUpper) != -1)
					{
						InsertKeys(hRoot, strSubKey, strSubName);
					}
				}

				// �ݹ�ö��
				EnumKeys(hRoot, strSubKey + L"\\" + szSubName);
			}
		}
	}

	// ���ֵ�����ݶ�����Ҫ,��ôֱ�ӷ�����
	if (!m_bData && !m_bValues)
	{
		RegCloseKey(hKeyTemp);
		return;
	}

	// ö��ֵ
	for (DWORD dwIndex = 0; !m_bStop && dwIndex < dwSubValues; dwIndex++)
	{
		DWORD dwLen = 1024;
		TCHAR szSubName[1024];
		memset(szSubName, 0, 1024 * sizeof(TCHAR));

		DWORD dwType = 0, dwDataLen = 0x2000;
		BYTE  Data[0x2000] = {0};
		ulRet = RegEnumValue(hKeyTemp, dwIndex, szSubName, &dwLen, NULL, &dwType, Data, &dwDataLen);
		if (ulRet == ERROR_SUCCESS)
		{ 
			// ���ö��ֵ��ѡ����
			if (m_bValues)
			{
				CString strSubName = CString(szSubName);

				// ���ִ�Сд & ƥ��ȫ�ַ�
				if (m_bMachCase && m_bMachWholeString)
				{
					if (!strSubName.Compare(m_strFindWhat))
					{
						InsertVlaues(hRoot, strSubKey, strSubName);
					}
				}

				// ���ִ�Сд,���ǲ�ȫ��ƥ��
				else if (m_bMachCase && !m_bMachWholeString)
				{
					if (-1 != strSubName.Find(m_strFindWhat))
					{
						InsertVlaues(hRoot, strSubKey, strSubName);
					}
				}	

				// �����ִ�Сд & ȫ��ƥ��
				else if (!m_bMachCase && m_bMachWholeString)
				{
					if (!strSubName.CompareNoCase(m_strFindWhat))
					{
						InsertVlaues(hRoot, strSubKey, strSubName);
					}
				}

				// �����ִ�Сд & ��ȫ��ƥ��
				else if (!m_bMachCase && !m_bMachWholeString)
				{
					CString szTemp = strSubName;
					szTemp.MakeUpper();

					if (szTemp.Find(m_strFindWhatUpper) != -1)
					{
						InsertVlaues(hRoot, strSubKey, strSubName);
					}
				}
			}

			// ���ö�����ݱ�ѡ����
			if (m_bData)
			{
				InsertData(hRoot, strSubKey, szSubName, dwType, Data, dwDataLen);
			}
		}
	}

	RegCloseKey(hKeyTemp);
}



void CRegFindDlg::InsertKeys(HKEY hRoot, CString strSubKey, CString strSubSubKey)
{
	if (!hRoot)
	{
		return;
	}

	CString strKeyPath = GetRootKeyString(hRoot);
	strKeyPath += L"\\" + strSubKey + L"\\" + strSubSubKey;
	int nItem = m_List.InsertItem(m_List.GetItemCount(), strKeyPath);
	m_List.SetItemText(nItem, 1, L"");
	m_List.SetItemText(nItem, 2, L"");
    m_nCnt++;
}


void CRegFindDlg::InsertVlaues(HKEY hRoot, CString strSubKey, CString strValue)
{
	if (!hRoot || strValue.IsEmpty())
	{
		return;
	}

	CString strKeyPath = GetRootKeyString(hRoot);
	strKeyPath += L"\\" + strSubKey;
	int nItem = m_List.InsertItem(m_List.GetItemCount(), strKeyPath);
	m_List.SetItemText(nItem, 1, strValue);
	m_List.SetItemText(nItem, 2, L"");
	m_nCnt++;
}



void CRegFindDlg::InsertData(HKEY hRoot, CString strSubKey, CString strValue, DWORD dwType, PBYTE Data, DWORD dwDataLen)
{
	if (!hRoot || !Data || dwDataLen <= 0)
	{
		return;
	}

	CString strRet, strCmp;
	switch (dwType)
	{
	case REG_SZ:
	case REG_EXPAND_SZ:
		strCmp = strRet = (WCHAR*)Data;
		break;

	case REG_DWORD:
		{
			strCmp.Format(L"%ld", *(PULONG)Data);
			if (m_strFindWhat.GetLength() == strCmp.GetLength())
			{
				BOOL bNumber = TRUE;
				for (int i = 0; i < m_strFindWhat.GetLength(); i++)
				{
					WCHAR ch = m_strFindWhat.GetAt(i);
					if (ch < '0' || ch > '9')
					{
						bNumber = FALSE;
						break;
					}
				}

				if (bNumber && *(PULONG)Data == _wtoi(m_strFindWhat))
				{
					strRet.Format(L"0x%08X (%d)", *(PULONG)Data, *(PULONG)Data);
				}
				else
				{
					return;
				}
			}
			else
			{
				return;
			}
		}
		break;

	case REG_DWORD_BIG_ENDIAN:
		{
			BYTE Value[4] = {0};
			Value[0] = *((PBYTE)Data + 3);
			Value[1] = *((PBYTE)Data + 2);
			Value[2] = *((PBYTE)Data + 1);
			Value[3] = *((PBYTE)Data + 0);
			strCmp.Format(L"%ld", *(PULONG)Value);
			strRet.Format(L"0x%08X (%ld)", *(PULONG)Value, *(PULONG)Value);
		}
		break;
	case REG_QWORD:
		{
			strCmp.Format(L"%ld", *(PQWORD)Data);
			if (m_strFindWhat.GetLength() == strCmp.GetLength())
			{
				BOOL bNumber = TRUE;
				for (int i = 0; i < m_strFindWhat.GetLength(); i++)
				{
					WCHAR ch = m_strFindWhat.GetAt(i);
					if (ch < '0' || ch > '9')
					{
						bNumber = FALSE;
						break;
					}
				}

				if (bNumber && *(PQWORD)Data == _wtoi(m_strFindWhat))
				{
					strRet.Format(L"0x%08X (%ld)", *(PQWORD)Data, *(PQWORD)Data);
				}
				else
				{
					return;
				}
			}
			else
			{
				return;
			}
		}
		break;
	case REG_MULTI_SZ:
		{
			DWORD len = 0;
			while (wcslen((WCHAR*)Data + len))
			{
				strRet += ((WCHAR*)Data + len);
				strRet += L" ";
				len += wcslen((WCHAR*)Data + len) + 1;
			}

			strCmp = strRet;
		}
		break;

	default:
		return;
	}

	if (strCmp.IsEmpty())
	{
		return;
	}

	CString strSubName = strCmp;
	BOOL bInsert = FALSE;

	// ���ִ�Сд & ƥ��ȫ�ַ�
	if (m_bMachCase && m_bMachWholeString)
	{
		if (!strSubName.Compare(m_strFindWhat))
		{
			bInsert = TRUE;
		}
	}

	// ���ִ�Сд,���ǲ�ȫ��ƥ��
	else if (m_bMachCase && !m_bMachWholeString)
	{
		if (-1 != strSubName.Find(m_strFindWhat))
		{
			bInsert = TRUE;
		}
	}	

	// �����ִ�Сд & ȫ��ƥ��
	else if (!m_bMachCase && m_bMachWholeString)
	{
		if (!strSubName.CompareNoCase(m_strFindWhat))
		{
			bInsert = TRUE;
		}
	}

	// �����ִ�Сд & ��ȫ��ƥ��
	else if (!m_bMachCase && !m_bMachWholeString)
	{
		CString strTemp = strSubName;
		strTemp.MakeUpper();

		if (strTemp.Find(m_strFindWhatUpper) != -1)
		{
			bInsert = TRUE;
		}
	}

	if (bInsert)
	{
		CString strKeyPath = GetRootKeyString(hRoot);
		strKeyPath += L"\\" + strSubKey;
		int nItem = m_List.InsertItem(m_List.GetItemCount(), strKeyPath);
		m_List.SetItemText(nItem, 1, strValue);
		m_List.SetItemText(nItem, 2, strRet);
		m_nCnt++;
	}
}



CString CRegFindDlg::GetRootKeyString(HKEY hRoot)
{
	CString strRet;
	if (!hRoot)
	{
		return strRet;
	}

	if (hRoot == HKEY_CLASSES_ROOT)
	{
		strRet = L"HKEY_CLASSES_ROOT";
	}
	else if (hRoot == HKEY_CURRENT_USER)
	{
		strRet = L"HKEY_CURRENT_USER";
	}
	else if (hRoot == HKEY_LOCAL_MACHINE)
	{
		strRet = L"HKEY_LOCAL_MACHINE";
	}
	else if (hRoot == HKEY_USERS)
	{
		strRet = L"HKEY_USERS";
	}
	else if (hRoot == HKEY_CURRENT_CONFIG)
	{
		strRet = L"HKEY_CURRENT_CONFIG";
	}

	return strRet;
}

void CRegFindDlg::OnBnClickedButtonStop()
{
	m_bStop = TRUE;
	InitControl(TRUE);
}



void CRegFindDlg::InitControl(BOOL bOk)
{
	GetDlgItem(IDC_CHECK_KEY)->EnableWindow(bOk);
	GetDlgItem(IDC_CHECK_VALUES)->EnableWindow(bOk);
	GetDlgItem(IDC_CHECK_DATA)->EnableWindow(bOk);
	GetDlgItem(IDC_CHECK_MACH_CASE)->EnableWindow(bOk);
	GetDlgItem(IDC_CHECK_MACH_WHOLE_STRING)->EnableWindow(bOk);
	GetDlgItem(IDC_RADIO_SEARCH_IN)->EnableWindow(bOk);
	GetDlgItem(IDC_RADIO_SEARCH_ALL)->EnableWindow(bOk);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(bOk);
	GetDlgItem(IDC_EDIT_FIND_WHAT)->EnableWindow(bOk);
	GetDlgItem(IDC_EDIT)->EnableWindow(bOk);
	GetDlgItem(IDC_EDIT_FIND_WHAT)->EnableWindow(bOk);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(!bOk);
}

void CRegFindDlg::OnClose()
{
	m_bStop = TRUE;
	if (m_hThread)
	{
		if (WaitForSingleObject(m_hThread, 500) != WAIT_OBJECT_0)
		{
			TerminateThread(m_hThread, 0);
		}

		CloseHandle(m_hThread);
		m_hThread = NULL;
	}


	CDialog::OnClose();

	delete this;
}



void CRegFindDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem = m_List.GetSelectionMark();
	if (nItem != -1)
	{
		CString strKey = m_List.GetItemText(nItem, 0);
		CString strValue = m_List.GetItemText(nItem, 1);
		
		JmpToRegistry(strKey, strValue);
	}
	*pResult = 0;
}


void CRegFindDlg::JmpToRegistry(CString strKey, CString strData)
{
	if (!strKey.IsEmpty() && g_Father)
	{

		((CRegisterManagerDlg*)g_Father)->JmpToReg(strKey, strData);
	}
}