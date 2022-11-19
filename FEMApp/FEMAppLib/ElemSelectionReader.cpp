#include "ElemSelectionReader.h"
#include "../LangLib/LangToken.h"
#include "../LangLib/statdef.h"

void CElemSelectionReader::read( istream &inp )
{

    EnumerationList2 param_numbers((string)"numbers",true,false);
    NamedParmeterTokenList2 param_domains((string)"domains",true,false);

    param_numbers.GetParam().SetMaxRange( elements.size() );

    inp >> param_numbers >> param_domains;

    if ( param_numbers.IsRead() )
    {
        CElemInstanceSelector::CSelectMethodByNumber sel( param_numbers.GetParam() );
        selector.AddSelector( sel );
    }

    if ( param_domains.IsRead() )
    {
        const ParamList &domainlist = param_domains.GetParam();
        unsigned k;
        for (k=0; k<domainlist.GetList().size(); k++ )
        {
            string domain = domainlist.GetList()[k].toString();
            CElemInstanceSelector::CSelectMethodByDomain dom( domain );
            selector.AddSelector( dom );
        }
    }



}
