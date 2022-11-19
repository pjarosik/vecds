#ifndef CGRAPHOBJECT_H
#define CGRAPHOBJECT_H

class CGraphObject
{
public:
    CGraphObject();
    bool IsOnBorder() const { return border; }
    void SetBorder( bool ib ) { border = ib; }
protected:
    bool border;
};



#endif // CGRAPHOBJECT_H
