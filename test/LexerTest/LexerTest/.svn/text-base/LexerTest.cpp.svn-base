// CRIScriptTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CRIScript.h"

//For Test Porpose only
cri::CParser g_Parser;

int _tmain(int argc, _TCHAR* argv[])
{
	if( argc == 1 )
	{
		//Output help info
		cout << "CRIScript Test" << endl;
        return 1;
	}
	int i=0;
	ifstream fin ( argv[ 1 ], ios::in | ios::binary );
    if (!fin){
		cout << "File Open Error: " << argv[ 1 ] << endl;
        return 1;
    }
	//Check file endianness;
	wchar_t c;
	fin.read( (char*)&c, sizeof( wchar_t ) );
	if( c != 0xfeff )
	{
		cout << "File format not supported!!" << endl;
        return 1;
	}

	//Initialize lexer;
	cri::CLexer* lex = new cri::CLexer;
	cri::CErrorPool* error = new cri::CErrorPool;
	lex->Init();
	
	//Set error handler to the lexer
	lex->SetErrorHandler( error );

	lex->SetFileStream( & fin );
	int iToken;
	wstring s;
	string sout;
	while (lex->Process())
	{
		iToken = lex->GetToken();
		if( iToken == LITERAL_DECIMAL )
		{
			double d = lex->GetDecimalTokenValue();
			cout <<lex->GetLineNumber() <<":"<< lex->GetColumn() <<":"<< iToken <<" "<< d << endl;
		}
		else  if( iToken == LITERAL_INTEGER )
		{
			__int64 i  = lex->GetIntegerTokenValue();
			cout <<lex->GetLineNumber() <<":"<< lex->GetColumn() <<":"<< iToken <<" "<< i << endl;
		}
		else
		{
			s = lex->GetIdentifier();
			wstring::iterator it;
			for( it= s.begin();it!=s.end(); it++)
			{
				sout+= (*it)&0xff;
			}
			cout << lex->GetLineNumber() <<":";
			cout << lex->GetColumn() <<":";
			cout <<iToken <<" "<< sout << endl;
			sout.erase();
		}

	}
        
    fin.close();
    
	return 0;
}

