#pragma once
#include "afxwin.h"


// CServiceDlg �Ի���

class CServiceDlg : public CDialog
{
	DECLARE_DYNAMIC(CServiceDlg)

public:
	CServiceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServiceDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SERVICE };


	CWnd* m_wParent;



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CButton m_demoBtn;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonTest2();
};
