#include "stdafx.h"
#include "FileFunc.h"
#include "Common.h"



COLUMNSTRUCT g_Column_Drive[] = 
{
	{	L"����",			150	},
	{	L"����",			150	},
	{	L"�ܴ�С",			150	},
	{	L"���ÿռ�",		150	}
};

UINT g_Column_Drive_Count  = 4;	  //�����б�����


COLUMNSTRUCT g_Column_File[] = 
{
	{	L"�ļ�����",		130	},
	{	L"��С",			100	},
	{	L"�ļ�����",		125	},
	{	L"����ʱ��",		125	},
	{	L"�޸�ʱ��",		125	}
};

UINT g_Column_File_Count  = 5;	  //�����б�����

extern int dpix;
extern int dpiy;


void HsInitDriveList(CListCtrl *m_ListCtrl)
{
	m_ListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

	while(m_ListCtrl->DeleteColumn(0));

	UINT i;
	for (i = 0;i<g_Column_Drive_Count;i++)
	{
		if (i == 0 || i == 1)
		{
			m_ListCtrl->InsertColumn(i, g_Column_Drive[i].szTitle,LVCFMT_LEFT,(int)(g_Column_Drive[i].nWidth*(dpix/96.0)));
		}
		else
		{
			m_ListCtrl->InsertColumn(i, g_Column_Drive[i].szTitle,LVCFMT_RIGHT,(int)(g_Column_Drive[i].nWidth*(dpix/96.0)));
		}
		
	}
}




