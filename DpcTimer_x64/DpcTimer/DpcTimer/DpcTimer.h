
// DpcTimer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDpcTimerApp:
// �йش����ʵ�֣������ DpcTimer.cpp
//

class CDpcTimerApp : public CWinApp
{
public:
	CDpcTimerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDpcTimerApp theApp;