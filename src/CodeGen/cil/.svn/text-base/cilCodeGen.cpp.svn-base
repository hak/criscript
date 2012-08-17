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
 * Module   : cil codegenerater
 * File     : cilCodegen.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
namespace cri {

/***************************************************************************
 *      Variables
 ***************************************************************************/
TYPECONSTRAINT_INFO CCilCodeGen::m_TypeConstraintTable[] = {
	{ L"boolean",	{ OPERAND_BOOLEAN,	OPERAND_FLAG_NONE, 0 } },
	{ L"char",		{ OPERAND_INT,		OPERAND_FLAG_NONE, 0 } },
	{ L"byte",		{ OPERAND_INT,		OPERAND_FLAG_NONE, 0 } },
	{ L"short",		{ OPERAND_INT,		OPERAND_FLAG_NONE, 0 } },
	{ L"long",		{ OPERAND_INT64,	OPERAND_FLAG_NONE, 0 } },
	{ L"int",		{ OPERAND_INT,		OPERAND_FLAG_NONE, 0 } },
	{ L"sbyte",		{ OPERAND_INT,		OPERAND_FLAG_NONE, 0 } },
	{ L"ushort",	{ OPERAND_INT,		OPERAND_FLAG_NONE, 0 } },
	{ L"ulong",		{ OPERAND_INT64,	OPERAND_FLAG_NONE, 0 } },
	{ L"uint",		{ OPERAND_INT,		OPERAND_FLAG_NONE, 0 } },
	{ L"float",		{ OPERAND_FLOAT,	OPERAND_FLAG_NONE, 0 } },
	{ L"double",	{ OPERAND_DOUBLE,	OPERAND_FLAG_NONE, 0 } },
	//Builtin Type name
	{ L"Object",	{ OPERAND_OBJECTREF, OPERAND_FLAG_NONE, 0 } },
	{ L"String",	{ OPERAND_OBJECTREF, OPERAND_FLAG_NONE, 0 } },
	{ L"Number",	{ OPERAND_OBJECTREF, OPERAND_FLAG_NONE, 0 } },
	{ L"Boolean",	{ OPERAND_BOOLEAN,	OPERAND_FLAG_NONE, 0 } },
//	{ L"Boolean",	{ OPERAND_OBJECTREF, OPERAND_FLAG_NONE, 0 } },
	{ L"Date",		{ OPERAND_OBJECTREF, OPERAND_FLAG_NONE, 0 } },
	{ L"Array",		{ OPERAND_OBJECTREF, OPERAND_FLAG_NONE, 0 } },
	{ L"RegExp",	{ OPERAND_OBJECTREF, OPERAND_FLAG_NONE, 0 } },
	{ L"Error",		{ OPERAND_OBJECTREF, OPERAND_FLAG_NONE, 0 } },
};

map< OP_CODE, OPCODE_TABLEENTRY*> CCilCodeGen::m_OpcodeMapByOpcode;
hash_map< string, OPCODE_TABLEENTRY*> CCilCodeGen::m_OpcodeMapByName;

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 * ctor
 ***************************************************************************/
CCilCodeGen::CCilCodeGen() :	m_iNumLabel( 0 ),
								m_iCurrentILPoolIndex( 0 ),
								m_bResultInEvalStack( 0 ),
								m_bInsideSwitchStatement( false ),
								m_iInsideIterationStatement( 0 ),
								m_iLabelBreak( 0 ),
								m_iLabelContinue( 0 ),
								m_iLabelEndLoop( 0 ),
								m_iNumTmpVariable( 0 ),
								m_bMultipass( true ),
								m_pRootNode( NULL ),
								m_iCurrentEHNestLevel( 0 ),
								m_bUseFinallyOps( false ),
								m_bRestArgsInArgList( false ),
								m_pError( NULL ),
								m_pLex( NULL ),
								m_pParser( NULL ),
								m_pStringLexer( NULL )
{
	checkCurrentEndianness();
	setTargetEndianness( getCurrentEndianness() );
	clearILOffsetCache();
};

/***************************************************************************
 * dtor
 ***************************************************************************/
CCilCodeGen::~CCilCodeGen()
{
	assert( m_WithStatementLookupStack.size() == 0 );
	m_MapTypeConstraint.clear();
	m_MetaData.terminate();

	m_mapStringConsantPool.clear();

	m_FloatConstantPool.clear();
	m_IntegerConstantPool.clear();
	m_mapIntegerConstantPool.clear();
	m_mapFloatConstantPool.clear();

	m_SymbolInfoPool.clear();
	m_ILPool.clear();
	m_ILPoolCallStack.clear();

	m_mapPeepholeOptimizer.clear();

	m_vecBranchTargetInfoTable.clear();
	m_vecArgumentListStack.clear();
};

/***************************************************************************
 * Initialize members
 ***************************************************************************/
bool CCilCodeGen::init ()
{
	m_MetaData.initialize();
	m_ILPoolCallStack.clear();
	m_ILPool.clear();
	m_vecArgumentListStack.clear();
	m_vecBranchTargetInfoTable.clear();

	//Initialize IL pool
	wstring s( L"" );
	enterNewScope( s, 0 );		//Create global scope

	initializeConstantPools();
	initializeBuiltInObjects();

	//Initialize OpCodeMap
	int32_t i = 0;
	if( !m_OpcodeMapByOpcode.size()
		&& !m_OpcodeMapByName.size() )
	{
		//They are static member, need to be initialzied once
		while ( g_Opecode[ i ].OpCode != CEE_FORCE_DWORD )
		{
				m_OpcodeMapByOpcode[ g_Opecode[ i ].OpCode ] = &g_Opecode[ i ];
				m_OpcodeMapByName[ g_Opecode[ i ].pName ] = &g_Opecode[ i ];
				i++;
		}
	}

	//Initalize type constraint map
	for( i = 0; i < ARRAYSIZE( m_TypeConstraintTable ); ++ i )
	{
		if(  m_TypeConstraintTable[ i ].type.type == OPERAND_OBJECTREF )
		{
			//Fill in the type info
			RID rid = getMetaData().queryBuiltinObjectRid( m_TypeConstraintTable[ i ].name );
			m_TypeConstraintTable[ i ].type.ridConstraintType = rid;
		}
		m_MapTypeConstraint[ m_TypeConstraintTable[ i ].name ] = m_TypeConstraintTable[ i ];
	}

	m_pError = NULL;
	m_bInitialized = true;

	//Initialize onmemory lexer for eval
	m_pStringLexer = new CStringLexer();
	m_pStringLexer->init();

	m_bTreatUnitializedValueAsZero = false;
	//m_bTreatFunctionNameReadOnly = true;
	m_bTreatFunctionNameReadOnly = false;

	//Initialize peephole optimizer
	initPeepholeOptimizer();

	return true;
};

/***************************************************************************
 * terminate members
 ***************************************************************************/
bool CCilCodeGen::terminate ()
{
	delete( m_pStringLexer );
	m_pStringLexer = NULL;

	return true;
};

/***************************************************************************
 * verify code generation
 ***************************************************************************/
bool CCilCodeGen::verify ()
{
	return true;
};

/***************************************************************************
 * Set lexer and error handler handles
 ***************************************************************************/
void CCilCodeGen::setInterfaces( CParser* pParser, CLexer* pLex, CErrorPool * pError )
{
	assert( m_bInitialized == true );
	assert( m_pLex == NULL );
	m_pParser = pParser;
	m_pError = pError;
	m_pLex = pLex;
	return;
}

/***************************************************************************
 * Initialize constant pools
 ***************************************************************************/
void CCilCodeGen::initializeConstantPools ()
{
	m_IntegerConstantPool.clear();
	m_FloatConstantPool.clear();
	m_mapStringConsantPool.clear();
	m_mapIntegerConstantPool.clear();
	m_mapFloatConstantPool.clear();


	m_IntegerConstantPool.push_back( 0 );
	m_IntegerConstantPool.push_back( 1 );
	m_FloatConstantPool.push_back( 0.f );
	m_FloatConstantPool.push_back( 1.f );

	assert( m_IntegerConstantPool[ CONSTANT_ZERO ] == 0 );
	assert( m_IntegerConstantPool[ CONSTANT_ONE ] == 1 );
	assert( m_FloatConstantPool[ CONSTANT_ZERO ] == 0.f );
	assert( m_FloatConstantPool[ CONSTANT_ONE ] == 1.f );
}

/***************************************************************************
 * End of file
 ***************************************************************************/
void CCilCodeGen::endOfFile ()
{
	emit( CEE_RET );	//emit return code from global scope
	return;
};

/***************************************************************************
 * Adjust the num of the eval stack entry
 ***************************************************************************/
void CCilCodeGen::adjustEvalStack( ILFRAGMENTINFO* pIL, OPCODE_TABLEENTRY* opcodeparam, int32_t i )
{
	//Adjust eval stack entries
	if( opcodeparam->iPushbehaivor >= 1 )
	{
		pIL->iNumEvalStackEntries += opcodeparam->iPushbehaivor;
	}
	if( opcodeparam->iPopbehaivor >= 1 )
	{
		pIL->iNumEvalStackEntries -= opcodeparam->iPopbehaivor;
	}

	//Special case for popVar
	if( opcodeparam->OpCode == CEE_EXT_STARGLIST_S || 
		opcodeparam->OpCode == CEE_EXT_STARGLIST )
	{
		pIL->iNumEvalStackEntries -= i;
	}
}

void CCilCodeGen::adjustEvalStack( int32_t i )
{
	//Adjust eval stack entries
	getCurrentILInfo().iNumEvalStackEntries += i;
}

/***************************************************************************
 * emit value to ILPOOL
 ***************************************************************************/
void CCilCodeGen::emit( uint32_t iParam )
{
	if( getTargetEndianness() == getCurrentEndianness() )
	{
		for( int32_t i = 0;i < sizeof( iParam ); i++)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}
	else
	{
		for( int32_t i = sizeof( iParam ) - 1 ;i >= 0; --i)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}
}

/***************************************************************************
 * emit CIL ops (non arg version)
 ***************************************************************************/
void CCilCodeGen::emit( OP_CODE opcode )
{
	assert( opcode != CEE_EXT_STARGLIST_S );
	assert( opcode != CEE_EXT_STARGLIST );
	assert( opcode != CEE_CALL );
	assert( opcode != CEE_CALLI );
	assert( opcode != CEE_NEWOBJ );

	//Special handling for argument list
	switch( opcode )
	{
	case CEE_EXT_CALLI:
		emitArgumentList();
	default:
		//verify parameters
		OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
		assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == OPERAND_PARAMS_INLINE_NONE );

		if( opcodeparam->bByte1 != 0xff )
			getCurrentILInfo().ILPool.push_back( opcodeparam->bByte1 );
		getCurrentILInfo().ILPool.push_back( opcodeparam->bByte2 );

		adjustEvalStack( &getCurrentILInfo(), opcodeparam, 0 );
		break;
	}
	UpdateILOffsetCache();
}

/***************************************************************************
 * emit CIL ops (1 arg version)
 ***************************************************************************/
void CCilCodeGen::emit( OP_CODE opcode, uint8_t cParam )
{
	assert( opcode != CEE_CALL );
	assert( opcode != CEE_CALLI );
	assert( opcode != CEE_EXT_CALLI );
	assert( opcode != CEE_NEWOBJ );

	//Special handling for argument list
	switch( opcode )
	{
	case CEE_EXT_STARGLIST_S:
		pushArgumentList( cParam );
		break;
	case CEE_EXT_STARGLIST:
		pushArgumentList( cParam );
		break;
	default:
		//verify parameters
		OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
		assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == sizeof( cParam ) );

		if( opcodeparam->bByte1 != 0xff )
			getCurrentILInfo().ILPool.push_back( opcodeparam->bByte1 );
		getCurrentILInfo().ILPool.push_back( opcodeparam->bByte2 );

		getCurrentILInfo().ILPool.push_back( cParam );

		adjustEvalStack( &getCurrentILInfo(), opcodeparam, cParam );
		break;
	}
	UpdateILOffsetCache();
}

/***************************************************************************
 * emit CIL ops (1 arg version)
 ***************************************************************************/
void CCilCodeGen::emit( OP_CODE opcode, int8_t cParam )
{
	assert( opcode != CEE_EXT_STARGLIST_S );
	assert( opcode != CEE_EXT_STARGLIST );
	assert( opcode != CEE_CALL );
	assert( opcode != CEE_CALLI );
	assert( opcode != CEE_EXT_CALLI );
	assert( opcode != CEE_NEWOBJ );

	//verify parameters
	OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
	assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == sizeof( cParam ) );

	if( opcodeparam->bByte1 != 0xff )
		getCurrentILInfo().ILPool.push_back( opcodeparam->bByte1 );
	getCurrentILInfo().ILPool.push_back( opcodeparam->bByte2 );

	getCurrentILInfo().ILPool.push_back( cParam );

	adjustEvalStack( &getCurrentILInfo(), opcodeparam, cParam );
	UpdateILOffsetCache();
}

/***************************************************************************
 * emit CIL ops (1 arg version)
 ***************************************************************************/
void CCilCodeGen::emit( OP_CODE opcode, int32_t iParam )
{
	//Special handling for argument list
	switch( opcode )
	{
	case CEE_EXT_STARGLIST_S:
		pushArgumentList( iParam );
		break;
	case CEE_EXT_STARGLIST:
		pushArgumentList( iParam );
		break;
	case CEE_CALL:
	case CEE_CALLI:
	case CEE_EXT_CALLI:
	case CEE_NEWOBJ:
		emitArgumentList();
	default:
		//verify parameters
		OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
		assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == sizeof( iParam ) );

		if( opcodeparam->bByte1 != 0xff )
			getCurrentILInfo().ILPool.push_back( opcodeparam->bByte1 );
		getCurrentILInfo().ILPool.push_back( opcodeparam->bByte2 );

		if( getTargetEndianness() == getCurrentEndianness() )
		{
			for( int32_t i = 0;i < sizeof( iParam ); i++)
				getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
		}
		else
		{
			for( int32_t i = sizeof( iParam ) - 1 ;i >= 0; --i)
				getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
		}

		adjustEvalStack( &getCurrentILInfo(), opcodeparam, iParam );
		break;
	}
	UpdateILOffsetCache();
}

/***************************************************************************
 * emit CIL ops (1 arg version)
 ***************************************************************************/
void CCilCodeGen::emit( OP_CODE opcode, uint32_t iParam )
{
	//Special handling for argument list
	switch( opcode )
	{
	case CEE_EXT_STARGLIST_S:
		pushArgumentList( iParam );
		break;
	case CEE_EXT_STARGLIST:
		pushArgumentList( iParam );
		break;
	case CEE_CALL:
	case CEE_CALLI:
	case CEE_EXT_CALLI:
	case CEE_NEWOBJ:
		emitArgumentList();
	default:
		//verify parameters
		OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
		assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == sizeof( iParam ) );

		if( opcodeparam->bByte1 != 0xff )
			getCurrentILInfo().ILPool.push_back( opcodeparam->bByte1 );
		getCurrentILInfo().ILPool.push_back( opcodeparam->bByte2 );

		if( getTargetEndianness() == getCurrentEndianness() )
		{
			for( int32_t i = 0;i < sizeof( iParam ); i++)
				getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
		}
		else
		{
			for( int32_t i = sizeof( iParam ) - 1 ;i >= 0; --i)
				getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
		}

		adjustEvalStack( &getCurrentILInfo(), opcodeparam, iParam );
	}
	UpdateILOffsetCache();
}

/***************************************************************************
 * emit CIL ops (arglist version)
 ***************************************************************************/
void CCilCodeGen::emitArglist( OP_CODE opcode, uint8_t iParam )
{
	assert( opcode != CEE_EXT_STARGLIST_S || opcode != CEE_EXT_STARGLIST );

	//verify parameters
	OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
	assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == sizeof( iParam ) );

	if( opcodeparam->bByte1 != 0xff )
		getCurrentILInfo().ILPool.push_back( opcodeparam->bByte1 );
	getCurrentILInfo().ILPool.push_back( opcodeparam->bByte2 );

	getCurrentILInfo().ILPool.push_back( iParam );

	adjustEvalStack( &getCurrentILInfo(), opcodeparam, iParam );
}

/***************************************************************************
 * emit CIL ops (arglist version)
 ***************************************************************************/
void CCilCodeGen::emitArglist( OP_CODE opcode, uint32_t iParam )
{
	assert( opcode == CEE_EXT_STARGLIST_S || opcode == CEE_EXT_STARGLIST );

	//verify parameters
	OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
	assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == sizeof( iParam ) );

	if( opcodeparam->bByte1 != 0xff )
		getCurrentILInfo().ILPool.push_back( opcodeparam->bByte1 );
	getCurrentILInfo().ILPool.push_back( opcodeparam->bByte2 );

	if( getTargetEndianness() == getCurrentEndianness() )
	{
		for( int32_t i = 0;i < sizeof( iParam ); i++)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}
	else
	{
		for( int32_t i = sizeof( iParam ) - 1 ;i >= 0; --i)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}

	adjustEvalStack( &getCurrentILInfo(), opcodeparam, iParam );
}

/***************************************************************************
 * emit CIL ops (1 arg version)
 ***************************************************************************/
void CCilCodeGen::emit( OP_CODE opcode, int64_t iParam )
{
	assert( opcode != CEE_EXT_STARGLIST_S );
	assert( opcode != CEE_EXT_STARGLIST );
	assert( opcode != CEE_CALL );
	assert( opcode != CEE_CALLI );
	assert( opcode != CEE_EXT_CALLI );
	assert( opcode != CEE_NEWOBJ );

	//verify parameters
	OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
	assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == sizeof( iParam ) );

	if( opcodeparam->bByte1 != 0xff )
		getCurrentILInfo().ILPool.push_back( opcodeparam->bByte1 );
	getCurrentILInfo().ILPool.push_back( opcodeparam->bByte2 );

	if( getTargetEndianness() == getCurrentEndianness() )
	{
		for( int32_t i = 0;i < sizeof( iParam ); i++)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}
	else
	{
		for( int32_t i = sizeof( iParam ) - 1 ;i >= 0; --i)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}

	adjustEvalStack( &getCurrentILInfo(), opcodeparam, 0 );
	UpdateILOffsetCache();
}

/***************************************************************************
 * emit CIL ops (1 arg version)
 ***************************************************************************/
void CCilCodeGen::emit( OP_CODE opcode, float iParam )
{
	assert( opcode != CEE_EXT_STARGLIST_S );
	assert( opcode != CEE_EXT_STARGLIST );
	assert( opcode != CEE_CALL );
	assert( opcode != CEE_CALLI );
	assert( opcode != CEE_EXT_CALLI );
	assert( opcode != CEE_NEWOBJ );

	//verify parameters
	OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
	assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == sizeof( iParam ) );

	if( opcodeparam->bByte1 != 0xff )
		getCurrentILInfo().ILPool.push_back( opcodeparam->bByte1 );
	getCurrentILInfo().ILPool.push_back( opcodeparam->bByte2 );

	if( getTargetEndianness() == getCurrentEndianness() )
	{
		for( int32_t i = 0;i < sizeof( iParam ); i++)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}
	else
	{
		for( int32_t i = sizeof( iParam ) - 1 ;i >= 0; --i)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}

	adjustEvalStack( &getCurrentILInfo(), opcodeparam, 0 );
	UpdateILOffsetCache();
}

/***************************************************************************
 * emit CIL ops (1 arg version)
 ***************************************************************************/
void CCilCodeGen::emit( OP_CODE opcode, double iParam )
{
	assert( opcode != CEE_EXT_STARGLIST_S );
	assert( opcode != CEE_EXT_STARGLIST );
	assert( opcode != CEE_CALL );
	assert( opcode != CEE_CALLI );
	assert( opcode != CEE_EXT_CALLI );
	assert( opcode != CEE_NEWOBJ );

	//verify parameters
	OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
	assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == sizeof( iParam ) );

	if( opcodeparam->bByte1 != 0xff )
		getCurrentILInfo().ILPool.push_back( opcodeparam->bByte1 );
	getCurrentILInfo().ILPool.push_back( opcodeparam->bByte2 );

	if( getTargetEndianness() == getCurrentEndianness() )
	{
		for( int32_t i = 0;i < sizeof( iParam ); i++)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}
	else
	{
		for( int32_t i = sizeof( iParam ) - 1 ;i >= 0; --i)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}

	adjustEvalStack( &getCurrentILInfo(), opcodeparam, 0 );
	UpdateILOffsetCache();
}

/***************************************************************************
 * emit CIL ops (3 arg version, only used with loca/arg resolution)
 ***************************************************************************/
void CCilCodeGen::emit( OP_CODE opcode, RID ridMethod, RID ridArg, int32_t iIndex )
{
	assert( opcode != CEE_EXT_STARGLIST_S );
	assert( opcode != CEE_EXT_STARGLIST );
	assert( opcode != CEE_CALL );
	assert( opcode != CEE_CALLI );
	assert( opcode != CEE_EXT_CALLI );
	assert( opcode != CEE_NEWOBJ );

	//verify opcode
	OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
	assert( opcode == CEE_EXT_LDARG || opcode == CEE_EXT_STARG || 
			opcode == CEE_EXT_LDLOC || opcode == CEE_EXT_STLOC );
	if( opcodeparam->bByte1 != 0xff )
		getCurrentILInfo().ILPool.push_back( opcodeparam->bByte1 );
	getCurrentILInfo().ILPool.push_back( opcodeparam->bByte2 );

	if( getTargetEndianness() == getCurrentEndianness() )
	{
		for( int32_t i = 0;i < sizeof( ridMethod ); i++)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&ridMethod) + i) );
		for( int32_t i = 0;i < sizeof( ridArg ); i++)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&ridArg) + i) );
		for( int32_t i = 0;i < sizeof( iIndex ); i++)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iIndex) + i) );
	}
	else
	{
		for( int32_t i = sizeof( ridMethod ) - 1 ;i >= 0; --i)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&ridMethod) + i) );
		for( int32_t i = sizeof( ridArg ) - 1 ;i >= 0; --i)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&ridArg) + i) );
		for( int32_t i = sizeof( iIndex ) - 1 ;i >= 0; --i)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iIndex) + i) );
	}

	adjustEvalStack( &getCurrentILInfo(), opcodeparam, 0 );
	UpdateILOffsetCache();
}

/***************************************************************************
 * emit CIL ops (1 arg version  w/ specific ILPool)
 ***************************************************************************/
void CCilCodeGen::emit( ILFRAGMENTINFO* pIL, OP_CODE opcode, int8_t iParam )
{
	assert( opcode != CEE_EXT_STARGLIST_S );
	assert( opcode != CEE_EXT_STARGLIST );
	assert( opcode != CEE_CALL );
	assert( opcode != CEE_CALLI );
	assert( opcode != CEE_EXT_CALLI );
	assert( opcode != CEE_NEWOBJ );

	//verify parameters
	OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
	assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == sizeof( iParam ) );

	if( opcodeparam->bByte1 != 0xff )
		pIL->ILPool.push_back( opcodeparam->bByte1 );
	pIL->ILPool.push_back( opcodeparam->bByte2 );

	pIL->ILPool.push_back( iParam );

	adjustEvalStack( pIL, opcodeparam, iParam );
	UpdateILOffsetCache();
}

/***************************************************************************
 * emit CIL ops (1 arg version w/ specific ILPool)
 ***************************************************************************/
void CCilCodeGen::emit( ILFRAGMENTINFO* pIL, OP_CODE opcode, int32_t iParam )
{
	assert( opcode != CEE_EXT_STARGLIST_S );
	assert( opcode != CEE_EXT_STARGLIST );
	assert( opcode != CEE_CALL );
	assert( opcode != CEE_CALLI );
	assert( opcode != CEE_EXT_CALLI );
	assert( opcode != CEE_NEWOBJ );

	//verify parameters
	OPCODE_TABLEENTRY* opcodeparam = m_OpcodeMapByOpcode[ opcode ];
	assert( (opcodeparam->OperandParams & OPERAND_PARAMS_SIZEMASK) == sizeof( iParam ) );

	if( opcodeparam->bByte1 != 0xff )
		pIL->ILPool.push_back( opcodeparam->bByte1 );
	pIL->ILPool.push_back( opcodeparam->bByte2 );

	if( getTargetEndianness() == getCurrentEndianness() )
	{
		for( int32_t i = 0;i < sizeof( iParam ); i++)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}
	else
	{
		for( int32_t i = sizeof( iParam ) - 1 ;i >= 0; --i)
			getCurrentILInfo().ILPool.push_back( *(((int8_t*)&iParam) + i) );
	}

	adjustEvalStack( pIL, opcodeparam, iParam );
	UpdateILOffsetCache();
}

/***************************************************************************
 * Helpers
 ***************************************************************************/
bool CCilCodeGen::isValue( CCgNodeData* object)
{
	bool bReturn = false;
	if( object == NULL ) return bReturn;

	switch( object->NodeType )
	{
	case CG_NODE_INTEGERLITERAL:
	case CG_NODE_FLOATLITERAL:
		bReturn = true;
		break;

	}
	return bReturn;
}

bool CCilCodeGen::isFloat( CCgNodeData* object)
{
	bool bReturn = false;
	if( object == NULL ) return bReturn;

	switch( object->NodeType )
	{
	case CG_NODE_FLOATLITERAL:
		bReturn = true;
		break;
	}
	return bReturn;
}


bool CCilCodeGen::isString( CCgNodeData* object)
{
	bool bReturn = false;
	if( object == NULL ) return bReturn;

	switch( object->NodeType )
	{
	case CG_NODE_STRINGLITERAL:
		bReturn = true;
		break;
	}
	return bReturn;
}

uint32_t CCilCodeGen::getCurrentILOffset()
{
	return (uint32_t)getCurrentILInfo().ILPool.size();
}

void CCilCodeGen::UpdateILOffsetCache()
{
	uint32_t iCurrent = getCurrentILOffset();
	if( iCurrent != m_iLastILOffset[ 0 ] )
	{
		m_iLastILOffset[ 2 ] = m_iLastILOffset[ 1 ];
		m_iLastILOffset[ 1 ] = m_iLastILOffset[ 0 ];
		m_iLastILOffset[ 0 ] = iCurrent;
	}
}

void CCilCodeGen::clearILOffsetCache()
{
	for( int32_t i = 0; i < ARRAYSIZE( m_iLastILOffset ); ++i )
		m_iLastILOffset[ i ] = 0;
}

/***************************************************************************
 * Report an error if the error handler is set
 ***************************************************************************/	
void CCilCodeGen::reportError( int32_t iErrorCode )
{
	assert( m_bInitialized == true );
	assert( m_pLex != NULL );
	if( m_pError )
	{
		wstring s( L"" );
		m_pError->report( iErrorCode, m_pLex->getLastTokenLine(),
									m_pLex->getLastTokenColumn(),
									m_pLex->getCurrentStreamName(),
									m_pLex->getLineString( m_pLex->getLastTokenLine() ),
									s );
	}
}

void CCilCodeGen::reportError( int32_t iErrorCode, const wstring& strErrorInfo )
{
	assert( m_bInitialized == true );
	assert( m_pLex != NULL );
	if( m_pError )
	{
		m_pError->report( iErrorCode, m_pLex->getLastTokenLine(),
									m_pLex->getLastTokenColumn(),
									m_pLex->getCurrentStreamName(), 
									m_pLex->getLineString( m_pLex->getLastTokenLine() ),
									(wstring &)strErrorInfo );
	}
}

void CCilCodeGen::reportError( int32_t iErrorCode,
				 const uint32_t iFileId,
				 const uint32_t iLineNumber,
				 const uint32_t iColumn )
{
	assert( m_bInitialized == true );
	assert( m_pLex != NULL );
	if( m_pError )
	{
		wstring s( L"" );
		m_pError->report( iErrorCode, iLineNumber,
									iColumn,
									m_pLex->getCurrentStreamName(),
									m_pLex->getLineString( iLineNumber ),
									s );
	}
}
void CCilCodeGen::reportError( int32_t iErrorCode, const wstring& strErrorInfo,
				 const uint32_t iFileId,
				 const uint32_t iLineNumber,
				 const uint32_t iColumn )
{
	assert( m_bInitialized == true );
	assert( m_pLex != NULL );
	if( m_pError )
	{
		m_pError->report( iErrorCode, iLineNumber,
									iColumn,
									m_pLex->getCurrentStreamName(), 
									m_pLex->getLineString( iLineNumber ),
									(wstring &)strErrorInfo );
	}
}

} //namespace CRI