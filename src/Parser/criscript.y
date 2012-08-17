	%{
//    Copyright (c) 2007 CRI Middleware inc.  All rights reserved.
#include "stdafx.h"
#include "criScript.h"
#include "parser.h"
#include "cilCodeGen.h"
#include "cilVm.h"

namespace cri {
#define YYMAXDEPTH 65535
extern "C" {
int32_t yylex( void );
void yyerror( char *pMessage);
}

using namespace cri;

#ifdef DEBUG_PARSING
#define dbgprint(x)     printf x
#define YYDEBUG 1
#else
#define dbgprint(x)     
#endif
 
%}

%union {
	struct {
		union {
			int64_t iValue;
			float fValue;
			double dValue;
			bool bValue;
			uint8_t op;
			const wchar_t * pString;
			CCgNodeData* pObjectData;
		};
		uint32_t iFileId;	//FileId, not used now
		uint32_t iLineNumber;	//Linenumber of the token
		uint32_t iColumn;		//Column of the token
	};
}

//---------------------------------------------------------
// keywords
//---------------------------------------------------------
%token <op> TOKEN_ERROR
%token <op> TOKEN_LINETERM
%token <op> TOKEN_SEMICOLONAUTO
%token <op> TOKEN_BREAK TOKEN_ELSE TOKEN_NEW TOKEN_VAR
%token <op> TOKEN_CASE TOKEN_FINALLY TOKEN_RETURN TOKEN_VOID
%token <op> TOKEN_CATCH TOKEN_FOR TOKEN_SWITCH TOKEN_WHILE
%token <op> TOKEN_CONTINUE TOKEN_FUNCTION TOKEN_THIS TOKEN_WITH
%token <op> TOKEN_DEFAULT TOKEN_IF TOKEN_THROW 
%token <op> TOKEN_DELETE TOKEN_IN TOKEN_TRY
%token <op> TOKEN_DO TOKEN_TYPEOF

//---------------------------------------------------------
//Extension for JScript compatibility
//---------------------------------------------------------
%token <op> TOKEN_ATSET
%token <op> TOKEN_ATIF
%token <op> TOKEN_ATELIF
%token <op> TOKEN_ATELSE
%token <op> TOKEN_ATEND
%token <op> TOKEN_ATCC_ON
%token <op> TOKEN_IMPORT

//---------------------------------------------------------
// reserved words
//---------------------------------------------------------
%token <op> TOKEN_ABSTRACT TOKEN_ENUM TOKEN_INT TOKEN_SHORT
%token <op> TOKEN_BOOLEAN TOKEN_EXPORT TOKEN_INTERFACE TOKEN_STATIC
%token <op> TOKEN_BYTE TOKEN_EXTENDS TOKEN_LONG TOKEN_SUPER
%token <op> TOKEN_CHAR TOKEN_FINAL TOKEN_NATIVE TOKEN_SYNCHRONIZED
%token <op> TOKEN_CLASS TOKEN_FLOAT TOKEN_PACKAGE TOKEN_THROWS
%token <op> TOKEN_CONST TOKEN_GOTO TOKEN_PRIVATE TOKEN_TRANSIENT
%token <op> TOKEN_DEBUGGER TOKEN_IMPLEMENTS TOKEN_PROTECTED TOKEN_VOLATILE
%token <op> TOKEN_DOUBLE TOKEN_PUBLIC
//
%token <op>     LITERAL_NULL LITERAL_UNDEFINED
%token <pString> LITERAL_REGULAR_EXPRESSION
%token <bValue> LITERAL_BOOLEAN
%token <pString> LITERAL_STRING
%token <iValue> LITERAL_INTEGER
%token <dValue> LITERAL_DECIMAL

%token <pString> IDENTIFIER_NAME
%token TOKEN_EOF

%type <pObjectData> error
%type <pObjectData> SemiColon

%type <pObjectData> LiteralString
%type <pObjectData> LiteralInteger
%type <pObjectData> LiteralFloat
%type <pObjectData> LiteralBoolean
%type <pObjectData> LiteralRegex
%type <pObjectData> identifier
%type <pObjectData> IdentifierOpt
%type <pObjectData> objectLiteral
%type <pObjectData> arrayLiteral

%type <pObjectData> SourceElement
%type <pObjectData> SourceElements
%type <pObjectData> FunctionSourceElements
%type <pObjectData> FunctionSourceElement
%type <pObjectData> Block
%type <pObjectData> StatementList
%type <pObjectData> StatementListOpt
%type <pObjectData> Statement
%type <pObjectData> ifStatement
%type <pObjectData> ElseStatementOpt
%type <pObjectData> EmptyStatement
%type <pObjectData> LabelledStatement
%type <pObjectData> expressionStatement
%type <pObjectData> IterationStatement
%type <pObjectData> continueStatement
%type <pObjectData> breakStatement
%type <pObjectData> returnStatement
%type <pObjectData> withStatement
%type <pObjectData> switchStatement
%type <pObjectData> throwStatement
%type <pObjectData> TryStatement

//---------------------------------------------------------
//ESMA4script compatibility extension
//---------------------------------------------------------
%token <op> TOKEN_RESTARG_PREFIX			//"..."
%type <pObjectData> ArrayTypedIdentifier	//identifier : arraytype
%type <pObjectData> TypedIdentifier			//identifier : type

//---------------------------------------------------------
//Jscript compatibility extension
//---------------------------------------------------------
%type <pObjectData> Extension_AtCcOnStatement
%type <pObjectData> Extension_ImportStatement
%type <pObjectData> Extension_ImportComponent
%type <pObjectData> Extension_AtIfStatement
%type <pObjectData> Extension_AtElseStatementOpt
%type <pObjectData> Extension_AtElifStatement
%type <pObjectData> Extension_AtExpression
%type <pObjectData> Extension_AtSetStatement
%type <pObjectData> Extension_AtLiteral
%type <pObjectData> Extension_AtIdentifier
%type <pObjectData> Extension_AtPrimaryExpression
%type <pObjectData> Extension_AtunaryExpression
%type <pObjectData> Extension_AtAdditiveExpression
%type <pObjectData> Extension_AtMultiplicativeExpression
%type <pObjectData> Extension_AtshiftExpression
%type <pObjectData> Extension_AtRelationalExpression
%type <pObjectData> Extension_AtEqualityExpression
%type <pObjectData> Extension_AtbitwiseANDExpression
%type <pObjectData> Extension_AtbitwiseORExpression
%type <pObjectData> Extension_AtbitwiseXORExpression
%type <pObjectData> Extension_AtlogicalANDExpression
%type <pObjectData> Extension_AtlogicalORExpression
%token <pString> ATIDENTIFIER_NAME


%type <pObjectData> PrimaryExpression
%type <pObjectData> Expression
%type <pObjectData> VariableStatement
%type <pObjectData> VariableDeclarationList
%type <pObjectData> VariableDeclarationListNoIn
%type <pObjectData> variableDeclaration
%type <pObjectData> VariableDeclarationNoIn
%type <pObjectData> InitialiserOpt
%type <pObjectData> InitialiserNoInOpt
%type <pObjectData> Initialiser
%type <pObjectData> InitialiserNoIn
%type <pObjectData> AssignmentExpression 
%type <pObjectData> AssignmentExpressionNoIn
%type <pObjectData> Literal
%type <pObjectData> LeftHandSideExpression
%type <pObjectData> LeftHandSideExpressionForIn
%type <pObjectData> AdditiveExpression
%type <pObjectData> MultiplicativeExpression
%type <pObjectData> unaryExpression
%type <pObjectData> RelationalExpression
%type <pObjectData> EqualityExpression
%type <pObjectData> RelationalExpressionNoIn
%type <pObjectData> EqualityExpressionNoIn
%type <pObjectData> shiftExpression
%type <pObjectData> conditionalExpression
%type <pObjectData> ConditionalExpressionNoIn
%type <pObjectData> logicalORExpression
%type <pObjectData> logicalORExpressionNoIn
%type <pObjectData> bitwiseANDExpression
%type <pObjectData> bitwiseANDExpressionNoIn
%type <pObjectData> bitwiseORExpression
%type <pObjectData> bitwiseORExpressionNoIn
%type <pObjectData> bitwiseXORExpression
%type <pObjectData> bitwiseXORExpressionNoIn
%type <pObjectData> ExpressionNoInOpt
%type <pObjectData> ExpressionNoIn
%type <pObjectData> ExpressionOpt
%type <pObjectData> functionDeclaration
%type <pObjectData> FunctionExpression
%type <pObjectData> FunctionBody
%type <pObjectData> FormalParameterList
%type <pObjectData> Identifiers
%type <pObjectData> Arguments
%type <pObjectData> ArgumentList
%type <pObjectData> callExpression
%type <pObjectData> callExpressionForIn
%type <pObjectData> MemberExpression
%type <pObjectData> MemberExpressionForIn
%type <pObjectData> NewExpression
%type <pObjectData> PostfixExpression
%type <pObjectData> logicalANDExpression
%type <pObjectData> logicalANDExpressionNoIn
%type <pObjectData> propertyNameAndValueList
%type <pObjectData> propertyNameAndValue
%type <pObjectData> propertyNameAndValueOpt
%type <pObjectData> PropertyName
%type <pObjectData> Elision
%type <pObjectData> ElementList
%type <pObjectData> Catch
%type <pObjectData> Finally
%type <pObjectData> CaseBlock
%type <pObjectData> caseClause
%type <pObjectData> CaseClauses
%type <pObjectData> DefaultClause

%left	<op>	','				// comma
%right	<op>	'='				// assignment	=, OP=
				TOKEN_MULASSIGNOP	// *=
				TOKEN_DIVASSIGNOP	// /=
				TOKEN_ADDASSIGNOP	// +=
				TOKEN_SUBASSIGNOP	// -=
				TOKEN_ANDASSIGNOP	// &=
				TOKEN_ORASSIGNOP	// |=
				TOKEN_XORASSIGNOP	// ^=
				TOKEN_MODASSIGNOP	// %=
				TOKEN_SHRASSIGNOP	// >>=
				TOKEN_SHRUASSIGNOP	// >>>=
				TOKEN_SHLASSIGNOP	// <<=
%right	<op>	'?' ':'			// conditional	?:
%left	<op>	TOKEN_LOGOR		// logical or	||
%left	<op>	TOKEN_LOGAND	// logical and	&&
%left	<op>	'|'				// bitwise or
%left	<op>	'^'				// bitwise xor
%left	<op>	'&'				// bitwise and
%left	<op>	TOKEN_STRICTEQUALOP
				TOKEN_STRICTNOTEQUALOP
				TOKEN_EQUALOP
				TOKEN_NOTEQUALOP			// equality		==, !=, ===, !==
%left	<op>	TOKEN_GREATER
				TOKEN_GREATEREQUAL
				TOKEN_LESS
				TOKEN_LESSEQUAL
				TOKEN_INSTANCEOF			// comparison <, <=, >, >=,instanceof
%left	<op>	TOKEN_SHIFTLEFT
				TOKEN_SHIFTRIGHT
				TOKEN_SHIFTRIGHTUNSIGNED	// shift	<<, >>, >>>
%left	<op>	'+' '-'						// add/sub +, -
%left	<op>	'*' '/' '%'					// mul/div *, /, %
%right	<op>	TOKEN_UNINC TOKEN_UNDEC		// unary	++,--,-,~,!,delete,new,typeof,void
%left	<op>	'.'							// ., [], ()

//%start decls
/*--------------------------------- declarations start here -----------------------------------*/
%%      
Program : SourceElements TOKEN_EOF
	{
	 	if( !CParser::getparser()->getCodeGen()->isMultiPasses() )
	 	{
			CParser::getparser()->getCodeGen()->endOfFile();
		}
		YYACCEPT;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
//At here, it don't have to parse anything.
//Things are already done by this stage (hopefully).
SourceElements : SourceElements SourceElement
	{
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | SourceElement
	{
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
SourceElement : functionDeclaration
	{
	 	if( !CParser::getparser()->getCodeGen()->isMultiPasses() )
	 	{
			CParser::getparser()->getCodeGen()->parseFunctionDeclarationNode( $1 );
		}
		else
		{
			//Keep statements to the list.
			CParser::getparser()->getCodeGen()->updateStatementTree( $1 );
		}
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Statement 
 	{
	 	if( !CParser::getparser()->getCodeGen()->isMultiPasses() )
	 	{
			CParser::getparser()->getCodeGen()->parseStatementNodeTree( $1 );
		}
		else
		{
			//Keep statements to the list.
			CParser::getparser()->getCodeGen()->updateStatementTree( $1 );
		}
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
PrimaryExpression : TOKEN_THIS
	{
		$$ = CParser::getparser()->getCodeGen()->identifier( STRING_INTERNAL_THIS, $<iFileId>1, $<iLineNumber>1, $<iColumn>1 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	} 
 | objectLiteral
	{
		$$ = CParser::getparser()->getCodeGen()->objectLiteral( $1 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '(' Expression ')'
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | identifier
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | arrayLiteral
	{
		$$ = CParser::getparser()->getCodeGen()->arrayLiteral( $1 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Literal
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Literal : LiteralFloat
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LiteralInteger
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LiteralString
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LiteralBoolean
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LITERAL_NULL
	{
		$$ = CParser::getparser()->getCodeGen()->identifier( STRING_INTERNAL_NULL, $<iFileId>1, $<iLineNumber>1, $<iColumn>1  );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	} 
 | LITERAL_UNDEFINED
	{
		$$ = CParser::getparser()->getCodeGen()->identifier( STRING_INTERNAL_UNDEFINED, $<iFileId>1, $<iLineNumber>1, $<iColumn>1  );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	} 
 | LiteralRegex
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
IdentifierOpt : /*Empty*/
 	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | identifier
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
arrayLiteral : '[' ']'
 	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '[' Elision ']'
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->identifier( STRING_INTERNAL_UNDEFINED, $<iFileId>1, $<iLineNumber>1, $<iColumn>1 );
		node = CParser::getparser()->getCodeGen()->propertyNameAndValue( NULL, node );
		// add 1 more 'undefined' node
		node->SetSibling( $2 );
		$$ = node;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '[' Elision ElementList Elision ']'
	{
		CCgNodeData* nodetop = CParser::getparser()->getCodeGen()->identifier( STRING_INTERNAL_UNDEFINED, $<iFileId>1, $<iLineNumber>1, $<iColumn>1 );
		nodetop = CParser::getparser()->getCodeGen()->propertyNameAndValue( NULL, nodetop );
		CCgNodeData* nodetail = CParser::getparser()->getCodeGen()->identifier( STRING_INTERNAL_UNDEFINED, $<iFileId>1, $<iLineNumber>1, $<iColumn>1  );
		nodetail = CParser::getparser()->getCodeGen()->propertyNameAndValue( NULL, nodetail );
		// add 1 more 'undefined' node

		if( $2 != NULL )
		{
			nodetop->SetSibling( $2 );
			$2->SetSibling( $3 );
			$3->SetSibling( nodetail );
			nodetail->SetSibling( $4 );
			$$ = nodetail;		
		}
		else
		{
			nodetop->SetSibling( $3 );
			$3->SetSibling( nodetail );
			nodetail->SetSibling( $4 );
			$$ = nodetop;		
		}
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '[' ElementList ']'
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '[' ElementList Elision ']'
	{
		CCgNodeData* nodetail = CParser::getparser()->getCodeGen()->identifier( STRING_INTERNAL_UNDEFINED, $<iFileId>1, $<iLineNumber>1, $<iColumn>1 );
		nodetail = CParser::getparser()->getCodeGen()->propertyNameAndValue( NULL, nodetail );
		$2->SetSibling( nodetail );
		nodetail->SetSibling( $3 );
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
ElementList : AssignmentExpression
	{
		$$ = CParser::getparser()->getCodeGen()->propertyNameAndValue( NULL, $1 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | 
	ElementList Elision AssignmentExpression
	{
		if( $2 != NULL )
		{
			$1->SetSibling( $2 );
			CCgNodeData* node = CParser::getparser()->getCodeGen()->propertyNameAndValue( NULL, $3 );
			$2->SetSibling( node );
			$$ = $1;
		}
		else
		{
			CCgNodeData* node = CParser::getparser()->getCodeGen()->propertyNameAndValue( NULL, $3 );
			$1->SetSibling( node );
			$$ = $1;
		}
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Elision : ','
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Elision ','
 	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->identifier( STRING_INTERNAL_UNDEFINED, $<iFileId>1, $<iLineNumber>1, $<iColumn>1 );
		node = CParser::getparser()->getCodeGen()->propertyNameAndValue( NULL, node );

 		if( $1 != NULL )
 		{
			$1->SetSibling( node );	
 		}
 		else
			$$ = node;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
objectLiteral : '{' propertyNameAndValueList '}'
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
// | '{' '}'
//	{ $$ = NULL }
 ;

propertyNameAndValueList : propertyNameAndValue propertyNameAndValueOpt
 	{
		$1->SetSibling( $2 );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | /*Empty*/
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
propertyNameAndValueOpt : /*Empty*/
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | ','
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | ',' propertyNameAndValue
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | propertyNameAndValueOpt ','
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | propertyNameAndValueOpt ',' propertyNameAndValue
 	{
 		if( $1 != NULL )
 		{ 
			$1->SetSibling( $3 );
			$$ = $1;
		}
		else
		{
			$$ = $3;
		}
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
propertyNameAndValue : PropertyName ':' AssignmentExpression 
 	{
		$$= CParser::getparser()->getCodeGen()->propertyNameAndValue( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
PropertyName : identifier 
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LiteralString
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LiteralInteger
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
MemberExpression : PrimaryExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | FunctionExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | MemberExpression '[' Expression ']'
	{
		$$ = CParser::getparser()->getCodeGen()->propertyAccessExpressionByExp( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | MemberExpression '.' identifier
	{
		$$ = CParser::getparser()->getCodeGen()->propertyAccessExpressionById( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_NEW MemberExpression Arguments
	{
		$$ = CParser::getparser()->getCodeGen()->newOperator( $2, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
NewExpression : MemberExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_NEW NewExpression
	{
		$$ = CParser::getparser()->getCodeGen()->newOperator( $2, NULL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
MemberExpressionForIn : MemberExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
callExpression : MemberExpression Arguments
	{
		$$ = CParser::getparser()->getCodeGen()->callExpression( $1, $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | callExpression Arguments
	{
		$$ = CParser::getparser()->getCodeGen()->callExpression( $1, $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | callExpression '[' Expression ']'
	{
		$$ = CParser::getparser()->getCodeGen()->propertyAccessExpressionByExp( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | callExpression '.' identifier
	{
		$$ = CParser::getparser()->getCodeGen()->propertyAccessExpressionById( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
callExpressionForIn : MemberExpressionForIn Arguments
	{
		$$ = CParser::getparser()->getCodeGen()->callExpression( $1, $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | callExpression Arguments
	{
		$$ = CParser::getparser()->getCodeGen()->callExpression( $1, $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | callExpression '[' Expression ']'
	{
		$$ = CParser::getparser()->getCodeGen()->propertyAccessExpressionByExp( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | callExpression '.' identifier
	{
		$$ = CParser::getparser()->getCodeGen()->propertyAccessExpressionById( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Arguments : '(' ')' 
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '(' ArgumentList ')' 
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
ArgumentList : AssignmentExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | ArgumentList ',' AssignmentExpression
	{
		//Note that flipping the order of the arg
		$3->SetSibling( $1 );
		$$ = $3;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
LeftHandSideExpression : callExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | NewExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
LeftHandSideExpressionForIn : callExpressionForIn 
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | MemberExpressionForIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
PostfixExpression : LeftHandSideExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_UNINC
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->unaryIncOperator( node, true );
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_UNDEC
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->unaryDecOperator( node, true );
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
unaryExpression : PostfixExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_DELETE unaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->unaryDeleteOperator( $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_TYPEOF unaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->unaryTypeofOperator( $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_VOID unaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->unaryVoidOperator( $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_UNINC unaryExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $2 );
		node = CParser::getparser()->getCodeGen()->unaryIncOperator( node, false );
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $2, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_UNDEC unaryExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $2 );
		node = CParser::getparser()->getCodeGen()->unaryDecOperator( node, false );
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $2, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '+' unaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->unaryPlusOperator( $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '-' unaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->unaryMinusOperator( $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '~' unaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->unaryBitwiseNotOperator( $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '!' unaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->unaryLogicalNotOperator( $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
MultiplicativeExpression : unaryExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | MultiplicativeExpression '*' unaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->multiplyOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | MultiplicativeExpression '/' unaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->divisionOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | MultiplicativeExpression '%' unaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->reminderOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ; 
AdditiveExpression : MultiplicativeExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | AdditiveExpression '+' MultiplicativeExpression
	{
		$$ = CParser::getparser()->getCodeGen()->addOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | AdditiveExpression '-' MultiplicativeExpression
	{
		$$ = CParser::getparser()->getCodeGen()->subtractOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
shiftExpression : AdditiveExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | shiftExpression TOKEN_SHIFTLEFT AdditiveExpression
	{
		$$ = CParser::getparser()->getCodeGen()->shiftLeftOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | shiftExpression TOKEN_SHIFTRIGHT AdditiveExpression
	{
		$$ = CParser::getparser()->getCodeGen()->shiftRightOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | shiftExpression TOKEN_SHIFTRIGHTUNSIGNED AdditiveExpression
	{
		$$ = CParser::getparser()->getCodeGen()->shiftRightUnsignedOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ; 
RelationalExpression : shiftExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  RelationalExpression TOKEN_GREATER shiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_GREATER );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  RelationalExpression TOKEN_GREATEREQUAL shiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_GREATEREQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  RelationalExpression TOKEN_LESS shiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_LESS );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  RelationalExpression TOKEN_LESSEQUAL shiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_LESSEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  RelationalExpression TOKEN_INSTANCEOF shiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->instanceOfOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  RelationalExpression TOKEN_IN shiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->inOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
RelationalExpressionNoIn : shiftExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | RelationalExpressionNoIn TOKEN_GREATER shiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_GREATER );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | RelationalExpressionNoIn TOKEN_GREATEREQUAL shiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_GREATEREQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | RelationalExpressionNoIn TOKEN_LESS shiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_LESS );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | RelationalExpressionNoIn TOKEN_LESSEQUAL shiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_LESSEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | RelationalExpressionNoIn TOKEN_INSTANCEOF shiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->instanceOfOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
  ;
EqualityExpression : RelationalExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | EqualityExpression TOKEN_EQUALOP RelationalExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_EQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | EqualityExpression TOKEN_NOTEQUALOP RelationalExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_NOTEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | EqualityExpression TOKEN_STRICTEQUALOP RelationalExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_STRICTEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | EqualityExpression TOKEN_STRICTNOTEQUALOP RelationalExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_STRICTNOTEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
EqualityExpressionNoIn : RelationalExpressionNoIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | EqualityExpressionNoIn TOKEN_EQUALOP RelationalExpressionNoIn
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_EQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | EqualityExpressionNoIn TOKEN_NOTEQUALOP RelationalExpressionNoIn
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_NOTEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | EqualityExpressionNoIn TOKEN_STRICTEQUALOP RelationalExpressionNoIn
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_STRICTEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | EqualityExpressionNoIn TOKEN_STRICTNOTEQUALOP RelationalExpressionNoIn
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_STRICTNOTEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
bitwiseANDExpression : EqualityExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | bitwiseANDExpression '&' EqualityExpression
	{
		$$ = CParser::getparser()->getCodeGen()->bitwiseAndOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
bitwiseANDExpressionNoIn : EqualityExpressionNoIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | bitwiseANDExpressionNoIn '&' EqualityExpressionNoIn
	{
		$$ = CParser::getparser()->getCodeGen()->bitwiseAndOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
bitwiseXORExpression : bitwiseANDExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  bitwiseXORExpression '^' bitwiseANDExpression
	{
		$$ = CParser::getparser()->getCodeGen()->bitwiseXorOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
bitwiseXORExpressionNoIn : bitwiseANDExpressionNoIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  bitwiseXORExpressionNoIn '^' bitwiseANDExpressionNoIn
	{
		$$ = CParser::getparser()->getCodeGen()->bitwiseXorOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
bitwiseORExpression : bitwiseXORExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | bitwiseORExpression '|' bitwiseXORExpression
	{
		$$ = CParser::getparser()->getCodeGen()->bitwiseOrOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
bitwiseORExpressionNoIn : bitwiseXORExpressionNoIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | bitwiseORExpressionNoIn '|' bitwiseXORExpressionNoIn
	{
		$$ = CParser::getparser()->getCodeGen()->bitwiseOrOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
logicalANDExpression : bitwiseORExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | logicalANDExpression TOKEN_LOGAND bitwiseORExpression
	{
		$$ = CParser::getparser()->getCodeGen()->logicalAndOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
logicalANDExpressionNoIn : bitwiseORExpressionNoIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | logicalANDExpressionNoIn TOKEN_LOGAND bitwiseORExpressionNoIn
	{
		$$ = CParser::getparser()->getCodeGen()->logicalAndOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
logicalORExpression : logicalANDExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | logicalORExpression TOKEN_LOGOR logicalANDExpression
	{
		$$ = CParser::getparser()->getCodeGen()->logicalOrOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
logicalORExpressionNoIn : logicalANDExpressionNoIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | logicalORExpressionNoIn TOKEN_LOGOR logicalANDExpressionNoIn
	{
		$$ = CParser::getparser()->getCodeGen()->logicalOrOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
conditionalExpression : logicalORExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | logicalORExpression '?' AssignmentExpression ':' AssignmentExpression
	{
		$$ = CParser::getparser()->getCodeGen()->conditionalExpression( $1, $3, $5 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
ConditionalExpressionNoIn : logicalORExpressionNoIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | logicalORExpressionNoIn '?' AssignmentExpressionNoIn ':' AssignmentExpressionNoIn
	{
		$$ = CParser::getparser()->getCodeGen()->conditionalExpression( $1, $3, $5 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
AssignmentExpression : LeftHandSideExpression '=' AssignmentExpression
	{
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_MULASSIGNOP AssignmentExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->multiplyOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_DIVASSIGNOP AssignmentExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->divisionOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_ADDASSIGNOP AssignmentExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->addOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_SUBASSIGNOP AssignmentExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->subtractOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_ORASSIGNOP AssignmentExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->bitwiseOrOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_XORASSIGNOP AssignmentExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->bitwiseXorOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_ANDASSIGNOP AssignmentExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->bitwiseAndOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_SHRASSIGNOP AssignmentExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->shiftRightOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_SHRUASSIGNOP AssignmentExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->shiftRightUnsignedOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_SHLASSIGNOP AssignmentExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->shiftLeftOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_MODASSIGNOP AssignmentExpression
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->reminderOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | conditionalExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
AssignmentExpressionNoIn : LeftHandSideExpression '=' AssignmentExpressionNoIn
	{
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_MULASSIGNOP AssignmentExpressionNoIn
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->multiplyOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_DIVASSIGNOP AssignmentExpressionNoIn
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->divisionOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_ADDASSIGNOP AssignmentExpressionNoIn
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->addOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_SUBASSIGNOP AssignmentExpressionNoIn
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->subtractOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_ORASSIGNOP AssignmentExpressionNoIn
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->bitwiseOrOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_XORASSIGNOP AssignmentExpressionNoIn
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->bitwiseXorOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_ANDASSIGNOP AssignmentExpressionNoIn
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->bitwiseAndOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_SHRASSIGNOP AssignmentExpressionNoIn
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->shiftRightOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_SHRUASSIGNOP AssignmentExpressionNoIn
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->shiftRightUnsignedOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_SHLASSIGNOP AssignmentExpressionNoIn
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->shiftLeftOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LeftHandSideExpression TOKEN_MODASSIGNOP AssignmentExpressionNoIn
	{
		CCgNodeData* node = CParser::getparser()->getCodeGen()->duplicateNode( $1 );
		node = CParser::getparser()->getCodeGen()->reminderOperator( node, $3 ); 
		$$= CParser::getparser()->getCodeGen()->variableAssignment( $1, node );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | ConditionalExpressionNoIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
ExpressionOpt : /**/
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Expression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Expression : AssignmentExpression
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Expression ',' AssignmentExpression
 	{
		$1->SetSibling( $3 );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
ExpressionNoInOpt : /**/
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | ExpressionNoIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
ExpressionNoIn : AssignmentExpressionNoIn
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | ExpressionNoIn ',' AssignmentExpressionNoIn
 	{
		$1->SetSibling( $3 );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Statement :  Block
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | VariableStatement
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | EmptyStatement
 	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LabelledStatement
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | expressionStatement
	{
		//Clear eval stack
		$$ = CParser::getparser()->getCodeGen()->expressionStatement( $1 ); 
	}
 | ifStatement
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | IterationStatement
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | continueStatement
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | breakStatement
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | returnStatement
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | withStatement
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | switchStatement
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | throwStatement
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TryStatement
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtIfStatement
 	{
 		/*JScript compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtSetStatement	
 	{
 		/*JScript compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtCcOnStatement	
 	{
 		$$ = NULL;
 		/*JScript compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
 		/* Do nothing*/
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_ImportStatement
 	{
 		$$ = NULL;
 		/*JScript compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
 		/* Do nothing*/
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}	
 | error SemiColon
	{
		CParser::getparser()->getCodeGen()->reportError( ERROR_SYNTAXERROR, $<iFileId>1, $<iLineNumber>1, $<iColumn>1 );
		$$ = NULL;
		yyerrok;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}		//Error handling
 | error TOKEN_EOF
	{
		$$ = NULL;
		//CParser::getparser()->getCodeGen()->endOfFile();
		YYACCEPT;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}		//Error handling
 ;
Block : '{' StatementList '}'
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '{' '}'
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
StatementList : Statement 
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | StatementList Statement
	{
		if( $1 != NULL )
		{
			$1->SetSibling( $2 );
			$$ = $1;
		}
		else
			$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
VariableStatement : TOKEN_VAR VariableDeclarationList { AUTOSEMICOLON } SemiColon
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
VariableDeclarationList : variableDeclaration
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | VariableDeclarationList ',' variableDeclaration
	{
		$1->SetSibling( $3 );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
VariableDeclarationListNoIn : VariableDeclarationNoIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | VariableDeclarationListNoIn ',' VariableDeclarationNoIn
	{
		$1->SetSibling( $3 );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
variableDeclaration : identifier InitialiserOpt
	{
		$$ = CParser::getparser()->getCodeGen()->variableDeclaration( $1, $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TypedIdentifier InitialiserOpt
	{
 		/*ECMA4 compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_ES4DRAFT );
		$$ = CParser::getparser()->getCodeGen()->typedVariableDeclaration( $1, $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
InitialiserOpt : /**/
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Initialiser
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
ArrayTypedIdentifier : identifier ':' '[' identifier ']'
	{
 		/*ECMA4 compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_ES4DRAFT );
 		$$ = CParser::getparser()->getCodeGen()->makeNode( CG_NODE_TYPEDIDENTIFIER,
 														(CG_OPERATIONTYPE)0,
 														$1, $4 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
TypedIdentifier : identifier ':' identifier
	{
 		/*ECMA4 compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_ES4DRAFT );
 		$$ = CParser::getparser()->getCodeGen()->makeNode( CG_NODE_TYPEDIDENTIFIER,
 														(CG_OPERATIONTYPE)0,
 														$1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
VariableDeclarationNoIn : identifier InitialiserNoInOpt 
	{
		$$ = CParser::getparser()->getCodeGen()->variableDeclaration( $1, $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TypedIdentifier InitialiserOpt
	{
 		/*ECMA4 compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_ES4DRAFT );
		$$ = CParser::getparser()->getCodeGen()->typedVariableDeclaration( $1, $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
 ;
InitialiserNoInOpt : /**/
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | InitialiserNoIn
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Initialiser : '=' AssignmentExpression
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
InitialiserNoIn : '=' AssignmentExpressionNoIn
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
EmptyStatement :  SemiColon
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
expressionStatement : Expression  { AUTOSEMICOLON } SemiColon
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
ifStatement : TOKEN_IF '(' Expression ')' Statement ElseStatementOpt
	{
		$$ = CParser::getparser()->getCodeGen()->ifStatement( $3, $5, $6 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
ElseStatementOpt : /**/
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_ELSE Statement
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
IterationStatement : TOKEN_DO Statement TOKEN_WHILE '(' Expression ')'
	{
		$$ = CParser::getparser()->getCodeGen()->forStatement( NULL, $5, NULL, $2, true ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_WHILE '(' Expression ')' Statement
	{
		$$ = CParser::getparser()->getCodeGen()->forStatement( NULL, $3, NULL, $5 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_FOR '(' ExpressionNoInOpt ';' ExpressionOpt ';' ExpressionOpt ')' Statement
	{
		$$ = CParser::getparser()->getCodeGen()->forStatement( $3, $5, $7, $9 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_FOR '(' TOKEN_VAR VariableDeclarationListNoIn ';' ExpressionOpt ';' ExpressionOpt ')' Statement
	{
		$$ = CParser::getparser()->getCodeGen()->forStatement( $4, $6, $8, $10 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_FOR '(' TOKEN_VAR VariableDeclarationNoIn TOKEN_IN Expression ')' Statement
	{
		CCgNodeData* dummy = CParser::getparser()->getCodeGen()->makeFormulaNode( CG_NONE, NULL, NULL, false, false );
//		assert( $4->rhsNode == NULL );
		$4->rhsNode = dummy;
		$$ = CParser::getparser()->getCodeGen()->forInStatement( $4, $6, $8 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_FOR '(' LeftHandSideExpressionForIn TOKEN_IN Expression ')' Statement
	{
		CCgNodeData* dummy = CParser::getparser()->getCodeGen()->makeFormulaNode( CG_NONE, NULL, NULL, false, false );
		CCgNodeData* node = CParser::getparser()->getCodeGen()->variableAssignment( $3, dummy );
		$$ = CParser::getparser()->getCodeGen()->forInStatement( node, $5, $7 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
continueStatement : TOKEN_CONTINUE IdentifierOpt { AUTOSEMICOLON } SemiColon 
	{
		$$ = CParser::getparser()->getCodeGen()->continueStatement( $2 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
breakStatement : TOKEN_BREAK IdentifierOpt { AUTOSEMICOLON } SemiColon
	{
		$$ = CParser::getparser()->getCodeGen()->breakStatement( $2 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
returnStatement : TOKEN_RETURN ExpressionOpt { AUTOSEMICOLON } SemiColon
	{
		$$ = CParser::getparser()->getCodeGen()->returnStatement( $2 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
withStatement : TOKEN_WITH '(' Expression ')' Statement 
	{
		$$ = CParser::getparser()->getCodeGen()->withStatement( $3, $5 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
switchStatement : TOKEN_SWITCH '(' Expression ')' CaseBlock 
	{
		$$ = CParser::getparser()->getCodeGen()->switchStatement( $3, $5 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
CaseBlock : '{' '}'
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '{' CaseClauses '}'
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
CaseClauses : caseClause
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | DefaultClause
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | CaseClauses caseClause
	{
		$1->SetSibling( $2 );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | CaseClauses DefaultClause
	{
		$1->SetSibling( $2 );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
caseClause : TOKEN_CASE Expression ':' StatementList
	{
		$$ = CParser::getparser()->getCodeGen()->caseClause( $2, $4 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_CASE Expression ':'
	{
		$$ = CParser::getparser()->getCodeGen()->caseClause( $2, NULL ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
DefaultClause : TOKEN_DEFAULT ':' StatementList
	{
		$$ = CParser::getparser()->getCodeGen()->caseClause( NULL, $3 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_DEFAULT ':'
	{
		$$ = CParser::getparser()->getCodeGen()->caseClause( NULL, NULL ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
LabelledStatement : identifier ':' Statement 
	{
		$$ = CParser::getparser()->getCodeGen()->labeledStatement( $1, $3 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
throwStatement : TOKEN_THROW Expression { AUTOSEMICOLON } SemiColon
	{
		$$ = CParser::getparser()->getCodeGen()->throwStatement( $2 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
TryStatement : TOKEN_TRY Block Finally
	{
		$$ = CParser::getparser()->getCodeGen()->exceptionHandlingClause( $2, NULL, $3 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_TRY Block Catch Finally
	{
		$$ = CParser::getparser()->getCodeGen()->exceptionHandlingClause( $2, $3, $4 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_TRY Block Catch
	{
		$$ = CParser::getparser()->getCodeGen()->exceptionHandlingClause( $2, $3, NULL ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_TRY Block error
    {
		CParser::getparser()->getCodeGen()->reportError( ERROR_EXPECTED_CATCH, $<iFileId>3, $<iLineNumber>3, $<iColumn>3 );
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Catch : TOKEN_CATCH '(' identifier ')' Block 
	{
		$$ = CParser::getparser()->getCodeGen()->catchBlock( $3, $5 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Finally : TOKEN_FINALLY Block
	{
		//$$ = $2;
		$$ = CParser::getparser()->getCodeGen()->finallyBlock( $2 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
//Function declarations
FunctionSourceElements : FunctionSourceElements FunctionSourceElement
	{
		if( $1 != NULL )
		{
			$1->SetSibling( $2 );
			$$ = $1;
		}
		else
			$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | FunctionSourceElement
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
FunctionSourceElement : functionDeclaration
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Statement
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
functionDeclaration : TOKEN_FUNCTION identifier '(' FormalParameterList ')' FunctionBody 
	{
		$$ = CParser::getparser()->getCodeGen()->functionDeclaration( $2, $4, $6 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_FUNCTION identifier '(' FormalParameterList ')' ':' identifier FunctionBody 
	{
 		/*ECMA4 compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_ES4DRAFT );
		$$ = CParser::getparser()->getCodeGen()->typedFunctionDeclaration( $2, $4, $8, $7 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
FunctionExpression : TOKEN_FUNCTION IdentifierOpt '(' FormalParameterList ')' FunctionBody 
	{
		$$ = CParser::getparser()->getCodeGen()->functionDeclaration( $2, $4, $6 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_FUNCTION IdentifierOpt '(' FormalParameterList ')' ':' identifier FunctionBody 
	{
 		/*ECMA4 compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_ES4DRAFT );
		$$ = CParser::getparser()->getCodeGen()->typedFunctionDeclaration( $2, $4, $8, $7 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
FormalParameterList : /*Empty*/
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Identifiers
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Identifiers ',' TOKEN_RESTARG_PREFIX identifier
	{
 		/*ECMA4 compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_ES4DRAFT );
 		CCgNodeData* node = CParser::getparser()->getCodeGen()->makeNode( CG_NODE_RESTARGS,
 														(CG_OPERATIONTYPE)0,
 														NULL, $4 );
		$1->SetSibling( node );
		$$ = $1;

		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Identifiers ',' TOKEN_RESTARG_PREFIX ArrayTypedIdentifier
	{
		//Array typed restArgs
		
 		/*ECMA4 compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_ES4DRAFT );
 		CCgNodeData* node = CParser::getparser()->getCodeGen()->makeNode( CG_NODE_RESTARGS,
 														(CG_OPERATIONTYPE)0,
 														NULL, $4 );
		$1->SetSibling( node );
		$$ = $1;

		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Identifiers : identifier
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Identifiers ',' identifier
	{
		$1->SetSibling( $3 );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TypedIdentifier
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Identifiers ',' TypedIdentifier
	{
		$1->SetSibling( $3 );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
FunctionBody : '{' FunctionSourceElements '}'
	{
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  '{' '}'
	{
		//For Empty statement
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
SemiColon : ';'
	{
		//CParser::getparser()->getCodeGen()->tagTheLine();
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
LiteralString : LITERAL_STRING
	{
		$$ = CParser::getparser()->getCodeGen()->literalConstant( $1,
													$<iFileId>1,
													$<iLineNumber>1,
													$<iColumn>1 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
LiteralInteger : LITERAL_INTEGER
	{
		$$ = CParser::getparser()->getCodeGen()->integerConstant( $1,
													$<iFileId>1,
													$<iLineNumber>1,
													$<iColumn>1 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
LiteralFloat : LITERAL_DECIMAL
	{
		$$ = CParser::getparser()->getCodeGen()->floatConstant( $1,
													$<iFileId>1,
													$<iLineNumber>1,
													$<iColumn>1 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
identifier : IDENTIFIER_NAME
	{
		$$ = CParser::getparser()->getCodeGen()->identifier( $1,
												$<iFileId>1,
												$<iLineNumber>1,
												$<iColumn>1 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
LiteralBoolean : LITERAL_BOOLEAN
	{
		$$ = CParser::getparser()->getCodeGen()->booleanConstant( $1,
													$<iFileId>1,
													$<iLineNumber>1,
													$<iColumn>1 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
LiteralRegex : { CParser::getparser()->getLexer()->allowRegexLiteral( true ); } '/' LITERAL_REGULAR_EXPRESSION
	{
		$$ = CParser::getparser()->getCodeGen()->regexLiteral( $3,
													$<iFileId>3,
													$<iLineNumber>3,
													$<iColumn>3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;

/********************************************************************************/
/* CRIScript extensions */
/********************************************************************************/
Extension_ImportStatement : TOKEN_IMPORT Extension_ImportComponent
	{
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
 		//Do nothing for now
		$$ = NULL;
	}
 ;
Extension_ImportComponent : identifier
 | Extension_ImportComponent '.' identifier
 ;
Extension_AtCcOnStatement : TOKEN_ATCC_ON
	{
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = NULL;
	}
 ;
StatementListOpt : /**/
	{
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | StatementList
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtIfStatement : TOKEN_ATIF '(' Extension_AtExpression ')' StatementListOpt Extension_AtElseStatementOpt TOKEN_ATEND
	{
  		/*JScript compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = CParser::getparser()->getCodeGen()->ifStatement( $3, $5, $6 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_ATIF '(' Extension_AtExpression ')' StatementListOpt Extension_AtElifStatement TOKEN_ATEND
	{
  		/*JScript compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = CParser::getparser()->getCodeGen()->ifStatement( $3, $5, $6 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtElifStatement : TOKEN_ATELIF '(' Extension_AtExpression ')' StatementListOpt Extension_AtElseStatementOpt
	{
  		/*JScript compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = CParser::getparser()->getCodeGen()->ifStatement( $3, $5, $6 ); 
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtElseStatementOpt : /**/
	{
  		/*JScript compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_ATELSE
	{
  		/*JScript compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = NULL;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | TOKEN_ATELSE StatementList
	{
  		/*JScript compatibility extension*/
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = $2;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtLiteral : LiteralFloat
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LiteralInteger
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | LiteralBoolean
 	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtIdentifier : ATIDENTIFIER_NAME
	{
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = CParser::getparser()->getCodeGen()->identifier( $1,
												$<iFileId>1,
												$<iLineNumber>1,
												$<iColumn>1 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtPrimaryExpression : Extension_AtLiteral
 	{
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtIdentifier
 	{
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtSetStatement : TOKEN_ATSET Extension_AtIdentifier '=' Extension_AtExpression
	{
 		CParser::getparser()->getCodeGen()->checkExtension( EXTENSION_JSCRIPT );
		$$ = CParser::getparser()->getCodeGen()->variableDeclaration( $2, $4 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;

/*
Operators below are allowed for @if,@set statement
http://msdn2.microsoft.com/en-us/library/thak6fez.aspx
! ~ 
* / % 
+ - 
<< >> >>> 
< <= > >= 
== != === !== 
& ^ | 
&& || 
*/
Extension_AtunaryExpression : Extension_AtPrimaryExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '+' Extension_AtunaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->unaryPlusOperator( $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '-' Extension_AtunaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->unaryMinusOperator( $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '~' Extension_AtunaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->unaryBitwiseNotOperator( $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | '!' Extension_AtunaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->unaryLogicalNotOperator( $2 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtMultiplicativeExpression : Extension_AtunaryExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtMultiplicativeExpression '*' Extension_AtunaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->multiplyOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtMultiplicativeExpression '/' Extension_AtunaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->divisionOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtMultiplicativeExpression '%' Extension_AtunaryExpression
	{
		$$ = CParser::getparser()->getCodeGen()->reminderOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ; 
Extension_AtAdditiveExpression : Extension_AtMultiplicativeExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtAdditiveExpression '+' Extension_AtMultiplicativeExpression
	{
		$$ = CParser::getparser()->getCodeGen()->addOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtAdditiveExpression '-' Extension_AtMultiplicativeExpression
	{
		$$ = CParser::getparser()->getCodeGen()->subtractOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtshiftExpression : Extension_AtAdditiveExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtshiftExpression TOKEN_SHIFTLEFT Extension_AtAdditiveExpression
	{
		$$ = CParser::getparser()->getCodeGen()->shiftLeftOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtshiftExpression TOKEN_SHIFTRIGHT Extension_AtAdditiveExpression
	{
		$$ = CParser::getparser()->getCodeGen()->shiftRightOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtshiftExpression TOKEN_SHIFTRIGHTUNSIGNED Extension_AtAdditiveExpression
	{
		$$ = CParser::getparser()->getCodeGen()->shiftRightUnsignedOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ; 
Extension_AtRelationalExpression : Extension_AtshiftExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  Extension_AtRelationalExpression TOKEN_GREATER Extension_AtshiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_GREATER );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  Extension_AtRelationalExpression TOKEN_GREATEREQUAL Extension_AtshiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_GREATEREQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  Extension_AtRelationalExpression TOKEN_LESS Extension_AtshiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_LESS );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  Extension_AtRelationalExpression TOKEN_LESSEQUAL Extension_AtshiftExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_LESSEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtEqualityExpression : Extension_AtRelationalExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtEqualityExpression TOKEN_EQUALOP Extension_AtRelationalExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_EQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtEqualityExpression TOKEN_NOTEQUALOP Extension_AtRelationalExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_NOTEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtEqualityExpression TOKEN_STRICTEQUALOP Extension_AtRelationalExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_STRICTEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtEqualityExpression TOKEN_STRICTNOTEQUALOP Extension_AtRelationalExpression
	{
		$$= CParser::getparser()->getCodeGen()->cmpOperator( $1, $3, CG_STRICTNOTEQUAL );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtbitwiseANDExpression : Extension_AtEqualityExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtbitwiseANDExpression '&' Extension_AtEqualityExpression
	{
		$$ = CParser::getparser()->getCodeGen()->bitwiseAndOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtbitwiseXORExpression : Extension_AtbitwiseANDExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 |  Extension_AtbitwiseXORExpression '^' Extension_AtbitwiseANDExpression
	{
		$$ = CParser::getparser()->getCodeGen()->bitwiseXorOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtbitwiseORExpression : Extension_AtbitwiseXORExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtbitwiseORExpression '|' Extension_AtbitwiseXORExpression
	{
		$$ = CParser::getparser()->getCodeGen()->bitwiseOrOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtlogicalANDExpression : Extension_AtbitwiseORExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtlogicalANDExpression TOKEN_LOGAND Extension_AtbitwiseORExpression
	{
		$$ = CParser::getparser()->getCodeGen()->logicalAndOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtlogicalORExpression : Extension_AtlogicalANDExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 | Extension_AtlogicalORExpression TOKEN_LOGOR Extension_AtlogicalANDExpression
	{
		$$ = CParser::getparser()->getCodeGen()->logicalOrOperator( $1, $3 );
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;
Extension_AtExpression : Extension_AtlogicalORExpression
	{
		$$ = $1;
		CParser::getparser()->getLexer()->debugRecentlyReduced( __LINE__ );
	}
 ;

%%
/********************************************************************************/
/* Code goes here */

/********************************************************************************/

}	//namespace CRI