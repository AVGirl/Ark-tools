
// RegisterManager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRegisterManagerApp:
// �йش����ʵ�֣������ RegisterManager.cpp
//

class CRegisterManagerApp : public CWinApp
{
public:
	CRegisterManagerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRegisterManagerApp theApp;