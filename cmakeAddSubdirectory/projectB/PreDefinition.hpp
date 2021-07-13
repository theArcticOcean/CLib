#pragma once

#if defined(__APPLE__)
#define UDEF_Class class
#else
#define UDEF_Class class __declspec(dllexport)
#endif
