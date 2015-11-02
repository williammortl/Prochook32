32-Bit Windows Prochook Example

by William Mortl
http://www.williammortl.com
(c)2015

Written in C++ / C# - Microsoft Visual Studio 2015 for Win64 API / Microsoft .NET 4.5
There is no warranty implied with this code, and it is not to be used for commercial purposes without proper recompence. Educational use is fine as long as appropriate citation is given.



NOTES:
------

This 32-bit project is mostly here for historical reasons as well as the purpose of preserving old knowledge. In general, almost all Windows users are running a 64-bit versions of Windows now. While this 32-bit technique will certainly work on 64-bit Windows, it will only allow you to hook functions in other 32-bit applications. To solve this problem I have created a 64-bit prochook which can be found here:

https://github.com/williammortl/Prochook64

However, if you do need to hook 32-bit Windows API calls or 32-bit DLL functions, then this is the project for you!

This example project uses a 32-bit Windows prochook to hook the following Windows API calls:

BOOL WINAPI ExtTextOutA(HDC, int, int, UINT, CONST RECT *, LPCSTR, UINT, CONST INT *);
BOOL WINAPI ExtTextOutW(HDC, int, int, UINT, CONST RECT *, LPCWSTR, UINT, CONST INT *);
BOOL WINAPI TextOutA(HDC, int, int, LPCSTR, int);
BOOL WINAPI TextOutW(HDC, int, int, LPCWSTR, int);

However, this technique can be used to hook any function/procedure within a 32-bit application library or any 32-bit Windows API call. 

32BitHookDLL.dll uses a Windows message hook to inject itself into all 32-bit processes running on the system that are at or below its equivalent privilege level. 

32BitHookDLL.dll scrapes text out of some Windows applications via procedure hooks. To test this, run the application within Visual Studio, click on "Start Hooking >>", then click on the Visual Studio menu "Debug->Options and Settings...", and then click the "Get Text >>" button on the test application. One should see some text show up that Visual Studio displayed using the API calls above. That said, any calls to any of the previously listed API calls by any 32-bit application are captured and stored in the text buffer as well. The retrieved text buffer is split by the process id of the application which called the display text API call. The process id can be found in the buffer in the following manner: *!~!*{process id here}*!~!*

Additionally, 32BitHookDLL.dll also implements a keystroke logger. The keystrokes are returned in a buffer in a similar manner to the text buffer containing intercepted text from the display text API calls. It is also split by the process id of the application where the text was typed.

The 32-bit procedure hook works by calling the Windows VirtualProtect API call and changing the attributes of the memory page containing the procedure to be hooked from PAGE_EXECUTE_READ to PAGE_EXECUTE_READWRITE. Next, the hooking dll retrieves the first 5 bytes of the procedure, and then holds these bytes in a variable. Next, the hooking dll writes 5 bytes of x86 assembly over top of the beginning of the procedure to be hooked. This new assembly causes a jump to the address of our "Shadow" function which intercepts the call. The 5 bytes of assembly are (in hex):

E9 {32 bit relative address of where you need to jump to}

The relative address = (address of the function to hook) - (the address of the Shadow function) - (size of PROCHOOK, which is 5 bytes)

If we want to pass the call on to the original procedure from the Shadow procedure, we simply re-write the original first 5 bytes over top of the procedure, call the procedure, and then re-write our 5 byte jump code back when complete.

On a personal note, I have been working with procedure hooks since 1997. I first implemented Matt Pietreks's 16-bit prochook for Windows 3.11 and Windows 95 from his book "Windows Internals" in 1997. You can find Matt Pietrek's book here:

http://www.amazon.com/Windows-Internals-Implementation-Operating-Environment/dp/0201622173

Next, I worked with another engineer (who was / is quite brilliant) on implementing a 32-bit prochook for Windows 9x operating systems. This prochook required a call to the VXD driver API call PageModifyPermissions. Due to Windows 9x's wonky architecture, this was the most difficult of all the prochooks to implement correctly. While finalizing the Windows 9x prochook I created a 32-bit Windows NT based procedure hook, which is very similar to the prochook in this project.