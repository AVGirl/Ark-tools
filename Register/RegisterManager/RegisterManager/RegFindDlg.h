#pragma once
#include "afxcmn.h"


// CRegFindDlg �Ի���

class CRegFindDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegFindDlg)

public:
	CRegFindDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegFindDlg();
	HKEY CRegFindDlg::GetRootKey(CString strKey);
	VOID CRegFindDlg::SearchRegistry();
	void CRegFindDlg::EnumKeys(HKEY hRoot, CString strSubKey);
	void CRegFindDlg::InsertKeys(HKEY hRoot, CString strSubKey, CString strSubSubKey);
	CString CRegFindDlg::GetRootKeyString(HKEY hRoot);
	void CRegFindDlg::InsertVlaues(HKEY hRoot, CString strSubKey, CString strValue);
	void CRegFindDlg::InsertData(HKEY hRoot, CString strSubKey, CString strValue, DWORD dwType, PBYTE Data, DWORD dwDataLen);
	void CRegFindDlg::InitControl(BOOL bOk);
	void CRegFindDlg::JmpToRegistry(CString strKey, CString strData);
// �Ի�������
	enum { IDD = IDD_FIND_REG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	CString m_strSearchInKey;
	int m_nRadio;
	afx_msg void OnBnClickedRadioSearchIn();
	afx_msg void OnBnClickedButtonStart();
	afx_msg LRESULT UpdateUI(WPARAM,LPARAM);
	CString m_strFindWhat;
	CString m_strFindWhatUpper;		// ��д����������
	BOOL m_bStop;
	HANDLE m_hThread;
	BOOL m_bKeys;
	BOOL m_bMachCase;
	BOOL m_bMachWholeString;
	BOOL m_bData;
	BOOL m_bValues;
	ULONG m_nCnt;    //��¼���ҵĸ���
	CString m_strSearchResult;
	CAnimateCtrl m_ctlAnimate;
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnClose();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
