
// InlineHookSSSDTӦ�ó���.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CInlineHookSSSDTӦ�ó���App:
// �йش����ʵ�֣������ InlineHookSSSDTӦ�ó���.cpp
//

class CInlineHookSSSDTӦ�ó���App : public CWinApp
{
public:
	CInlineHookSSSDTӦ�ó���App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CInlineHookSSSDTӦ�ó���App theApp;