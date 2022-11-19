#pragma once



template < typename elem_type, typename iterator,  typename Pred >
class iter_if
{
	iterator begin,end,i;
	Pred predicate;
public:
	
	 iter_if( iterator b, iterator e, const Pred &p ): begin(b),end(e), i( begin ), predicate( p ) {  }
	~iter_if(void)	{  }
	bool IsEnd() const { return i == end; }
	bool Next(); 
	const elem_type* GetElem() const { return dynamic_cast< const elem_type* >( *i ); }
};

template < typename elem_type, typename iterator,  typename Pred >
inline bool iter_if< elem_type, iterator,  Pred >::Next()
{
	if ( i != begin ) i++; 
	while ( !predicate( **i ) && i != end ) i++; 
	return IsEnd(); 
}
