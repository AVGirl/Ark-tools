/**************************************************************************************
* AUTHOR : HeavenShadow
* DATE   : 2014-10-28
* MODULE : System.h
*
* Command: 
*	�ں˴��ܵ����ļ�
*
* Description:
*	���ں���ص����й��ܼ����ļ�
*
****************************************************************************************
* Copyright (C) 2015 HeavenShadow.
****************************************************************************************/


#pragma once 


#if DBG
#define dprintf DbgPrint
#else
#define dprintf
#endif

#include <ntifs.h>
#include "common.h"


#define MAX_PATH 260


NTSTATUS HsDispatchControlForSystem(PIO_STACK_LOCATION  IrpSp, PVOID OutputBuffer, ULONG_PTR* ulRet);

VOID HsInitSystemGlobalVariable();





