#ifndef POSTPROCESSINGITEMOUTPUT_H
#define POSTPROCESSINGITEMOUTPUT_H

#include <string>
#include "PostProcessItem.h"
#include "NodeSelector.h"
#include "ElemInstanceSelector.h"
#include "FE_Equation.h"

using namespace std;

class OutputDef
{
public:

    string name;
    vector< string > dataecho, analysis, stages, nvals, evals;
    string pattern;
    vector< CFE_Equation* > eqs;
    CNodeSelector nsel;
    CElemInstanceSelector esel;

     DomainContainer< CFENode > &nodes;
     DomainContainer< CFiniteElementInstance > &elements;
     map_named_ptr< CFiniteElement > &felements;
     map_named_ptr< CMaterial > &materials;

    const OutputDef& operator=( const OutputDef &od );


    OutputDef(
                 DomainContainer< CFENode > &nd,
                 DomainContainer< CFiniteElementInstance > &ielems,
                 map_named_ptr< CFiniteElement > &felems,
                 map_named_ptr< CMaterial > &mats
             );

};

class CPostProcessingItemOutput : public CPostProcessingItem
{
public:

    CPostProcessingItemOutput( const OutputDef &od );
    void SetOutputDef( const OutputDef &od ) { OD = od; stages = od.stages; }
    const OutputDef& GetOutputDef() const { return OD; }

protected:

    OutputDef OD;

protected:


};

#endif // POSTPROCESSINGITEMOUTPUT_H
