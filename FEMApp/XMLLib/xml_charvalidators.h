#pragma once

template<char cc>
class ValidNotChar
{
public:
	static const bool IsValid( char c ) { return c != cc;  }
};

class ValidWhite
{
public:
	static const bool IsValid( char c ) { return c <= ' ';  }
};

class ValidNotWhite
{
public:
	static const bool IsValid( char c ) { return c > ' ';  }
};

class ValidNonWhite
{
public:
	static const bool IsValid( char c ) { return c > ' ';  }
};

class ValidDigit
{
public:
	static const bool IsValid( char c ) { return c >= '0' && c <= '9';  }
};

class ValidCapitalLetter
{
public:
	static const bool IsValid( char c ) { return c >= 'A' && c <= 'Z';  }
};

class ValidSmallLetter
{
public:	
	static const bool IsValid( char c ) { return c >= 'a' && c <= 'z';  }
};


class ValidLetter
{
public:
	static const bool IsValid( char c ) { return ValidCapitalLetter::IsValid( c ) || ValidSmallLetter::IsValid( c );  }
};

class ValidToken
{
public:
	static const bool IsValid( char c ) { return ValidLetter::IsValid( c ) || ValidDigit::IsValid( c ) || c == '_';  }
};

class ValidFilename
{
public:
	static const bool IsValid( char c ) { return ValidLetter::IsValid( c ) || ValidDigit::IsValid( c ) || c == '.' || c == '/' || c == '_' || c == '\\';  }
};


class ValidXMLNodeName
{
public:	
	static const bool IsValid( char c ) { return ValidLetter::IsValid( c ) || ValidDigit::IsValid( c ) || c == '.' || c == '-' || c == '_' || c == ':';  }
};

class ValidXMLBeginName
{
public:	
	static const bool IsValid( char c ) { return ValidLetter::IsValid( c ) || c == '-' || c == '_';  }
};

class ValidXMLAttributeName
{
public:	
	static const bool IsValid( char c ) { return ValidLetter::IsValid( c ) || ValidDigit::IsValid( c ) || c == '-' || c == '.' || c == '_';  }
};

template < class V1, class V2 >
class ValidAnd
{
public:	
	static const bool IsValid( char c ) { return V2::IsValid( c ) && V1::IsValid( c );   }
};


template < class V1, class V2 >
class ValidOr
{
public:	
	static const bool IsValid( char c ) { return V2::IsValid( c ) || V1::IsValid( c );   }
};


template < class V1 >
class ValidNot
{
public:	
	static const bool IsValid( char c ) { return !V1::IsValid( c );   }
};


typedef ValidNotChar<'<'> ValidXMLText;