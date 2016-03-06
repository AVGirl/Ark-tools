// ServiceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeavenShadow.h"
#include "ServiceDlg.h"
#include "afxdialogex.h"
#include "HeavenShadowDlg.h"
#include "Common.h"

#include "AlertWnd.h"


// CServiceDlg �Ի���

IMPLEMENT_DYNAMIC(CServiceDlg, CDialog)

CServiceDlg::CServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServiceDlg::IDD, pParent)
{
	m_wParent = pParent;
}

CServiceDlg::~CServiceDlg()
{
}

void CServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TEST2, m_demoBtn);
}


BEGIN_MESSAGE_MAP(CServiceDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_TEST2, &CServiceDlg::OnBnClickedButtonTest2)
END_MESSAGE_MAP()


// CServiceDlg ��Ϣ�������



void CServiceDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CRect   rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(255,255,255));
}


BOOL CServiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CServiceDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������

	if (bShow == TRUE)
	{
		((CHeavenShadowDlg*)m_wParent)->m_bNowWindow = HS_DIALOG_SERVICE;

		((CHeavenShadowDlg*)m_wParent)->m_btnServ.EnableWindow(FALSE);

		HsSendStatusDetail(L"ϵͳ�������ڼ��ء�");
		HsSendStatusTip(L"����");

	}

	
}


BOOL CServiceDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN ||pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CServiceDlg::OnBnClickedButtonTest2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CAlertWnd *dlg = new CAlertWnd(this);

	dlg->Create(IDD_DIALOG_ALERT,GetDesktopWindow());

	dlg->ShowWindow(SW_SHOW);
}
