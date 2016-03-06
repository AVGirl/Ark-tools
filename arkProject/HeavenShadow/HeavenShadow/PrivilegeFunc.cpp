#include "stdafx.h"
#include "PrivilegeFunc.h"
#include "Common.h"

#include "resource.h"

extern HANDLE g_hDevice;

extern ULONG_PTR g_ulProcessId;

typedef struct _PRIVILEGE_DATA_
{
	ULONG_PTR  ProcessID;
	TOKEN_PRIVILEGES TokenPrivileges;
}PRIVILEGEDATA, *PPRIVILEGEDATA;



COLUMNSTRUCT g_Column_Privilege[] = 
{
	{	L"Ȩ������",			170	},
	{	L"Ȩ������",			200	},
	{	L"Ȩ��״̬",			150	}
};

UINT g_Column_Privilege_Count  = 3;	  //�����б�����



extern int dpix;
extern int dpiy;



void HsInitPrivilegeList(CMyList *m_ListCtrl)
{

	while(m_ListCtrl->DeleteColumn(0));
	m_ListCtrl->DeleteAllItems();

	m_ListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

	UINT i;
	for (i = 0;i<g_Column_Privilege_Count;i++)
	{
		m_ListCtrl->InsertColumn(i, g_Column_Privilege[i].szTitle,LVCFMT_LEFT,(int)(g_Column_Privilege[i].nWidth*(dpix/96.0)));
	}
}




VOID
HsQueryProcessPrivilege(CMyList *m_ListCtrl)
{

	ULONG_PTR  ProcessID = g_ulProcessId;

	ULONG dwReturnSize = 0;
	ULONG dwRet = 0;

	PVOID Temp = NULL;
	ULONG nSize = 1000;

	if (ProcessID == 0)
	{
		return;
	}

	m_ListCtrl->DeleteAllItems();

	Temp = (PTOKEN_PRIVILEGES)malloc(sizeof(char)*nSize);
	if (!Temp)
	{
		return;
	}

	dwRet = DeviceIoControl(g_hDevice,HS_IOCTL(HS_IOCTL_PROC_PROCESSPRIVILEGE),
		&ProcessID,
		sizeof(ULONG_PTR),
		Temp,
		nSize,
		&dwReturnSize,
		NULL);

	if (dwRet && ((PTOKEN_PRIVILEGES)Temp)->PrivilegeCount > 0)
	{
		for (ULONG i = 0; i <((PTOKEN_PRIVILEGES)Temp)->PrivilegeCount;i++)
		{
			WCHAR PrivilegeName[MAX_PATH] = {0};
			WCHAR DisplayName[MAX_PATH] = {0};
			DWORD LanguageId = 0;
			DWORD dwRet1 = MAX_PATH;
			DWORD dwRet2 = MAX_PATH;

			LookupPrivilegeName(NULL, &((PTOKEN_PRIVILEGES)Temp)->Privileges[i].Luid, PrivilegeName, &dwRet1);
			LookupPrivilegeDisplayName(NULL,PrivilegeName,DisplayName,&dwRet2,&LanguageId);


			printf("%S\r\n",PrivilegeName);
			wprintf(L"%s",DisplayName);

			if (wcslen(PrivilegeName) == 0)
			{
				break;
			}

			m_ListCtrl->InsertItem(i, PrivilegeName);

			m_ListCtrl->SetItemText(i,1,DisplayName);

			if (((PTOKEN_PRIVILEGES)Temp)->Privileges[i].Attributes & 1)
			{
				//printf("\t\tDefault Enabled");
				m_ListCtrl->SetItemText(i,2,L"Default Enabled");
			}
			else if ( ((PTOKEN_PRIVILEGES)Temp)->Privileges[i].Attributes & 2 )
			{
				//printf("\t\tEnabled");
				m_ListCtrl->SetItemText(i,2,L"Enabled");
			}
			else
			{
				//printf("\t\tDisabled");
				m_ListCtrl->SetItemText(i,2,L"Disabled");
			}
		}
	}


	//����IO ������

	if (dwRet==0)
	{
		//cout<<"Send IoCode Error"<<endl;
	}
	if (Temp!=NULL)
	{
		free(Temp);
	}

	return;
}



//Ȩ�޽��浯���˵�
VOID HsProcessPrivilegePopupMenu(CMyList *m_ListCtrl, CWnd* parent)
{
	CMenu	popup;
	popup.LoadMenu(IDR_MENU_PROCESS_PRIVILEGE);		//���ز˵���Դ
	CMenu*	pM = popup.GetSubMenu(0);				//��ò˵�������
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();
	if (m_ListCtrl->GetSelectedCount() == 0)		//���û��ѡ��
	{ 
		for (int i = 0;i<count;i++)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
		}
	}
	else
	{
		POSITION pos = m_ListCtrl->GetFirstSelectedItemPosition();

		while(pos)
		{
			int iItem = m_ListCtrl->GetNextSelectedItem(pos);

			if (_wcsnicmp(m_ListCtrl->GetItemText(iItem,2),L"Default Enabled",15) == 0)
			{
				for (int i = 1;i<count;i++)
				{
					pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //�˵�ȫ�����
				}
			}
			else if (_wcsnicmp(m_ListCtrl->GetItemText(iItem,2),L"Enabled",7) == 0)
			{
				pM->CheckMenuRadioItem(2,3,2, MF_BYPOSITION | MF_CHECKED);
			}
			else
			{
				pM->CheckMenuRadioItem(2,3,3, MF_BYPOSITION | MF_CHECKED);
			}
		}
	}
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, parent);
}



BOOL HsAdjustPrivilege(ULONG_PTR ProcessID, WCHAR* PrivilegeName, BOOL bIsEnable)
{

	PRIVILEGEDATA PrivilegeData = {0};
	TOKEN_PRIVILEGES TokenPrivileges;
	TokenPrivileges.PrivilegeCount = 1;


	if (LookupPrivilegeValueW(NULL, PrivilegeName, &TokenPrivileges.Privileges[0].Luid))
	{
		if (bIsEnable)
		{
			TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		}
		else
		{

			TokenPrivileges.Privileges[0].Attributes = 0;
		}


		//������������Ϣ

		PrivilegeData.ProcessID = ProcessID;
		PrivilegeData.TokenPrivileges = TokenPrivileges;


		if (g_hDevice==NULL)
		{
			return FALSE;
		}

		ULONG dwReturnSize = 0;
		ULONG dwRet = 0;

		BOOL bFeedback = FALSE;


		dwRet = DeviceIoControl(g_hDevice,HS_IOCTL(HS_IOCTL_PROC_PRIVILEGE_ADJUST),
			&PrivilegeData,
			sizeof(PRIVILEGEDATA),
			&bFeedback,
			sizeof(BOOL),
			&dwReturnSize,
			NULL);

		//����IO ������


		return bFeedback;
	}

	return FALSE;
}


