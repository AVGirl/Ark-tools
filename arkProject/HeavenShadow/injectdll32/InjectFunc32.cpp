#include "stdafx.h"

#include "InjectFunc32.h"

WIN_VERSION  GetWindowsVersion();
WIN_VERSION  WinVersion = WinUnknown;

typedef long (__fastcall *pfnRtlAdjustPrivilege64)(ULONG,ULONG,ULONG,PVOID);
typedef long (__fastcall *pfnRtlAdjustPrivilege32)(ULONG,ULONG,ULONG,PVOID);
pfnRtlAdjustPrivilege64 RtlAdjustPrivilege64;
pfnRtlAdjustPrivilege32 RtlAdjustPrivilege32;

static OPENFILENAME ofn ;

DWORD HsRemoteThreadInjectDll32(WCHAR* ProcessInfo)
{
	ULONG_PTR ProcessId = 0;
	WCHAR*    ProcessPath = NULL;
	WCHAR wzFileFilter[] = L"Ӧ�ó�����չ (*.dll)\0*.dll\0�����ļ� (*.*)\0*.*\0";
	WCHAR wzFileChoose[] = L"���ļ�";

	int i;
	int j;

	WCHAR szPid[10] = {0};

	for (i = 0; _wcsnicmp(ProcessInfo+i,L"-d ",wcslen(L"-d "))!=0 && i<=10 ;i++);

	if (i == 10)
	{
		return FALSE;
	}

	WCHAR* PidTemp = ProcessInfo + i + wcslen(L"-d ");

	j = i;

	for ( ; _wcsnicmp(ProcessInfo + j,L" -p ",wcslen(L" -p "))!=0 && j-i<=10 ;j++);

	if (j-i==10)
	{
		return FALSE;
	}

	memcpy(szPid,PidTemp,j-i);

	ProcessId   = _ttoi(szPid);

	ProcessPath = ProcessInfo + j + wcslen(L" -p ");

	static TCHAR szFileName[MAX_PATH], szTitleName[MAX_PATH] ;
	HWND hwnd;
	hwnd=GetForegroundWindow(); //��ȡǰ̨���ھ�����������е�ǰ̨���ھ��ǿ���̨���ڡ� 
	PopFileInitialize (hwnd);   //��ʼ��ofn
	PopFileOpenDlg(hwnd, szFileName, szTitleName);//���ļ��Ի���

	CString strDllPath = szFileName;

	BOOL bResult = FALSE;

	if (PathFileExists(strDllPath))
	{
		if (HsIs32BitFile(ProcessPath)==TRUE &&
			HsIs32BitFile(strDllPath.GetBuffer())==TRUE)
		{
			bResult = HsInjectDll(TRUE,&strDllPath,ProcessId);
		}
	}

	if (bResult == FALSE)
	{
		::MessageBox(NULL,L"Զ���߳�ע��ʧ�ܡ�",L"��Ӱ��ʿ",MB_SYSTEMMODAL|MB_OK);

	}
	else
	{
		::MessageBox(NULL,L"Զ���߳�ע��ɹ���",L"��Ӱ��ʿ",MB_SYSTEMMODAL|MB_OK);
	}

	return bResult;
}





BOOL __stdcall HsIs32BitFile(const WCHAR * pwszFullPath)
{
	FILE * peFile = NULL;
	_wfopen_s(&peFile, pwszFullPath, L"rb");
	if (peFile == NULL)
	{
		fclose(peFile);
		return -1;
	}

	IMAGE_DOS_HEADER imageDosHeader;
	fread(&imageDosHeader, sizeof(IMAGE_DOS_HEADER), 1, peFile);
	if (imageDosHeader.e_magic != IMAGE_DOS_SIGNATURE)
	{
		fclose(peFile);
		return FALSE;
	}

	IMAGE_NT_HEADERS imageNtHeaders;
	fseek(peFile, imageDosHeader.e_lfanew, SEEK_SET);
	fread(&imageNtHeaders, sizeof(IMAGE_NT_HEADERS), 1, peFile);
	fclose(peFile);
	if (imageNtHeaders.Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}

	if (imageNtHeaders.FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
	{
		return TRUE;
	}
	// 	if (imageNtHeaders.FileHeader.Machine == IMAGE_FILE_MACHINE_IA64 ||
	// 		imageNtHeaders.FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
	// 	{
	// 		return FALSE;	//64BIT
	// 	}

	return FALSE;
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

				HsDebugPrivilege(SE_DEBUG_NAME, TRUE);
				bResult = HsInjectDllByRemoteThreadWin7(wzPath,ProcessID,Is32Bit);
				HsDebugPrivilege(SE_DEBUG_NAME, FALSE);

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









BOOL HsDebugPrivilege(const WCHAR *pName, BOOL bEnable)
{
	BOOL              bResult = TRUE;
	HANDLE            hToken;
	TOKEN_PRIVILEGES  TokenPrivileges;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		bResult = FALSE;
		return bResult;
	}
	TokenPrivileges.PrivilegeCount = 1;
	TokenPrivileges.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

	LookupPrivilegeValue(NULL, pName, &TokenPrivileges.Privileges[0].Luid);
	AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS)
	{
		bResult = FALSE;
	}

	CloseHandle(hToken);
	return bResult;	
}


void PopFileInitialize (HWND hwnd)
{
	static TCHAR szFilter[] = TEXT ("Ӧ�ó�����չ (*.dll)\0*.dll\0\0");

	ofn.lStructSize       = sizeof (OPENFILENAME) ;
	ofn.hwndOwner         = hwnd ;
	ofn.hInstance         = NULL ;
	ofn.lpstrFilter       = szFilter ;
	ofn.lpstrCustomFilter = NULL ;
	ofn.nMaxCustFilter    = 0 ;
	ofn.nFilterIndex      = 0 ;
	ofn.lpstrFile         = NULL ;          // Set in Open and Close functions
	ofn.nMaxFile          = MAX_PATH ;
	ofn.lpstrFileTitle    = NULL ;          // Set in Open and Close functions
	ofn.nMaxFileTitle     = MAX_PATH ;
	ofn.lpstrInitialDir   = NULL ;
	ofn.lpstrTitle        = NULL ;
	ofn.Flags             = 0 ;             // Set in Open and Close functions
	ofn.nFileOffset       = 0 ;
	ofn.nFileExtension    = 0 ;
	ofn.lpstrDefExt       = TEXT ("dll") ;
	ofn.lCustData         = 0L ;
	ofn.lpfnHook          = NULL ;
	ofn.lpTemplateName    = NULL ;
}

BOOL PopFileOpenDlg (HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
	ofn.hwndOwner         = hwnd ;
	ofn.lpstrFile         = pstrFileName ;
	ofn.lpstrFileTitle    = pstrTitleName ;
	ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;

	return GetOpenFileName (&ofn) ;
}
