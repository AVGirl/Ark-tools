/***************************************************************************************
* AUTHOR : HeavenShadow
* DATE   : 2014-10-28
* MODULE : HeavenShadowDrv.C
* 
* Command: 
*	
*
* Description:
*		Demonstrates communications between USER and KERNEL.
*
****************************************************************************************
* Copyright (C) 2014 HeavenShadow.
****************************************************************************************/


#ifndef _HeavenShadowDrv_H
# include "HeavenShadowDrv.h"
#endif

#include "Process.h"
#include "Module.h"
#include "Kernel.h"
#include "System.h"

PDEVICE_OBJECT g_DeviceObject;
PDRIVER_OBJECT g_DriverObject;

WIN_VERSION  WinVersion = WINDOWS_UNKNOW;

PEPROCESS    SystemEProcess = NULL;

//////////////////////////////////////////////////////////////////////////

NTSTATUS
DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegisterPath)
{
	ULONG i = 0;
	
	NTSTATUS Status;
	UNICODE_STRING DeviceName;
	UNICODE_STRING LinkName;
	PDEVICE_OBJECT DeviceObject;


	WinVersion =  HsGetWindowsVersion();

	//��ǲ����
	for (i=0;i<IRP_MJ_MAXIMUM_FUNCTION;i++)
	{
		DriverObject->MajorFunction[i] = HsDispatchPass;
	}

	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HsDispatchControl;

	DriverObject->DriverUnload = HsDriverUnload;

	//����һ��UniCode ���ֵĿ����豸����ʹ��

	RtlInitUnicodeString(&DeviceName,HS_DEVICE_NAME);

	//����һ�������豸���� ��Ӧ�ò���н���



	Status = IoCreateDevice(DriverObject,(ULONG)NULL,&DeviceName,
		FILE_DEVICE_UNKNOWN,0,FALSE,&DeviceObject);


	if (!NT_SUCCESS(Status))
	{
		return STATUS_SUCCESS;
	}



	//��������

	RtlInitUnicodeString(&LinkName,HS_LINK_NAME);


	Status = IoCreateSymbolicLink(&LinkName,&DeviceName);

	if (!NT_SUCCESS(Status))
	{
		IoDeleteDevice(DeviceObject);
		return STATUS_UNSUCCESSFUL;
	}

	g_DeviceObject = DeviceObject;
	g_DriverObject = DriverObject;

	SystemEProcess = PsGetCurrentProcess();


	return STATUS_SUCCESS;
}

VOID
HsDriverUnload(IN PDRIVER_OBJECT DriverObject)
{	
	UNICODE_STRING  uniLinkName;

	RtlInitUnicodeString(&uniLinkName,HS_LINK_NAME);

	IoDeleteSymbolicLink(&uniLinkName);

	if (DriverObject->DeviceObject!=NULL)
	{
		IoDeleteDevice(DriverObject->DeviceObject);
	}

	return;
}

NTSTATUS 
HsDispatchControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	PIO_STACK_LOCATION  IrpSp;
	ULONG				ulIoControlCode  = 0;
	NTSTATUS			Status = STATUS_UNSUCCESSFUL;
	ULONG_PTR			ulRet = 0;
	PVOID               pvInputBuffer  = NULL;
	PVOID               pvOutputBuffer = NULL;
	ULONG_PTR			ulOutputLen    = 0;


	//////////////////////////////////////////////////////////////////////////
	ULONG  ulIoControlFunc = 0;	   //HS_IOCTL_����
	//////////////////////////////////////////////////////////////////////////


	IrpSp = IoGetCurrentIrpStackLocation(Irp);

	pvOutputBuffer = Irp->UserBuffer;	//UserBuffer
	ulOutputLen    = IrpSp->Parameters.DeviceIoControl.OutputBufferLength;
	ProbeForWrite(pvOutputBuffer,ulOutputLen,sizeof(CHAR));

	ulRet = ulOutputLen;

	DbgPrint("Hello HeavenShadow!\r\n");

	ulIoControlCode = IrpSp->Parameters.DeviceIoControl.IoControlCode;

	ulIoControlFunc = (ulIoControlCode>>2)&0x00000FFF&0xF80;
	DbgPrint("Major Func Code: %x\r\n",ulIoControlFunc);

	switch(ulIoControlFunc) //�жϵ���H_IOCTL_���루�Զ���ö�٣������Ǵ����CTL_CODE
	{
	case HS_IOCTL_PROC:		//���̴���
		{
			Status = HsDispatchControlForProcess(IrpSp, pvOutputBuffer, &ulRet);
			break;
		}
	case HS_IOCTL_MODU:		//ģ�����
		{
			Status = HsDispatchControlForModule(IrpSp, pvOutputBuffer, &ulRet);
			break;
		}
	case HS_IOCTL_KRNL:		//�ں˹��Ӵ���
		{
			Status = HsDispatchControlForKernel(IrpSp, pvOutputBuffer, &ulRet);
			break;
		}
	case HS_IOCTL_SYSK:		//�ں˴���
		{
			Status = HsDispatchControlForSystem(IrpSp, pvOutputBuffer, &ulRet);
			break;
		}
	default:				//��������
		{
			Status = STATUS_UNSUCCESSFUL;
		}
	}


	
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = ulRet;
	IoCompleteRequest(Irp,IO_NO_INCREMENT);

	return Status;
}



NTSTATUS
HsDispatchPass(PDEVICE_OBJECT DeviceObject,PIRP Irp)
{

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp,IO_NO_INCREMENT);

	return STATUS_SUCCESS;

}