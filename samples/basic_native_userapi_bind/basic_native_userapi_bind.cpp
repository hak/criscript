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
 * Desc.    : Demonstrates how to bind a native user function to the scripting engine
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
	var v = TestAPI( 1.0 );\
	print( v );\
";

//----------------------------------------------------------------
//test API, invoked from the script side
//----------------------------------------------------------------
void Test( cri::CCilVm* const pVm,
			const int32_t iNumArguments, 
			CVariable* pArguments )
{
	//Skip "this" value
	pArguments ++;
	
	//Get Argument value
	cri::CVariable var = *pArguments;
	
	//Convert value to float
	var.toFloat();

	//Push a value to EvalStack
	pVm->pushEvalStack( var.fValue * 2.0f );	
	return;
}

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
	//Register my API
	//----------------------------------------------------------------
	codegen->registerBuiltinMethod( wstring( L"TestAPI" ),			//Function Name
								cri::METHOD_NATIVE,				//Method type
								cri::RID_BUILTINPARAM_DEFAULT_ONE );//Number of Parameters

	//----------------------------------------------------------------
	//Set filestream to the lexer
	//----------------------------------------------------------------
	lex->setStream( & in );

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
		//Bind my API
		//----------------------------------------------------------------

		//API binding should be done after binding Metadata.
		//Since the binding information is in the Metadata
		vm->bindBuiltinMethod( wstring( L"TestAPI" ),
			(cri::PMETHOD)Test,
			cri::RID_BUILTINPARAM_DEFAULT_ONE
			);

		//----------------------------------------------------------------
		//execute the script
		//----------------------------------------------------------------
		iRet = vm->execute();
		iRet = vm->getExitCode(); 

		delete( vm );
	}

	delete( error );
	delete( codegen );
	delete( lex );
	delete( parser );
	return iRet;
}
