#include "stdafx.h"
#include "InjectFunc.h"
#include "Common.h"
#include "ProcessFunc.h"

WIN_VERSION  GetWindowsVersion();
WIN_VERSION  WinVersion = WinUnknown;

typedef long (__fastcall *pfnRtlAdjustPrivilege64)(ULONG,ULONG,ULONG,PVOID);
typedef long (__fastcall *pfnRtlAdjustPrivilege32)(ULONG,ULONG,ULONG,PVOID);
pfnRtlAdjustPrivilege64 RtlAdjustPrivilege64;
pfnRtlAdjustPrivilege32 RtlAdjustPrivilege32;

DWORD WINAPI HsRemoteThreadInjectDll(CListCtrl *m_ListCtrl)
{
	CString ProcessPath;
	ULONG_PTR ProcessId = 0;
	CString CmdLine;

	POSITION pos = m_ListCtrl->GetFirstSelectedItemPosition();

	while(pos)
	{
		int nItem = m_ListCtrl->GetNextSelectedItem(pos);

		ProcessPath = m_ListCtrl->GetItemText(nItem,HS_PROCESS_COLUMN_PATH);
		ProcessId = _ttoi(m_ListCtrl->GetItemText(nItem,HS_PROCESS_COLUMN_PID));

		CmdLine += L"-d ";
		CmdLine += m_ListCtrl->GetItemText(nItem,HS_PROCESS_COLUMN_PID);
		CmdLine += L" -p ";
		CmdLine += ProcessPath;
	}
	
	


	if (ProcessId<=4)
	{
		return FALSE;
	}

	if (HsIs32BitFile(ProcessPath.GetBuffer())==TRUE)
	{
		WinVersion = GetWindowsVersion();

		if (WinVersion >= WindowsVista)
		{
			WCHAR wzInjectPath[260] = {0};
			WCHAR* p = NULL;
			CString InjectPath;
			HMODULE module = GetModuleHandle(0);
			GetModuleFileName(module,wzInjectPath,sizeof(wzInjectPath));
			p = wcsrchr(wzInjectPath,L'\\');
			*p = 0;
			InjectPath = wzInjectPath;
			InjectPath += L"\\injectdll32.exe";

			SHELLEXECUTEINFO sei = {sizeof(SHELLEXECUTEINFO)};
			sei.lpVerb = L"runas";
			sei.lpFile = InjectPath.GetBuffer();
			sei.hwnd = GetDesktopWindow();
			sei.nShow = SW_NORMAL;
			sei.lpParameters = CmdLine.GetBuffer();

			if (!ShellExecuteEx(&sei))
			{
				MessageBox(NULL,L"ע��ʧ��",L"ע��ʧ��",0);
			}
		}
		else
		{
			ShellExecuteW(NULL,
				L"open",
				L"injectdll32.exe",
				CmdLine.GetBuffer(),
				L"",
				SW_SHOWMAXIMIZED);
		}

	}
	else
	{
		if (WinVersion >= WindowsVista)
		{
			WCHAR wzInjectPath[260] = {0};
			WCHAR* p = NULL;
			CString InjectPath;
			HMODULE module = GetModuleHandle(0);
			GetModuleFileName(module,wzInjectPath,sizeof(wzInjectPath));
			p = wcsrchr(wzInjectPath,L'\\');
			*p = 0;
			InjectPath = wzInjectPath;
			InjectPath += L"\\injectdll64.exe";

			SHELLEXECUTEINFO sei = {sizeof(SHELLEXECUTEINFO)};
			sei.lpVerb = L"runas";
			sei.lpFile = InjectPath.GetBuffer();
			sei.hwnd = GetDesktopWindow();
			sei.nShow = SW_NORMAL;
			sei.lpParameters = CmdLine.GetBuffer();

			if (!ShellExecuteEx(&sei))
			{
				MessageBox(NULL,L"ע��ʧ��",L"ע��ʧ��",0);
			}

		}
		else
		{
			ShellExecuteW(NULL,
				L"open",
				L"injectdll64.exe",
				CmdLine.GetBuffer(),
				L"",
				SW_SHOWMAXIMIZED);
		}
	}


	
	return TRUE;

	WCHAR wzFileFilter[] = L"Ӧ�ó�����չ (*.dll)\0*.dll\0�����ļ� (*.*)\0*.*\0";
	WCHAR wzFileChoose[] = L"���ļ�";


	CFileDialog FileDlg(TRUE);
	FileDlg.m_ofn.lpstrTitle  = wzFileChoose;
	FileDlg.m_ofn.lpstrFilter = wzFileFilter;

	if (IDOK != FileDlg.DoModal())
	{
		return FALSE;
	}

	

	CString strDllPath = FileDlg.GetPathName();

	BOOL bResult = FALSE;

	if (PathFileExists(strDllPath) && ProcessId > 4)   //ע��������ж������Ҫѡ��64λ��Dll��Ҫ�����64λ
	{
		if (HsIs32BitFile(ProcessPath.GetBuffer())==TRUE &&
			HsIs32BitFile(strDllPath.GetBuffer())==TRUE)
		{
			bResult = HsInjectDll(TRUE,&strDllPath,ProcessId);
		}
		else if (HsIs32BitFile(ProcessPath.GetBuffer())==FALSE &&
			HsIs32BitFile(strDllPath.GetBuffer())==FALSE)
		{
			bResult = HsInjectDll(FALSE,&strDllPath,ProcessId);
		}
	}

	if (bResult == FALSE)
	{
		::MessageBox(NULL,L"Զ���߳�ע��ʧ�ܡ�",L"��Ӱ��ʿ",0);

	}
	else
	{
		::MessageBox(NULL,L"Զ���߳�ע��ɹ���",L"��Ӱ��ʿ",0);
	}

	return bResult;
}


//�ĳ�������64λ����ע��64λ �����32λ����ע��32λ

BOOL HsInjectDll(BOOL Is32Bit, CString* strDllPath, ULONG_PTR ProcessID)
{
	BOOL bResult = FALSE;

	if (ProcessID <= 0)
	{
		return FALSE;
	}


	if (PathFileExists(*strDllPath))
	{
		WinVersion = GetWindowsVersion();

		switch(WinVersion)
		{
		case Windows7:   //ע������������Ե���64λ��Win7
			{

				WCHAR wzPath[MAX_PATH] = {0};
				wcscpy_s(wzPath, strDllPath->GetBuffer());
				strDllPath->ReleaseBuffer();

				bResult = HsInjectDllByRemoteThreadWin7(wzPath,ProcessID,Is32Bit);

				break;
			}

		case WindowsXP:  //��������Ե�32λ��XP
			{
				WCHAR wzPath[MAX_PATH] = {0};
				wcscpy_s(wzPath, strDllPath->GetBuffer());

				strDllPath->ReleaseBuffer();

				bResult = HsInjectDllByRemoteThreadWinXP(wzPath,ProcessID);

				break;
			}
		}
	}
	return bResult;
}


BOOL HsInjectDllByRemoteThreadWin7(const TCHAR* wzDllFile, ULONG_PTR ProcessId, BOOL Is32Bit)
{
	if (NULL == wzDllFile || 0 == ::_tcslen(wzDllFile) || ProcessId == 0 || -1 == _taccess(wzDllFile, 0))
	{
		return FALSE;
	}
	HANDLE                 hProcess = NULL;
	HANDLE                 hThread  = NULL;
	DWORD                  dwRetVal    = 0;
	LPTHREAD_START_ROUTINE FuncAddress = NULL;
	DWORD  dwSize = 0;
	TCHAR* VirtualAddress = NULL;

#ifdef _UNICODE
	FuncAddress = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "LoadLibraryW");
#else
	FuncAddress = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "LoadLibraryA");
#endif

	if (FuncAddress==NULL)
	{
		return FALSE;
	}

	if (Is32Bit == FALSE)
	{
		RtlAdjustPrivilege64=(pfnRtlAdjustPrivilege64)GetProcAddress((HMODULE)(FuncAddress(L"ntdll.dll")),"RtlAdjustPrivilege");

		if (RtlAdjustPrivilege64==NULL)
		{
			return FALSE;
		}
		RtlAdjustPrivilege64(20,1,0,&dwRetVal);  //19
	}
	else
	{
		RtlAdjustPrivilege32=(pfnRtlAdjustPrivilege32)GetProcAddress((HMODULE)(FuncAddress(L"ntdll.dll")),"RtlAdjustPrivilege");

		if (RtlAdjustPrivilege32==NULL)
		{
			return FALSE;
		}
		RtlAdjustPrivilege32(20,1,0,&dwRetVal);  //19
	}


	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, (DWORD)ProcessId);

	if (NULL == hProcess)
	{
		printf("Open Process Fail\r\n");
		return FALSE;
	}

	// ��Ŀ������з����ڴ�ռ�
	dwSize = (DWORD)::_tcslen(wzDllFile) + 1;
	VirtualAddress = (TCHAR*)::VirtualAllocEx(hProcess, NULL, dwSize * sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);  
	if (NULL == VirtualAddress)
	{

		printf("Virtual Process Memory Fail\r\n");
		CloseHandle(hProcess);
		return FALSE;
	}

	// ��Ŀ����̵��ڴ�ռ���д���������(ģ����)
	if (FALSE == ::WriteProcessMemory(hProcess, VirtualAddress, (LPVOID)wzDllFile, dwSize * sizeof(TCHAR), NULL))
	{
		printf("Write Data Fail\r\n");
		VirtualFreeEx(hProcess, VirtualAddress, dwSize, MEM_DECOMMIT);
		CloseHandle(hProcess);
		return FALSE;
	}

	hThread = ::CreateRemoteThread(hProcess, NULL, 0, FuncAddress, VirtualAddress, 0, NULL);
	if (NULL == hThread)
	{
		printf("CreateRemoteThread Fail\r\n");

		VirtualFreeEx(hProcess, VirtualAddress, dwSize, MEM_DECOMMIT);
		CloseHandle(hProcess);
		return FALSE;
	}


	// �ȴ�Զ���߳̽���
	WaitForSingleObject(hThread, INFINITE);
	// ����
	VirtualFreeEx(hProcess, VirtualAddress, dwSize, MEM_DECOMMIT);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;

}




BOOL HsInjectDllByRemoteThreadWinXP(const TCHAR* wzDllFile, ULONG_PTR ProcessId)
{
	//��߱����̵�Ȩ��

	if (!HsInjectUpPrivilige())  //��Ȩ
	{
		printf("Up Privilige Is Error\n");
		return FALSE;
	}

	CStringA *Dll = new CStringA(wzDllFile);


	//���Ǿ�Ҫ����Ҫ�򿪵Ľ���
	HANDLE hProcess = NULL;
	HANDLE hThread  = NULL;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS,false,(DWORD)ProcessId);  //explorer.exe  hProcess
	if (hProcess==NULL)
	{
		printf("Open Process Is Error\n");

		return FALSE;
	}

	char* szDllName = NULL;

	szDllName = (char*)VirtualAllocEx(hProcess,
		NULL,Dll->GetLength()+1,MEM_COMMIT,PAGE_READWRITE);   

	if (szDllName==NULL)
	{

		CloseHandle(hProcess);
		printf("Apply Memory Is Error\n");

		return FALSE;
	}

	//Ȼ��������·��ֱ��д���ڴ�

	if (!WriteProcessMemory(hProcess,szDllName,Dll->GetBuffer(),Dll->GetLength()+1,NULL))
	{

		CloseHandle(hProcess);

		printf("Write Memory Is Error\n");

		return FALSE;
	}

	LPTHREAD_START_ROUTINE StartRoutine = NULL;

	StartRoutine = 
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")),
		"LoadLibraryA");

	if(StartRoutine == NULL)
	{
		printf("Get ProcAddress Error\n");
		return FALSE;
	}

	hThread = CreateRemoteThread(hProcess,NULL,0,StartRoutine,szDllName,0,NULL);

	if (hThread==NULL)
	{
		CloseHandle(hProcess);

		printf("Create Remote Thread Is Error\n");

		return FALSE;
	}

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	VirtualFreeEx(hProcess,szDllName,0,MEM_RELEASE);
	CloseHandle(hProcess);

	return TRUE;
}


BOOL HsInjectUpPrivilige()	//XP
{

	HANDLE hToken = NULL;

	_TOKEN_PRIVILEGES tp = {0};

	LUID luid={0};


	if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hToken))
	{
		printf("OpenProcess Is Error\n");

		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&luid))
	{

		printf("Lookup Is Error\n");

		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;


	if (!AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),NULL,NULL))
	{
		printf("Adjust Is Error\n");

		return FALSE;
	}

	return TRUE;
}


/*
BOOL HsInjectDllByRemoteThreadWinXP(const TCHAR* wzDllFile, ULONG_PTR ProcessId)
{
// ������Ч
if (NULL == wzDllFile || 0 == ::_tcslen(wzDllFile) || ProcessId == 0 || -1 == _taccess(wzDllFile, 0))
{

return FALSE;
}

HANDLE hProcess = NULL;
HANDLE hThread  = NULL;
DWORD dwSize = 0;
TCHAR* VirtualAddress = NULL;
LPTHREAD_START_ROUTINE FuncAddress = NULL;

// ��ȡĿ����̾��
hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, (DWORD)ProcessId);
if (NULL == hProcess)
{
printf("Open Process Fail\r\n");
return FALSE;
}

// ��Ŀ������з����ڴ�ռ�
dwSize = (DWORD)::_tcslen(wzDllFile) + 1;
VirtualAddress = (TCHAR*)::VirtualAllocEx(hProcess, NULL, dwSize * sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);
if (NULL == VirtualAddress)
{

printf("Virtual Process Memory Fail\r\n");
CloseHandle(hProcess);
return FALSE;
}

// ��Ŀ����̵��ڴ�ռ���д���������(ģ����)
if (FALSE == ::WriteProcessMemory(hProcess, VirtualAddress, (LPVOID)wzDllFile, dwSize * sizeof(TCHAR), NULL))
{
printf("Write Data Fail\r\n");
VirtualFreeEx(hProcess, VirtualAddress, dwSize, MEM_DECOMMIT);
CloseHandle(hProcess);
return FALSE;
}

// �� Kernel32.dll �л�ȡ LoadLibrary ������ַ
#ifdef _UNICODE
FuncAddress = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "LoadLibraryW");
#else
FuncAddress = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "LoadLibraryA");
#endif

if (NULL == FuncAddress)
{
printf("Get LoadLibrary Fail\r\n");
VirtualFreeEx(hProcess, VirtualAddress, dwSize, MEM_DECOMMIT);
CloseHandle(hProcess);
return false;
}

// ����Զ���̵߳��� LoadLibrary
hThread = ::CreateRemoteThread(hProcess, NULL, 0, FuncAddress, VirtualAddress, 0, NULL);
if (NULL == hThread)
{
printf("CreateRemoteThread Fail\r\n");

VirtualFreeEx(hProcess, VirtualAddress, dwSize, MEM_DECOMMIT);
CloseHandle(hProcess);
return FALSE;
}

// �ȴ�Զ���߳̽���
WaitForSingleObject(hThread, INFINITE);
// ����
VirtualFreeEx(hProcess, VirtualAddress, dwSize, MEM_DECOMMIT);
CloseHandle(hThread);
CloseHandle(hProcess);

return TRUE;
}


*/




WIN_VERSION  GetWindowsVersion()
{
	OSVERSIONINFOEX	OsVerInfoEx;
	OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&OsVerInfoEx); // ע��ת������

	switch (OsVerInfoEx.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		{
			if (OsVerInfoEx.dwMajorVersion <= 4 )
			{
				return WindowsNT;
			}
			if (OsVerInfoEx.dwMajorVersion == 5 && OsVerInfoEx.dwMinorVersion == 0)
			{
				return Windows2000;
			}

			if (OsVerInfoEx.dwMajorVersion == 5 && OsVerInfoEx.dwMinorVersion == 1)
			{
				return WindowsXP;
			}
			if (OsVerInfoEx.dwMajorVersion == 5 && OsVerInfoEx.dwMinorVersion == 2)
			{
				return Windows2003;
			}
			if (OsVerInfoEx.dwMajorVersion == 6 && OsVerInfoEx.dwMinorVersion == 0)
			{
				return WindowsVista;
			}

			if (OsVerInfoEx.dwMajorVersion == 6 && OsVerInfoEx.dwMinorVersion == 1)
			{
				return Windows7;
			}
			if (OsVerInfoEx.dwMajorVersion == 6 && OsVerInfoEx.dwMinorVersion == 2 )
			{
				return Windows8;
			}
			if (OsVerInfoEx.dwMajorVersion == 6 && OsVerInfoEx.dwMinorVersion == 3 )
			{
				return Windows8_1;
			}
			if (OsVerInfoEx.dwMajorVersion == 10 && OsVerInfoEx.dwMinorVersion == 0 )
			{
				return Windows10;
			}

			break;
		}

	default:
		{
			return WinUnknown;
		}
	}

	return WinUnknown;
}



