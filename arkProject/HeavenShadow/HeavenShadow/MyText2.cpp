// MyText2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeavenShadow.h"
#include "MyText2.h"


// CMyText2

IMPLEMENT_DYNAMIC(CMyText2, CStatic)

CMyText2::CMyText2()
{

}

CMyText2::~CMyText2()
{
}


BEGIN_MESSAGE_MAP(CMyText2, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CMyText2 ��Ϣ�������



HBRUSH CMyText2::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(75,75,75));

	return (HBRUSH)::GetStockObject(NULL_BRUSH);
}


LRESULT CMyText2::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (message == WM_SETTEXT)
	{
		CRect rect;

		GetWindowRect(&rect);

		CWnd* pParent = GetParent();

		if (pParent)
		{
			pParent->ScreenToClient(&rect);
			pParent->InvalidateRect(&rect);
		}
	}

	return CStatic::DefWindowProc(message, wParam, lParam);
}

