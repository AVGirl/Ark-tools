#pragma once


// CKeyDlg �Ի���


typedef enum DLG_TYPE
{
	enumRenameKey,
	enumCreateKey,
	enumSetValueKey,
	enumRenameValue,
};
class CKeyDlg : public CDialog
{
	DECLARE_DYNAMIC(CKeyDlg)

public:
	CKeyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKeyDlg();

	DLG_TYPE m_nDlgType;
	
	
// �Ի�������
	enum { IDD = IDD_KEY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strKeyNameStatic;
	CString m_strKeyNameEdit;
	virtual BOOL OnInitDialog();
};
