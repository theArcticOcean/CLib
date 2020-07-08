#pragma once


#define vSP vtkSmartPointer    
#define vSPNew(Var, Type)    vSP<Type> Var = vSP<Type>::New(); 
#define VTK_FREE(Object)	 if(Object) { Object->Delete(); Object = NULL; }

#define CPP_New(Var, Type)   Type *Var = new Type(); 
#define CPP_FREE(Object)	 if(Object) { delete Object;	 Object = 0; } 

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

//
// Pointer checker.  The macro is used in the functions which return bool data.
//
#define POINTER_CHECK_WITHBOOL( pointer ) \
      if( nullptr == pointer ) { \
        printf( "[%s, %d]: here is nullptr, so jump out.\n", __FILE__, __LINE__ );  \
        return false; \
    }

//
// Pointer checker.  The macro is used in the void functions.
//
#define POINTER_CHECK( pointer ) \
    if( nullptr == pointer ) { \
        printf( "[%s, %d]: here is nullptr, so jump out.\n", __FILE__, __LINE__ );  \
        return ; \
    }

//********************Module************************
#if defined WIN32
#define MODULEMAP_INSERT(Module,ModuleType)    {QString InputModuleName = QString(typeid(ModuleType).name());InputModuleName =InputModuleName.right(InputModuleName.length() - 6); if (!CUModuleBase::Get_g_ModulesMap()->contains(InputModuleName))   CUModuleBase::Get_g_ModulesMap()->insert(InputModuleName, (CUModuleBase*)Module);}
#else
#define MODULEMAP_INSERT(Module,ModuleType)    {QString InputModuleName = QString(typeid(ModuleType).name());InputModuleName =InputModuleName.right(InputModuleName.length() - 2); if (!CUModuleBase::Get_g_ModulesMap()->contains(InputModuleName))   CUModuleBase::Get_g_ModulesMap()->insert(InputModuleName, (CUModuleBase*)Module);}
#endif

#define MODULEMAP_INSERT_BYNAME(Module,ModuleName)  if (!CUModuleBase::Get_g_ModulesMap()->contains(ModuleName))CUModuleBase::Get_g_ModulesMap()->insert(ModuleName, (CUModuleBase*)Module);

#ifdef __APPLE__
#define MODULEMAP_GET(ModulePointor,ModuleType) ModuleType *ModulePointor = NULL; {QString InputModuleName = QString(typeid(ModuleType).name());InputModuleName = InputModuleName.right(InputModuleName.length()-2);if (CUModuleBase::Get_g_ModulesMap()->contains(InputModuleName)){ModulePointor = (ModuleType *)(*CUModuleBase::Get_g_ModulesMap())[InputModuleName];}}
#else
#define MODULEMAP_GET(ModulePointor,ModuleType) ModuleType *ModulePointor = NULL;{QString InputModuleName = QString(typeid(ModuleType).name());InputModuleName = InputModuleName.right(InputModuleName.length() - 6);if (CUModuleBase::Get_g_ModulesMap()->contains(InputModuleName)){ModulePointor = (ModuleType *)(*CUModuleBase::Get_g_ModulesMap())[InputModuleName];}}
#endif


#define MODULEMAP_GET_BYNAME(ModulePointor,ModuleName)  \
	CUModuleBase *ModulePointor = NULL;  \
if (CUModuleBase::Get_g_ModulesMap()->contains(ModuleName))  \
	{ \
		ModulePointor = (CUModuleBase *)(*CUModuleBase::Get_g_ModulesMap())[ModuleName];  \
	} \



//********************Dialog************************
#define DIALOGMAP_INSERT(Dialog,DialogName)  if (!CUQDialogBase::g_DialogsMap.contains(DialogName))CUQDialogBase::g_DialogsMap.insert(DialogName, (CUQDialogBase*)Dialog);

#define DIALOGMAP_GET(DialogPointor,DialogType,DialogName)  \
	DialogType *DialogPointor = NULL;  \
if (CUQDialogBase::g_DialogsMap.contains(DialogName))  \
	{ \
	DialogPointor = (DialogType *)CUQDialogBase::g_DialogsMap[DialogName];  \
	} \


#define DIALOGMAP_GET_BYNAME(DialogPointor,DialogName)  \
	CUQDialogBase *DialogPointor = NULL;  \
if (CUQDialogBase::g_DialogsMap.contains(DialogName))  \
	{ \
	DialogPointor = (CUQDialogBase *)CUQDialogBase::g_DialogsMap[DialogName];  \
	} \




//#define WRITE_LOG_INTERACTIVEOPERATION1(Object,LogContent)  MODULEMAP_GET(mvd,CUModuleViewDirection);Object->WriteInteractiveOperationToLog(__FUNCTION__,LogContent.append("#"+mvd->GetCameraParameter()));
#define WRITE_LOG_INTERACTIVEOPERATION(LogContent)  \
	QString BackupContent=LogContent; \
	MODULEMAP_GET(mvd_log, CUModuleViewDirection);  \
if (mvd_log != NULL) \
{ \
	CUObject::WriteInteractiveOperationToLog(__FUNCTION__, BackupContent.append("#" + mvd_log->GetCameraParameter())); \
} \



#define WRITE_LOG_INTERACTIVEOPERATION2(FunctionName,LogContent)  \
	QString BackupContent=LogContent; \
	MODULEMAP_GET(mvd_log, CUModuleViewDirection);  \
if (mvd_log != NULL)CUObject::WriteInteractiveOperationToLog(FunctionName, BackupContent.append("#" + mvd_log->GetCameraParameter())); \

#ifdef __APPLE__
#define WRITE_LOG_INTERACTIVEOPERATION3(UObject,LogContent) QString BackupContent = LogContent; MODULEMAP_GET(mvd_log, CUModuleViewDirection); if (mvd_log != NULL) { QString Function = __FUNCTION__; QStringList sl = Function.split("::"); QString ClassName = QString(typeid(*UObject).name()); ClassName = ClassName.right(ClassName.length() - 2);  Function = ClassName; CUObject::WriteInteractiveOperationToLog(Function, BackupContent.append("#" + mvd_log->GetCameraParameter())); }
#else
#define WRITE_LOG_INTERACTIVEOPERATION3(UObject,LogContent)  QString BackupContent = LogContent; MODULEMAP_GET(mvd_log, CUModuleViewDirection);  if (mvd_log != NULL) { QString Function = __FUNCTION__; QStringList sl = Function.split("::"); QString ClassName = QString(typeid(*UObject).name()); ClassName = ClassName.right(ClassName.length() - 6);  Function = ClassName + "::" + sl[1];  CUObject::WriteInteractiveOperationToLog(Function, BackupContent.append("#" + mvd_log->GetCameraParameter())); }
#endif




#define WRITE_LOG_EnvironmentParas(Paras)  \
{ \
MODULEMAP_GET(mvd_log, CUModuleViewDirection);  \
if (mvd_log != NULL)CUObject::WriteInteractiveOperationToLog("CUModuleReplay::SetEnvironmentParas", Paras.append("#" + mvd_log->GetCameraParameter())); \
} \

#define InvalidValue -99999999
#define PI 3.1415926


typedef  int  SYS_BYTE_TYPE;
#define SYS_BYTE_32 0 
#define SYS_BYTE_64 1


#define BYTEARRAY_TO_VTKIDTYPE(ByteArray_Pointer,vtkIdType_value)\
{	long long value; \
if (CUObject::Get_g_Case_Byte_Type() == SYS_BYTE_32)\
{\
	value = (long long)((ByteArray_Pointer[0] & 0xFF) | ((ByteArray_Pointer[1] << 8) & 0xFF00) | ((ByteArray_Pointer[2] << 16) & 0xFF0000) | ((ByteArray_Pointer[3] << 24) & 0xFF000000)); \
}\
if (CUObject::Get_g_Case_Byte_Type() == SYS_BYTE_64)\
{\
	value = (long long)((ByteArray_Pointer[0] & 0xFF) | ((ByteArray_Pointer[1] << 8) & 0xFF00) | ((ByteArray_Pointer[2] << 16) & 0xFF0000) | ((ByteArray_Pointer[3] << 24) & 0xFF000000) | ((ByteArray_Pointer[4] << 32) & 0xFF00000000) | ((ByteArray_Pointer[5] << 40) & 0xFF0000000000) | ((ByteArray_Pointer[6] << 48) & 0xFF000000000000) | ((ByteArray_Pointer[7] << 56) & 0xFF00000000000000)); \
}\
	vtkIdType_value = (vtkIdType)value;} \



#define VTKIDTYPE_TO_BYTEARRAY(vtkIdType_value,ByteArray_Pointer)\
for (int i = 0; i < 8; i++)ByteArray_Pointer[i] = 0; \
	ByteArray_Pointer[0] = (char)(0xff & vtkIdType_value); \
	ByteArray_Pointer[1] = (char)((0xff00 & vtkIdType_value) >> 8); \
	ByteArray_Pointer[2] = (char)((0xff0000 & vtkIdType_value) >> 16); \
	ByteArray_Pointer[3] = (char)((0xff000000 & vtkIdType_value) >> 24); \
if (CUObject::Get_g_App_Byte_Type() == SYS_BYTE_64)\
{\
	ByteArray_Pointer[4] = (char)((0xff00000000 & vtkIdType_value) >> 32); \
	ByteArray_Pointer[5] = (char)((0xff0000000000 & vtkIdType_value) >> 40); \
	ByteArray_Pointer[6] = (char)((0xff000000000000 & vtkIdType_value) >> 48); \
	ByteArray_Pointer[7] = (char)((0xff00000000000000 & vtkIdType_value) >> 56); \
}\

#if defined(__APPLE__)
#define UDEF_Class class
#else
#ifdef CLASSDEFPREFIX
#define  UDEF_Class class __declspec(dllexport)
#else
#define  UDEF_Class class __declspec(dllimport)
#endif
#endif


enum Partner_Type
{
	puLab=0,
	P3M = 1,
	PSdc = 2,
	PJapan = 3,
	PDSD=4,
	PABO=5,
	puFab = 6,
};


#define Partner_Version(PartnerType,ValueType,Version)\
ValueType Version=0;\
if (PartnerType == Partner_Type::P3M)Version = 1;  \
if (PartnerType == Partner_Type::PSdc)Version = 1;  \


//********************DirPath************************
//#ifdef __APPLE__
#include <QCoreApplication>
#define ULabExeDirPath (QCoreApplication::applicationDirPath() + "/")
//#else
//#define DirPath
//#endif

#define PIC(temp)  QString("QPushButton {border-image: url(\%1%2);background: transparent;}").arg(ULabExeDirPath).arg(temp);

#define PICAddTag(temp1,temp2,temp3,temp4)  QString("QRadioButton{spacing: 2px;color: white;}QRadioButton::indicator {width: 45px;height: 30px;}QRadioButton::indicator:unchecked {image: url(\%1);}QRadioButton::indicator:unchecked:pressed {image: url(\%2);}QRadioButton::indicator:checked {image: url(\%3);}QRadioButton::indicator:checked:pressed {image: url(\%4);}").arg(temp1).arg(temp2).arg(temp3).arg(temp4);
#define SwitchObjFormt(temp1,temp2,temp3,temp4)  QString("QRadioButton{spacing: 2px;color: white;}QRadioButton::indicator {width: 60px;height: 40px;}QRadioButton::indicator:unchecked {image: url(\%1);}QRadioButton::indicator:unchecked:pressed {image: url(\%2);}QRadioButton::indicator:checked {image: url(\%3);}QRadioButton::indicator:checked:pressed {image: url(\%4);}").arg(temp1).arg(temp2).arg(temp3).arg(temp4);


#define OFFLINE_VERSION
//#define ONLINE_VERSION
