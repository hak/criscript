// CRIScriptTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CRIScript.h"

int32_t _tmain(int32_t argc, _TCHAR* argv[])
{

	int32_t iRet = 0;
	if( argc == 1 )
	{
		//Output help info
		cout << "CRIScript Test" << endl;
        return 1;
	}
	int32_t i=0;
	ifstream fin ( argv[ 1 ], ios::in | ios::binary );
    if (!fin){
		cout << "File Open Error: " << argv[ 1 ] << endl;
        return 1;
    }
	//Check file endianness;
	wchar_t c;
	fin.read( (int8_t*)&c, sizeof( wchar_t ) );
	if( c != 0xfeff )
	{
		cout << "File format not supported!!" << endl;
        return 1;
	}

	cri::CParser* parser = new cri::CParser;
	CRISCRIPT_VERSIONS ver = parser->GetVersions();
	cout << "Parser version: ";
	cout << ver.major << ".";
	cout << ver.minor << ".";
	cout << ver.revision << ".";
	cout << ver.build << "\n";
	
	cri::CFileStreamLexer *lex = new cri::CFileStreamLexer;
	cri::CCilCodeGen *codegen = new cri::CCilCodeGen;
	cri::CErrorPool *error = new cri::CErrorPool;
	error->SetErrorTable( cri::g_ErrorTable_English, sizeof( cri::g_ErrorTable_English ) 
													 / sizeof( cri::g_ErrorTable_English[ 0 ] ) );

	parser->Init( (CLexer*)lex, codegen, error );
	
	//Set filestream to the lexer
	lex->SetStream( & fin );


#ifdef DEBUG_PARSING
	//debug
	cri::yydebug = 1;
#endif
	i = parser->Process();

	fin.close();

	if( error->HasError() )
	{
		error->DumpError();
		iRet = 1;
	}
	else
	{
		if( error->HasInformation() )
			error->DumpError();
		cri::CCilVm *vm = new cri::CCilVm;
		cri::CCilDebugger *debugger = new cri::CCilDebugger;

		vm->SetILPool( parser->GetILPool() );
		//Note:
		//SetSymbolInformation need to be invoked before SetMetaData() for now
		vm->SetSymbolInformation( parser->GetSymbolInformation() );
		vm->SetMetaData( parser->GetMetaData() );

		cout << "-----------------------------------------------------------------\n";
		cri::CCilDisasm * disasm = new cri::CCilDisasm;
		disasm->SetILStream( parser->GetILPool() );
		disasm->SetSymbolInformation( parser->GetSymbolInformation() );
		disasm->SetMetaData( parser->GetMetaData() );
		disasm->DumpDisasm();
		cout << "-----------------------------------------------------------------\n";

		iRet = vm->Execute();
		iRet = vm->GetExitCode(); 

		debugger->Attach( vm );
		debugger->SetSymbolInformation( parser->GetSymbolInformation() );
		debugger->SetMetaData( parser->GetMetaData() );
		
		debugger->DebugDumpStats();
		debugger->DebugDumpStaticFields( 0 );	//Dump all fields
		debugger->DebugDumpObjects( 0 );		//Dump all objects

		delete( disasm );
		delete( debugger );
		delete( vm );
	}


	delete( error );
	delete( codegen );
	return iRet;
}

