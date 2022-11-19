#ifndef STATEMENTFEAP_H
#define STATEMENTFEAP_H

#include "FEMProject.h"
#include "../LangLib/Statement.h"

class CStatementFeap : public CStatement
{
public:
    CStatementFeap( CFEMProject &fp );
    virtual ~CStatementFeap( void );
    virtual void Execute( istream &i );

    void Import( const string &filename );

protected:

    unsigned m_ndisp,m_dim;
    map< unsigned, unsigned > numbers;
    CFEMProject &FP;

    void SkipLines( istream &in, unsigned nl );
    bool Find( istream &in, const string tofind );

};


#endif // STATEMENTFEAP_H
