/***************************************************************************************
* AUTHOR : HeavenShadow
* DATE   : 2014-10-28
* MODULE : HeavenShadowDrv.H
*
* ��Ӱ �����������ļ�
*
* Description:
*		��Ӱ �����������ļ�
*
****************************************************************************************
* Copyright (C) 2015 HeavenShadow.
****************************************************************************************/


#ifndef _HeavenShadowDrv_H
#define _HeavenShadowDrv_H


#include <ntifs.h>
#include <devioctl.h>
#include "common.h"

#endif	//_HeavenShadowDrv_H


/***************************************************************************************
* NAME:			DriverEntry
*
* DESCRIPTION:	ע����ǲ����
*					
* PARAMETERS:		DriverObject					IN		
*						��NT�������ڴ�������DRIVER_OBJECT�ĵ�ַ
*					RegisterPath					IN		
*						������������صķ���ע������UNICODE_STRING 	
*
* RETURNS:		NTSTATUS
****************************************************************************************/
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegisterPath);

/***************************************************************************************
* NAME:			HsDriverUnload
*
* DESCRIPTION:	��̬ж����ڵ������
*					
* PARAMETERS:		DriverObject					IN		DRIVER_OBJECT�ĵ�ַ
*
* RETURNS:		None
****************************************************************************************/
VOID HsDriverUnload(IN PDRIVER_OBJECT DriverObject);

/***************************************************************************************
* NAME:			HsDispatchControl
*
* DESCRIPTION:	IRP_MJ_DEVICE_CONTROL����ǲ��ڵ�
*					
* PARAMETERS:		DriverObject				IN		DRIVER_OBJECT�ĵ�ַ
*					Irp							IN		IRP�ĵ�ַ
*
* RETURNS:		NTSTATUS
*
* NOTES:			IRP_MJ_DEVICE_CONTROL
*					Parameters:
*					Parameters.DeviceIoControl.OutputBufferLength	OutBuffer�ĳ���
*						�ֽڼ� (����GUI�Ļ���������)
*					Parameters.DeviceIoControl.InputBufferLength	InBuffer�ĳ���
*						�ֽڼ� (����DRIVER�Ļ���������)
*					Parameters.DeviceIoControl.ControlCode			I/O ������
****************************************************************************************/
NTSTATUS HsDispatchControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);



NTSTATUS
HsDispatchPass(PDEVICE_OBJECT DeviceObject,PIRP Irp);



