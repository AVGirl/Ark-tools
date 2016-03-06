#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSystemDlg �Ի���

class CSystemDlg : public CDialog
{
	DECLARE_DYNAMIC(CSystemDlg)

public:
	CSystemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSystemDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SYSTEM };

	CWnd* m_wParent;

	void InitSystemList(void);

	CImageList m_TreeSystemImageList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	CXTPEditListBox m_ListSystem;
	CListCtrl m_ListSystemCtrl;
	afx_msg void OnSelchangeListSystem();
	afx_msg void OnRclickListSystemCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuSyskIoTimerRefresh();
	afx_msg void OnMenuSyskOperiotimer();
	afx_msg void OnMenuSyskCallbackRefresh();
	afx_msg void OnMenuSyskCallbackRemove();
	afx_msg void OnMenuSyskRemoveiotimer();
	afx_msg void OnMenuSyskDpctimerRefresh();
	afx_msg void OnMenuSyskDpctimerRemove();
	afx_msg void OnMenuSyskFilterRefresh();
	afx_msg void OnMenuSyskFilterRemovefilter();
};
