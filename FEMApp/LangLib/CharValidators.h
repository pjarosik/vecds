#pragma once

template<char cc>
class IsValidNotChar
{
public:
	static const bool Validate( char c ) { return c != cc;  }
};

class IsValidWhite
{
public:
	static const bool Validate( char c ) { return c <= ' ';  }
};


class IsValidNotEol
{
public:
    static const bool Validate( char c ) { return c != '\n' && c != '\r';  }
};


class IsValidWhiteNotEol
{
public:
    static const bool Validate( char c ) { return c <= ' ' && IsValidNotEol::Validate( c );  }
};

class IsValidDigit
{
public:
	static const bool Validate( char c ) { return c >= '0' && c <= '9';  }
};

class IsValidCapitalLetter
{
public:
	static const bool Validate( char c ) { return c >= 'A' && c <= 'Z';  }
};

class IsValidSmallLetter
{
public:	
	static const bool Validate( char c ) { return c >= 'a' && c <= 'z';  }
};

class IsValidLetter
{
public:
    static const bool Validate( char c ) { return IsValidCapitalLetter::Validate( c ) || IsValidSmallLetter::Validate( c );  }
};

class IsValidToken
{
public:
    static const bool Validate( char c ) { return IsValidLetter::Validate( c ) || IsValidDigit::Validate( c ) || c == '_';  }
};

class IsValidFilename
{
public:
    static const bool Validate( char c ) { return IsValidLetter::Validate( c ) || IsValidDigit::Validate( c ) || c == '.' || c == '/' || c == '_' || c == '\\' || c == '-';  }
};

class IsValidExpression
{
public:
    static const bool Validate( char c ) { return IsValidWhiteNotEol::Validate( c ) || IsValidLetter::Validate( c ) || IsValidDigit::Validate( c ) || c == '.' || c == '/' || c == '_' || c == '*' || c == '-' || c == '+' || c == '^' || c == '(' || c == ')' || c == '[' || c == ']';  }
};
