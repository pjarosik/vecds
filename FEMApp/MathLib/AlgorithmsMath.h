#ifndef ALGORITHMSMATH_H
#define ALGORITHMSMATH_H

template< class C >
inline bool CompareContainers( const C &c1,const C &c2 )
{
    if ( c1.size() != c2.size() ) return false;

    typename C::const_iterator i;
    for(i=c1.begin(); i!=c1.end(); i++)
        if ( find( c2.begin(), c2.end(), *i ) == c2.end() ) return false;

    return true;
}


template< class C >
inline bool Contains( const C &c1,const C &c2 )
{
    typename C::const_iterator i;
    for(i=c2.begin(); i!=c2.end(); i++)
        if ( find( c1.begin(), c1.end(), *i ) == c1.end() ) return false;

    return true;
}

// Gather elements from c1 which are not ptresent in c2
template< class C >
inline void GetUnique( const C &c1, C &c2 )
{
    typename C::const_iterator i;
    for(i=c1.begin(); i!=c1.end(); i++)
        if ( find( c2.begin(), c2.end(), *i ) == c2.end() ) c2.push_back(*i);
}


#endif // ALGORITHMSMATH_H
