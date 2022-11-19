#include "StatementMaterial.h"
#include "../LangLib/statdef.h"

extern "C"
{
    void  user_init_(char*,char*);
}

CStatementMaterial::CStatementMaterial(CFEMProject &fp):FP(fp)
{

}


CStatementMaterial::~CStatementMaterial( void )
{

}


void CStatementMaterial::Execute( istream &i )
{
	TokenParam name(string("name")), type(string("type"));
    NamedParameterListDouble parameters;
	char c;
    map< string, double > mc;

	i >> name >> type >> c;
	if ( c!= ':' ) CFEException( " ':' Expected instead of :" + c ); 


	CMaterial *mat = FP.GetMaterial( name.GetParam().toString() );
	if ( mat ) throw CFEException( "Material named :" + name.GetParam().toString() + " already exists");


	unsigned k;

    if ( type.GetParam().toString() == "Elem14" )
    {
        i >> c;
        if ( c != '{' ) CFEException( " '{' Expected instead of :" + c );
        c = i.get();
        ofstream fout("elem14.txt");
        while ( c!= '}' )
        {
            fout << c;
            c = i.get();
            if ( i.eof() ) CFEException( "Unexpected end of input file" );
        }
        fout.close();
        user_init_("elem14.txt","elem14out.txt");
    }
    else
    {
        i >> parameters;
        for (k=0; k<parameters.GetList().size(); k++)
            mc[ parameters.GetList()[k].GetName().toString() ] = parameters.GetList()[ k ].GetParam();
    }

	mat = new CMaterial( type.GetParam().toString(), mc );
	FP.AddMaterial( name.GetParam().toString(), mat );
}
