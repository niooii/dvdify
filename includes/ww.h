#pragma once

#include <windows.h>
#include <wtypes.h>

#ifdef WW_EXPORTS
#define WW_API __declspec(dllexport)
#else
#define WW_API __declspec(dllimport)
#endif

WW_API BOOL InstallHook();
WW_API void SetCallbackFn(void (*process_msg_fn)(CWPSTRUCT* message_info));
WW_API void UninstallHook();