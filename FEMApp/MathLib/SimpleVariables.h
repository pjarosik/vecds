#ifndef SIMPLEVARIABLES_H
#define SIMPLEVARIABLES_H

#include "VariableTemplate.h"
#include "matrix.h"

typedef VariableTemplate< bool,     VT_BOOL     > VariableBool;
typedef VariableTemplate< int,      VT_INT      > VariableInt;
typedef VariableTemplate< unsigned, VT_UNSIGNED > VariableUnsigned;
typedef VariableTemplate< double,   VT_DOUBLE   > VariableDouble;
typedef VariableTemplate< mvector,  VT_VECTOR   > VariableVector;
typedef VariableTemplate< matrix,   VT_MATRIX   > VariableMatrix;
typedef VariableTemplate< string,   VT_STRING   > VariableString;

#endif // SIMPLEVARIABLES_H
