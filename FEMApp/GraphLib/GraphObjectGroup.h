#pragma once
#include "GraphObject.h"
#include <list>

class CGraphObjectGroup :
	public CGraphObject
{
public:
	CGraphObjectGroup(void);
	virtual ~CGraphObjectGroup(void);

	virtual void Flush( CGraphDevice *GD )=0;
	virtual void GetBoundingBox ( DRect &bb )=0;

protected:

	list< CGraphObject* >  objects;
};
