#pragma once

#include "ProcessFunc.h"
#include "MyList.h"
#include "afxwin.h"



enum HS_ENUM_PROCVIEW_TYPE
{
	HS_PROCVIEW_TYPE_DETAIL,
	HS_PROCVIEW_TYPE_THREAD,
	HS_PROCVIEW_TYPE_PRIVILEGE,
	HS_PROCVIEW_TYPE_HANDLE,
	HS_PROCVIEW_TYPE_WINDOW,
	HS_PROCVIEW_TYPE_MODULE,
	HS_PROCVIEW_TYPE_MEMORY
};



// CProcessViewDlg �Ի���

class CProcessViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessViewDlg)

public:
	CProcessViewDlg(ULONG_PTR ulViewType, PHSPROCESSINFO piProcInfo, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProcessViewDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROCESS_VIEW };

	// ����������Ϣ
	PHSPROCESSINFO m_piProcInfo;

	// ������ָ��
	CWnd* m_wParent;

	HICON m_hIcon;

	// ������ʾ����
	ULONG_PTR m_ulViewType;

	CImageList m_TreeKernelImageList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMyList m_viewList;
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickListProcessView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuProcessprivilegeRefresh();
	afx_msg void OnMenuProcessprivilegeEnable();
	afx_msg void OnMenuProcessprivilegeDisable();
	afx_msg void OnPaint();
	CListBox m_typeList;
	afx_msg void OnSelchangeListProcessviewtype();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_ViewIcon;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMenuProcessthreadRefresh();
	afx_msg void OnMenuProcessthreadClosethread();
};
