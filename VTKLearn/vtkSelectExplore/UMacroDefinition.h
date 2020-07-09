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
