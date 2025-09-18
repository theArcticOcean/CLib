// Disable warning messages 4507 and 4034.
//#pragma warning( disable : 4507 34 )

// Issue warning C4385 only once.
//#pragma warning( once : 4385 )

#pragma once
#include <vtkSmartPointer.h>

#define vSP vtkSmartPointer    
#define vSPNew(Var, Type)    vSP<Type> Var = vSP<Type>::New(); 
#define VTK_FREE(Object)	 if(Object) { Object->Delete(); Object = NULL; }

#define CPP_New(Var, Type)   Type *Var = new Type(); 
#define CPP_FREE(Object)	 if(Object) { delete Object;	 Object = nullptr; } 

#define CPP_SET_MACRO(name,type) \
	virtual void Set##name(type _arg) \
  { \
	if (this->name != _arg) \
	{ \
	this->name = _arg; \
	} \
  }

//
// Get built-in type.  Creates member Get"name"() (e.g., GetVisibility());
//
#define CPP_GET_MACRO(name,type) \
	virtual type Get##name() {\
	return this->name; \
	}

#define CPP_GET_CONST_MACRO(name,type) \
	virtual type Get##name() const {\
	return this->name; \
}
//
// Pointer checker.  The macro is used in the functions which return bool data.
//
#define POINTER_CHECK_WITHBOOL( pointer ) \
      if( nullptr == pointer ) { \
        /*LOG( ERR, "nullptr, jump out" );*/ \
        return false; \
    }

//
// Pointer checker.  The macro is used in the void functions.
//
#define POINTER_CHECK( pointer ) \
    if( nullptr == pointer ) { \
        LOG( ERR, "nullptr, jump out" ); \
        return ; \
    }



#define InvalidValue -99999999
#define PI 3.1415926


typedef  int  SYS_BYTE_TYPE;
#define SYS_BYTE_32 0 
#define SYS_BYTE_64 1


#define BYTEARRAY_TO_VTKIDTYPE(ByteArray_Pointer,vtkIdType_value)\
{	long long value; \
if (UBasicInfo::Getm_ByteType() == SYS_BYTE_32)\
{\
	value = (long long)((ByteArray_Pointer[0] & 0xFF) | ((ByteArray_Pointer[1] << 8) & 0xFF00) | ((ByteArray_Pointer[2] << 16) & 0xFF0000) | ((ByteArray_Pointer[3] << 24) & 0xFF000000)); \
}\
if (UBasicInfo::Getm_ByteType() == SYS_BYTE_64)\
{\
	value = (long long)(((uint64_t)ByteArray_Pointer[0] & 0xFF) | (((uint64_t)ByteArray_Pointer[1] << 8) & 0xFF00) | (((uint64_t)ByteArray_Pointer[2] << 16) & 0xFF0000) | (((uint64_t)ByteArray_Pointer[3] << 24) & 0xFF000000) | (((uint64_t)ByteArray_Pointer[4] << 32) & 0xFF00000000) | (((uint64_t)ByteArray_Pointer[5] << 40) & 0xFF0000000000) | (((uint64_t)ByteArray_Pointer[6] << 48) & 0xFF000000000000) | (((uint64_t)ByteArray_Pointer[7] << 56) & 0xFF00000000000000)); \
}\
	vtkIdType_value = (vtkIdType)value;} \



#define VTKIDTYPE_TO_BYTEARRAY(vtkIdType_value,ByteArray_Pointer)\
for (int i = 0; i < 8; i++)ByteArray_Pointer[i] = 0; \
	ByteArray_Pointer[0] = (char)(0xff & vtkIdType_value); \
	ByteArray_Pointer[1] = (char)((0xff00 & vtkIdType_value) >> 8); \
	ByteArray_Pointer[2] = (char)((0xff0000 & vtkIdType_value) >> 16); \
	ByteArray_Pointer[3] = (char)((0xff000000 & vtkIdType_value) >> 24); \
if (UBasicInfo::Getm_ByteType() == SYS_BYTE_64)\
{\
	ByteArray_Pointer[4] = (char)((0xff00000000 & vtkIdType_value) >> 32); \
	ByteArray_Pointer[5] = (char)((0xff0000000000 & vtkIdType_value) >> 40); \
	ByteArray_Pointer[6] = (char)((0xff000000000000 & vtkIdType_value) >> 48); \
	ByteArray_Pointer[7] = (char)((0xff00000000000000 & vtkIdType_value) >> 56); \
}\

#if defined(__APPLE__) || defined(__EMSCRIPTEN__) || defined(__linux__) || defined(__unix__)
#define UDEF_Class
#else
#ifdef CLASSDEFPREFIX
#define UDEF_Class  __declspec(dllexport)
#else
#define UDEF_Class  __declspec(dllimport)
#endif
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <malloc.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#endif // __EMSCRIPTEN__
