
#ifndef __POINTER_H
#define __POINTER_H

template< class T >
class pointer
{
     T *data;
	  public:
		  pointer( T *ptr ):data(ptr) { }
		  void SetPointer( T *d ) { data = d; }
		  const T& operator[](unsigned k) const { return data[ k ]; }
		  T& operator[](unsigned k) { return data[ k ]; }
		  const T* GetPointer() const { return data; }
		  T* GetPointer() { return data; }
		  T** GetLocation() { return &data; }
		  void SetMem( const pointer<T> &ptr, unsigned n ) { memcpy( data, ptr.GetPointer(), n * sizeof( T )  ); }
		  void SetMem( const T *ptr, unsigned n ) { memcpy( data, ptr, n * sizeof( T )  ); }
};

#endif

