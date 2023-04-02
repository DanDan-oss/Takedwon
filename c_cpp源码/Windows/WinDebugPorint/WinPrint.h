void __cdecl OutputDebugStringF(const char* format, ...);
#ifdef _DEBUG
#define WinPrint OutputDebugStringF
#else
#define WinPrint
#endif