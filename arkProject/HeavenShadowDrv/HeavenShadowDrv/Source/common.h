/**************************************************************************************
* AUTHOR : HeavenShadow
* DATE   : 2014-10-28
* MODULE : common.h
*
* Command: 
*	IOCTRL ͨ��ͷ�ļ�
*
* Description:
*	������Ӧ�ó����IoCtrl��ͨ������
*
****************************************************************************************
* Copyright (C) 2015 HeavenShadow.
****************************************************************************************/

#pragma once 

#include <ntifs.h>

#if DBG
#define dprintf DbgPrint
#else
#define dprintf
#endif

//��֧�ַ��������û������
#define HS_DEVICE_NAME                  L"\\Device\\HeavenShadowDevice"
#define HS_LINK_NAME                    L"\\DosDevices\\HeavenShadowLink"

#define HS_EVENT_PROCESS_NAME           L"\\BaseNamedObjects\\HeavenShadowProcessEvent"

#define HS_WIN32_LINK_NAME              "\\\\.\\HeavenShadowLink"

//֧�ַ��������û������
#define SYMBOLIC_LINK_GLOBAL_NAME       L"\\DosDevices\\Global\\HeavenShadowDevice"

#define DATA_TO_APP                     "HeavenShadow"


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
	HS_IOCTL_SYSK_REMOVEIOTIMER,			//IOTIMERɾ��
	HS_IOCTL_SYSK_CALLBACKLIST,				//ϵͳ�ص�����������
	HS_IOCTL_SYSK_REMOVECALLBACK,			//ɾ��ϵͳ�ص�
	HS_IOCTL_SYSK_DPCTIMER,					//DPCTimer
	HS_IOCTL_SYSK_REMOVEDPCTIMER,			//DPCTimerɾ��
	HS_IOCTL_SYSK_FILTERDRIVER,				//��������
	HS_IOCTL_SYSK_FILTERUNLOAD,				//ж�ع�������
};


#define HS_IOCTL(i)			 \
	CTL_CODE                 \
	(                        \
	FILE_DEVICE_UNKNOWN,     \
	i,						 \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS          \
	)

// HS_IOCTL(HS_IOCTL_PROC_PROCESSLIST)




typedef 
NTSTATUS 
(*pfnRtlGetVersion)(OUT PRTL_OSVERSIONINFOW lpVersionInformation);

typedef enum WIN_VERSION {
	WINDOWS_UNKNOW,
	WINDOWS_XP,
	WINDOWS_7,
	WINDOWS_8,
	WINDOWS_8_1
} WIN_VERSION;

WIN_VERSION HsGetWindowsVersion();





//////////////////////////////////////////////////////////////////////////
///�Զ��庯��
//////////////////////////////////////////////////////////////////////////


//ͨ�� �������� �õ�������ַ
PVOID 
HsGetFunctionAddressByName(WCHAR *szFunction);

NTSTATUS HsSafeCopyMemory(PVOID SrcAddr, PVOID DstAddr, ULONG SrcSize);

VOID HsWcharToChar(WCHAR *wzFuncName,CHAR *szFuncName);


CHAR HsChangePreMode(PETHREAD EThread);
VOID HsRecoverPreMode(PETHREAD EThread, CHAR PreMode);

extern NTSTATUS PsLookupProcessByProcessId(PVOID ProcessID, PEPROCESS *Process);

BOOLEAN HsIsUnicodeStringValid(PUNICODE_STRING uniString);