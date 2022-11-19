#ifndef ARRAYS_H
#define ARRAYS_H
#include <valarray>
using namespace std;

class array3
{
    valarray< valarray< valarray< double > > > A;
public:
    array3(unsigned d1, unsigned d2, unsigned d3);

};

#endif // ARRAYS_H
