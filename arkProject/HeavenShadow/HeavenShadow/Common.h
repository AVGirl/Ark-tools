#pragma once

#include "stdafx.h"

#include <WinIoCtl.h>

typedef struct _COLUMNSTRUCT
{
	WCHAR*	szTitle;
	UINT    nWidth;
}COLUMNSTRUCT;


enum HS_ENUM_DLG_MSG	//�ӶԻ������Ի���֮��ͨ��ö��
{
	HS_MESSAGE_STATUSDETAIL = 5425,	//�޸�״̬����ϸ��Ϣ
	HS_MESSAGE_STATUSTIP
};

enum HS_ENUM_IOCTL
{
	HS_IOCTL_PROC = 0x100,					//�������
	HS_IOCTL_PROC_SENDSELFPID,				//�����Լ���PID��������
	HS_IOCTL_PROC_PROCESSCOUNT,				//���̼���
	HS_IOCTL_PROC_PROCESSLIST,				//�оٽ����б�
	HS_IOCTL_PROC_PROTECTPROCESS,			//��������
	HS_IOCTL_PROC_KILLPROCESSBYFORCE,		//ǿ�ƹرս���
	HS_IOCTL_PROC_PROCESSTHREAD,			//�߳�
	HS_IOCTL_PROC_PROCESSTHREADMODULE,		//�߳�ģ��
	HS_IOCTL_PROC_PROCESSPRIVILEGE,			//�оٽ���Ȩ��
	HS_IOCTL_PROC_PRIVILEGE_ADJUST,			//�ı����Ȩ��
	HS_IOCTL_PROC_PROCESSHANDLE,			//���
	HS_IOCTL_PROC_PROCESSWINDOW,			//����
	HS_IOCTL_PROC_PROCESSMODULE,			//����ģ��
	HS_IOCTL_PROC_PROCESSMEMORY,			//�����ڴ�

	HS_IOCTL_MODU = 0x180,					//ģ�����
	HS_IOCTL_MODU_MODULELIST,				//�о�ϵͳģ���б�
	HS_IOCTL_MODU_REMOVEMODULE,				//ж��ϵͳģ��

	HS_IOCTL_KRNL = 0x200,					//�ں˹������
	HS_IOCTL_KRNL_SSDTLIST,					//�о�SSDT�б�
	HS_IOCTL_KRNL_KISRVTAB,					//��ȡ�����
	HS_IOCTL_KRNL_RESUMESSDT,				//�ָ�SSDT����
	HS_IOCTL_KRNL_SSSDTLIST,				//�о�SSSDT�б�
	HS_IOCTL_KRNL_WIN32KSERVICE,			//��ȡWin32k�����
	HS_IOCTL_KRNL_KRNLFILE,					//��ȡ�ں��ļ�		//�����ĸ��ļ���InputBuffer�Ĳ�������ȷ��
	HS_IOCTL_KRNL_KRNLIAT,					//��ȡ�ں˵����
	HS_IOCTL_KRNL_KRNLEAT,					//��ȡ�ں˵�����

	HS_IOCTL_SYSK = 0x280,					//�ں����
	HS_IOCTL_SYSK_SYSTHREAD,				//�ں��߳�
	HS_IOCTL_SYSK_IOTIMER,					//IOTIMER
	HS_IOCTL_SYSK_OPERIOTIMER,				//IOTIMER�л�
	HS_IOCTL_SYSK_REMOVEIOTIMER,			//IOTIMER�Ƴ�
	HS_IOCTL_SYSK_CALLBACKLIST,				//ϵͳ�ص�����������
	HS_IOCTL_SYSK_REMOVECALLBACK,			//ɾ��ϵͳ�ص�
	HS_IOCTL_SYSK_DPCTIMER,					//DPCTimer
	HS_IOCTL_SYSK_REMOVEDPCTIMER,			//DPCTimerɾ��
	HS_IOCTL_SYSK_FILTERDRIVER,				//��������
	HS_IOCTL_SYSK_FILTERUNLOAD,				//ж�ع�������
};

enum HS_DLG_NUM
{
	HS_DIALOG_ABOUT = 1,		//���ڶԻ���
	HS_DIALOG_PROCESS,			//���̶Ի���
	HS_DIALOG_MODULE,			//ģ��Ի���
	HS_DIALOG_SYSTEM,			//�ں˶Ի���
	HS_DIALOG_KERNEL,			//�ں˹��ӶԻ���
	HS_DIALOG_SERVICE,			//����Ի���
	HS_DIALOG_FILE,				//�ļ��Ի���
	HS_DIALOG_SETTING,			//���öԻ���
	HS_DIALOG_TOOLS				//������Ի���
};

typedef enum _WIN_VERSION
{
	WindowsNT,
	Windows2000,
	WindowsXP,
	Windows2003,
	WindowsVista,
	Windows7,
	Windows8,
	Windows8_1,
	Windows10,
	WinUnknown
}WIN_VERSION;

//��֧�ַ��������û������
#define HS_DEVICE_NAME                  L"\\Device\\HeavenShadowDevice"             // Driver Name
#define HS_LINK_NAME					L"\\\\.\\HeavenShadowLink"                  // Win32 Link Name

#define HS_EVENT_PROCESS_NAME			L"\\BaseNamedObjects\\HeavenShadowProcessEvent"

#define HS_DRIVER_NAME                  L"HeavenShadowDrv"
#define HS_DRIVER_PATH                  L"HeavenShadowDrv.sys"




#define HS_IOCTL(i)			 \
	CTL_CODE                 \
	(                        \
	FILE_DEVICE_UNKNOWN,     \
	i,						 \
	METHOD_NEITHER,          \
	FILE_ANY_ACCESS          \
	)

// HS_IOCTL(HS_IOCTL_PROC_PROCESSLIST)


//////////////////////////////////////////////////////////////////////////
//���б���





//////////////////////////////////////////////////////////////////////////
//���к���
HANDLE OpenDevice(LPCTSTR lpDevicePath);

BOOL HsIs64BitWindows();

VOID HsSendStatusDetail(LPCWSTR szBuffer);

VOID HsSendStatusTip(LPCWSTR szBuffer);

CString TrimPath(WCHAR * wzPath);

CString GetLongPath(CString szPath);

CHAR* HsLoadDllContext(char* szFileName);

DWORD FileLen(char* szFileName);

ULONG_PTR HsGetKernelBase(char* szNtosName);

char *Strcat(char *Str1, char *Str2);

int HsReloc(ULONG_PTR NewBase, ULONG_PTR OrigBase);

CHAR *HsGetTempNtdll();

CHAR* HsGetSystemDir();

DWORD HsGetSpecialIndex(char *FunctionName);

DWORD HsGetSSDTFunctionIndex(char *FunctionName);

CHAR* HsGetTempWin32k();

ULONG_PTR HsGetWin32kBase();

ULONG_PTR HsGetWin32kImageBase(char *szFileName);