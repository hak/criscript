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
 * Module   : Codegen interface definition
 * File     : icodegen.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

namespace cri {

/***************************************************************************
 *      Typedef
 ***************************************************************************/
/*EN RID(Resource ID is a key value to manage data in the CRI Script*/
typedef uint32_t RID;

/***************************************************************************
 *      Variable operand type
 ***************************************************************************/
/*EN Operand Type settings
Note: don't change the order of enumration, since some codes has dependency to the value.
*/
enum OPERAND_TYPE {
	OPERAND_UNDEFINED	= 0,			/*EN< UNDEFINED Operand*/
	OPERAND_NULL		= 1,			/*EN< NULL Operand*/
	OPERAND_OBJECTREF	= 2,			/*EN< Object reference Operand*/
	OPERAND_INT			= 3,			/*EN< Integer Operand*/
	OPERAND_UNSIGNEDINT	= 4,			/*EN< Unsigned integer Operand. Currently not used*/
	OPERAND_INT64		= 5,			/*EN< Integer64 Operand*/
	OPERAND_UNSIGNEDINT64=6,			/*EN< Unsigned integer64 Operand. Currently not used*/
	OPERAND_FLOAT		= 7,			/*EN< 32bit float Operand*/
	OPERAND_DOUBLE		= 8,			/*EN< 64bit float Operand*/
	OPERAND_STRING		= 9,			/*EN< String value Operand*/
	OPERAND_BOOLEAN		=10,			/*EN< Boolean Operand*/
	OPERAND_NAN			=11,			/*EN< NaN Operand. Note: Would be merged to float operand*/
	OPERAND_SYSTEM_ACCESSOR	=0xe,		/*EN< Internal use. Need to invoke setter/getter for the property*/
	OPERAND_SYSTEM_DELETION	=0xf,		/*EN< Internal use. Mark an operand deleted*/
};

/*EN Operand flag values*/
enum OPERAND_FLAG {
	OPERAND_FLAG_STRICT			= 0x8,	/*EN< The operand has type constraintd*/
	OPERAND_FLAG_READONLY		= 0x4,	/*EN< The operand is read only*/
	OPERAND_FLAG_DONTENUM		= 0x2,	/*EN< The operand is not enumerated*/
	OPERAND_FLAG_DONTDELETE		= 0x1,	/*EN< The operand can not be deleted*/
	OPERAND_FLAG_NONE			= 0x0,	/*EN< No flags*/
};

#define OperandType(type) (type)

/*EN Struct that represents the operand type. 32 bit total*/
struct OPERAND_TYPE_FLAG {
	OPERAND_TYPE	type : 4;	/*EN< Operand type. 4 bit bitfield*/
	OPERAND_FLAG	flag : 4;	/*EN< Operand flag. 4 bit bitfield*/
	RID				ridConstraintType : 24;		/*EN< Operand constraint type. 24 bit bitfield since the operand type should be always objref*/

};

/***************************************************************************
 *      Method data definition
 ***************************************************************************/
/*EN Method type enumeration*/
enum METHOD_TYPE {
	METHOD_NATIVE	= 0,	/*EN<Native method*/
	METHOD_WRAPPED	= 1,	/*EN<Wrapped method*/
	METHOD_CIL		= 2,	/*EN<CIL method (A method defined inside a script)*/
	METHOD_EHCLAUSE	= 3,	/*EN<Exception handler*/
};

#define MethodType(t) (t)

/*EN Structure that has method definitions*/
struct METHOD_DEF {
	RID			ridMethod;					/*EN<Method RID*/
	RID			ridScope;					/*EN<Enclosing scope's RID*/
	RID			ridReturnTypeConstraint;	/*EN<Return type if the method has return type constraint*/
	METHOD_TYPE	MethodType;					/*EN<Type of the mothod*/

	/*EN Union of ILOffset/Function pointer*/
	union {					
		int32_t	iILOffset;					/*EN< Offset value of IL stream of the method*/
		void	(*lpFunction)();			/*EN< Function pointer to the method if the method is native one*/
	};
	int32_t	iNumArgments;					/*EN< Number of arguments*/
	bool	bRestArgments;					/*EN< true if it has rest arguments*/

	/*EN Union of a number of local variables/a function body if the method is a wrapped function*/
	union {
		int32_t	iNumLocals;					/*EN< Number of local variables*/
		void	(*lpFunctionBody)();		/*EN< Pointer to the function body if the method is a wrapped function*/
	};
	uint32_t	iNumberOfReturnPass;		/*EN< Number of return passes in the method*/

	/*EN Union of argument list RID/catch clause RID*/
	union {
		RID			ridArgList;				/*EN<RID of the argument list*/
		RID			ridCatch;				/*EN<RID of the 'catch' clause*/
	};

	/*EN Union of local variable list RID/finally clause RID*/
	union {
		RID			ridLocalVariableList;	/*EN<RID of the local variable list*/
		RID			ridFinally;				/*EN<RID of the 'finally' clause*/
	};
};

/***************************************************************************
 *      Code geneartion node tree object type
 ***************************************************************************/
enum CG_NODE_TYPE {
	CG_NODE_NULL = 0,
	CG_NODE_STRINGLITERAL,
	CG_NODE_REGEXLITERAL,
	CG_NODE_INTEGERLITERAL,
	CG_NODE_BOOLEANLITERAL,
	CG_NODE_FLOATLITERAL,
	CG_NODE_IDENTIFIER,
	CG_NODE_FORMULANODE,
	CG_NODE_IFNODE,
	CG_NODE_FORNODE,
	CG_NODE_FORINNODE,
	CG_NODE_DOWHILENODE,
	CG_NODE_FUNCTIONDECLNODE,
	CG_NODE_EXPRESSIONNODE,
	CG_NODE_RETNODE,
	CG_NODE_EXCEPTIONHANDLINGNODE,
	CG_NODE_CATCHNODE,
	CG_NODE_FINALLYNODE,
	CG_NODE_THROWNODE,
	CG_NODE_LABELEDNODE,
	CG_NODE_BREAKNODE,
	CG_NODE_CONTINUENODE,
	CG_NODE_SWITCHNODE,
	CG_NODE_CASECLAUSENODE,
	CG_NODE_WITHNODE,
	CG_NODE_TYPEDIDENTIFIER,
	CG_NODE_RESTARGS,
};

enum CG_OPERATIONTYPE {
	CG_NONE,
	CG_ADD,
	CG_MUL,
	CG_SUB,
	CG_DIV,
	CG_REM,
	CG_VARIABLEDECL,
	CG_TYPEDVARIABLEDECL,
	CG_VARIABLEASSIGN,
	CG_EQUAL,
	CG_NOTEQUAL,
	CG_STRICTEQUAL,
	CG_STRICTNOTEQUAL,
	CG_GREATER,
	CG_GREATEREQUAL,
	CG_LESS,
	CG_LESSEQUAL,
	CG_BITWISEAND,
	CG_BITWISEOR,
	CG_BITWISEXOR,
	CG_BITWISENOT,
	CG_SHL,
	CG_SHR,
	CG_SHRUN,
	CG_UNARYPLUS,
	CG_UNARYMINUS,
	CG_UNARYINC,
	CG_UNARYDEC,
	CG_UNARYPOSTINC,
	CG_UNARYPOSTDEC,
	CG_UNARYLOGICALNOT,
	CG_UNARYBITWISENOT,
	CG_UNARYVOID,
	CG_UNARYDELETE,
	CG_UNARYTYPEOF,
	CG_CALLEXPRESSION,
	CG_PROPERTYBYID,
	CG_PROPERTYBYEXP,
	CG_objectLiteral,
	CG_arrayLiteral,
	CG_PROPERTYNAMEANDVALUE,
	CG_LOGICALAND,
	CG_LOGICALOR,
	CG_CONDITIONAL,
	CG_NEW,
	CG_INSTANCEOF,
	CG_IN,
};

/***************************************************************************
 *      Extension Type
 ***************************************************************************/
/*EN CRI Script extension setting*/
enum EXTENSION_FLAGS {
	EXTENSION_CRISCRIPT,	/*EN< CRI Script specific extension*/
	EXTENSION_JSCRIPT,		/*EN< JScript compatible extension*/
	EXTENSION_ES4DRAFT,		/*EN< ES4 Draft compatible extension*/
};

/***************************************************************************
 *      Target settings
 ***************************************************************************/
/*EN Target Endianness setting*/
enum TARGET_ENDIANNESS {
	TARGET_LITTLEENDIAN,	/*EN< The target is little endian*/
	TARGET_BIGENDIAN,		/*EN< The target is big endian*/
};

/***************************************************************************
 *      Object node tree
 ***************************************************************************/
class CCgNodeData {
public:
	CCgNodeData (): NodeType(CG_NODE_NULL), rid(0),
					  siblingNode(NULL), rhsNode(NULL), lhsNode(NULL),
					  initialNode(NULL), statementNode(NULL),
					  conditionNode(NULL), argumentlistNode(NULL),
					  bBlockTraverseLeft(false),
					  bBlockTraverseRight(false),
					  iFileId( 0 ),
					  iLineNumber( 0 ),
					  iColumn( 0 )
	{
	}
	~CCgNodeData ()
	{
 		if( NodeType == CG_NODE_IDENTIFIER )
		{
			//Delete string
			delete pIdentifier;
		}
		if( initialNode )	delete initialNode;
		if( statementNode )	delete statementNode;
		if( conditionNode )	delete conditionNode;
		if( argumentlistNode )	delete argumentlistNode;
		if( lhsNode ) delete lhsNode;
		if( rhsNode ) delete rhsNode;
		if( siblingNode ) delete siblingNode;
	}
	CG_NODE_TYPE NodeType;
	union {
		RID rid;
		CG_OPERATIONTYPE iOperation;
		int32_t iIndex;
		wstring* pIdentifier;
	};

	void SetSibling( CCgNodeData * pSibling )
	{
		CCgNodeData* pData = this;
		while( pData->siblingNode != NULL )
		{
			pData = pData->siblingNode; 
		}
		pData->siblingNode = pSibling;
	}
	
	bool bBlockTraverseLeft;
	bool bBlockTraverseRight;
	CCgNodeData * siblingNode;
	CCgNodeData * lhsNode;
	CCgNodeData * rhsNode;
	CCgNodeData * initialNode;
	CCgNodeData * statementNode;
	uint32_t iFileId;
	uint32_t iLineNumber;
	uint32_t iColumn;
	
	union {
		CCgNodeData * conditionNode;
		CCgNodeData * identifier;
	};
	
	union {
		CCgNodeData * loopendNode;
		CCgNodeData * argumentlistNode;
	};
};

//Now does not use boost due to yacc does not like copy constructor
//typedef boost::shared_ptr<OBJECTDATA> OBJECTDATAPTR;
};

namespace cri {
/***************************************************************************
 *      Header generated by bison
 ***************************************************************************/
#include "criscriptparser.hpp"
};

namespace cri {
/***************************************************************************
 *      Symbol information
 *      Used for tracking debug information & RID
 ***************************************************************************/
/*EN Symbol flag information*/
enum CG_SYMBOL_FLAG {
	CG_SYMBOL_FLAG_GLOBAL	= 0,		/*EN< Global Symbol*/
	CG_SYMBOL_FLAG_STATIC	= 0,		/*EN< Static Symbol*/
	CG_SYMBOL_FLAG_LOCAL	= 0x1,		/*EN< Local Symbol*/
	CG_SYMBOL_FLAG_ARG		= 0x2,		/*EN< Symbol in Argument list*/
	CG_SYMBOL_FLAG_PROPERTY = 0x3,		/*EN< Property member*/
	CG_SYMBOL_FLAG_INSTANCE = 0x4,		/*EN< Instance member*/
	CG_SYMBOL_FLAG_READONLY =0x10,		/*EN< The symbol is READ ONLY*/
};
#define CG_SYMBOL_FLAG_TYPEMASK (0xf)
#define CG_SYMBOL_FLAG_ATTRIBUTEMASK (0xf0)

/*EN
\def SymbolTypeFromFrag(f)
A macro to retrieve symbol flag
*/
#define SymbolTypeFromFrag(f) ((f) & CG_SYMBOL_FLAG_TYPEMASK )

/*EN
\def SymbolAttributeFromFrag(f)
A macro to retrieve symbol attribute
*/
#define SymbolAttributeFromFrag(f) ((f) & CG_SYMBOL_FLAG_ATTRIBUTEMASK )

/*EN Symbol information structure for a codegen*/
struct CG_SYMBOL_INFORMATION {
	RID rid;				/*EN< RID of the symbol*/
	RID ridScope;			/*EN< RID of the enclosing scope*/
	CG_SYMBOL_FLAG Flags;	/*EN< Flags of the symbol*/
	union {
		int32_t iIndex;		/*EN< Index of the symbol in the local/argument list. Union.*/
		RID ridObject;		/*EN< RID of the object. Union.*/
	};
};

/***************************************************************************
 *      VM object metadata
 ***************************************************************************/

class CParser;
class CCilVm;
class CVmObject;
}

#include "variable.h"

namespace cri {
/*EN Object type information structure
*/
class CObjectTypeInfo {
public:
	CObjectTypeInfo();	/*EN< ctor*/
	~CObjectTypeInfo();	/*EN< dtor*/
	CObjectTypeInfo( const CObjectTypeInfo& right );	/*EN< Copy ctor*/

	/*EN Adds a property to the object.
	\param name a name string of the property
	\param value a value of the property
	*/
	void addProperty( const wstring& name, const CVariable& value );

	/*EN Retrieves RID of the Object name
	\return Object name's RID
	*/
	RID getObjectName( void );

	/*EN Retrieves RID of the Object
	\return Object's RID
	*/
	RID getRID( void ) { return m_ridObject; }
	
	/*EN Retrieves RID of the Object constructor
	\return An object constructor's RID
	*/
	RID getConstructor( void ) { return m_ridConstructor; }

	/*EN Retrieves RID of the Object call attribute
	\return Object call attribute's RID
	*/
	RID getCall( void ) { return m_ridCall; }
	
	/*EN Sets RID of the Object
	\param rid Object's RID
	*/
	void setRID( const RID rid );

	/*EN Sets RID of the Object name
	\param rid Object name's RID
	*/
	void setObjectName( const RID rid );

	/*EN Sets RID of the Object call attribute
	\param rid Object call attribute's RID
	*/
	void setCallProperty( const RID rid ) { m_ridCall = rid; }

	/*EN Sets RID of the Object ctor attribute
	\param rid Object ctor attribute's RID
	*/
	void setConstructorProperty( const RID rid ) { m_ridConstructor = rid; }

	/*EN Retrieves a reference of the property map of the object.
	\return Reference of the object proeprty./
	*/
	hash_map< wstring, CVariable >& getPropertyMap() { return m_mapProperty; }
private:
protected:
	RID m_ridObject;				/*EN< RID of the object definition*/
	RID m_ridObjectName;			/*EN< RID of the object name string*/
	RID m_ridConstructor;			/*EN< RID of Constructorl attribute of the object*/
	RID m_ridCall;					/*EN< RID call attribute of the object*/
	hash_map< wstring, CVariable > m_mapProperty;	/*EN< Property map*/
};

/*EN Pointer to a built-in method*/
typedef void (*PMETHOD)(void);

/*EN Pointer to a built-in method
\param pVm A pointer to the VM.
\param iNumArguments A number of the argument that is passed to the callback
\param pArguments A vector iterator of the arguments
*/
typedef void (*PDEFAULTMETHOD)(CCilVm* pVm, int32_t iNumArguments, CVariable* pArguments );

/*EN Pointer to a built-in method with a semi automatically generated wrapper code
\param pVm A pointer to the VM.
\param iNumArguments A number of the argument that is passed to the callback
\param pArguments A vector iterator of the arguments
\param pMethod A pointer to the method
*/
typedef void (*PWRAPPEDMETHOD)(CCilVm* pVm, int32_t iNumArguments, CVariable* pArguments, PMETHOD pMethod );

/*EN Pointer to a Property Setter/Getter method
Accessor spec:
\param pVm A pointer to the VM. It would be NULL if the caller does not know it. A callee should check if the pointer is not NULL.
\param pObject A pointer to the object that includes a property. A property name is not given since the user can register callback for each property name.
\param bSet true for getter, false for setter.
\param pSrcVar A source variable for the setter. For getter, it should be NULL.
\return A pointer to the variable. Note that the pointer is not freed by caller. A setter should return NULL value.
*/
typedef CVariable* (*PACCESSORMETHOD)(CCilVm* const pVm, CVmObject* const pObject, const wstring* const pName, CVariable* const pSrcVar, const bool bSet );

//---------------------------------------------------------------------------
// Meta data managing class
//---------------------------------------------------------------------------

	/*EN Metadata information class
	*/
	class CMetaData
	{
	public:
		CMetaData();					/*EN< ctor*/
		~CMetaData();					/*EN< dtor*/
		CMetaData( CMetaData& right );	/*EN< Copy ctor*/
		CMetaData& operator=( CMetaData& right );
										/*EN< =Operator*/

		/*EN Initializes the metadata class*/
		void initialize( void );

		/*EN Teminates the metadata class*/
		void terminate( void );

		/*EN Retrieves a table of static fields
		\return Reference of the static fields table
		*/
		vector< OPERAND_TYPE_FLAG >& getStaticFieldTypeTable() { return m_StaticFieldTypeTable; }

		/*EN Retrieves a table of methods
		\return Reference of the methods table
		*/
		vector< METHOD_DEF >& getMethodTable() { return m_MethodTable; }

		/*EN Retrieves a table of builtin methods
		\return Reference of the builtin methods
		*/
		vector< METHOD_DEF >& getBuiltinMethodTable() { return m_BuiltinMethodTable; }

		/*EN Retrieves a table of parameter list
		\return Reference of the parameter list
		*/
		vector< vector< OPERAND_TYPE_FLAG > >& getParamListTable() { return m_ParamListTable; }

		/*EN Retrieves a table of parameter list of built in function.
		\return Reference of the parameter list of built in function
		*/
		vector< vector< OPERAND_TYPE_FLAG > >& getBuiltinParamListTable() { return m_BuiltinParamListTable; }

		/*EN Retrieves a table of an object list
		\return Reference of the object list
		*/
		vector< CObjectTypeInfo >& getObjectTable() { return m_ObjectTable; }

		/*EN Retrieves a table of a builtin object list
		\return Reference of the builtint object list
		*/
		vector< CObjectTypeInfo >& getBuiltinObjectTable() { return m_BuiltinObjectTable; }

		/*EN Retrieves a table of string constants
		\return Reference of the table of string constants
		*/
		vector< wstring >& getStringConstantPool() { return m_StringConstantPool; }

		/*EN Retrieves a table of builtin string constants
		\return Reference of the table of builtin string constants
		*/
		vector< wstring >& getBuiltinStringConstantPool() { return m_BuiltinStringConstantPool; }

		//---------------------------------------------------------------------------
		//Meta DATA helpers
		//---------------------------------------------------------------------------
		/*EN Retrieves a string constant that has given RID
		\param rid RID of the string constant
		\return String value that has given RID value
		*/
		wstring& getStringConstant( const RID rid );

		/*EN Retrieves a method informationthat has given RID
		\param rid RID of the method
		\return Method information that has given RID value
		*/
		METHOD_DEF& getMethod( const RID rid );

		/*EN Retrieves an object informationthat has given RID
		\param rid RID of the object
		\return Object information that has given RID value
		*/
		CObjectTypeInfo& getObject( const RID rid );

		/*EN Retrieves an object name RID has given Object RID
		\param ObjectRID RID of the object
		\return Object name RID that has given Object RID value
		*/
		RID getObjectNameRID( const RID ObjectRID );

		/*EN Queries an object RID that has specific qualified name
		\param strQualifiedName Qualified name of the object
		\return RID that has given Object name
		*/
		RID queryBuiltinObjectRid( const wstring& strQualifiedName ); 

		/*EN Queries an stgring RID that has specific qualified name
		\param strName Qualified name of the string
		\return RID of given string
		*/
		RID queryBuiltinStringRid( const wstring& strName ); 

		/*EN Queries an method RID that has specific qualified name
		\param strQualifiedName Qualified name of the method
		\return RID of given method
		*/
		RID queryBuiltinMethodRid( const wstring& strQualifiedName ); 

		/*EN Verifies object map information
		\return true if the object info that is attached is correct, false otherwise.
		*/
		bool verifyObjectMap();

		//---------------------------------------------------------------------------
		//Object map initialization and verification
		//---------------------------------------------------------------------------
		RID registerBuiltinObject( const wstring& strQualifiedName ); 
		RID registerBuiltinMethod( const wstring& strQualifiedName,
										const METHOD_TYPE methodType,
										const RID_BUILTIN_PARAM ridParam );

		//---------------------------------------------------------------------------
		//RID manipulation
		//---------------------------------------------------------------------------
		RID reserveBuiltinStringConstantRID( const RID ridBuiltinStringConstant, const wstring& s = wstring( L"" ) );
		RID generateUserStringConstantRID( const wstring& s = wstring( L"" ) );

		RID generateUserObjectRID();
		RID generateBuiltinObjectRID();
		RID generateBuiltinStringConstantRID();
		RID generateBuiltinMethodRID();

		RID generateStaticFieldRID( const OPERAND_TYPE type = OPERAND_UNDEFINED,
									const OPERAND_FLAG flag = OPERAND_FLAG_NONE,
									const RID ridConstraintType = 0 );
		RID generateMethodRID( const METHOD_TYPE type,
										 const RID ridScope,
										 const RID ridArg,
										 const RID ridLocalVariable );
		RID generateEhRID( const METHOD_TYPE type,
										 const RID ridScope,
										 const RID ridArg,
										 const RID ridLocalVariable );

		RID generateParamListRID();
		RID generatePropertyRID();
		int32_t	addParamListEntry( const RID ridParamList,
									const OPERAND_TYPE op,
									const OPERAND_FLAG flag = OPERAND_FLAG_NONE,
									const RID ridConstraintType = 0 );
		size_t getParamListSize( const RID ridParamList );

		RID reserveBuiltinParamListRID( const RID ridBuiltinObjectParamList );
		RID reserveBuiltinMethodRID(	const RID ridBuiltinMethod,
										const METHOD_TYPE type,
										PMETHOD const pFunction,
										const RID	ridScope,
										const RID	ridArg,
										const RID	ridLocalVariable );

		bool setBuiltinString(  const RID ridObjectName,
								const wstring& strString );
	private:
		//---------------------------------------------------------------------------
		//RID manipulation
		//---------------------------------------------------------------------------
		RID reserveBuiltinObjectRID( const RID ridBuiltinObject );

		//---------------------------------------------------------------------------
		//High level helpers
		//---------------------------------------------------------------------------
		bool setBuiltinObject(  const RID ridObject,
								const RID ridObjectName,
								const RID ridCtor = (RID)0,
								const RID ridCall = (RID)0	);
		bool setBuiltinMethod(  const RID ridMethod,
								const METHOD_TYPE methodType,
								const PMETHOD pMethod,
								const RID ridParam );


		//---------------------------------------------------------------------------
		//Meta DATA tables
		//---------------------------------------------------------------------------
		int32_t m_iNumCurrentStaticField;						//# of global Static field 
		vector< OPERAND_TYPE_FLAG > m_StaticFieldTypeTable;			//Static field type table

		int32_t m_iNumUserMethod;								//# of Methods 
		vector< METHOD_DEF > m_MethodTable;						//Method type data table
		int32_t m_iNumBuiltinMethod;							//# of Methods 
		vector< METHOD_DEF > m_BuiltinMethodTable;				//Method type data table

		int32_t m_iNumUserParamList;							//# of Param list
		vector< vector< OPERAND_TYPE_FLAG > > m_ParamListTable;		//Param list table
		int32_t m_iNumBuiltinParamList;							//# of Builtin Param list
		vector< vector< OPERAND_TYPE_FLAG > > m_BuiltinParamListTable;//Builtin Param list table

		int32_t m_iNumUserObject;								//# of Object info
		vector< CObjectTypeInfo > m_ObjectTable;				//Object info table
		int32_t m_iNumBuiltinObject;							//# of Builtin Object info
		vector< CObjectTypeInfo > m_BuiltinObjectTable;			//Builtin Object info table

		int32_t m_iNumUserString;								//# of string pool entries
		vector< wstring > m_StringConstantPool;					//String constant pool
		int32_t m_iNumBuitinString;								//# of builtin string pool entries
		vector< wstring > m_BuiltinStringConstantPool;			//Builtin string constant pool

		int32_t m_iNumProperty;									//Property RID

		hash_map< wstring, CVariable > m_ObjectMap;
		//Helper function
		CVariable* registerVariable( hash_map< wstring, CVariable >& map, const wstring& strName );
		CVariable* queryVariable( hash_map< wstring, CVariable >& map, const wstring& strName );
		
	};

	/***************************************************************************
	 *      Forward decl
	 ***************************************************************************/
	class CLexer;
	class CErrorPool;

	/***************************************************************************
	 *      Code gen base class
	 ***************************************************************************/
	/*EN
	 \brief Interface class of CRI Script Code Generator
	 \ingroup MDL_LIB_COMPILER
	 \par Compiler class should implement this interface
	*/
	class ICodeGen {
	public:
		ICodeGen() {};							/*EN< ctor*/
		virtual ~ICodeGen() {};					/*EN< dtor*/
		virtual bool init() = 0;				/*EN< Initializes a code generator instance*/
		virtual bool terminate() = 0;			/*EN< Terminates a code generator instance*/
		virtual bool verify() = 0;				/*EN< Performs a verification process*/
		virtual bool link() = 0;				/*EN< Performs a link process*/

		virtual CCgNodeData* makeNode(	CG_NODE_TYPE NodeType,
								CG_OPERATIONTYPE iOperation = CG_NONE,
								CCgNodeData* lhs = NULL,
								CCgNodeData* rhs = NULL,
								CCgNodeData* siblingNode = NULL,
								CCgNodeData* initialNode = NULL,
								CCgNodeData* statementNode = NULL,
								CCgNodeData* conditionNode = NULL,
								CCgNodeData* argumentlistNode = NULL
								) = 0;

		//Code gen interfaces whose are invoked from YACC generated code
		virtual CCgNodeData* variableDeclaration( CCgNodeData*, CCgNodeData* ) = 0;
		virtual CCgNodeData* typedVariableDeclaration( CCgNodeData*, CCgNodeData* ) = 0;
		virtual CCgNodeData* variableAssignment( CCgNodeData*, CCgNodeData* ) = 0;

		virtual	CCgNodeData* addOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* subtractOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* multiplyOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* divisionOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* reminderOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* ifStatement( CCgNodeData*, CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* conditionalExpression( CCgNodeData*, CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* forStatement( CCgNodeData*, CCgNodeData*, CCgNodeData*, CCgNodeData*, bool = false ) = 0;
		virtual	CCgNodeData* forInStatement( CCgNodeData* variable,
												 CCgNodeData* obj,
												 CCgNodeData* statement ) = 0;
		virtual	CCgNodeData* expressionStatement( CCgNodeData* ) = 0;
		virtual	CCgNodeData* withStatement( CCgNodeData*, CCgNodeData* ) = 0;
		virtual CCgNodeData* functionDeclaration( CCgNodeData* identifier,
													   CCgNodeData* ParameterList,
													   CCgNodeData* FunctionBody ) = 0;
		virtual CCgNodeData* typedFunctionDeclaration( CCgNodeData* identifier,
													   CCgNodeData* ParameterList,
													   CCgNodeData* FunctionBody,
													   CCgNodeData* ReturnValue ) = 0;
		
		virtual	CCgNodeData* bitwiseAndOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* bitwiseOrOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* logicalAndOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* logicalOrOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* bitwiseXorOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* shiftLeftOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* shiftRightOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual	CCgNodeData* shiftRightUnsignedOperator( CCgNodeData*, CCgNodeData* ) = 0;
		
		virtual	CCgNodeData* unaryPlusOperator( CCgNodeData* ) = 0;
		virtual	CCgNodeData* unaryMinusOperator( CCgNodeData* ) = 0;
		virtual	CCgNodeData* unaryIncOperator( CCgNodeData*, bool bDup ) = 0;
		virtual	CCgNodeData* unaryDecOperator( CCgNodeData*, bool bDup  ) = 0;
		virtual	CCgNodeData* unaryBitwiseNotOperator( CCgNodeData* ) = 0;
		virtual	CCgNodeData* unaryLogicalNotOperator( CCgNodeData* ) = 0;
		virtual	CCgNodeData* unaryVoidOperator( CCgNodeData* ) = 0;
		virtual	CCgNodeData* unaryDeleteOperator( CCgNodeData* ) = 0;
		virtual	CCgNodeData* unaryTypeofOperator( CCgNodeData* ) = 0;

		virtual	CCgNodeData* callExpression( CCgNodeData* lhs, CCgNodeData* rhs ) = 0;
		virtual	CCgNodeData* returnStatement( CCgNodeData* ) = 0;
		virtual CCgNodeData* propertyAccessExpressionById( CCgNodeData*, CCgNodeData* ) = 0;
		virtual CCgNodeData* propertyAccessExpressionByExp( CCgNodeData*, CCgNodeData* ) = 0;
		virtual CCgNodeData* propertyNameAndValue( CCgNodeData*, CCgNodeData* ) = 0;
		virtual CCgNodeData* newOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual CCgNodeData* instanceOfOperator( CCgNodeData*, CCgNodeData* ) = 0;
		virtual CCgNodeData* inOperator( CCgNodeData*, CCgNodeData*  ) = 0;

		virtual	CCgNodeData* exceptionHandlingClause(	CCgNodeData* bodyNode,
									CCgNodeData* catchNode,
									CCgNodeData* finallyNode ) = 0;
		virtual	CCgNodeData* catchBlock( CCgNodeData* identifier,
											CCgNodeData* Body ) = 0;
		virtual	CCgNodeData* finallyBlock( CCgNodeData* Body ) = 0;
		virtual	CCgNodeData* throwStatement( CCgNodeData* Expression ) = 0;
		virtual	CCgNodeData* labeledStatement( CCgNodeData* identifier, CCgNodeData* Statement ) = 0;
		virtual	CCgNodeData* breakStatement( CCgNodeData* Expression ) = 0;
		virtual	CCgNodeData* continueStatement( CCgNodeData* Expression ) = 0;
		virtual CCgNodeData* switchStatement( CCgNodeData* Expression,
											   CCgNodeData* CaseBlock ) = 0;
		virtual CCgNodeData* caseClause( CCgNodeData* Expression,
											CCgNodeData* Statement ) = 0;


		virtual void endOfFile( void ) = 0;

		virtual	CCgNodeData* cmpOperator( CCgNodeData*, CCgNodeData*, CG_OPERATIONTYPE iOpCode ) = 0;
		//Constant literals
		virtual CCgNodeData* literalConstant( const wchar_t* string,
									 const uint32_t iFileId,
									 const uint32_t iLineNumber,
									 const uint32_t iColumn ) = 0;
		virtual CCgNodeData* integerConstant( const int64_t value,
									 const uint32_t iFileId,
									 const uint32_t iLineNumber,
									 const uint32_t iColumn ) = 0;
		virtual	CCgNodeData* booleanConstant( const bool value,
									 const uint32_t iFileId,
									 const uint32_t iLineNumber,
									 const uint32_t iColumn ) = 0;
		virtual CCgNodeData* floatConstant( const double value,
									 const uint32_t iFileId,
									 const uint32_t iLineNumber,
									 const uint32_t iColumn ) = 0;
		virtual CCgNodeData* regexLiteral( const wchar_t* string,
									 const uint32_t iFileId,
									 const uint32_t iLineNumber,
									 const uint32_t iColumn ) = 0;
		//identifier
		virtual CCgNodeData* identifier( const wchar_t* string,
									 const uint32_t iFileId,
									 const uint32_t iLineNumber,
									 const uint32_t iColumn ) = 0;

		//Node tree
		virtual CCgNodeData* makeFormulaNode( CG_OPERATIONTYPE iOperation, CCgNodeData* lhs, CCgNodeData* rhs, bool bBlockTraverseLeft, bool bBlockTraverseRight ) = 0;
		virtual CCgNodeData* duplicateNode( CCgNodeData* srcNode ) = 0;
		virtual CCgNodeData* parseStatementNodeTree( CCgNodeData* Formula ) = 0;
		virtual CCgNodeData* parseExpressionNodeTree( CCgNodeData* Formula, bool bAdjustStackEntries ) = 0;
		virtual CCgNodeData* makeIfNode(CCgNodeData* condition,
											CCgNodeData* statementtrue,
											CCgNodeData* statementfalse ) = 0;
		virtual CCgNodeData* parseIfNode( CCgNodeData* ifnode ) = 0;

		virtual CCgNodeData* makeFunctionDeclarationNode(CCgNodeData* identifier,
											CCgNodeData* parameterlist,
											CCgNodeData* functionbody ) = 0;
		virtual CCgNodeData* parseFunctionDeclarationNode( CCgNodeData* ifnode, RID* ridMethod = NULL ) = 0;
		virtual CCgNodeData* parseFunctionExpressionNode( CCgNodeData* nodetree ) = 0;
		virtual CCgNodeData* parseExceptionHandlingNode( CCgNodeData* nodetree ) = 0;
		virtual CCgNodeData* objectLiteral( CCgNodeData* Formula ) = 0;
		virtual CCgNodeData* arrayLiteral( CCgNodeData* Formula ) = 0;

		virtual vector<uint8_t> & getILPool() = 0;
		virtual	CMetaData& getMetaData() = 0;

		virtual uint32_t tagTheLine() = 0;
		virtual	void setInterfaces( CParser* pParser, CLexer* pLex, CErrorPool * pError ) = 0;

		//---------------------------------------------------------
		//Multipass compilation control
		//---------------------------------------------------------
		virtual bool isMultiPasses() = 0;
		virtual void setMultiPasses( bool b) = 0;
		virtual void parseStatements() = 0;
		virtual void updateStatementTree( CCgNodeData* ) = 0;

		//---------------------------------------------------------
		//Debug info retrieval
		//---------------------------------------------------------
		virtual hash_map<wstring, CG_SYMBOL_INFORMATION>& getSymbolInformation() = 0;
		virtual	void generateSymbolInformation( const RID rid,
											const wstring& strScopedVariableName,
											const CG_SYMBOL_FLAG Flag = CG_SYMBOL_FLAG_GLOBAL,
											const int32_t iIndex = 0 ) = 0;

		//---------------------------------------------------------
		//Error report
		//---------------------------------------------------------
		virtual void reportError( const int32_t iErrorCode ) = 0;
		virtual void reportError( const int32_t iErrorCode, const wstring& strErrorInfo ) = 0;
		virtual	void reportError( const int32_t iErrorCode,
							 const uint32_t iFileId,
							 const uint32_t iLineNumber,
							 const uint32_t iColumn ) = 0;
		virtual void reportError( const int32_t iErrorCode, const wstring& strErrorInfo,
							 const uint32_t iFileId,
							 const uint32_t iLineNumber,
							 const uint32_t iColumn ) = 0;

		//---------------------------------------------------------
		//Initialize built in objects
		//---------------------------------------------------------
		virtual bool initializeBuiltInObjects() = 0;
		virtual RID registerBuiltinObject( const wstring& strQualifiedName ) = 0;
		virtual RID registerBuiltinMethod( const wstring& strQualifiedName,
										const METHOD_TYPE methodType,
										const RID_BUILTIN_PARAM ridParam ) = 0;

		//---------------------------------------------------------
		//Manage target settings
		//---------------------------------------------------------
		virtual void setTargetEndianness( TARGET_ENDIANNESS ) = 0;
		virtual TARGET_ENDIANNESS getTargetEndianness() = 0;
		virtual TARGET_ENDIANNESS getCurrentEndianness() = 0;

		//---------------------------------------------------------
		//Extension enabled
		//---------------------------------------------------------
		virtual bool checkExtension( EXTENSION_FLAGS ) = 0;
	};


}//namespace cri

