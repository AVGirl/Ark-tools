// HsAboutDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeavenShadow.h"
#include "HsAboutDlg.h"
#include "afxdialogex.h"

#include "HeavenShadowDlg.h"


extern BOOL bDriverIsOK;
extern WIN_VERSION GetWindowsVersion();
extern WIN_VERSION WinVersion;

extern int dpix;
extern int dpiy;



// CHsAboutDlg �Ի���

IMPLEMENT_DYNAMIC(CHsAboutDlg, CDialog)

CHsAboutDlg::CHsAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHsAboutDlg::IDD, pParent)
{
	m_wParent = pParent;
}

CHsAboutDlg::~CHsAboutDlg()
{
}

void CHsAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_CHECK_ALT, m_TextCheckAlt);
	DDX_Control(pDX, IDC_STATIC_CHECK_ALT, m_PicCheckAlt);
	DDX_Control(pDX, IDC_STATIC_SYS_BIT, m_PicSysBit);
	DDX_Control(pDX, IDC_TEXT_SYS_BIT, m_TextSysBit);
	DDX_Control(pDX, IDC_TEXT_SOFT_BIT, m_TextSoftBit);
	DDX_Control(pDX, IDC_STATIC_HOMECHECK, m_CheckBtn);
}


BEGIN_MESSAGE_MAP(CHsAboutDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_HOMECHECK, &CHsAboutDlg::OnClickedStaticHomecheck)
END_MESSAGE_MAP()


// CHsAboutDlg ��Ϣ�������


BOOL CHsAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CHsAboutDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������


	if (bShow == TRUE)
	{
		((CHeavenShadowDlg*)m_wParent)->m_bNowWindow = HS_DIALOG_ABOUT;

		((CHeavenShadowDlg*)m_wParent)->m_btnAbou.EnableWindow(FALSE);

		HsSendStatusDetail(L"��Ӱ��ʿ��ȫ�������ߡ�");

		HsSendStatusTip(L"��Ӱ��ʿ");


		if (sizeof(ULONG_PTR) == sizeof(ULONG32))
		{
			m_TextSoftBit.SetWindowTextW(L"��Ӱ��ʿ 32λ�档");
		}
		else
		{
			m_TextSoftBit.SetWindowTextW(L"��Ӱ��ʿ 64λ�档");
		}

		CString SysBit;

		WinVersion = GetWindowsVersion();

		switch(WinVersion)
		{
		case Windows7:
			{
				SysBit += L"Windows7";
				break;
			}
		case WindowsXP:
			{
				SysBit += L"WindowsXP";
				break;
			}
		default:
			{
				SysBit += L"Other";
			}
		}

		if (HsIs64BitWindows())
		{
			SysBit += L" 64λ����ϵͳ��";
		}
		else
		{
			SysBit += L" 32λ����ϵͳ��";
		}

		m_TextSysBit.SetWindowTextW(SysBit.GetBuffer());

		if (bDriverIsOK)
		{
			HINSTANCE hIns_proc = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_CHECK_ALT_V),RT_GROUP_ICON);
			HBITMAP   hBmp_proc = ::LoadBitmap(hIns_proc, MAKEINTRESOURCE(IDB_BITMAP_CHECK_ALT_V));
			m_PicCheckAlt.SetBitmap(hBmp_proc);

			m_TextCheckAlt.SetWindowTextW(L"�������ں˲�ͨѶ������");
		}
		else
		{
			HINSTANCE hIns_proc = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_CHECK_ALT_X),RT_GROUP_ICON);
			HBITMAP   hBmp_proc = ::LoadBitmap(hIns_proc, MAKEINTRESOURCE(IDB_BITMAP_CHECK_ALT_X));
			m_PicCheckAlt.SetBitmap(hBmp_proc);

			m_TextCheckAlt.SetWindowTextW(L"�������ں˲�ͨѶ�쳣��");
		}

	}
}


void CHsAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CRect   rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(255,255,255));

	ClientToScreen(&rect);

	CRect picrect;
	m_PicSysBit.GetWindowRect(&picrect);
	picrect.left = picrect.left - rect.left;

	CPoint startPoint;
	startPoint.x = (LONG)(picrect.left-(20)*(dpix/96.0));
	startPoint.y = 13;
	CPoint endPoint;
	endPoint.x = (LONG)(picrect.left-(20)*(dpix/96.0));
	endPoint.y = rect.Height()-13;


	COLORREF m_Color(RGB(225,225,255));

	CClientDC aDC(this); //CClientDC�Ĺ��캯����Ҫһ�����������������ָ���ͼ���ڵ�ָ�룬������thisָ��Ϳ�����
	CPen pen(PS_SOLID,1,m_Color); ////����һ����������󣬹���ʱ���û�������
	aDC.SelectObject(&pen);
	aDC.MoveTo(startPoint);
	aDC.LineTo(endPoint);
}


BOOL CHsAboutDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN ||pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CHsAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����


	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CHsAboutDlg::OnClickedStaticHomecheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HINSTANCE hIns_proc = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_HOMECHECK_BTN_CLICK),RT_GROUP_ICON);
	HBITMAP   hBmp_proc = ::LoadBitmap(hIns_proc, MAKEINTRESOURCE(IDB_BITMAP_HOMECHECK_BTN_CLICK));
	m_CheckBtn.SetBitmap(hBmp_proc);

	Sleep(100);
	((CHeavenShadowDlg*)m_wParent)->OnClickedStaticProcess();
}
