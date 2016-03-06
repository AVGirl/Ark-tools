// MyCheckBtn.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeavenShadow.h"
#include "MyCheckBtn.h"


// CMyCheckBtn

IMPLEMENT_DYNAMIC(CMyCheckBtn, CStatic)

CMyCheckBtn::CMyCheckBtn()
{

}

CMyCheckBtn::~CMyCheckBtn()
{
}


BEGIN_MESSAGE_MAP(CMyCheckBtn, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CMyCheckBtn ��Ϣ�������




void CMyCheckBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TRACKMOUSEEVENT csTME;
	csTME.cbSize = sizeof (csTME);
	csTME.dwFlags = TME_LEAVE|TME_HOVER;
	csTME.hwndTrack = m_hWnd;		// ָ��Ҫ ׷�� �Ĵ��� 
	csTME.dwHoverTime = 50;			// ����ڰ�ť��ͣ������ 50ms ������Ϊ״̬Ϊ HOVER
	::_TrackMouseEvent (&csTME);	// ���� Windows �� WM_MOUSELEAVE �� WM_MOUSEHOVER �¼�֧��
	CStatic::OnMouseMove(nFlags, point);
}


void CMyCheckBtn::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_HOMECHECK_BTN),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_HOMECHECK_BTN));

	SetBitmap(hBmp);

	CStatic::OnMouseLeave();
}


void CMyCheckBtn::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_HOMECHECK_BTN_HOVER),RT_GROUP_ICON);

	HBITMAP   hBmp = ::LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP_HOMECHECK_BTN_HOVER));

	SetBitmap(hBmp);

	CStatic::OnMouseHover(nFlags, point);
}


BOOL CMyCheckBtn::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ʹ���Լ��������ƺ�����
	::SetCursor(::LoadCursorW(NULL,IDC_HAND));

	return TRUE;
}
