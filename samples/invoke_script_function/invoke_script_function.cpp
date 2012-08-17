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
 * Sample   : CRIScript basic native userapi bind
 * Desc.    : Demonstrates how to invoke a script function from C++ code
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
wchar_t strScript[] = L"\
	print( \"CRIScript\" );\
	var varCounter = 0;\
	function Test() {\
		print( \"Invoked from the C++ code \" + varCounter++ );\
	}\
";

//----------------------------------------------------------------
//main
//----------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	int32_t iRet = 0;

	//Open the script file on memory
	istringstream in ( string( (char*)strScript, sizeof( strScript ) ), ios::binary );
    if (!in){
		cout << "Stream Open Error"<< endl;
        return 1;
    }

	//----------------------------------------------------------------
	//Initialize a parser
	//----------------------------------------------------------------
	cri::CParser *parser = new cri::CParser;
	cri::CFileStreamLexer *lex = new cri::CFileStreamLexer;
	cri::CCilCodeGen *codegen = new cri::CCilCodeGen;
	cri::CErrorPool *error = new cri::CErrorPool;
	parser->init( lex, codegen, error );
	
	//----------------------------------------------------------------
	//Set filestream to the lexer
	//----------------------------------------------------------------
	lex->setStream( (istream*)&in );

	//----------------------------------------------------------------
	//Compile the  script
	//----------------------------------------------------------------
	iRet = parser->process();

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

		//----------------------------------------------------------------
		//Bind IL and meta data
		//----------------------------------------------------------------
		vm->setILPool( parser->getILPool() );
		vm->setSymbolInformation( parser->getSymbolInformation() );
		vm->setMetaData( parser->getMetaData() );

		//----------------------------------------------------------------
		//execute the script
		//----------------------------------------------------------------
		iRet = vm->execute();
		iRet = vm->getExitCode(); 

		//----------------------------------------------------------------
		//Invoke the Script method
		//----------------------------------------------------------------
		CG_SYMBOL_INFORMATION info = vm->querySymbolInformation( wstring( L"Test" ) );
		CVariable var;
		vm->invokeMethod( info.rid, 0, &var );

		vm->invokeMethod( info.rid, 0, &var );

		vm->invokeMethod( info.rid, 0, &var );

		delete( vm );
	}

	delete( error );
	delete( codegen );
	delete( lex );
	delete( parser );
	return iRet;
}
