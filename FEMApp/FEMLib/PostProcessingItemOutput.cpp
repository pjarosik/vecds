#include "PostProcessingItemOutput.h"

OutputDef::OutputDef(  DomainContainer< CFENode > &nd,
                       DomainContainer< CFiniteElementInstance > &ielems,
                       map_named_ptr< CFiniteElement > &felems,
                       map_named_ptr< CMaterial > &mats
                      ):nsel(nd,true), esel( ielems, true ), nodes(nd), elements(ielems), felements(felems), materials(mats)
{
        stages.push_back("analysis");

        dataecho.push_back("nodes");
        dataecho.push_back("elems");
        dataecho.push_back("materials");
        dataecho.push_back("output");
        dataecho.push_back("analysis");

        nvals.push_back("all");
        evals.push_back("all");
        pattern = "gauss";

}

const OutputDef& OutputDef::operator=( const OutputDef &od )
{
    name        = od.name;
    dataecho    = od.dataecho;
    analysis    = od.analysis;
    stages      = od.stages,
    nvals       = od.nvals,
    evals       = od.evals;
    pattern     = od.pattern;
    eqs         = od.eqs;
    nsel        = od.nsel;
    esel        = od.esel;

}


CPostProcessingItemOutput::CPostProcessingItemOutput( const OutputDef &od ):CPostProcessingItem(od.stages),OD(od)
{

}


