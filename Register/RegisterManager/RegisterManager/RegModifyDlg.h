#pragma once
#include "afxwin.h"


// CRegModifyDlg �Ի���

class CRegModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegModifyDlg)

public:
	CRegModifyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegModifyDlg();

// �Ի�������
	enum { IDD = IDD_REG_MODIFY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strValueNameStatic;

	CString m_strValueNameEdit;
	CString m_strValueDataStatic;
	CString m_strValueDataEdit;
};
