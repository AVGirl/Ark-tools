

#ifndef CXX_IDTHOOK_H
#define CXX_IDTHOOK_H

#include <ntifs.h>
#include <devioctl.h>
#endif	

#pragma  pack(2)  //Ĭ����4�ֽڶ���
typedef struct _IDTR
{
	USHORT	limit;	//��Χ����ռ�ڴ��С
	ULONG	base;	//IDT������ʼ��ַ
}IDTR,*PIDTR;
#pragma pack()

//ռ8�ֽ�
typedef struct _IDTENTRY
{
	unsigned short LowOffset;    //LowOffset��HiOffset���һ4�ֽڵĵ�ַ���Ǵ������̵ĵ�ַ
	unsigned short selector;
	unsigned char retention:5;
	unsigned char zero1:3;
	unsigned char gate_type:1;
	unsigned char zero2:1;
	unsigned char interrupt_gate_size:1;
	unsigned char zero3:1;
	unsigned char zero4:1;
	unsigned char DPL:2;
	unsigned char P:1;
	unsigned short HiOffset;
} IDTENTRY,*PIDTENTRY;


typedef struct _X86_KTRAP_FRAME {
	ULONG   DbgEbp;
	ULONG   DbgEip;
	ULONG   DbgArgMark;
	ULONG   DbgArgPointer;
	ULONG   TempSegCs;
	ULONG   TempEsp;
	ULONG   Dr0;
	ULONG   Dr1;
	ULONG   Dr2;
	ULONG   Dr3;
	ULONG   Dr6;
	ULONG   Dr7;
	ULONG   SegGs;
	ULONG   SegEs;
	ULONG   SegDs;
	ULONG   Edx;
	ULONG   Ecx;
	ULONG   Eax;
	ULONG   PreviousPreviousMode;
	ULONG   ExceptionList;
	ULONG   SegFs;
	ULONG   Edi;
	ULONG   Esi;
	ULONG   Ebx;
	ULONG   Ebp;
	ULONG   ErrCode;

	ULONG   Eip;
	ULONG   SegCs;
	ULONG   EFlags;
	ULONG   HardwareEsp;    // WARNING - segSS:esp are only here for stacks
	ULONG   HardwareSegSs;  // that involve a ring transition.
	ULONG   V86Es;          // these will be present for all transitions from
	ULONG   V86Ds;          // V86 mode
	ULONG   V86Fs;
	ULONG   V86Gs;
} X86_KTRAP_FRAME, *PX86_KTRAP_FRAME;



VOID UnloadDriver(PDRIVER_OBJECT DriverObject);
VOID WPOFF();
VOID WPON();
VOID __stdcall FilterExceptionInfo(PX86_KTRAP_FRAME pTrapFrame);
BOOLEAN KeGetIdt(PIDTENTRY *pIdtArray);
typedef KAFFINITY (*pfnKESETAFFINITYTHREAD)(
	__inout PKTHREAD Thread,
	__in KAFFINITY Affinity
	);