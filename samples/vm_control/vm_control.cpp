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
 * Sample   : CRIScript 'vm_control'
 * Desc.    : Demonstrates how to use a vm control APIs of CRIScript compiler and VM
 * File     :
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#include "stdafx.h"
#include "CRIScript.h"
#include <sstream>

//----------------------------------------------------------------
//Script body. Need to be in UTF16
//----------------------------------------------------------------
//print a text
wchar_t strScript[] = L"\
	print( \"Hello World\" );\
";


int _tmain(int argc, _TCHAR* argv[])
{
	int32_t iRet = 0;

	//Open a script file in memory
	istringstream in ( string( (char*)strScript, sizeof( strScript ) ), ios::binary );
    if (!in){
		cout << "Stream Open Error"<< endl;
        return 1;
    }

	//----------------------------------------------------------------
	//Initialize a parser
	//----------------------------------------------------------------
	cri::CParser* parser = new cri::CParser;
	cri::CFileStreamLexer *lex = new cri::CFileStreamLexer;
	cri::CCilCodeGen *codegen = new cri::CCilCodeGen;
	cri::CErrorPool *error = new cri::CErrorPool;
	parser->init( lex, codegen, error );
	
	//----------------------------------------------------------------
	//Set filestream to the lexer
	//----------------------------------------------------------------
	lex->setStream( & in );

	//----------------------------------------------------------------
	//Compile the  script
	//----------------------------------------------------------------
	iRet = parser->process();			//Compile the script

	if( error->hasError() )
	{
		error->dumpError();
		iRet = 1;
	}
	else
	{
		if( error->hasInformation() )
			error->dumpError();

		//----------------------------------------------------------------
		//Initialize the VM
		//----------------------------------------------------------------
		cri::CCilVm *vm = new cri::CCilVm;

		vm->setILPool( parser->getILPool() );	//Set compiled byte code stream

		//Set metadata and debug information to VM
		vm->setSymbolInformation( parser->getSymbolInformation() );
		vm->setMetaData( parser->getMetaData() );

		//----------------------------------------------------------------
		//Execute the script
		//----------------------------------------------------------------
		iRet = vm->execute();		//execute the script
		iRet = vm->getExitCode();	//retrieve an exit code

		//----------------------------------------------------------------
		//Reset the VM
		//----------------------------------------------------------------
		vm->reset( VM_RESET_DEFAULT );					//reset the vm
		//Now the VM is reset and ready to execute the script again
		
		iRet = vm->execute();		//execute the script
		iRet = vm->getExitCode();	//retrieve an exit code


		delete( vm );	//delete VM instance
	}

	delete( error );	//delete error pool instance
	delete( codegen );	//delete code gen instance
	delete( lex );		//delete lexer
	delete( parser );
	return iRet;
}
