/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2008 CRI Middleware, Inc. All rights reserved.
 *
 * Use, modification and distribution are subject to the CRI Script Software
 * License, Version 1.0(see accompanying file "CriScript_License_1_0.txt" or
 * copy at www.criscript.com/trac/wiki/CRI%20Script%20Software%20License%201.0 ).
 *
 *
 * Library  : CRIScript
 * Module   : cil VM definitions
 * File     : cilVm.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

#include "criscript.h"
#include "versions.h"
#include "iCodeGen.h"
#include "cilCodeGen.h"
#include "variable.h"
#include "vmRuntimeError.h"

using namespace std;

namespace cri {

/***************************************************************************
 *      VM costants
 ***************************************************************************/
const int32_t LONGBR_ADJUST = 4;
const int32_t SHORTBR_ADJUST = 1;
const int32_t SIZE_LONGBR = 5;
const int32_t SIZE_SHORTBR = 2;

class CVmObject;

typedef CVmObject* VMOBJECTREF;

const int32_t VM_REFCOUNT_MANAGED = 0;//0x40000000;

/*EN Maximum callstack entries in a VM. assertion is triggerred when the callstack entries exceeds this number.
Note: Need tail recursion optimization
\ingroup MDL_LIB_VM
*/
const int32_t MAX_CALLSTACK = 512;

/*EN Maximum evalstack entries in a VM. assertion is triggerred when the evalstack entries exceeds this number.
Note: Future optimization: Now eval stack push/pop involves ref count inc/dec. Needs non ref counter implementation.
\ingroup MDL_LIB_VM
*/
const int32_t MAX_EVALSTACK = 4096;

/*EN Maximum local variable stack entries in a VM. assertion is triggerred when the local variable stack entries exceeds this number.
\ingroup MDL_LIB_VM
*/
const int32_t MAX_LOCAL_VARIABLE = 256;

/*EN Maximum argument variable stack entries in a VM. assertion is triggerred when the argument variable stack entries exceeds this number.
\ingroup MDL_LIB_VM
*/
const int32_t MAX_ARG_VARIABLE = 1024;


/***************************************************************************
 *      VM HANDLER Type
 ***************************************************************************/
/*EN User speficied VM handler types
\ingroup MDL_LIB_VM
*/
enum VM_HANDLER_TYPE {
	VM_HANDLER_EXCEPTIONNOTHANDLED,	/*EN< Callback invoked when the exception is not handled*/
	VM_HANDLER_MAX,
};

/*EN VM callback function signature
\ingroup MDL_LIB_VM
*/
typedef void( *VM_HANDLER_CALLBACK )( int32_t iType, CVariable* pVar );


/***************************************************************************
 *      VM status enum
 ***************************************************************************/
/*EN Current VM statuses
\ingroup MDL_LIB_VM
*/
enum VM_STATUS_TYPE {
	VM_EXECUTE_INITIALIZING,	/*EN< VM instance is created but still not initialized yet*/
	VM_EXECUTE_INITIALIZED,		/*EN< VM instance is initialized but still not executed yet*/
	VM_EXECUTE_EXECUTING,		/*EN< VM instance is initialized but still not executed yet*/
	VM_EXECUTE_DONE,			/*EN< Byte code execution is finished successfully*/
	VM_EXECUTE_SUSPENDED,		/*EN< VM suspended by resume() call. Not implemented yet*/
	VM_EXECUTE_SWAPPEDOUT,		/*EN< VM time slice is ended and current context is swapped out. not implemented yet.*/
	VM_EXECUTE_ERROR,			/*EN< VM is in the error state*/
};

/***************************************************************************
 *      VM reset Type
 ***************************************************************************/
/*EN VM reset type
\ingroup MDL_LIB_VM
*/
enum VM_RESET_TYPE {
	VM_RESET_DEFAULT,	/*EN< Reset VM to the initialized status
								All the fields values, objects statuses are reset.*/
};


/***************************************************************************
 *      Callstack slot info
 ***************************************************************************/
/*EN Callstack type
\ingroup MDL_LIB_VM
*/
enum VM_CALLSTACK_TYPE {
	VM_CALLSTACK_CALLSTACK,	/*EN< Regular call stack */
	VM_CALLSTACK_EH,		/*EN< Excepiton handling clause */
};

/*EN Execution status of the current clause
\ingroup MDL_LIB_VM
*/
enum VM_EHCLAUSESTATUS_TYPE {
	VM_EHCLAUSE_NORMAL				=  0,		/*EN< Executing normally */
	VM_EHCLAUSE_EXECUTING_CATCH		= -1,		/*EN< Executing catch clause */
	VM_EHCLAUSE_EXECUTING_FINALLY	= -2,		/*EN< Executing finally clause */
	VM_EHCLAUSE_RETURNING			= -3,		/*EN< Returning from EH clause */
	VM_EHCLAUSE_EXECUTED_CATCH		= -4,		/*EN< Executed catch clause */
};

/*EN Structure that represent a call stack information
\ingroup MDL_LIB_VM
*/
struct VM_CALLSTACK {
	VM_CALLSTACK_TYPE		type;					/*EN< Type of call stack*/
	RID						ridMethod;				/*EN< Method RID*/
	uint8_t*				pInstruction;			/*EN< Return Instruction pointer*/
	VM_EHCLAUSESTATUS_TYPE	status;					/*EN< Current execution status*/
	union {
		/*EN Local variable start index*/
		int32_t			iLocalVariableStartIndex;	
		/*EN Catch clause RID*/
		RID					ridCatch;
	};
	union {
		/*EN Argument list start index*/
		int32_t			iArgumentListStartIndex;
		/*EN Finally clause RID*/
		RID					ridFinally;
	};
	/*EN Number of an argument used in the method*/
	uint8_t				iNumArgument;
	/*EN Number of an argument given by callee*/
	uint8_t				iNumGivenArgument;
	/*EN Flag that indicates if it's calling ctor.
	When calling ctor, return value needed to be avoided*/
	bool				bConstructor;
};

/***************************************************************************
 *      VM object definition
 ***************************************************************************/
/*EN
 A pointer to the CVmObject. Inherit a Code generation object metadata
 \ingroup MDL_LIB_VM
*/
class CVmObject : public CObjectTypeInfo {
	int32_t		m_iRefCount;	/*EN< Reference Counter*/
	CVariable	m_varPrototype;	/*EN< A prototype object value*/
	CVariable	m_varParent;	/*EN< A parent value, needed for a property access*/
	CVariable	m_varValue;		/*EN< A value of the object*/

	/*EN An accessor for the prototype varialbe*/
	static CVariable* prototypeAccessor(CCilVm* const pVm,
										CVmObject* const pObject, 
										const wstring* const pName,
										CVariable* const var,
										const bool bSet );

public:
	CVmObject();							/*EN< ctor*/
	~CVmObject();							/*EN< dtor*/
	CVmObject( const CVmObject& right );	/*EN< Copy ctor*/

	/*EN Increments a reference count of the object.
	\return current reference count value
	*/
	int32_t addRef();

	/*EN Decrements a reference count of the object.
	\return current reference count value
	*/
	int32_t release();

	/*EN Enforce to set a reference count of the object.
	\param iCount new reference count value to be set
	*/
	void setRefCount( int32_t iCount ) { m_iRefCount = iCount; }

	/*EN Retrieves a reference count of the object.
	\return areference count value
	*/
	int32_t getRefCount() { return m_iRefCount; }

	/*EN Clears a reference count value to 0.
	*/
	void clearRefCount( void );
	/*EN Sets a prototype object.
	\param pPrototype A pointer to the prototype VM Object
	\param type operand type, usually OPERAND_OBJECTREF.
	\param flag flags
	*/
	void setPrototypeObject( CVmObject* const pPrototype,
								const OPERAND_TYPE type = OPERAND_OBJECTREF,
								const OPERAND_FLAG flag = OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
													| OPERAND_FLAG_DONTENUM ) )
	{
		assert( type == OPERAND_OBJECTREF );
		m_varPrototype.setObjectRefWithFlags( pPrototype, flag, RID_NULL );
	}

	/*EN Retrieves a prototype object.
	\return A prototype object reference.
	A prototype object has a chain and finally goes to the Object prototype object.
	*/
	CVmObject* getPrototypeObject( void ) { return m_varPrototype.refObject; }

	/*EN Looks up the property with the given name
	Use this version only when you are sure getter/setters are not involved in.
	\param str A name of the property
	\return A pointers to a value of the property. Returns NULL if a requested property does not exist.
	*/
	CVariable* lookupProperty( const wstring& str );

	/*EN Retrieves an iterator tothe property with the given name
	Use this version only when you are sure getter/setters are not involved in.
	\param str A name of the property
	\return An iterator of the property. Returns invalid iterator when the property does not exist.
	*/
	hash_map< wstring, CVariable >::iterator lookupPropertyIterator( const wstring& str );

	/*EN Sets the property value with the given name
	Use this version only when you are sure getter/setters are not involved in.
	\param str A name of the property
	\param value value of the property
	*/
	void setProperty( const wstring& str, CVariable& value );

	/*EN Looks up the property with the given name.
	This version can invoke a getter.
	\param pVm A pointer to the VM instance
	\param str A name of the property
	\return A pointers to a value of the property. Returns NULL if a requested property does not exist.
	*/
	CVariable* lookupProperty( CCilVm* const pVm, const wstring& str, CVariable* const pThis = NULL );

	/*EN Sets the property value with the given name
	This version can invoke a setter.
	Use this version only when you are sure getter/setters are not involved in.
	\param pVm A pointer to the VM instance
	\param str A name of the property
	\param value value of the property
	*/
	void setProperty( CCilVm* const pVm, const wstring& str, CVariable& value );

	/*EN Sets the method property with the given name
	\param pVm A pointer to the VM instance
	\param name A name of the property
	\param ridMethod RID value of the method
	\param iLength A number of parameters that the method can accept. This value is set to method.length property.
	*/
	void setProperty( CCilVm* const pVm,
				 const wstring&	name,
				 const RID ridMethod,
				 const int32_t iLength );

	/*EN Removes a specified property.
	\param name A name of the property to be removed
	*/
	void removeProperty( const wstring& name );

	/*EN Sets a property accessor.
	\param name A name of the property that the accessor will be set
	\param pAccessro A pointer of the accessor callback
	*/
	void setPropertyAccessor( const wstring& name, PACCESSORMETHOD pAccessor );

	/*EN Retrieves a value of the object.
	\return value of the object
	*/
	CVariable& getValue( void )
	{
		return m_varValue;
	}

	/*EN Set a value of the object.
	\param var an object value
	*/
	void setValue( CVariable& var )
	{
		var.moveTo( m_varValue );
		return;
	}

	/*EN Retrieves a parent variable of the object.
	\return value of the object
	*/
	CVariable& getParentVariable( void ) { return m_varParent; }

	/*EN Set a parent value of the object.
	\param variable a parent value of the object value
	*/
	void setParentVariable( const CVariable& variable ) { m_varParent = variable; }

};

//Some forward decl
class CCilDebugger;

/***************************************************************************
 *      CIL VM implementation
 ***************************************************************************/
	/*EN
	 \brief The VM class for CRI Script byte code strem
	 \ingroup MDL_LIB_VM
	*/
	class CCilVm
	{
	public:
		CCilVm();	/*EN< ctor*/
		~CCilVm();	/*EN< dtor*/

		/*EN Initialize the VM.
		\return true when the initialization succeeded.
		*/
		bool init();

		/*EN verify an attathed byte code stream.
		\return true when the verification succeeded.
		*/
		bool verify();

		/*EN reset VM status to Initilized status
		\param iResetType VM reset type
		\return true when the reset succeeded.
		*/
		bool reset( const VM_RESET_TYPE iResetType );

		/*EN Retrieve Version Number of the VM */
		const CRISCRIPT_VERSIONS& getVersions(void) { return criscript_version_struct; }

		//----------------------------------------------------------------------
		//VM Management
		//----------------------------------------------------------------------
		/*EN execute the byte code.
		\return current execution status.
		*/
		VM_STATUS_TYPE execute();

		/*EN Retrieves an exit code of the script.
		\return exit code value. If the exit code can not be converted via ToNumber(), 0 is returned.
		An exit code is set from the script variable 'System.Environment.ExitCode' .
		*/
		int32_t getExitCode();

		//----------------------------------------------------------------------
		//Register Handler
		//----------------------------------------------------------------------
		/*EN Register VM handlers.
		\return a handler which was set prior to the given handler. New handler should make sure to call prior set handler.
		\param handlertype handler type to set
		\param pCallBack a callback pointer
		*/
		VM_HANDLER_CALLBACK registerHandler( VM_HANDLER_TYPE handlertype, VM_HANDLER_CALLBACK pCallBack );

		//----------------------------------------------------------------------
		//Initialize engines
		//----------------------------------------------------------------------
		/*EN Attaches an IL(intermediate language) stream to the VM.
		return: true when succeeded.
		*/
		bool setILPool( const std::vector< uint8_t >& vecIL);
		
		/*EN Attaches an IL(intermediate language) stream to the VM.
		 return: true when succeeded.
		 */
		bool setILPool( const uint8_t* pILPool, const size_t size_t);

		/*EN Attaches a metadata for corresponding IL(intermediate language) stream to the VM.
		\return true when succeeded.
		*/
		bool setMetaData( CMetaData& MetaData );

		/*EN Retrieves currently set metadata.
		\return currently set metadata.
		*/
		CMetaData& getMetaData() { return m_MetaData; }

		/*EN Binds a built in method to the VM.
		\param strQualifiedName Fully qualified name of the method that need to be bound.
		\param pMethod A pointer to the method that need to be bound.
		\param iLength A number of arguments that the method can accept. This number is set to method.length property.
		\return true when the binding succeeded, false otherwise.
		*/
		bool bindBuiltinMethod( const wstring& strQualifiedName,
								PMETHOD const pMethod,
								const int32_t iLength );

		/*EN Binds a built in method to the VM with a wrapper code given.
		\param strQualifiedName Fully qualified name of the method that need to be bound.
		\param pMethod A pointer to the wrapper code that is generated semi-automatically.
		\param pMethodBody A pointer to the method that need to be bound.
		\return true when the binding succeeded, false otherwise.
		*/
		bool bindBuiltinMethod( const wstring& strQualifiedName,
								PMETHOD const pMethod, PMETHOD const pMethodBody );

		/*EN Binds a built in method to the VM.
		\param strQualifiedName Fully qualified name of the variable that need to be bound.
		\param pvar A pointer to the variable that need to be bound.
		\return true when the binding succeeded, false otherwise.
		*/
		bool bindBuiltinVariable( const wstring& strQualifiedName,
								CVariable* const pvar );

		//----------------------------------------------------------------------
		//Field accesssor
		//----------------------------------------------------------------------
		/*EN Retrieve a static field value with given RID.
		\param rid RID to retrieve a value.
		\return variable that with given RID
		*/
		CVariable& getStaticField( const RID rid );

		/*EN Set a static field value with given RID.
		\param rid RID to set a value.
		\param value a reference to a value to set.
		\return true when the value is set successfully, false otherwise.
		*/
		bool setStaticField( const RID rid, const CVariable& value );

		/*EN Set a static field value with given RID.
		\param rid RID of the static field to be initialized
		\param value a reference to a value to set.
		\param bForce true enforces an overwriting the value even READONLY flag is set.
		\return true when the value is set successfully, false otherwise.
		*/
		bool setStaticField( const RID rid, const CVariable& value, bool bForce );
		
		//----------------------------------------------------------------------
		//Debug functions
		//----------------------------------------------------------------------
		/*EN Attaches a debugger interface to the VM.
		\param pDebugger A pointer to the debugger interface
		\return true when the debugger successfully attached. false otherwise.
		*/
		bool attachDebugger( CCilDebugger* const pDebugger ) { m_pDebugger = pDebugger; return true; }
		
		/*EN Dettaches a debugger interface from the VM.
		\return true when the debugger successfully dettached. false otherwise.
		*/
		bool detachDebugger() { m_pDebugger = NULL; return true; }
		
		/*EN Sets a map of symbol information.
		\attention Symbol information and metadata APIs will be redesigned later.
		\param symbolinfo map of the symbol information.
		\return true when the symbol information map successfully set. false otherwise.
		*/
		bool setSymbolInformation( hash_map<wstring, CG_SYMBOL_INFORMATION>& symbolinfo);

		/*EN Query a symbol information
		\param symbolName Fully qualified name of the symbol.
		\return Information related to hte symbol
		*/
		CG_SYMBOL_INFORMATION& querySymbolInformation( const wstring& symbolName );

		/*EN dump current VM status including variables and IL stream to stdout. This API is for Debug purpose.
		*/
		void debugDumpStats( void );

		//----------------------------------------------------------------------
		//Invoke method from outside of the current context
		//----------------------------------------------------------------------
		/*EN Invokes a methods directly from C++ code.
		\return A return value from the byte code method.
			Returned value need to be freed as a caller's responsibility.
		\param ridMethod RID of the calling method.
		\param iNumArg A number of arguments for the methods.
		\param pArguments A pointer to the argument array
		*/
		CVariable* invokeMethod( const RID ridMethod, const int32_t iNumArg,
											   CVariable* pArguments );

		//----------------------------------------------------------------------
		//Get object pool
		//----------------------------------------------------------------------
		/*EN Retrieves an object pool list
		\return A reference to the object pool. An object pool is a implemented with STL::list
		*/
		list< CVmObject >& getObjectPool() { return m_ObjectPool; }

		/*EN Retrieves a prototype object for given RID
		\return A reference to the prototype object.
		*/
		CVmObject& getPrototypeObject( const RID ridPrototype );

		//----------------------------------------------------------------------
		//Throw an exception from the API
		//----------------------------------------------------------------------
		/*EN Throws an exception from inside of built-in functions.
		\param errObject a pointer to the error object.
		*/
		void throwException( CVariable* errObject );
		/*EN Throws an exception from inside of built-in functions.
		\param pstrMessage A pointer to an error description
		\param pstrName A pointer to an error name
		\param iNumber A pointer to an error number
		*/
		void throwException( wstring* pstrMessage, wstring* pstrName = NULL , int32_t iNumber = -1 );

		//----------------------------------------------------------------------
		//Object handlings
		//----------------------------------------------------------------------
		/*EN Creates a prototype objecft with given RIDs
		\param ridObject RID of the prototype object
		\param ridObjectName String RID of the object name
		\param ridPrototype Object RID of a prototype object for the object
		\return RID of created prototype object
		*/
		RID createPrototypeObject( const RID ridObject,
								const RID ridObjectName,
								const RID ridPrototype );

		/*EN Creates an object with given RID and store it to the object pool
		\param ridObject RID of the object to be created
		\return A pointer to the created object. An object resides in a object pool and has reference count for Garbage collector,
		*/
		VMOBJECTREF createObject( const RID ridObject );

		/*EN Retrieve an object from the variable. If the variable is not an object, a prototype object reference is returned.
		\param var A reference to the variable
		\return A pointer to the object or prototype object that corresponds to the variable type
		*/
		VMOBJECTREF getObjectPointer( CVariable& var );

		//----------------------------------------------------------------------
		//Static members
		//----------------------------------------------------------------------
		/*EN Retrieves Object prototype object RID
		\return RID of the Object prototype object
		*/
		static RID getObjectRID() { return m_ridObject; }
		/*EN Retrieves Array prototype object RID
		\return RID of the Array prototype object
		*/
		static RID getArrayObjectRID() { return m_ridArrayObject; }
		/*EN Retrieves String prototype object RID
		\return RID of the String prototype object
		*/
		static RID getStringObjectRID() { return m_ridStringObject; }
		/*EN Retrieves Function prototype object RID
		\return RID of the Function prototype object
		*/
		static RID getFunctionObjectRID() { return m_ridFunctionObject; }
		/*EN Retrieves System prototype object RID
		\return RID of the System prototype object
		*/
		static RID getSystemObjectRID() { return m_ridSystemObject; }
		/*EN Retrieves Boolean prototype object RID
		\return RID of the Boolean prototype object
		*/
		static RID getBooleanObjectRID() { return m_ridBooleanObject; }
		/*EN Retrieves Number prototype object RID
		\return RID of the Number prototype object
		*/
		static RID getNumberObjectRID() { return m_ridNumberObject; }
		/*EN Retrieves Date prototype object RID
		\return RID of the Date prototype object
		*/
		static RID getDateObjectRID() { return m_ridDateObject; }
		/*EN Retrieves Regex prototype object RID
		\return RID of the Regex prototype object
		*/
		static RID getRegexObjectRID() { return m_ridRegexObject; }
		/*EN Retrieves Error prototype object RID
		\return RID of the Error prototype object
		*/
		static RID getErrorObjectRID() { return m_ridErrorObject; }
		/*EN Retrieves Math prototype object RID
		\return RID of the Math prototype object
		*/
		static RID getMathObjectRID() { return m_ridMathObject; }

		//----------------------------------------------------------------------
		//Eval stack manipulations
		//Those need to be exposed for built in/user supplied functions
		//----------------------------------------------------------------------
		void pushEvalStack( const CVariable& operand );
		void pushEvalStack( const int32_t i );
		void pushEvalStack( const int64_t i );
		void pushEvalStack( const float f );
		void pushEvalStack( const double d );
		void pushEvalStack( const bool b );
		void pushEvalStackUndefined();
		void pushEvalStackNaN();
		void pushEvalStackInfinity();
		void pushEvalStackNull();
		void pushEvalStackObjectRef( const VMOBJECTREF refObject );
		void pushEvalStackVariable( const CVariable& var );
		void pushEvalStackString( const wchar_t* );
		void pushEvalStackString( const wstring* str );
		void pushEvalStackStringPointer( wstring* str );	//Pointer is stored directory and should not be deleted

		//----------------------------------------------------------------------
		//Eval stack helpers
		//----------------------------------------------------------------------
		void popPushEvalStack( const int32_t i ) { popEvalStack(); popEvalStack(); pushEvalStack( i ); };
		void popPushEvalStack( const int64_t i ) { popEvalStack(); popEvalStack(); pushEvalStack( i ); };
		void popPushEvalStack( const float f ) { popEvalStack(); popEvalStack(); pushEvalStack( f ); };
		void popPushEvalStack( const double d ) { popEvalStack(); popEvalStack(); pushEvalStack( d ); };
		void popPushEvalStack( const bool b ) { popEvalStack(); popEvalStack(); pushEvalStack( b ); };
		void popPushEvalStackUndefined() { popEvalStack(); popEvalStack(); pushEvalStackUndefined(); };
		void popPushEvalStackNaN() { popEvalStack(); popEvalStack(); pushEvalStackNaN(); };
		void popPushEvalStackNull() { popEvalStack(); popEvalStack(); pushEvalStackNull(); };
		void popPushEvalStackInfinity() { popEvalStack(); popEvalStack(); pushEvalStackInfinity(); };
		void popThrowTypeError() {	popEvalStack();
									popEvalStack();
									pushEvalStackNaN();
									ThrowExceptionFromVm( &wstring( ERRORSTRING_TYPEERROR_STORESTATICFIELD ),
										&wstring( NAME_BUILTIN_ERROR_OBJECT ),
										ERROR_TYPEERROR_STORESTATICFIELD );
								};

		void popEvalStack( void );
		void popEvalStackFast( void );
		bool isEvalStackEmpty();
		void dupEvalStack( void );
		size_t getEvalStackSize();
		CVariable& getEvalStackFirstEntry();
		CVariable& getEvalStackSecondEntry();
		CVariable& getEvalStackNewEntry();

		void clearEvalStack();			/*EN< Clear eval stack*/
		void clearLocalVariableList();	/*EN< Clear local variable list*/
		void clearArgList();			/*EN< Clear argument list*/
		void clearCallStack();			/*EN< Clear call stack*/
		void clearStaticFields();		/*EN< Clear static field*/

		//----------------------------------------------------------------------
		//Operator routines
		//Those need to be exposed for built-in/user supplied functions
		//----------------------------------------------------------------------
		/*EN Retrieve an argument list
		\return reference of the array list
		*/ 
		CVariable* getArgumentList() { return m_ArgList; };

		/*EN Retrieve a local variable list
		\return reference of the local list
		*/ 
		CVariable* getLocalList() { return m_LocalVariableList; };

		/*EN Takes a variable from the eval stack and stores it to the static field with given RID
		\param rid RID of the static field that the value to be stored in.
		*/
		void storeStaticField( const RID rid );

		/*EN Takes a variable from static field with given RID and put the variable to the eval stack
		\param rid RID of the static field that the value is to be loaded from.
		*/
		void loadStaticField( const RID rid );

		/*EN Takes a variable from the eval stack and stores it to the local variable with given index
		\param iIndex An index of a local variable. Negative index means a local variable in the enclosing scope.
		*/
		void storeLocalVariable( const int32_t iIndex );

		/*EN Takes a variable of the local variable with given index and push it to the eval stack.
		\param iIndex An index of a local variable. Negative index means a local variable in the enclosing scope.
		*/
		void loadLocalVariable( const int32_t iIndex );

		/*EN Takes a variable from the eval stack and stores it to the argument list with given index
		\param iIndex An index of an argument list. 0 means this pointer, Negative index means a local variable in the enclosing scope.
		*/
		void storeArgument( const int32_t iIndex );

		/*EN Takes a variable from the argument list with given index and push it to the eval stack.
		\param iIndex An index of an argument list. 0 means this pointer, Negative index means a local variable in the enclosing scope.
		*/
		void loadArgument( const int32_t iIndex );

		/*EN Retrieves a table of the static field vector
		\return A vector of the static field variables
		*/
		CVariable* getStaticFieldTable() { return m_pStaticFieldTable; }

		/*EN Takes a variable from the eval stack and stores it to the destination variable with a respect of a type constraint.
		\param varDest A reference of a variable that the value from an eval stack is stored in.
		*/
		void storeAsRestrictedType( CVariable& varDest );

		/*EN Calls given RID, invoked from Function.apply*/ 
		void CallOperator( const CVariable* thisPointer,
							const RID rid,
							const bool bConstructor = false );
		/*EN Stores a variable to argument list by hand. Invoked from Function.apply*/ 
		void storeArgumentList( const CVariable& var );

		/*EN Generates 'arguments' property based on current callstack and arguments list information on the fly.
		*/
		CVariable* generateArgumentsProperty();
		
		/*EN Generates 'restArguments' argument if the method signature requires it.
		This helper should be called immediate before function call.
		*/
		void generateRestArguments( int32_t iNumArgsToStore );

		/*EN Generates 'caller' property based on current callstack and arguments list information on the fly.
		*/
		CVariable* generateCallerProperty();

		/*EN Disposes arguments given to the API explicitly*/ 
		void disposeArgumentList();

		/*EN Resets number of arguments in the list*/ 
		void resetArgumentList();

		/*EN Returns exception handler nesting level
		\return Current exception handler nesting level.
		        Return non zero if current code is inside an exception handler clause.
		*/
		int32_t getExectipnHandlerNestingLevel() { return m_ExceptionHandlerNestingLevel; }


		//----------------------------------------------------------------------
		//Print() management
		//----------------------------------------------------------------------
		/*EN Flush print() buffer if some strings are in the buffer, otherwise, do nothing..
		*/
		void flushPrintBuffer() { wprintf( m_strPrintBuffer.data() ); m_strPrintBuffer.clear(); }

		/*EN Retrieves print() buffer if some strings are in the buffer, otherwise, do nothing..
		\return A reference for the wstring that is used for print() intermedeiate buffer
		*/
		wstring& getPrintBuffer() { return m_strPrintBuffer; }

		/*EN Retrieves print() buffer if some strings are in the buffer, otherwise, do nothing..
		\param str wstring that will be printed.
		*/
		void appendPrintBuffer( wstring& str ) { m_strPrintBuffer.append( str ); }

	private:
		//----------------------------------------------------------------------
		//Initialize engines
		//----------------------------------------------------------------------
		bool setStaticFieldTypeTable( vector< OPERAND_TYPE_FLAG >& vecSFType );
		void initializePrototypeObjects();

		/*EN Set a static field value with a function object with given method RID.
		This method is a helper function to initialize a static field 
c		*/
		bool setStaticFieldFunctionObject( const RID rid, const RID ridMethod );

		//----------------------------------------------------------------------
		//Garbage collector helper
		//----------------------------------------------------------------------
		void clearRefCount();
		void cleanObjectPool();

		//----------------------------------------------------------------------
		//Throw an exception from the VM
		//----------------------------------------------------------------------
		/*
		EN Throw an exception from inside VM routine.
		\param pstrMessage Exception description
		\param pstrName Exception name
		\param iNumber Exception Number
		\return true if the exception handled by a handler
		*/
		bool ThrowExceptionFromVm( wstring* pstrMessage, wstring* pstrName = NULL , int32_t iNumber = -1 );

		//----------------------------------------------------------------------
		//Byte code stream accesser
		//----------------------------------------------------------------------
		uint8_t getNextBytecode( void );	/*EN< Retrieve next byte code from an attached byte code stream*/
		RID getNextToken( void );			/*EN< Retrieve next Token from an attached byte code stream*/
		int32_t getNextInt32( void );		/*EN< Retrieve next int32_t value from an attached byte code stream*/
		int64_t getNextInt64( void );		/*EN< Retrieve next int64_t value from an attached byte code stream*/
		int8_t getNextInt8( void );			/*EN< Retrieve next int8_t value from an attached byte code stream*/
		uint8_t getNextUint8( void );		/*EN< Retrieve next uint8_t value from an attached byte code stream*/
		float getNextFloat( void );			/*EN< Retrieve next float value from an attached byte code stream*/
		double getNextDouble( void );		/*EN< Retrieve next double value from an attached byte code stream*/

		//----------------------------------------------------------------------
		//Private Field accessor
		//----------------------------------------------------------------------
		CVariable& getLocalVariable( const int32_t iIndex );
		bool setLocalVariable( const int32_t iIndex, const CVariable& value );
		CVariable& getArgument( const int32_t iIndex );
		bool setArgument( const int32_t iIndex, const CVariable& value );

		//----------------------------------------------------------------------
		//call stack accesses
		//----------------------------------------------------------------------
		/*EN Retrieves a top of the callstack regardless of a stack type.
		A callstack could be one of the callstack, exception handlers
		\return A pointer to the callstack top entry
		*/
		VM_CALLSTACK* getCurrentCallStack() { return &m_CallStack.back(); }

		/*EN Search and retrieves a callstack entries with given argument criteria matches
		\param type A type of callstack to be returned
		\param iLevel A callstack level to be returned.
		\return A pointer to the callstack entry that matches an arguments criateria
		*/
		VM_CALLSTACK* getCurrentCallStack( const VM_CALLSTACK_TYPE type, int iLevel = 1 )
		{
			vector< VM_CALLSTACK >::reverse_iterator it = m_CallStack.rbegin();
			while( it != m_CallStack.rend() )
			{
				if( it->type == type && !(--iLevel) )
				{
					return &*it;
				}
				++it;
			}
			return NULL;
		}

		//----------------------------------------------------------------------
		//Operator routines
		//----------------------------------------------------------------------
		void addOperator();
		void subtractOperator();
		void multiplyOperator();
		void divisionOperator();
		void negOperator();
		void reminderOperator();
		void incStaticFieldOperator( const RID rid );
		void incOperator();
		void decOperator();

		void bitwiseAndOperator();
		void bitwiseOrOperator();
		void bitwiseXorOperator();
		void bitwiseNotOperator();

		void ShlOperator();
		void ShrOperator();
		void ShrUnOperator();

		void BrTrueOperator();
		void BrTrueShortOperator();
		void BrFalseOperator();
		void BrFalseShortOperator();

		void BeqOperator();
		void BeqShortOperator();
		void BneOperator();
		void BneShortOperator();
		void BngeOperator();
		void BngeShortOperator();
		void BngtOperator();
		void BngtShortOperator();
		void BnleOperator();
		void BnleShortOperator();
		void BnltOperator();
		void BnltShortOperator();

		void CeqOperator();
		void CneOperator();
		void CgtOperator();
		void CgeOperator();
		void CleOperator();
		void CltOperator();

		void CallOperator();									//call given rid
		void CallOperator( const CVariable& var );					//call given variable
		bool RetOperator();
		void CreateNewObject();
		void CreateNewStringValue();
		void LoadObjectRef();
		void EnterOperator();
		void LeaveOperator();
		bool ThrowOperator();
		void FinallyOperator();

		void convertToBoolean();

		//----------------------------------------------------------------------
		//Local variables
		//----------------------------------------------------------------------
		void reserveLocalVariables( const RID ridLocalsToReserve );
		void disposeLocalVariables( const uint32_t iLocalsToDispose );
		uint32_t getCurrentLocalVariableIndex( void ) { return (uint32_t)m_iLocalVariableListIndex; }

		//----------------------------------------------------------------------
		//Argument list
		//----------------------------------------------------------------------
		void storeArgumentListWithThisPointer( const uint32_t iArgToStore );
		void storeArgumentListWithoutThisPointer( const uint32_t iArgToStore );
		void disposeArgumentList( const uint32_t iArgToDispose );
		uint32_t getCurrentArgumentListIndex( void ) { return (uint32_t)m_iArgListIndex; }
		void pushArgumentListUndefined( void );
		void popArgumentList( void );
		
		//----------------------------------------------------------------------
		//ILPool vector
		//each function object has 1 ILFRAGMENTINFO
		//----------------------------------------------------------------------
		ILFRAGMENTINFO m_CurrentILPool;

		//----------------------------------------------------------------------
		// VM status
		//----------------------------------------------------------------------
		uint8_t* m_pCurrentInstruction;
		int32_t m_iTickCount;
		VM_STATUS_TYPE m_status;
		
		//----------------------------------------------------------------------
		//Debug Variables
		//----------------------------------------------------------------------
		CStopWatch m_stopwatch;
		CCilDebugger* m_pDebugger;
		map<wstring, CG_SYMBOL_INFORMATION> m_SymbolInfoPool;		//Symbol information
		
		//----------------------------------------------------------------------
		//Evaluation stack
		//----------------------------------------------------------------------
		CVariable	m_OperandEvalStack[ MAX_EVALSTACK ];
		CVariable*	m_pCurrentEvalStack;
		CVariable*	m_pMaxEvalStack;

		CVariable	m_LocalVariableList[ MAX_LOCAL_VARIABLE ];
		uint32_t	m_iLocalVariableListIndex;
		uint32_t	m_iMaxLocalVariableListIndex;

		CVariable	m_ArgList[ MAX_ARG_VARIABLE ];
		uint32_t	m_iArgListIndex;
		uint32_t	m_iMaxArgListIndex;

		void updateEvalStackIndex() {
			m_pCurrentEvalStack++;
#ifdef VM_TRACK_MAXSTACKUSAGE
			if( m_pCurrentEvalStack > m_pMaxEvalStack )
				m_pMaxEvalStack = m_pCurrentEvalStack;
#endif
		}
		void updateLocalVariableListIndex() {
			m_iLocalVariableListIndex++;
#ifdef VM_TRACK_MAXSTACKUSAGE
			if( m_iLocalVariableListIndex > m_iMaxLocalVariableListIndex )
				m_iMaxLocalVariableListIndex++;
#endif
		}
		void updateArgListIndex() {
			m_iArgListIndex++;
#ifdef VM_TRACK_MAXSTACKUSAGE
			if( m_iArgListIndex > m_iMaxArgListIndex )
				m_iMaxArgListIndex++;
#endif
		}

		//----------------------------------------------------------------------
		//Variable Tables
		//----------------------------------------------------------------------
		CVariable*		m_pStaticFieldTable;	//This talbe is accesible through ld/stsfld instructions, accessed via RID
		uint32_t		m_iStaticFieldTalbeSize;
		list< CVmObject >	m_ObjectPool;
		vector< CVmObject >	m_BuiltinPrototypeObjectTable;
		CMetaData			m_MetaData;

		//----------------------------------------------------------------------
		//Caller/calee stack
		//----------------------------------------------------------------------
		vector< VM_CALLSTACK > m_CallStack;
		uint32_t m_iCurrentThisPointerIndex;
		int32_t m_iCallStackRoot;
		int32_t m_iNumGivenArguments;	//Keep tracking # of arguments for Exception throw

		//----------------------------------------------------------------------
		//GC
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//MetaData
		//----------------------------------------------------------------------
		static RID m_ridObject;					//Object RID
		static RID m_ridFunctionObject;			//Function Object RID
		static RID m_ridSystemObject;			//System Object RID
		static RID m_ridStringObject;			//String object RID
		static RID m_ridArrayObject;			//Array object RID
		static RID m_ridErrorObject;			//Error object RID
		static RID m_ridRegexObject;			//Regex object RID
		static RID m_ridBooleanObject;			//Boolean object RID
		static RID m_ridNumberObject;			//Numer object RID
		static RID m_ridDateObject;				//Date object RID
		static RID m_ridMathObject;				//Math object RID

		//----------------------------------------------------------------------
		//print buffer, flushed when exiting the VM
		//----------------------------------------------------------------------
		wstring m_strPrintBuffer;				/*<EN Buffer for print().*/

		//----------------------------------------------------------------------
		//Exception handling
		//----------------------------------------------------------------------
		int32_t m_ExceptionHandlerNestingLevel;	/*Excepiton Handler Nest Level*/

		//----------------------------------------------------------------------
		//VM Handler
		//----------------------------------------------------------------------
		VM_HANDLER_CALLBACK m_Handler[ VM_HANDLER_MAX ];
		
	};

	/***************************************************************************
	 *      VM helper APIs
	 ***************************************************************************/
#define	VMRET_INT int32_t
#define	VMRET_FLOAT float
#define	VMRET_DOUBLE double
	
#define VMARG_INT int32_t
#define VMARG_FLOAT float
#define VMARG_DOUBLE double
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 no parameter, with return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET >
	void MakeWrapperStdP0( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		RET ret = ((RET (*)( void ) )pMethodBody)();
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 1 parameter, with return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET, class P1 >
	void MakeWrapperStdP1( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		//Skip This pointer
		pArguments ++;
		
		P1 param1;
		switch( iNumArguments )
		{
			default:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		RET ret = ((RET (*)( P1 ) )pMethodBody)( param1 );
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 2 parameters, with return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET, class P1, class P2 >
	void MakeWrapperStdP2( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		//Skip This pointer
		pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		switch( iNumArguments )
		{
			default:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		RET ret = ((RET (*)( P1, P2 ) )pMethodBody)( param1, param2 );
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 3 parameters, with return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET, class P1, class P2, class P3 >
	void MakeWrapperStdP3( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		//Skip This pointer
		pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		switch( iNumArguments )
		{
			default:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		RET ret = ((RET (*)( P1, P2, P3 ) )pMethodBody)( param1, param2, param3 );
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 4 parameters, with return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET, class P1, class P2, class P3, class P4 >
	void MakeWrapperStdP4( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		//Skip This pointer
		pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		P4 param4 = (P4)0;
		switch( iNumArguments )
		{
			default:
				param4 = (P4)*(pArguments+3);
			case 4:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		RET ret = ((RET (*)( P1, P2, P3, P4 ) )pMethodBody)( param1, param2, param3, param4 );
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 5 parameters, with return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET, class P1, class P2, class P3, class P4, class P5 >
	void MakeWrapperStdP5( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		//Skip This pointer
		pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		P4 param4 = (P4)0;
		P5 param5 = (P5)0;
		switch( iNumArguments )
		{
			default:
				param5 = (P5)*(pArguments+4);
			case 5:
				param4 = (P4)*(pArguments+3);
			case 4:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		RET ret = ((RET (*)( P1, P2, P3, P4, P5 ) )pMethodBody)( param1, param2, param3, param4, param5 );
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	//----------------------
	//Void return version
	//----------------------
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 no parameter, without return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	void MakeWrapperStdVoidP0( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody );
	/*
	 {
	 ((void (*)( void ) )pMethodBody)();
	 pVm->pushEvalStackUndefined();
	 
	 return;
	 }*/
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 1 parameter, without return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class P1 >
	void MakeWrapperStdVoidP1( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody )
	{
		//Skip This pointer
		pArguments ++;
		
		P1 param1;
		switch( iNumArguments )
		{
			default:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		((void (*)( P1 ) )pMethodBody)( param1 );
		pVm->pushEvalStackUndefined();
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 2 parameters, without return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class P1, class P2 >
	void MakeWrapperStdVoidP2( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody )
	{
		//Skip This pointer
		pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		switch( iNumArguments )
		{
			default:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		
		((void (*)( P1, P2 ) )pMethodBody)( param1, param2 );
		pVm->pushEvalStackUndefined();
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 3 parameters, without return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class P1, class P2, class P3 >
	void MakeWrapperStdVoidP3( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody )
	{
		//Skip This pointer
		pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		switch( iNumArguments )
		{
			default:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		((void (*)( P1, P2, P3 ) )pMethodBody)( param1, param2, param3 );
		pVm->pushEvalStackUndefined();
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 4 parameters, without return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class P1, class P2, class P3, class P4 >
	void MakeWrapperStdVoidP4( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody )
	{
		//Skip This pointer
		pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		P4 param4 = (P4)0;
		switch( iNumArguments )
		{
			default:
				param4 = (P4)*(pArguments+3);
			case 4:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		((void (*)( P1, P2, P3, P4 ) )pMethodBody)( param1, param2, param3, param4 );
		pVm->pushEvalStackUndefined();
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function that does not require 'this' value.
	 5 parameters, without return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class P1, class P2, class P3, class P4, class P5 >
	void MakeWrapperStdVoidP5( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody )
	{
		//Skip This pointer
		pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		P4 param4 = (P4)0;
		P5 param5 = (P5)0;
		switch( iNumArguments )
		{
			default:
				param5 = (P5)*(pArguments+4);
			case 5:
				param4 = (P4)*(pArguments+3);
			case 4:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		((void (*)( P1, P2, P3, P4, P5 ) )pMethodBody)( param1, param2, param3, param4, param5 );
		pVm->pushEvalStackUndefined();
		
		return;
	}
	//-------------------------------------------
	//Member version
	//-------------------------------------------
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 No parameter, with a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET >
	void MakeWrapperMtdP0( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		//Keep This pointer
		CVariable* pVar = &*pArguments ++;
		
		RET ret = ((RET (*)( CVariable* ) )pMethodBody)( pVar );
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 1 parameter, with a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET, class P1 >
	void MakeWrapperMtdP1( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		//Keep This pointer
		CVariable* pVar = &*pArguments ++;
		
		P1 param1;
		switch( iNumArguments )
		{
			default:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		RET ret = ((RET (*)( CVariable*, P1 ) )pMethodBody)( pVar, param1 );
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 2 parameters, with a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET, class P1, class P2 >
	void MakeWrapperMtdP2( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		//Keep This pointer
		CVariable* pVar = &*pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		switch( iNumArguments )
		{
			default:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		RET ret = ((RET (*)( CVariable*, P1, P2 ) )pMethodBody)( pVar, param1, param2 );
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 3 parameters, with a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET, class P1, class P2, class P3 >
	void MakeWrapperMtdP3( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		//Keep This pointer
		CVariable* pVar = &*pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		switch( iNumArguments )
		{
			default:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		RET ret = ((RET (*)( CVariable*, P1, P2, P3 ) )pMethodBody)( pVar, param1, param2, param3 );
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 4 parameters, with a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET, class P1, class P2, class P3, class P4 >
	void MakeWrapperMtdP4( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		//Keep This pointer
		CVariable* pVar = &*pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		P4 param4 = (P4)0;
		switch( iNumArguments )
		{
			default:
				param4 = (P4)*(pArguments+3);
			case 4:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		RET ret = ((RET (*)( CVariable*, P1, P2, P3, P4 ) )pMethodBody)( pVar, param1, param2, param3, param4 );
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 5 parameters, with a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class RET, class P1, class P2, class P3, class P4, class P5 >
	void MakeWrapperMtdP5( CCilVm* const pVm,
						  const int32_t iNumArguments,
						  CVariable* pArguments,
						  PMETHOD pMethodBody )
	{
		//Keep This pointer
		CVariable* pVar = &*pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		P4 param4 = (P4)0;
		P5 param5 = (P5)0;
		switch( iNumArguments )
		{
			default:
				param5 = (P5)*(pArguments+4);
			case 5:
				param4 = (P4)*(pArguments+3);
			case 4:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		RET ret = ((RET (*)( CVariable*, P1, P2, P3, P4, P5 ) )pMethodBody)( pVar, param1, param2, param3, param4, param5 );
		pVm->pushEvalStack( ret );
		
		return;
	}
	
	//----------------------
	//Void return version
	//----------------------
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 no parameter, without a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	void MakeWrapperMtdVoidP0( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody );
	/*
	 {
	 //Keep This pointer
	 CVariable* pVar = &*pArguments ++;
	 
	 ((void (*)( CVariable* ) )pMethodBody)( pVar );
	 pVm->pushEvalStackUndefined();
	 
	 return;
	 }
	 */
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 1 parameter, without a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class P1 >
	void MakeWrapperMtdVoidP1( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody )
	{
		//Keep This pointer
		CVariable* pVar = &*pArguments ++;
		
		P1 param1;
		switch( iNumArguments )
		{
			default:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		((void (*)( CVariable*, P1 ) )pMethodBody)( pVar, param1 );
		pVm->pushEvalStackUndefined();
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 2 parameters, without a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class P1, class P2 >
	void MakeWrapperMtdVoidP2( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody )
	{
		//Keep This pointer
		CVariable* pVar = &*pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		switch( iNumArguments )
		{
			default:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		
		((void (*)( CVariable*, P1, P2 ) )pMethodBody)( pVar, param1, param2 );
		pVm->pushEvalStackUndefined();
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 3 parameters, without a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class P1, class P2, class P3 >
	void MakeWrapperMtdVoidP3( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody )
	{
		//Keep This pointer
		CVariable* pVar = &*pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		switch( iNumArguments )
		{
			default:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		((void (*)( CVariable*, P1, P2, P3 ) )pMethodBody)( pVar, param1, param2, param3 );
		pVm->pushEvalStackUndefined();
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 4 parameters, without a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class P1, class P2, class P3, class P4 >
	void MakeWrapperMtdVoidP4( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody )
	{
		//Keep This pointer
		CVariable* pVar = &*pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		P4 param4 = (P4)0;
		switch( iNumArguments )
		{
			default:
				param4 = (P4)*(pArguments+3);
			case 4:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		((void (*)( CVariable*, P1, P2, P3, P4 ) )pMethodBody)( pVar, param1, param2, param3, param4 );
		pVm->pushEvalStackUndefined();
		
		return;
	}
	
	/*EN Wrapper template classes for user specified functions.
	 This wrapper is for a function thatrequires 'this' value as a first parameter.
	 5 parameters, without a return variable version.
	 \param pVm A pointer to the VM
	 \param iNumArguments A number of given parameters.
	 \param pArguments A pointer to the arguments array.
	 \param pMethodBody An API body.
	 \ingroup MDL_LIB_VM
	 */
	template< class P1, class P2, class P3, class P4, class P5 >
	void MakeWrapperMtdVoidP5( CCilVm* const pVm,
							  const int32_t iNumArguments,
							  CVariable* pArguments,
							  PMETHOD pMethodBody )
	{
		//Keep This pointer
		CVariable* pVar = &*pArguments ++;
		
		P1 param1 = (P1)0;
		P2 param2 = (P2)0;
		P3 param3 = (P3)0;
		P4 param4 = (P4)0;
		P5 param5 = (P5)0;
		switch( iNumArguments )
		{
			default:
				param5 = (P5)*(pArguments+4);
			case 5:
				param4 = (P4)*(pArguments+3);
			case 4:
				param3 = (P3)*(pArguments+2);
			case 3:
				param2 = (P2)*(pArguments+1);
			case 2:
				param1 = (P1)*pArguments;
			case 1:
				break;
		}
		
		((void (*)( CVariable*, P1, P2, P3, P4, P5 ) )pMethodBody)( pVar, param1, param2, param3, param4, param5 );
		pVm->pushEvalStackUndefined();
		
		return;
	}
	
}//namespace cri
