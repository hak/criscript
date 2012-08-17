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
 * Module   : cil Virtual Machine
 * File     : CCilVm.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criscript.h"
#include "cilVm.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 * VM main execution routine
 ***************************************************************************/
#ifdef VM_DISPATCH_SWITCH
VM_STATUS_TYPE CCilVm::execute()
{
	if( m_status != VM_EXECUTE_INITIALIZED
		&& m_status != VM_EXECUTE_EXECUTING		//Canbe executing while InvokeMethod() call
		&& m_status != VM_EXECUTE_SUSPENDED
		&& m_status != VM_EXECUTE_SWAPPEDOUT )
		return m_status;

	m_status = VM_EXECUTE_EXECUTING;

	bool bExecute = true;
	uint8_t bIL;

	m_stopwatch.Start();
	
	while( 1 )
	{
		m_iTickCount ++;
		bIL = getNextBytecode();

		switch( bIL )
		{
		case CEE_NOP:
			//No operation
			break;
		case CEE_BREAK:
			//If debugger is attached, do some debugger staff here.
			//No operation
			break;
		//---------------------------------------------------------------
		//Exception handlers
		//---------------------------------------------------------------
		case CEE_EXT_ENTER:
			EnterOperator();
			break;
		case CEE_EXT_LEAVE:
			LeaveOperator();
			break;
		case CEE_EXT_FINALLY:
			FinallyOperator();	//execute Finally clause but not adjust Instruction pointer
			break;
		case CEE_THROW:
			//Take variable from eval stack and throw exception
			ThrowOperator();
			break;
		//---------------------------------------------------------------
		//Type conversion
		//---------------------------------------------------------------
		case CEE_EXT_CONV_B:
			convertToBoolean();
			break;
		//---------------------------------------------------------------
		//Branch instructions
		//---------------------------------------------------------------
		case CEE_CALL:
			CallOperator();
			break;
		case CEE_EXT_CALLI:
			{
			//Note:
			//CallI oprator is expanded for generic argument
			CVariable& var = getEvalStackFirstEntry();
			if( OperandType( var.iOperandType ) != OPERAND_OBJECTREF )
			{
				ThrowExceptionFromVm( &wstring( ERRORSTRING_ILLEGALCALL ),
					&wstring( NAME_BUILTIN_ERROR_OBJECT ),
					ERROR_ILLEGALCALL );
				//Pop illegal function reference
				popEvalStack();

				//Push undefined return value for later operations
				pushEvalStackUndefined();
				break;
			}
			RID ridCall = var.refObject->getCall();
			CVariable& varParent = var.refObject->getParentVariable();
			popEvalStack();
			CallOperator( &varParent, ridCall );
			}
			break;
		case CEE_RET:
			bExecute = RetOperator();
			if( !bExecute ) goto END_PROGRAM;
			break;
		case CEE_JMP:
		/*
		for Jump, scope check need to be done at a compile time. Opcode works just as same as CEE_BR
		*/
		//No break here;
		case CEE_BR:		//Unconditional branch
			{
				int32_t* pOffset = (int32_t*)m_pCurrentInstruction;
				m_pCurrentInstruction += *pOffset + LONGBR_ADJUST;
			}
			break;
		case CEE_BR_S:		//Unconditional branch/s
			{
				int8_t* pOffset = (int8_t*)m_pCurrentInstruction;
				m_pCurrentInstruction += *pOffset + SHORTBR_ADJUST;
			}
			break;
		case CEE_REM:
			reminderOperator();
			break;
		case CEE_BRFALSE:
			BrFalseOperator();
			break;
		case CEE_BRFALSE_S:
			BrFalseShortOperator();
			break;
		case CEE_BRTRUE:
			BrTrueOperator();
			break;
		case CEE_BRTRUE_S:
			BrTrueShortOperator();
			break;
		case CEE_BEQ:
			BeqOperator();
			break;
		case CEE_BEQ_S:
			BeqShortOperator();
			break;
		case CEE_BNE_UN:
			BneOperator();
			break;
		case CEE_BNE_UN_S:
			BneShortOperator();
			break;
		case CEE_EXT_BNGE:
			BngeOperator();
			break;
		case CEE_EXT_BNGE_S:
			BngeShortOperator();
			break;
		case CEE_EXT_BNGT:
			BngtOperator();
			break;
		case CEE_EXT_BNGT_S:
			BngtShortOperator();
			break;
		case CEE_EXT_BNLE:
			BnleOperator();
			break;
		case CEE_EXT_BNLE_S:
			BnleShortOperator();
			break;
		case CEE_EXT_BNLT:
			BnltOperator();
			break;
		case CEE_EXT_BNLT_S:
			BnltShortOperator();
			break;
		//---------------------------------------------------------------
		//Stack management
		//---------------------------------------------------------------
		case CEE_DUP:
			assert( isEvalStackEmpty() );
			dupEvalStack();
			break;
		case CEE_POP:
			assert( isEvalStackEmpty() );
			popEvalStack();
			break;
		//---------------------------------------------------------------
		//Object management
		//---------------------------------------------------------------
		case CEE_NEWOBJ:
			CreateNewObject();
			break;
		case CEE_LDSTR:
			CreateNewStringValue();
			break;
		//---------------------------------------------------------------
		//Load/store instructions
		//---------------------------------------------------------------
		case CEE_LDNULL:
			//Load NULL to eval stack
			pushEvalStackNull();
			break;
		case CEE_LDC_I4:
			{
				int32_t i = getNextInt32();
				pushEvalStack( i );
			}
			break;
		case CEE_LDC_I4_S:
			{
				int8_t c = getNextInt8();
				pushEvalStack( (int32_t)c );
			}
			break;
		case CEE_LDC_I4_M1:
			pushEvalStack( -1 );
			break;
		case CEE_LDC_I4_0:
			pushEvalStack( 0 );
			break;
		case CEE_LDC_I4_1:
			pushEvalStack( 1 );
			break;
		case CEE_LDC_I4_2:
			pushEvalStack( 2 );
			break;
		case CEE_LDC_I4_3:
			pushEvalStack( 3 );
			break;
		case CEE_LDC_I4_4:
			pushEvalStack( 4 );
			break;
		case CEE_LDC_I4_5:
			pushEvalStack( 5 );
			break;
		case CEE_LDC_I4_6:
			pushEvalStack( 6 );
			break;
		case CEE_LDC_I4_7:
			pushEvalStack( 7 );
			break;
		case CEE_LDC_I4_8:
			pushEvalStack( 8 );
			break;
		case CEE_LDC_I8:
			{
				int64_t i = getNextInt64();
				pushEvalStack( i );
			}
			break;
		case CEE_LDC_R4:
			{
				float f = getNextFloat();
				pushEvalStack( f );
			}
			break;
		case CEE_LDC_R8:
			{
				double d = getNextDouble();
				pushEvalStack( d );
			}
			break;
		case CEE_LDSFLD:
			loadStaticField( getNextToken() );
			break;
		case CEE_STSFLD:
			storeStaticField( getNextToken() );
			break;
		case CEE_LDLOC_0:
			loadLocalVariable( 0 );
			break;
 		case CEE_LDLOC_1:
			loadLocalVariable( 1 );
			break;
 		case CEE_LDLOC_2:
			loadLocalVariable( 2 );
			break;
 		case CEE_LDLOC_3:
			loadLocalVariable( 3 );
			break;
		case CEE_LDLOC_S:
			loadLocalVariable( getNextInt8() );
			break;
		case CEE_STLOC_0:
			storeLocalVariable( 0 );
			break;
 		case CEE_STLOC_1:
			storeLocalVariable( 1 );
			break;
 		case CEE_STLOC_2:
			storeLocalVariable( 2 );
			break;
 		case CEE_STLOC_3:
			storeLocalVariable( 3 );
			break;
		case CEE_STLOC_S:
			storeLocalVariable( getNextInt8() );
			break;
		//---------------------------------------------------------------
		//Arithmetical instructions
		//---------------------------------------------------------------
		case CEE_EXT_INCSFLD:
			incStaticFieldOperator( getNextToken() );
			break;
		case CEE_EXT_INC:
			incOperator();
			break;
		case CEE_EXT_DEC:
			decOperator();
			break;
		case CEE_ADD:
			addOperator();
			break;
		case CEE_SUB:
			subtractOperator();
			break;
		case CEE_MUL:
			multiplyOperator();
			break;
		case CEE_DIV:
			divisionOperator();
			break;
		case CEE_AND:
			bitwiseAndOperator();
			break;
		case CEE_OR:
			bitwiseOrOperator();
			break;
		case CEE_XOR:
			bitwiseXorOperator();
			break;
		case CEE_NOT:
			bitwiseNotOperator();
			break;
		case CEE_SHL:
			ShlOperator();
			break;
		case CEE_SHR:
			ShrOperator();
			break;
		case CEE_SHR_UN:
			ShrUnOperator();
			break;
		case CEE_NEG:
			negOperator();
			break;
		//---------------------------------------------------------------
		//Argument list related one
		//---------------------------------------------------------------
		case CEE_EXT_STARGLIST_S:
			storeArgumentListWithThisPointer( getNextUint8() );
			break;
		case CEE_EXT_STARGLIST:
			storeArgumentListWithThisPointer( getNextInt32() );
			break;
		case CEE_LDARG_0:
			//Dedicated to load this pointer
			loadArgument( 0 );
			break;
 		case CEE_LDARG_1:
			loadArgument( 1 );
			break;
 		case CEE_LDARG_2:
			loadArgument( 2 );
			break;
 		case CEE_LDARG_3:
			loadArgument( 3 );
			break;
 		case CEE_LDARG_S:
			loadArgument( getNextInt8() );
			break;
		case CEE_STARG_S:
			storeArgument( getNextInt8() );
			break;

		//---------------------------------------------------------------
		//Not implemented
		//---------------------------------------------------------------
		case CEE_BGE:
		case CEE_BGE_S:
		case CEE_BGT:
		case CEE_BGT_S:
		case CEE_BLE:
		case CEE_BLE_S:
		case CEE_BLT:
		case CEE_BLT_S:
		case CEE_CALLI:
		case CEE_SWITCH:
		case CEE_LDARGA_S:
		case CEE_LDLOCA_S:
		case CEE_LDIND_I1:
		case CEE_LDIND_U1:
		case CEE_LDIND_I2:
		case CEE_LDIND_U2:
		case CEE_LDIND_I4:
		case CEE_LDIND_U4:
		case CEE_LDIND_I8:
		case CEE_LDIND_I:
		case CEE_LDIND_R4:
		case CEE_LDIND_R8:
		case CEE_LDIND_REF:
		case CEE_STIND_REF:
		case CEE_STIND_I1:
		case CEE_STIND_I2:
		case CEE_STIND_I4:
		case CEE_STIND_I8:
		case CEE_STIND_R4:
		case CEE_STIND_R8:
		case CEE_DIV_UN:
		case CEE_REM_UN:
		case CEE_CONV_I1:
		case CEE_CONV_I2:
		case CEE_CONV_I4:
		case CEE_CONV_I8:
		case CEE_CONV_R4:
		case CEE_CONV_R8:
		case CEE_CONV_U4:
		case CEE_CONV_U8:
		case CEE_CALLVIRT:
		case CEE_CPOBJ:
		case CEE_LDOBJ:
		case CEE_CASTCLASS:
		case CEE_ISINST:
		case CEE_CONV_R_UN:
		case CEE_UNBOX:
		case CEE_LDFLD:
		case CEE_LDFLDA:
		case CEE_STFLD:
		case CEE_LDSFLDA:
		case CEE_STOBJ:
		case CEE_CONV_OVF_I1_UN:
		case CEE_CONV_OVF_I2_UN:
		case CEE_CONV_OVF_I4_UN:
		case CEE_CONV_OVF_I8_UN:
		case CEE_CONV_OVF_U1_UN:
		case CEE_CONV_OVF_U2_UN:
		case CEE_CONV_OVF_U4_UN:
		case CEE_CONV_OVF_U8_UN:
		case CEE_CONV_OVF_I_UN:
		case CEE_CONV_OVF_U_UN:
		case CEE_BOX:
		case CEE_NEWARR:
		case CEE_LDLEN:
		case CEE_LDELEMA:
		case CEE_LDELEM_I1:
		case CEE_LDELEM_U1:
		case CEE_LDELEM_I2:
		case CEE_LDELEM_U2:
		case CEE_LDELEM_I4:
		case CEE_LDELEM_U4:
		case CEE_LDELEM_I8:
		case CEE_LDELEM_I:
		case CEE_LDELEM_R4:
		case CEE_LDELEM_R8:
		case CEE_LDELEM_REF:
		case CEE_STELEM_I:
		case CEE_STELEM_I1:
		case CEE_STELEM_I2:
		case CEE_STELEM_I4:
		case CEE_STELEM_I8:
		case CEE_STELEM_R4:
		case CEE_STELEM_R8:
		case CEE_STELEM_REF:
		case CEE_LDELEM:
		case CEE_STELEM:
		case CEE_UNBOX_ANY:
		case CEE_CONV_OVF_I1:
		case CEE_CONV_OVF_U1:
		case CEE_CONV_OVF_I2:
		case CEE_CONV_OVF_U2:
		case CEE_CONV_OVF_I4:
		case CEE_CONV_OVF_U4:
		case CEE_CONV_OVF_I8:
		case CEE_CONV_OVF_U8:
		case CEE_REFANYVAL:
		case CEE_CKFINITE:
		case CEE_MKREFANY:
		case CEE_LDTOKEN:
		case CEE_CONV_U2:
		case CEE_CONV_U1:
		case CEE_CONV_I:
		case CEE_CONV_OVF_I:
		case CEE_CONV_OVF_U:
		case CEE_ADD_OVF:
		case CEE_ADD_OVF_UN:
		case CEE_MUL_OVF:
		case CEE_MUL_OVF_UN:
		case CEE_SUB_OVF:
		case CEE_SUB_OVF_UN:
		case CEE_ENDFINALLY:
		case CEE_LEAVE:
		case CEE_LEAVE_S:
		case CEE_STIND_I:
		case CEE_CONV_U:
		case CEE_PREFIX6:
		case CEE_PREFIX5:
		case CEE_PREFIX4:
		case CEE_PREFIX3:
		case CEE_PREFIX2:
		case CEE_PREFIXREF:

			Debug_Fatal( "Not implemented yet" );
			break;
		case CEE_PREFIX1:	//2 bytes instructions
			bIL = getNextBytecode();
			switch( bIL )
			{
			case CEE_CODE_LABEL & 0xff:
				//Just ingnore it
				getNextToken();
				break;
			//---------------------------------------------------------------
			//Compare
			//---------------------------------------------------------------
			case CEE_CEQ  & 0xff:
				CeqOperator();
				break;
			case CEE_EXT_CNE  & 0xff:
				CneOperator();
				break;
			case CEE_CGT  & 0xff:
				CgtOperator();
				break;
			case CEE_CLT  & 0xff:
				CltOperator();
				break;
			case CEE_EXT_CGE  & 0xff:
				CgeOperator();
				break;
			case CEE_EXT_CLE  & 0xff:
				CleOperator();
				break;
			case CEE_CGT_UN & 0xff:
			case CEE_CLT_UN & 0xff:
				break;
			case CEE_LDLOC & 0xff:
				loadLocalVariable( getNextInt32() );
				break;
			case CEE_STLOC & 0xff:
				storeLocalVariable( getNextInt32() );
				break;
			case CEE_LDARG & 0xff:
				loadArgument( getNextInt32() );
				break;
			case CEE_STARG & 0xff:
				storeArgument( getNextInt32() );
				break;
			//---------------------------------------------------------------
			//Not implemented
			//---------------------------------------------------------------
			case CEE_ARGLIST & 0xff:
			case CEE_LDFTN & 0xff:
			case CEE_LDVIRTFTN & 0xff:
			case CEE_LDARGA & 0xff:
			case CEE_LDLOCA & 0xff:
			case CEE_LOCALLOC & 0xff:
			case CEE_ENDFILTER & 0xff:
			case CEE_UNALIGNED & 0xff:
			case CEE_VOLATILE & 0xff:
			case CEE_TAILCALL & 0xff:
			case CEE_INITOBJ & 0xff:
			case CEE_CONSTRAINED & 0xff:
			case CEE_CPBLK & 0xff:
			case CEE_INITBLK & 0xff:
			case CEE_RETHROW & 0xff:
			case CEE_SIZEOF & 0xff:
			case CEE_REFANYTYPE & 0xff:
			case CEE_READONLY & 0xff:
			default:
				Debug_Fatal( "Not implemented yet" );
				break;
		}
			break;
		default:
			Debug_Fatal( "Not implemented yet" );
			break;
		}
		//Should I sleep??
	};
END_PROGRAM:

	m_stopwatch.Stop();
	m_status = VM_EXECUTE_DONE;

	return m_status;
}
#endif

} //namespace CRI