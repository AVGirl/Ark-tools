
#ifndef CXX_IDTHOOK_H
#	include "IDTHook.h"
#endif
#include <WINDEF.H>


ULONG_PTR g_OrigKiTrap03;
KIRQL  Irql;


_declspec(naked) void NewKiTrap03()
{

	__asm
	{
		//����
		//jmp g_OrigKiTrap03

		//����Trap03���쳣֡
		//�����ֳ�����,��ԭʼTrap03һ��
		push    0   ;ErrorCode
		push    ebp
		push    ebx
		push    esi
		push    edi
		push    fs
		mov     ebx,30h
		mov     fs,bx
		mov     ebx,dword ptr fs:[0]
		push    ebx
		sub     esp,4
		push    eax
		push    ecx
		push    edx
		push    ds
		push    es
		push    gs

		sub     esp,30h    //esp��ʱ��ָ������֡

		push    esp         //FilterExceptionInfo�Լ�������

		call   FilterExceptionInfo   //���˺���

		add     esp , 0x30
		pop		gs
		pop		es
		pop		ds
		pop		edx
		pop		ecx
		pop		eax
		add		esp , 4
		pop		ebx
		pop		fs
		pop		edi
		pop		esi
		pop		ebx
		pop		ebp
		add     esp , 0x4
		jmp     g_OrigKiTrap03
	}
}



VOID __stdcall FilterExceptionInfo(PX86_KTRAP_FRAME pTrapFrame)
{

	//eip��ֵ��һ��int3�������������dec�� 
	DbgPrint("Eip:%x\r\n",(pTrapFrame->Eip)-1);
}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryString)
{
	NTSTATUS	Status = STATUS_SUCCESS;
	IDTR Idtr;
	PIDTENTRY pIdtArray = NULL;
	ULONG_PTR Index = 0;

	DriverObject->DriverUnload = UnloadDriver;
    __asm sidt Idtr
	//������ǵ��˵ģ�ֻ��һ���Ϳ�����
	if(KeGetIdt(&pIdtArray))
	{
		DbgPrint("%x---%x\r\n",Idtr.base,Idtr.limit);
		for (Index =0;Index<(Idtr.limit+1)/sizeof(IDTENTRY);Index++) 		
		{
			DbgPrint("TrapHandle[%d]:%x\r\n",Index,MAKELONG(pIdtArray[Index].LowOffset,pIdtArray[Index].HiOffset));
		}

		g_OrigKiTrap03 = MAKELONG(pIdtArray[3].LowOffset,pIdtArray[3].HiOffset);

		WPOFF();
		pIdtArray[3].LowOffset = (ULONG_PTR)NewKiTrap03 & 0xFFFF;  //��16λ
		pIdtArray[3].HiOffset =  (ULONG_PTR)NewKiTrap03 >> 16;     //��16λ
		WPON();

	}
	
	//limit 0x7ff (����0)  0x800  = 2048  Entryÿ���С8�ֽڣ���2048/8 = 256 ��Ա
	//!idt -a   0ff  = 256 

	//MAKELONG
	//#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
	return Status;
}





BOOLEAN KeGetIdt(PIDTENTRY *pIdtArray)
{
	ULONG Index,Affinity,CurrentAffinity;
	pfnKESETAFFINITYTHREAD fnpKeSetAffinityThread;

	UNICODE_STRING usFuncName;
	PIDTENTRY pIdtEntry;

	RtlInitUnicodeString(&usFuncName,L"KeSetAffinityThread");
	fnpKeSetAffinityThread = (pfnKESETAFFINITYTHREAD)MmGetSystemRoutineAddress(&usFuncName);

	if (fnpKeSetAffinityThread==0)
	{
		return FALSE;
	}

	Affinity = KeQueryActiveProcessors();                    
	//KeQueryActiveProcessors��ȡ��������ص�λͼ
	//(�����λͼ�������Ϊ���������緵��1����һ��������������3��ʾ����������������7��ʾ�������������������ơ�
	//Ҳ����˵���ж��ٸ�����������ôAffinity��ֵ�ͻ�ӵ�λ����λ����������λ)

	CurrentAffinity = 1;
	Index = 0;
	while(Affinity)
	{
		//����ֻ�Ǹ��򵥵��㷨��ʹ��ǰ�߳����е���ͬ�Ĵ�������
		Affinity &= ~CurrentAffinity;
		fnpKeSetAffinityThread(PsGetCurrentThread(),(KAFFINITY)CurrentAffinity);
		CurrentAffinity <<= 1;

		__asm{
			push        eax
			mov         eax,fs:[0x38]
			mov         pIdtEntry,eax
			pop         eax
		}
		//�õ�����Ҫ�Ķ���
		pIdtArray[Index] = pIdtEntry;
		Index++;
	}

	return TRUE;
}


VOID WPOFF()
{
	ULONG_PTR cr0 = 0;
	Irql = KeRaiseIrqlToDpcLevel();
	cr0 =__readcr0();
	cr0 &= 0xfffffffffffeffff;
	__writecr0(cr0);

}





VOID WPON()
{

	ULONG_PTR cr0=__readcr0();
	cr0 |= 0x10000;
	__writecr0(cr0);
	KeLowerIrql(Irql);
}



VOID UnloadDriver(PDRIVER_OBJECT DriverObject)
{
	//�ָ�
	PIDTENTRY pIdtEntry;
	if (g_OrigKiTrap03 && KeGetIdt(&pIdtEntry))
	{
		WPOFF();
		pIdtEntry[3].LowOffset = g_OrigKiTrap03 & 0xFFFF;
		pIdtEntry[3].HiOffset = g_OrigKiTrap03 >> 16;
		WPON();
	}
}