#pragma once


// CToolsDlg �Ի���

class CToolsDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolsDlg)

public:
	CToolsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CToolsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TOOLS };

	CWnd* m_wParent;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
