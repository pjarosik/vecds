#pragma once

#include "ListParameter.h"
#include "ListParameterWhite.h"
#include "CharValidators.h"
#include "LangToken.h"
#include "NamedParameter.h"
#include "ListParameterWhite.h"
#include "ParamList2.h"
#include "ParamList2.h"
#include "NumberList.h"
#include "multiread.h"

// name = param
typedef CNamedParameter< 
    CLangToken< IsValidToken >,
	'=',
    CLangToken< IsValidToken >
> TokenParam;

// name = sin(x)*y^2
typedef CNamedParameter<
    CLangToken< IsValidToken >,
    '=',
    CLangToken< IsValidExpression >
> TokenParamExpression;

// sin(x)*y^2*y^3
typedef CLangToken< IsValidExpression > TokenExpression;

// 3.0
typedef CLangToken< IsValidFilename > TokenFilename;

// name = param
typedef CNamedParameter< 
    CLangToken< IsValidToken >,
	'=',
    CLangToken< IsValidFilename >
> TokenParamFilename;

// name = 1.0E4
typedef CNamedParameter< 
    CLangToken< IsValidToken >,
	'=',
	double 
> TokenParamDouble;

// name = 3
typedef CNamedParameter<
    CLangToken< IsValidToken >,
    '=',
    unsigned
> TokenParamUnsigned;

// { param1, param2, param3 }
typedef  CListParameter< '{', '}',  CLangToken< IsValidToken > > ParamList;

// { x^2*cos(x), tan(y), (a+b)^2 }
typedef  CListParameter< '{', '}',  CLangToken< IsValidExpression > > ParamListExpressions;

// < 1.0, 2.0, 3.0 >
typedef  CListParameter< '<', '>',  double > DoubleVector;

// < x, x^3, sin(x^2) >
typedef  CListParameter< '<', '>',  CLangToken< IsValidExpression > > ExpressionVector;

// one of the coords < x, x^3, sin(x^2) > or pointname
typedef TVariantRead2<
                        CListParameter< '<', '>',  CLangToken< IsValidExpression >, false >,
                        CLangToken< IsValidToken >
> PointVariant;


// x = < 1.0, 2.0, 3.0 >
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	'=',
    DoubleVector
> DoubleVectorParameter;

// x = < x, x^3, sin(x^2) >
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    '=',
    ExpressionVector
> ExpressionVectorParameter;

// x : < 1.0, 2.0, 3.0 >
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	':',
    DoubleVector
> DoubleVectorParameter2;

// list =  { 0, 1, 2, 3 }
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	'=',
	CListParameter< '{', '}',  unsigned > 
> UnsignedList;


// list =  { 0.0, 1.0, 2.0, 3.0 }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    '=',
    CListParameter< '{', '}',  double >
> DoubleList;

// list =  { 0 1 2 3 }
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	'=',
	CListParameterWhite< '{', '}',  unsigned > 
> UnsignedListWhite;

// list :  { 0 1 2 3 }
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	':',
	CListParameterWhite< '{', '}',  unsigned > 
> UnsignedListWhite2;

// list =  { 0-5, 10-45, 200-500 }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
	'=',
	CNumberList< '{', ',', '}' >  
> EnumerationList;

// list =  { 0-5, 10-45, 200-500 }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    ':',
    CNumberList< '{', ',', '}' >
> EnumerationList2;

// list = { 1.0 2.0 3.0 }
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	'=',
	CListParameterWhite< '{', '}',  double >
> NamedParameterDoubleListWhite;

// list : { 1.0 2.0 3.0 }
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	':',
	CListParameterWhite< '{', '}',  double >
> NamedParameterDoubleListWhite2;

// name = { param1, param2, param3 }
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	'=',
    ParamList
> NamedParmeterTokenList;

// name : { param1, param2, param3 }
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	':',
    ParamList
> NamedParmeterTokenList2;


// { pname=1.0, pname2 = 2.0, pname3 = 3.0 }
typedef CListParameter< '{', '}',  CNamedParameter< 
                        CLangToken< IsValidToken >,
                        '=',
						double> 
> NamedParameterListDouble;

// { pname=token1, pname2 = token2, pname3 = token3 }
typedef CListParameter< '{', '}',  CNamedParameter< 
                        CLangToken< IsValidToken >,
						'=',
                        CLangToken< IsValidToken >
						> 
> NamedParameterListToken;

// { pname=x^2, pname2 = r*sin(fi), pname3 = atan(y^2) }
typedef CListParameter< '{', '}',  CNamedParameter<
                        CLangToken< IsValidToken >,
                        '=',
                        CLangToken< IsValidExpression >
                        >
> NamedParameterListTokenExpressions;


// name = { pname=token1, pname2 = token2, pname3 = token3 }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    '=',
    NamedParameterListToken
> NamedParameterListTokenParameter;

// name = { pname=x*y, pname2 = sin(x)*cos(y), pname3 = x*ln(y) }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    '=',
    NamedParameterListTokenExpressions
> NamedParameterListTokenParameterExpressions;

// name : { pname=x*y, pname2 = sin(x)*cos(y), pname3 = x*ln(y) }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    ':',
    NamedParameterListTokenExpressions
> NamedParameterListTokenParameterExpressions2;


// name = { pname=1.0, pname2 = 2.0, pname3 = 3.0 }
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	'=',
	NamedParameterListDouble
> NamedParameterListDoubleParameter;

// name : { pname=1.0, pname2 = 2.0, pname3 = 3.0 }
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	':',
	NamedParameterListDouble
> NamedParameterListDoubleParameter2;


// name = { pname=token1, pname2 = token2, pname3 = token3 }
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	'=',
	NamedParameterListToken
> NamedParameterListTokenParameter;

// name : { pname=token1, pname2 = token2, pname3 = token3 }
typedef	CNamedParameter< 
    CLangToken< IsValidToken >,
	':',
	NamedParameterListToken
> NamedParameterListTokenParameter2;

// name : { pname=r*sin(fi), pname2 = x^2+y^2, pname3 = a / b }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    ':',
    NamedParameterListTokenExpressions
> NamedParameterListTokenParameterExpressions2;


// { <1.0, 3.0, 5.0 >, <4.0, 3.7, 5.0 >, <1.0, 3.9, 4.2 > }
typedef  CListParameter< '{', '}',  DoubleVector > DoubleVectorList;

// { <x, y, x*y >, < sin(x), cos(y), 5.0 >, < y*log(x), 2*x, 4.2 > }
typedef  CListParameter< '{', '}',  ExpressionVector > ExpressionVectorList;

// { a, < sin(x), cos(y), 5.0 >, b }
typedef  CListParameter< '{', '}',  PointVariant > PointVariantList;

// name = { a, < sin(x), cos(y), 5.0 >, b }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    '=',
    PointVariantList
> PointVariantListParameter;

// name : { a, < sin(x), cos(y), 5.0 >, b }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    '=',
    PointVariantList
> PointVariantListParameter2;


// { a=<1.0, 3.0, 5.0 >, b=<4.0, 3.7, 5.0 >, c=<1.0, 3.9,4.2 > }
typedef  CListParameter< '{', '}',  ExpressionVectorParameter > ExpressionVectorParameterList;

// name : { <1.0, 3.0, 5.0 >, <4.0, 3.7, 5.0 >, <1.0, 3.9,4.2 > }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    ':',
    DoubleVectorList
> DoubleVectorListParameter2;

// name : { <x, y, x*y >, < sin(x), cos(y), 5.0 >, < y*log(x), 2*x, 4.2 >  }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    ':',
    ExpressionVectorList
> ExpressionVectorListParameter2;


// { name : { param1, param2, param3 }, name = { param1, param2, param3 }, name = { param1, param2, param3 }  }
typedef CListParameter< '{', '}',  DoubleList > NamedParmeterTokenListList;

// { name : { param1, param2, param3 }, name = { param1, param2, param3 }, name = { param1, param2, param3 }  }
typedef	CNamedParameter<
    CLangToken< IsValidToken >,
    ':', NamedParmeterTokenListList
> NamedParameterOfNamedParmeterTokenListList;


