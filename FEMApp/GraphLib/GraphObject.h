#pragma once

#include "GraphDevice.h"

class CGraphObject
{
public:
	CGraphObject(void);
	virtual ~CGraphObject(void);
	virtual void Flush( CGraphDevice *GD )=0;
    virtual void GetBoundingBox( DRect &bb )=0;
};
