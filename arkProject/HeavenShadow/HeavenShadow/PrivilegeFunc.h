#pragma once
#include "stdafx.h"

#include "MyList.h"






VOID HsInitPrivilegeList(CMyList *m_ListCtrl);


//��ѯ����Ȩ��
VOID HsQueryProcessPrivilege(CMyList *m_ListCtrl);

VOID HsProcessPrivilegePopupMenu(CMyList *m_ListCtrl, CWnd* parent);

//��������Ȩ��
BOOL HsAdjustPrivilege(ULONG_PTR ProcessID, WCHAR* PrivilegeName, BOOL bIsEnable);