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
 * File     : CCilVmThreadedDispatcher.cpp
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

#ifdef VM_DISPATCH_DIRECT_THREADED


/***************************************************************************
//macros
/***************************************************************************/
#define DISPATCH() m_iTickCount ++;\
	getNextBytecode();\
	__asm and eax, 0xff\
	__asm jmp DWORD PTR [eax * 4 + tableDispatch]

#define DISPATCH2() 	m_iTickCount ++;\
	getNextBytecode();\
	__asm and eax, 0xff\
	__asm jmp DWORD PTR [eax * 4 + tableDispatchExt]

//	goto tableDispatchExt[ bIL ];\

/***************************************************************************
 * VM main execution routine
 ***************************************************************************/
VM_STATUS_TYPE CCilVm::execute()
{
	if( m_status != VM_EXECUTE_INITIALIZED
		&& m_status != VM_EXECUTE_SUSPENDED
		&& m_status != VM_EXECUTE_SWAPPEDOUT )
	m_status = VM_EXECUTE_EXECUTING;

	static const void* tableDispatch[0xff];
	static const void* tableDispatchExt[0xff];

	__asm {
		lea eax, LABEL_CEE_NOP
		mov tableDispatch + CEE_NOP * 4, eax
		lea eax, LABEL_CEE_BREAK
		mov tableDispatch + CEE_BREAK * 4, eax
		lea eax, LABEL_CEE_LDARG_0
		mov tableDispatch + CEE_LDARG_0 * 4, eax
		lea eax, LABEL_CEE_LDARG_1
		mov tableDispatch + CEE_LDARG_1 * 4, eax
		lea eax, LABEL_CEE_LDARG_2
		mov tableDispatch + CEE_LDARG_2 * 4, eax
		lea eax, LABEL_CEE_LDARG_3
		mov tableDispatch + CEE_LDARG_3 * 4, eax
		lea eax, LABEL_CEE_LDLOC_0
		mov tableDispatch + CEE_LDLOC_0 * 4, eax
		lea eax, LABEL_CEE_LDLOC_1
		mov tableDispatch + CEE_LDLOC_1 * 4, eax
		lea eax, LABEL_CEE_LDLOC_2
		mov tableDispatch + CEE_LDLOC_2 * 4, eax
		lea eax, LABEL_CEE_LDLOC_3
		mov tableDispatch + CEE_LDLOC_3 * 4, eax
		lea eax, LABEL_CEE_STLOC_0
		mov tableDispatch + CEE_STLOC_0 * 4, eax
		lea eax, LABEL_CEE_STLOC_1
		mov tableDispatch + CEE_STLOC_1 * 4, eax
		lea eax, LABEL_CEE_STLOC_2
		mov tableDispatch + CEE_STLOC_2 * 4, eax
		lea eax, LABEL_CEE_STLOC_3
		mov tableDispatch + CEE_STLOC_3 * 4, eax
		lea eax, LABEL_CEE_LDARG_S
		mov tableDispatch + CEE_LDARG_S * 4, eax
		lea eax, LABEL_CEE_LDARGA_S
		mov tableDispatch + CEE_LDARGA_S * 4, eax
		lea eax, LABEL_CEE_STARG_S
		mov tableDispatch + CEE_STARG_S * 4, eax
		lea eax, LABEL_CEE_LDLOC_S
		mov tableDispatch + CEE_LDLOC_S * 4, eax
		lea eax, LABEL_CEE_LDLOCA_S
		mov tableDispatch + CEE_LDLOCA_S * 4, eax
		lea eax, LABEL_CEE_STLOC_S
		mov tableDispatch + CEE_STLOC_S * 4, eax
		lea eax, LABEL_CEE_LDNULL
		mov tableDispatch + CEE_LDNULL * 4, eax
		lea eax, LABEL_CEE_LDC_I4_M1
		mov tableDispatch + CEE_LDC_I4_M1 * 4, eax
		lea eax, LABEL_CEE_LDC_I4_0
		mov tableDispatch + CEE_LDC_I4_0 * 4, eax
		lea eax, LABEL_CEE_LDC_I4_1
		mov tableDispatch + CEE_LDC_I4_1 * 4, eax
		lea eax, LABEL_CEE_LDC_I4_2
		mov tableDispatch + CEE_LDC_I4_2 * 4, eax
		lea eax, LABEL_CEE_LDC_I4_3
		mov tableDispatch + CEE_LDC_I4_3 * 4, eax
		lea eax, LABEL_CEE_LDC_I4_4
		mov tableDispatch + CEE_LDC_I4_4 * 4, eax
		lea eax, LABEL_CEE_LDC_I4_5
		mov tableDispatch + CEE_LDC_I4_5 * 4, eax
		lea eax, LABEL_CEE_LDC_I4_6
		mov tableDispatch + CEE_LDC_I4_6 * 4, eax
		lea eax, LABEL_CEE_LDC_I4_7
		mov tableDispatch + CEE_LDC_I4_7 * 4, eax
		lea eax, LABEL_CEE_LDC_I4_8
		mov tableDispatch + CEE_LDC_I4_8 * 4, eax
		lea eax, LABEL_CEE_LDC_I4_S
		mov tableDispatch + CEE_LDC_I4_S * 4, eax
		lea eax, LABEL_CEE_LDC_I4
		mov tableDispatch + CEE_LDC_I4 * 4, eax
		lea eax, LABEL_CEE_LDC_I8
		mov tableDispatch + CEE_LDC_I8 * 4, eax
		lea eax, LABEL_CEE_LDC_R4
		mov tableDispatch + CEE_LDC_R4 * 4, eax
		lea eax, LABEL_CEE_LDC_R8
		mov tableDispatch + CEE_LDC_R8 * 4, eax
		lea eax, LABEL_CEE_DUP
		mov tableDispatch + CEE_DUP * 4, eax
		lea eax, LABEL_CEE_POP
		mov tableDispatch + CEE_POP * 4, eax
		lea eax, LABEL_CEE_JMP
		mov tableDispatch + CEE_JMP * 4, eax
		lea eax, LABEL_CEE_CALL
		mov tableDispatch + CEE_CALL * 4, eax
		lea eax, LABEL_CEE_CALLI
		mov tableDispatch + CEE_CALLI * 4, eax
		lea eax, LABEL_CEE_RET
		mov tableDispatch + CEE_RET * 4, eax
		lea eax, LABEL_CEE_BR_S
		mov tableDispatch + CEE_BR_S * 4, eax
		lea eax, LABEL_CEE_BRFALSE_S
		mov tableDispatch + CEE_BRFALSE_S * 4, eax
		lea eax, LABEL_CEE_BRTRUE_S
		mov tableDispatch + CEE_BRTRUE_S * 4, eax
		lea eax, LABEL_CEE_BEQ_S
		mov tableDispatch + CEE_BEQ_S * 4, eax
		lea eax, LABEL_CEE_BGE_S
		mov tableDispatch + CEE_BGE_S * 4, eax
		lea eax, LABEL_CEE_BGT_S
		mov tableDispatch + CEE_BGT_S * 4, eax
		lea eax, LABEL_CEE_BLE_S
		mov tableDispatch + CEE_BLE_S * 4, eax
		lea eax, LABEL_CEE_BLT_S
		mov tableDispatch + CEE_BLT_S * 4, eax
		lea eax, LABEL_CEE_BNE_UN_S
		mov tableDispatch + CEE_BNE_UN_S * 4, eax
		lea eax, LABEL_CEE_BGE_UN_S
		mov tableDispatch + CEE_EXT_BNGE_S * 4, eax
		lea eax, LABEL_CEE_BGT_UN_S
		mov tableDispatch + CEE_EXT_BNGT_S * 4, eax
		lea eax, LABEL_CEE_BLE_UN_S
		mov tableDispatch + CEE_EXT_BNLE_S * 4, eax
		lea eax, LABEL_CEE_BLT_UN_S
		mov tableDispatch + CEE_EXT_BNLT_S * 4, eax
		lea eax, LABEL_CEE_BR
		mov tableDispatch + CEE_BR * 4, eax
		lea eax, LABEL_CEE_BRFALSE
		mov tableDispatch + CEE_BRFALSE * 4, eax
		lea eax, LABEL_CEE_BRTRUE
		mov tableDispatch + CEE_BRTRUE * 4, eax
		lea eax, LABEL_CEE_BEQ
		mov tableDispatch + CEE_BEQ * 4, eax
		lea eax, LABEL_CEE_BGE
		mov tableDispatch + CEE_BGE * 4, eax
		lea eax, LABEL_CEE_BGT
		mov tableDispatch + CEE_BGT * 4, eax
		lea eax, LABEL_CEE_BLE
		mov tableDispatch + CEE_BLE * 4, eax
		lea eax, LABEL_CEE_BLT
		mov tableDispatch + CEE_BLT * 4, eax
		lea eax, LABEL_CEE_BNE_UN
		mov tableDispatch + CEE_BNE_UN * 4, eax
		lea eax, LABEL_CEE_BGE_UN
		mov tableDispatch + CEE_EXT_BNGE * 4, eax
		lea eax, LABEL_CEE_BGT_UN
		mov tableDispatch + CEE_EXT_BNGT * 4, eax
		lea eax, LABEL_CEE_BLE_UN
		mov tableDispatch + CEE_EXT_BNLE * 4, eax
		lea eax, LABEL_CEE_BLT_UN
		mov tableDispatch + CEE_EXT_BNLT * 4, eax
		lea eax, LABEL_CEE_SWITCH
		mov tableDispatch + CEE_SWITCH * 4, eax
		lea eax, LABEL_CEE_LDIND_I1
		mov tableDispatch + CEE_LDIND_I1 * 4, eax
		lea eax, LABEL_CEE_LDIND_U1
		mov tableDispatch + CEE_LDIND_U1 * 4, eax
		lea eax, LABEL_CEE_LDIND_I2
		mov tableDispatch + CEE_LDIND_I2 * 4, eax
		lea eax, LABEL_CEE_LDIND_U2
		mov tableDispatch + CEE_LDIND_U2 * 4, eax
		lea eax, LABEL_CEE_LDIND_I4
		mov tableDispatch + CEE_LDIND_I4 * 4, eax
		lea eax, LABEL_CEE_LDIND_U4
		mov tableDispatch + CEE_LDIND_U4 * 4, eax
		lea eax, LABEL_CEE_LDIND_I8
		mov tableDispatch + CEE_LDIND_I8 * 4, eax
		lea eax, LABEL_CEE_LDIND_I
		mov tableDispatch + CEE_LDIND_I * 4, eax
		lea eax, LABEL_CEE_LDIND_R4
		mov tableDispatch + CEE_LDIND_R4 * 4, eax
		lea eax, LABEL_CEE_LDIND_R8
		mov tableDispatch + CEE_LDIND_R8 * 4, eax
		lea eax, LABEL_CEE_LDIND_REF
		mov tableDispatch + CEE_LDIND_REF * 4, eax
		lea eax, LABEL_CEE_STIND_REF
		mov tableDispatch + CEE_STIND_REF * 4, eax
		lea eax, LABEL_CEE_STIND_I1
		mov tableDispatch + CEE_STIND_I1 * 4, eax
		lea eax, LABEL_CEE_STIND_I2
		mov tableDispatch + CEE_STIND_I2 * 4, eax
		lea eax, LABEL_CEE_STIND_I4
		mov tableDispatch + CEE_STIND_I4 * 4, eax
		lea eax, LABEL_CEE_STIND_I8
		mov tableDispatch + CEE_STIND_I8 * 4, eax
		lea eax, LABEL_CEE_STIND_R4
		mov tableDispatch + CEE_STIND_R4 * 4, eax
		lea eax, LABEL_CEE_STIND_R8
		mov tableDispatch + CEE_STIND_R8 * 4, eax
		lea eax, LABEL_CEE_ADD
		mov tableDispatch + CEE_ADD * 4, eax
		lea eax, LABEL_CEE_SUB
		mov tableDispatch + CEE_SUB * 4, eax
		lea eax, LABEL_CEE_MUL
		mov tableDispatch + CEE_MUL * 4, eax
		lea eax, LABEL_CEE_DIV
		mov tableDispatch + CEE_DIV * 4, eax
		lea eax, LABEL_CEE_DIV_UN
		mov tableDispatch + CEE_DIV_UN * 4, eax
		lea eax, LABEL_CEE_REM
		mov tableDispatch + CEE_REM * 4, eax
		lea eax, LABEL_CEE_REM_UN
		mov tableDispatch + CEE_REM_UN * 4, eax
		lea eax, LABEL_CEE_AND
		mov tableDispatch + CEE_AND * 4, eax
		lea eax, LABEL_CEE_OR
		mov tableDispatch + CEE_OR * 4, eax
		lea eax, LABEL_CEE_XOR
		mov tableDispatch + CEE_XOR * 4, eax
		lea eax, LABEL_CEE_SHL
		mov tableDispatch + CEE_SHL * 4, eax
		lea eax, LABEL_CEE_SHR
		mov tableDispatch + CEE_SHR * 4, eax
		lea eax, LABEL_CEE_SHR_UN
		mov tableDispatch + CEE_SHR_UN * 4, eax
		lea eax, LABEL_CEE_NEG
		mov tableDispatch + CEE_NEG * 4, eax
		lea eax, LABEL_CEE_NOT
		mov tableDispatch + CEE_NOT * 4, eax
		lea eax, LABEL_CEE_CONV_I1
		mov tableDispatch + CEE_CONV_I1 * 4, eax
		lea eax, LABEL_CEE_CONV_I2
		mov tableDispatch + CEE_CONV_I2 * 4, eax
		lea eax, LABEL_CEE_CONV_I4
		mov tableDispatch + CEE_CONV_I4 * 4, eax
		lea eax, LABEL_CEE_CONV_I8
		mov tableDispatch + CEE_CONV_I8 * 4, eax
		lea eax, LABEL_CEE_CONV_R4
		mov tableDispatch + CEE_CONV_R4 * 4, eax
		lea eax, LABEL_CEE_CONV_R8
		mov tableDispatch + CEE_CONV_R8 * 4, eax
		lea eax, LABEL_CEE_CONV_U4
		mov tableDispatch + CEE_CONV_U4 * 4, eax
		lea eax, LABEL_CEE_CONV_U8
		mov tableDispatch + CEE_CONV_U8 * 4, eax
		lea eax, LABEL_CEE_CALLVIRT
		mov tableDispatch + CEE_CALLVIRT * 4, eax
		lea eax, LABEL_CEE_CPOBJ
		mov tableDispatch + CEE_CPOBJ * 4, eax
		lea eax, LABEL_CEE_LDOBJ
		mov tableDispatch + CEE_LDOBJ * 4, eax
		lea eax, LABEL_CEE_LDSTR
		mov tableDispatch + CEE_LDSTR * 4, eax
		lea eax, LABEL_CEE_NEWOBJ
		mov tableDispatch + CEE_NEWOBJ * 4, eax
		lea eax, LABEL_CEE_CASTCLASS
		mov tableDispatch + CEE_CASTCLASS * 4, eax
		lea eax, LABEL_CEE_ISINST
		mov tableDispatch + CEE_ISINST * 4, eax
		lea eax, LABEL_CEE_CONV_R_UN
		mov tableDispatch + CEE_CONV_R_UN * 4, eax
		lea eax, LABEL_CEE_UNBOX
		mov tableDispatch + CEE_UNBOX * 4, eax
		lea eax, LABEL_CEE_THROW
		mov tableDispatch + CEE_THROW * 4, eax
		lea eax, LABEL_CEE_LDFLD
		mov tableDispatch + CEE_LDFLD * 4, eax
		lea eax, LABEL_CEE_LDFLDA
		mov tableDispatch + CEE_LDFLDA * 4, eax
		lea eax, LABEL_CEE_STFLD
		mov tableDispatch + CEE_STFLD * 4, eax
		lea eax, LABEL_CEE_LDSFLD
		mov tableDispatch + CEE_LDSFLD * 4, eax
		lea eax, LABEL_CEE_LDSFLDA
		mov tableDispatch + CEE_LDSFLDA * 4, eax
		lea eax, LABEL_CEE_STSFLD
		mov tableDispatch + CEE_STSFLD * 4, eax
		lea eax, LABEL_CEE_STOBJ
		mov tableDispatch + CEE_STOBJ * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_I1_UN
		mov tableDispatch + CEE_CONV_OVF_I1_UN * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_I2_UN
		mov tableDispatch + CEE_CONV_OVF_I2_UN * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_I4_UN
		mov tableDispatch + CEE_CONV_OVF_I4_UN * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_I8_UN
		mov tableDispatch + CEE_CONV_OVF_I8_UN * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_U1_UN
		mov tableDispatch + CEE_CONV_OVF_U1_UN * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_U2_UN
		mov tableDispatch + CEE_CONV_OVF_U2_UN * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_U4_UN
		mov tableDispatch + CEE_CONV_OVF_U4_UN * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_U8_UN
		mov tableDispatch + CEE_CONV_OVF_U8_UN * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_I_UN
		mov tableDispatch + CEE_CONV_OVF_I_UN * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_U_UN
		mov tableDispatch + CEE_CONV_OVF_U_UN * 4, eax
		lea eax, LABEL_CEE_BOX
		mov tableDispatch + CEE_BOX * 4, eax
		lea eax, LABEL_CEE_NEWARR
		mov tableDispatch + CEE_NEWARR * 4, eax
		lea eax, LABEL_CEE_LDLEN
		mov tableDispatch + CEE_LDLEN * 4, eax
		lea eax, LABEL_CEE_LDELEMA
		mov tableDispatch + CEE_LDELEMA * 4, eax
		lea eax, LABEL_CEE_LDELEM_I1
		mov tableDispatch + CEE_LDELEM_I1 * 4, eax
		lea eax, LABEL_CEE_LDELEM_U1
		mov tableDispatch + CEE_LDELEM_U1 * 4, eax
		lea eax, LABEL_CEE_LDELEM_I2
		mov tableDispatch + CEE_LDELEM_I2 * 4, eax
		lea eax, LABEL_CEE_LDELEM_U2
		mov tableDispatch + CEE_LDELEM_U2 * 4, eax
		lea eax, LABEL_CEE_LDELEM_I4
		mov tableDispatch + CEE_LDELEM_I4 * 4, eax
		lea eax, LABEL_CEE_LDELEM_U4
		mov tableDispatch + CEE_LDELEM_U4 * 4, eax
		lea eax, LABEL_CEE_LDELEM_I8
		mov tableDispatch + CEE_LDELEM_I8 * 4, eax
		lea eax, LABEL_CEE_LDELEM_I
		mov tableDispatch + CEE_LDELEM_I * 4, eax
		lea eax, LABEL_CEE_LDELEM_R4
		mov tableDispatch + CEE_LDELEM_R4 * 4, eax
		lea eax, LABEL_CEE_LDELEM_R8
		mov tableDispatch + CEE_LDELEM_R8 * 4, eax
		lea eax, LABEL_CEE_LDELEM_REF
		mov tableDispatch + CEE_LDELEM_REF * 4, eax
		lea eax, LABEL_CEE_STELEM_I
		mov tableDispatch + CEE_STELEM_I * 4, eax
		lea eax, LABEL_CEE_STELEM_I1
		mov tableDispatch + CEE_STELEM_I1 * 4, eax
		lea eax, LABEL_CEE_STELEM_I2
		mov tableDispatch + CEE_STELEM_I2 * 4, eax
		lea eax, LABEL_CEE_STELEM_I4
		mov tableDispatch + CEE_STELEM_I4 * 4, eax
		lea eax, LABEL_CEE_STELEM_I8
		mov tableDispatch + CEE_STELEM_I8 * 4, eax
		lea eax, LABEL_CEE_STELEM_R4
		mov tableDispatch + CEE_STELEM_R4 * 4, eax
		lea eax, LABEL_CEE_STELEM_R8
		mov tableDispatch + CEE_STELEM_R8 * 4, eax
		lea eax, LABEL_CEE_STELEM_REF
		mov tableDispatch + CEE_STELEM_REF * 4, eax
		lea eax, LABEL_CEE_LDELEM
		mov tableDispatch + CEE_LDELEM * 4, eax
		lea eax, LABEL_CEE_STELEM
		mov tableDispatch + CEE_STELEM * 4, eax
		lea eax, LABEL_CEE_UNBOX_ANY
		mov tableDispatch + CEE_UNBOX_ANY * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_I1
		mov tableDispatch + CEE_CONV_OVF_I1 * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_U1
		mov tableDispatch + CEE_CONV_OVF_U1 * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_I2
		mov tableDispatch + CEE_CONV_OVF_I2 * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_U2
		mov tableDispatch + CEE_CONV_OVF_U2 * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_I4
		mov tableDispatch + CEE_CONV_OVF_I4 * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_U4
		mov tableDispatch + CEE_CONV_OVF_U4 * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_I8
		mov tableDispatch + CEE_CONV_OVF_I8 * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_U8
		mov tableDispatch + CEE_CONV_OVF_U8 * 4, eax
		lea eax, LABEL_CEE_REFANYVAL
		mov tableDispatch + CEE_REFANYVAL * 4, eax
		lea eax, LABEL_CEE_CKFINITE
		mov tableDispatch + CEE_CKFINITE * 4, eax
		lea eax, LABEL_CEE_LDTOKEN
		mov tableDispatch + CEE_LDTOKEN * 4, eax
		lea eax, LABEL_CEE_CONV_U2
		mov tableDispatch + CEE_CONV_U2 * 4, eax
		lea eax, LABEL_CEE_CONV_U1
		mov tableDispatch + CEE_CONV_U1 * 4, eax
		lea eax, LABEL_CEE_CONV_I
		mov tableDispatch + CEE_CONV_I * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_I
		mov tableDispatch + CEE_CONV_OVF_I * 4, eax
		lea eax, LABEL_CEE_CONV_OVF_U
		mov tableDispatch + CEE_CONV_OVF_U * 4, eax
		lea eax, LABEL_CEE_ADD_OVF
		mov tableDispatch + CEE_ADD_OVF * 4, eax
		lea eax, LABEL_CEE_ADD_OVF_UN
		mov tableDispatch + CEE_ADD_OVF_UN * 4, eax
		lea eax, LABEL_CEE_MUL_OVF
		mov tableDispatch + CEE_MUL_OVF * 4, eax
		lea eax, LABEL_CEE_MUL_OVF_UN
		mov tableDispatch + CEE_MUL_OVF_UN * 4, eax
		lea eax, LABEL_CEE_SUB_OVF
		mov tableDispatch + CEE_SUB_OVF * 4, eax
		lea eax, LABEL_CEE_SUB_OVF_UN
		mov tableDispatch + CEE_SUB_OVF_UN * 4, eax
		lea eax, LABEL_CEE_ENDFINALLY
		mov tableDispatch + CEE_ENDFINALLY * 4, eax
		lea eax, LABEL_CEE_LEAVE
		mov tableDispatch + CEE_LEAVE * 4, eax
		lea eax, LABEL_CEE_LEAVE_S
		mov tableDispatch + CEE_LEAVE_S * 4, eax
		lea eax, LABEL_CEE_STIND_I
		mov tableDispatch + CEE_STIND_I * 4, eax
		lea eax, LABEL_CEE_CONV_U
		mov tableDispatch + CEE_CONV_U * 4, eax
		lea eax, LABEL_CEE_EXT_INCSFLD
		mov tableDispatch + CEE_EXT_INCSFLD * 4, eax
		lea eax, LABEL_CEE_EXT_INC
		mov tableDispatch + CEE_EXT_INC * 4, eax
		lea eax, LABEL_CEE_EXT_DEC
		mov tableDispatch + CEE_EXT_DEC * 4, eax
		lea eax, LABEL_CEE_EXT_CONV_B
		mov tableDispatch + CEE_EXT_CONV_B * 4, eax
		lea eax, LABEL_CEE_EXT_ENTER
		mov tableDispatch + CEE_EXT_ENTER * 4, eax
		lea eax, LABEL_CEE_EXT_LEAVE
		mov tableDispatch + CEE_EXT_LEAVE * 4, eax
		lea eax, LABEL_CEE_EXT_CALLI
		mov tableDispatch + CEE_EXT_CALLI * 4, eax
		lea eax, LABEL_CEE_EXT_STARGLIST_S
		mov tableDispatch + CEE_EXT_STARGLIST_S * 4, eax
		lea eax, LABEL_CEE_EXT_STARGLIST
		mov tableDispatch + CEE_EXT_STARGLIST * 4, eax
		lea eax, LABEL_CEE_EXT_FINALLY
		mov tableDispatch + CEE_EXT_FINALLY * 4, eax
		lea eax, LABEL_CEE_PREFIX6
		mov tableDispatch + CEE_PREFIX6 * 4, eax
		lea eax, LABEL_CEE_PREFIX5
		mov tableDispatch + CEE_PREFIX5 * 4, eax
		lea eax, LABEL_CEE_PREFIX4
		mov tableDispatch + CEE_PREFIX4 * 4, eax
		lea eax, LABEL_CEE_PREFIX3
		mov tableDispatch + CEE_PREFIX3 * 4, eax
		lea eax, LABEL_CEE_PREFIX2
		mov tableDispatch + CEE_PREFIX2 * 4, eax
		lea eax, LABEL_CEE_PREFIX1
		mov tableDispatch + CEE_PREFIX1 * 4, eax
		lea eax, LABEL_CEE_PREFIXREF
		mov tableDispatch + CEE_PREFIXREF * 4, eax
	}

	__asm {
		lea eax, LABEL_CEE_ARGLIST
		mov tableDispatchExt + (CEE_ARGLIST & 0xff) * 4, eax
		lea eax, LABEL_CEE_CEQ
		mov tableDispatchExt + (CEE_CEQ & 0xff)  * 4, eax
		lea eax, LABEL_CEE_CGT
		mov tableDispatchExt + (CEE_CGT & 0xff)  * 4, eax
		lea eax, LABEL_CEE_CGT_UN
		mov tableDispatchExt + (CEE_CGT_UN & 0xff)  * 4, eax
		lea eax, LABEL_CEE_CLT
		mov tableDispatchExt + (CEE_CLT & 0xff) * 4, eax
		lea eax, LABEL_CEE_CLT_UN
		mov tableDispatchExt + (CEE_CLT_UN & 0xff) * 4, eax
		lea eax, LABEL_CEE_LDFTN
		mov tableDispatchExt + (CEE_LDFTN & 0xff) * 4, eax
		lea eax, LABEL_CEE_LDVIRTFTN
		mov tableDispatchExt + (CEE_LDVIRTFTN & 0xff ) * 4, eax
		lea eax, LABEL_CEE_LDARG
		mov tableDispatchExt + (CEE_LDARG & 0xff ) * 4, eax
		lea eax, LABEL_CEE_LDARGA
		mov tableDispatchExt + (CEE_LDARGA & 0xff ) * 4, eax
		lea eax, LABEL_CEE_STARG
		mov tableDispatchExt + (CEE_STARG & 0xff ) * 4, eax
		lea eax, LABEL_CEE_LDLOC
		mov tableDispatchExt + (CEE_LDLOC & 0xff ) * 4, eax
		lea eax, LABEL_CEE_LDLOCA
		mov tableDispatchExt + (CEE_LDLOCA & 0xff ) * 4, eax
		lea eax, LABEL_CEE_STLOC
		mov tableDispatchExt + (CEE_STLOC & 0xff ) * 4, eax
		lea eax, LABEL_CEE_LOCALLOC
		mov tableDispatchExt + (CEE_LOCALLOC & 0xff ) * 4, eax
		lea eax, LABEL_CEE_ENDFILTER
		mov tableDispatchExt + (CEE_ENDFILTER & 0xff ) * 4, eax
		lea eax, LABEL_CEE_UNALIGNED
		mov tableDispatchExt + (CEE_UNALIGNED & 0xff ) * 4, eax
		lea eax, LABEL_CEE_VOLATILE
		mov tableDispatchExt + (CEE_VOLATILE & 0xff ) * 4, eax
		lea eax, LABEL_CEE_TAILCALL
		mov tableDispatchExt + (CEE_TAILCALL & 0xff ) * 4, eax
		lea eax, LABEL_CEE_INITOBJ
		mov tableDispatchExt + (CEE_INITOBJ & 0xff ) * 4, eax
		lea eax, LABEL_CEE_CONSTRAINED
		mov tableDispatchExt + (CEE_CONSTRAINED & 0xff ) * 4, eax
		lea eax, LABEL_CEE_CPBLK
		mov tableDispatchExt + (CEE_CPBLK & 0xff ) * 4, eax
		lea eax, LABEL_CEE_INITBLK
		mov tableDispatchExt + (CEE_INITBLK & 0xff ) * 4, eax
		lea eax, LABEL_CEE_RETHROW
		mov tableDispatchExt + (CEE_RETHROW & 0xff ) * 4, eax
		lea eax, LABEL_CEE_SIZEOF
		mov tableDispatchExt + (CEE_SIZEOF & 0xff ) * 4, eax
		lea eax, LABEL_CEE_REFANYTYPE
		mov tableDispatchExt + (CEE_REFANYTYPE & 0xff ) * 4, eax
		lea eax, LABEL_CEE_READONLY
		mov tableDispatchExt + (CEE_READONLY & 0xff ) * 4, eax
		lea eax, LABEL_CEE_EXT_CNE
		mov tableDispatchExt + (CEE_EXT_CNE & 0xff ) * 4, eax
		lea eax, LABEL_CEE_EXT_CGE
		mov tableDispatchExt + (CEE_EXT_CGE & 0xff ) * 4, eax
		lea eax, LABEL_CEE_EXT_CLE
		mov tableDispatchExt + (CEE_EXT_CLE & 0xff ) * 4, eax
	}

	m_status = VM_EXECUTE_DONE;
	bool bExecute = true;

	m_stopwatch.Start();
	
	DISPATCH();	//Kick off execution
LABEL_CEE_NOP:
	//No operation
	DISPATCH();
LABEL_CEE_BREAK:
	//If debugger is attached, do some debugger staff here.
	//No operation
	DISPATCH();
//---------------------------------------------------------------
//Exception handlers
//---------------------------------------------------------------
LABEL_CEE_EXT_ENTER:
	EnterOperator();
	DISPATCH();
LABEL_CEE_EXT_LEAVE:
	LeaveOperator();
	DISPATCH();
LABEL_CEE_EXT_FINALLY:
	FinallyOperator();	//execute Finally clause but not adjust Instruction pointer
	DISPATCH();
LABEL_CEE_THROW:
	//Take variable from eval stack and throw exception
	ThrowOperator();
	DISPATCH();
//---------------------------------------------------------------
//Type conversion
//---------------------------------------------------------------
LABEL_CEE_EXT_CONV_B:
	convertToBoolean();
	DISPATCH();
//---------------------------------------------------------------
//Branch instructions
//---------------------------------------------------------------
LABEL_CEE_CALL:
	CallOperator();
	DISPATCH();
LABEL_CEE_EXT_CALLI:
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
		DISPATCH();
	}
	RID ridCall = var.refObject->getCall();
	CVariable& varParent = var.refObject->getParentVariable();
	popEvalStack();
	CallOperator( &varParent, ridCall );
	}
	DISPATCH();
LABEL_CEE_RET:
	bExecute = RetOperator();
	if( !bExecute )
		goto LABEL_ENDPROGRAM;
	DISPATCH();
LABEL_CEE_JMP:
/*
for Jump, scope check need to be done at a compile time. Opcode works just as same as CEE_BR
*/
LABEL_CEE_BR:		//Unconditional branch
	{
		int32_t* pOffset = (int32_t*)m_pCurrentInstruction;
		m_pCurrentInstruction += *pOffset + LONGBR_ADJUST;
	}
	DISPATCH();
LABEL_CEE_BR_S:		//Unconditional branch/s
	{
		int8_t* pOffset = (int8_t*)m_pCurrentInstruction;
		m_pCurrentInstruction += *pOffset + SHORTBR_ADJUST;
	}
	DISPATCH();
LABEL_CEE_REM:
	reminderOperator();
	DISPATCH();
LABEL_CEE_BRFALSE:
	BrFalseOperator();
	DISPATCH();
LABEL_CEE_BRFALSE_S:
	BrFalseShortOperator();
	DISPATCH();
LABEL_CEE_BRTRUE:
	BrTrueOperator();
	DISPATCH();
LABEL_CEE_BRTRUE_S:
	BrTrueShortOperator();
	DISPATCH();
LABEL_CEE_BEQ:
	BeqOperator();
	DISPATCH();
LABEL_CEE_BEQ_S:
	BeqShortOperator();
	DISPATCH();
LABEL_CEE_BNE_UN:
	BneOperator();
	DISPATCH();
LABEL_CEE_BNE_UN_S:
	BneShortOperator();
	DISPATCH();
LABEL_CEE_BGE:
	BngeOperator();
	DISPATCH();
LABEL_CEE_BGE_S:
	BngeShortOperator();
	DISPATCH();
LABEL_CEE_BGT:
	BngtOperator();
	DISPATCH();
LABEL_CEE_BGT_S:
	BngtShortOperator();
	DISPATCH();
LABEL_CEE_BLE:
	BnleOperator();
	DISPATCH();
LABEL_CEE_BLE_S:
	BnleShortOperator();
	DISPATCH();
LABEL_CEE_BLT:
	BnltOperator();
	DISPATCH();
LABEL_CEE_BLT_S:
	BnltShortOperator();
	DISPATCH();
LABEL_CEE_BGE_UN:
	BngeOperator();
	DISPATCH();
LABEL_CEE_BGE_UN_S:
	BngeShortOperator();
	DISPATCH();
LABEL_CEE_BGT_UN:
	BngtOperator();
	DISPATCH();
LABEL_CEE_BGT_UN_S:
	BngtShortOperator();
	DISPATCH();
LABEL_CEE_BLE_UN:
	BnleOperator();
	DISPATCH();
LABEL_CEE_BLE_UN_S:
	BnleShortOperator();
	DISPATCH();
LABEL_CEE_BLT_UN:
	BnltOperator();
	DISPATCH();
LABEL_CEE_BLT_UN_S:
	BnltShortOperator();
	DISPATCH();
//---------------------------------------------------------------
//Stack management
//---------------------------------------------------------------
LABEL_CEE_DUP:
	assert( isEvalStackEmpty() );
	dupEvalStack();
	DISPATCH();
LABEL_CEE_POP:
	assert( isEvalStackEmpty() );
	popEvalStack();
	DISPATCH();
//---------------------------------------------------------------
//Object management
//---------------------------------------------------------------
LABEL_CEE_NEWOBJ:
	CreateNewObject();
	DISPATCH();
LABEL_CEE_LDSTR:
	CreateNewStringValue();
	DISPATCH();
//---------------------------------------------------------------
//Load/store instructions
//---------------------------------------------------------------
LABEL_CEE_LDNULL:
	//Load NULL to eval stack
	pushEvalStackNull();
	DISPATCH();
LABEL_CEE_LDC_I4:
	{
		int32_t i = getNextInt32();
		pushEvalStack( i );
	}
	DISPATCH();
LABEL_CEE_LDC_I4_S:
	{
		int8_t c = getNextInt8();
		pushEvalStack( (int32_t)c );
	}
	DISPATCH();
LABEL_CEE_LDC_I4_M1:
	pushEvalStack( -1 );
	DISPATCH();
LABEL_CEE_LDC_I4_0:
	pushEvalStack( 0 );
	DISPATCH();
LABEL_CEE_LDC_I4_1:
	pushEvalStack( 1 );
	DISPATCH();
LABEL_CEE_LDC_I4_2:
	pushEvalStack( 2 );
	DISPATCH();
LABEL_CEE_LDC_I4_3:
	pushEvalStack( 3 );
	DISPATCH();
LABEL_CEE_LDC_I4_4:
	pushEvalStack( 4 );
	DISPATCH();
LABEL_CEE_LDC_I4_5:
	pushEvalStack( 5 );
	DISPATCH();
LABEL_CEE_LDC_I4_6:
	pushEvalStack( 6 );
	DISPATCH();
LABEL_CEE_LDC_I4_7:
	pushEvalStack( 7 );
	DISPATCH();
LABEL_CEE_LDC_I4_8:
	pushEvalStack( 8 );
	DISPATCH();
LABEL_CEE_LDC_I8:
	{
		int64_t i = getNextInt64();
		pushEvalStack( i );
	}
	DISPATCH();
LABEL_CEE_LDC_R4:
	{
		float f = getNextFloat();
		pushEvalStack( f );
	}
	DISPATCH();
LABEL_CEE_LDC_R8:
	{
		double d = getNextDouble();
		pushEvalStack( d );
	}
	DISPATCH();
LABEL_CEE_LDSFLD:
	loadStaticField( getNextToken() );
	DISPATCH();
LABEL_CEE_STSFLD:
	storeStaticField( getNextToken() );
	DISPATCH();
LABEL_CEE_LDLOC_0:
	loadLocalVariable( 0 );
	DISPATCH();
LABEL_CEE_LDLOC_1:
	loadLocalVariable( 1 );
	DISPATCH();
LABEL_CEE_LDLOC_2:
	loadLocalVariable( 2 );
	DISPATCH();
LABEL_CEE_LDLOC_3:
	loadLocalVariable( 3 );
	DISPATCH();
LABEL_CEE_LDLOC_S:
	loadLocalVariable( getNextInt8() );
	DISPATCH();
LABEL_CEE_STLOC_0:
	storeLocalVariable( 0 );
	DISPATCH();
LABEL_CEE_STLOC_1:
	storeLocalVariable( 1 );
	DISPATCH();
LABEL_CEE_STLOC_2:
	storeLocalVariable( 2 );
	DISPATCH();
LABEL_CEE_STLOC_3:
	storeLocalVariable( 3 );
	DISPATCH();
LABEL_CEE_STLOC_S:
	storeLocalVariable( getNextInt8() );
	DISPATCH();
//---------------------------------------------------------------
//Arithmetical instructions
//---------------------------------------------------------------

LABEL_CEE_EXT_INCSFLD:
	incStaticFieldOperator( getNextToken() );
	DISPATCH();
LABEL_CEE_EXT_INC:
	incOperator();
	DISPATCH();
LABEL_CEE_EXT_DEC:
	decOperator();
	DISPATCH();
LABEL_CEE_ADD:
	addOperator();
	DISPATCH();
LABEL_CEE_SUB:
	subtractOperator();
	DISPATCH();
LABEL_CEE_MUL:
	multiplyOperator();
	DISPATCH();
LABEL_CEE_DIV:
	divisionOperator();
	DISPATCH();
LABEL_CEE_AND:
	bitwiseAndOperator();
	DISPATCH();
LABEL_CEE_OR:
	bitwiseOrOperator();
	DISPATCH();
LABEL_CEE_XOR:
	bitwiseXorOperator();
	DISPATCH();
LABEL_CEE_NOT:
	bitwiseNotOperator();
	DISPATCH();
LABEL_CEE_SHL:
	ShlOperator();
	DISPATCH();
LABEL_CEE_SHR:
	ShrOperator();
	DISPATCH();
LABEL_CEE_SHR_UN:
	ShrUnOperator();
	DISPATCH();
LABEL_CEE_NEG:
	negOperator();
	DISPATCH();
//---------------------------------------------------------------
//Argument list related one
//---------------------------------------------------------------
LABEL_CEE_EXT_STARGLIST_S:
	storeArgumentListWithThisPointer( getNextUint8() );
	DISPATCH();
LABEL_CEE_EXT_STARGLIST:
	storeArgumentListWithThisPointer( getNextInt32() );
	DISPATCH();
LABEL_CEE_LDARG_0:
	//Dedicated to load this pointer
	loadArgument( 0 );
	DISPATCH();
LABEL_CEE_LDARG_1:
	loadArgument( 1 );
	DISPATCH();
LABEL_CEE_LDARG_2:
	loadArgument( 2 );
	DISPATCH();
LABEL_CEE_LDARG_3:
	loadArgument( 3 );
	DISPATCH();
LABEL_CEE_LDARG_S:
	loadArgument( getNextInt8() );
	DISPATCH();
LABEL_CEE_STARG_S:
	storeArgument( getNextInt8() );
	DISPATCH();
//---------------------------------------------------------------
//Not implemented
//---------------------------------------------------------------
LABEL_CEE_CALLI:
LABEL_CEE_SWITCH:
LABEL_CEE_LDARGA_S:
LABEL_CEE_LDLOCA_S:
LABEL_CEE_LDIND_I1:
LABEL_CEE_LDIND_U1:
LABEL_CEE_LDIND_I2:
LABEL_CEE_LDIND_U2:
LABEL_CEE_LDIND_I4:
LABEL_CEE_LDIND_U4:
LABEL_CEE_LDIND_I8:
LABEL_CEE_LDIND_I:
LABEL_CEE_LDIND_R4:
LABEL_CEE_LDIND_R8:
LABEL_CEE_LDIND_REF:
LABEL_CEE_STIND_REF:
LABEL_CEE_STIND_I1:
LABEL_CEE_STIND_I2:
LABEL_CEE_STIND_I4:
LABEL_CEE_STIND_I8:
LABEL_CEE_STIND_R4:
LABEL_CEE_STIND_R8:
LABEL_CEE_DIV_UN:
LABEL_CEE_REM_UN:
LABEL_CEE_CONV_I1:
LABEL_CEE_CONV_I2:
LABEL_CEE_CONV_I4:
LABEL_CEE_CONV_I8:
LABEL_CEE_CONV_R4:
LABEL_CEE_CONV_R8:
LABEL_CEE_CONV_U4:
LABEL_CEE_CONV_U8:
LABEL_CEE_CALLVIRT:
LABEL_CEE_CPOBJ:
LABEL_CEE_LDOBJ:
LABEL_CEE_CASTCLASS:
LABEL_CEE_ISINST:
LABEL_CEE_CONV_R_UN:
LABEL_CEE_UNBOX:
LABEL_CEE_LDFLD:
LABEL_CEE_LDFLDA:
LABEL_CEE_STFLD:
LABEL_CEE_LDSFLDA:
LABEL_CEE_STOBJ:
LABEL_CEE_CONV_OVF_I1_UN:
LABEL_CEE_CONV_OVF_I2_UN:
LABEL_CEE_CONV_OVF_I4_UN:
LABEL_CEE_CONV_OVF_I8_UN:
LABEL_CEE_CONV_OVF_U1_UN:
LABEL_CEE_CONV_OVF_U2_UN:
LABEL_CEE_CONV_OVF_U4_UN:
LABEL_CEE_CONV_OVF_U8_UN:
LABEL_CEE_CONV_OVF_I_UN:
LABEL_CEE_CONV_OVF_U_UN:
LABEL_CEE_BOX:
LABEL_CEE_NEWARR:
LABEL_CEE_LDLEN:
LABEL_CEE_LDELEMA:
LABEL_CEE_LDELEM_I1:
LABEL_CEE_LDELEM_U1:
LABEL_CEE_LDELEM_I2:
LABEL_CEE_LDELEM_U2:
LABEL_CEE_LDELEM_I4:
LABEL_CEE_LDELEM_U4:
LABEL_CEE_LDELEM_I8:
LABEL_CEE_LDELEM_I:
LABEL_CEE_LDELEM_R4:
LABEL_CEE_LDELEM_R8:
LABEL_CEE_LDELEM_REF:
LABEL_CEE_STELEM_I:
LABEL_CEE_STELEM_I1:
LABEL_CEE_STELEM_I2:
LABEL_CEE_STELEM_I4:
LABEL_CEE_STELEM_I8:
LABEL_CEE_STELEM_R4:
LABEL_CEE_STELEM_R8:
LABEL_CEE_STELEM_REF:
LABEL_CEE_LDELEM:
LABEL_CEE_STELEM:
LABEL_CEE_UNBOX_ANY:
LABEL_CEE_CONV_OVF_I1:
LABEL_CEE_CONV_OVF_U1:
LABEL_CEE_CONV_OVF_I2:
LABEL_CEE_CONV_OVF_U2:
LABEL_CEE_CONV_OVF_I4:
LABEL_CEE_CONV_OVF_U4:
LABEL_CEE_CONV_OVF_I8:
LABEL_CEE_CONV_OVF_U8:
LABEL_CEE_REFANYVAL:
LABEL_CEE_CKFINITE:
//LABEL_CEE_MKREFANY:
LABEL_CEE_LDTOKEN:
LABEL_CEE_CONV_U2:
LABEL_CEE_CONV_U1:
LABEL_CEE_CONV_I:
LABEL_CEE_CONV_OVF_I:
LABEL_CEE_CONV_OVF_U:
LABEL_CEE_ADD_OVF:
LABEL_CEE_ADD_OVF_UN:
LABEL_CEE_MUL_OVF:
LABEL_CEE_MUL_OVF_UN:
LABEL_CEE_SUB_OVF:
LABEL_CEE_SUB_OVF_UN:
LABEL_CEE_ENDFINALLY:
LABEL_CEE_LEAVE:
LABEL_CEE_LEAVE_S:
LABEL_CEE_STIND_I:
LABEL_CEE_CONV_U:
LABEL_CEE_PREFIX6:
LABEL_CEE_PREFIX5:
LABEL_CEE_PREFIX4:
LABEL_CEE_PREFIX3:
LABEL_CEE_PREFIX2:
LABEL_CEE_PREFIXREF:
	Debug_Fatal( "Not implemented yet" );
	DISPATCH();
LABEL_CEE_PREFIX1:	//2 bytes instructions
	DISPATCH2();
//---------------------------------------------------------------
//2bytes instructions
//---------------------------------------------------------------
//---------------------------------------------------------------
//Compare
//---------------------------------------------------------------
LABEL_CEE_CEQ:
	CeqOperator();
	DISPATCH();
LABEL_CEE_EXT_CNE:
	CneOperator();
	DISPATCH();
LABEL_CEE_CGT:
	CgtOperator();
	DISPATCH();
LABEL_CEE_CLT:
	CltOperator();
	DISPATCH();
LABEL_CEE_EXT_CGE:
	CgeOperator();
	DISPATCH();
LABEL_CEE_EXT_CLE:
	CleOperator();
	DISPATCH();
LABEL_CEE_CGT_UN:
LABEL_CEE_CLT_UN:
	DISPATCH();
LABEL_CEE_LDLOC:
	loadLocalVariable( getNextInt32() );
	DISPATCH();
LABEL_CEE_STLOC:
	storeLocalVariable( getNextInt32() );
	DISPATCH();
LABEL_CEE_LDARG:
	loadArgument( getNextInt32() );
	DISPATCH();
LABEL_CEE_STARG:
	storeArgument( getNextInt32() );
	DISPATCH();
//---------------------------------------------------------------
//Not implemented
//---------------------------------------------------------------
LABEL_CEE_ARGLIST:
LABEL_CEE_LDFTN:
LABEL_CEE_LDVIRTFTN:
LABEL_CEE_LDARGA:
LABEL_CEE_LDLOCA:
LABEL_CEE_LOCALLOC:
LABEL_CEE_ENDFILTER:
LABEL_CEE_UNALIGNED:
LABEL_CEE_VOLATILE:
LABEL_CEE_TAILCALL:
LABEL_CEE_INITOBJ:
LABEL_CEE_CONSTRAINED:
LABEL_CEE_CPBLK:
LABEL_CEE_INITBLK:
LABEL_CEE_RETHROW:
LABEL_CEE_SIZEOF:
LABEL_CEE_REFANYTYPE:
LABEL_CEE_READONLY:
	Debug_Fatal( "Not implemented yet" );
	DISPATCH();

LABEL_ENDPROGRAM:
	m_stopwatch.Stop();
	m_status = VM_EXECUTE_DONE;

	return m_status;
}
#endif

} //namespace CRI