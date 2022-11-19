#pragma once

#include <vector>
#include <string>
using namespace std;

#define CLONE_MEMBER( parent, member ) AddNode( member = parent.member->Clone() );
#define CLONE_FN(classtype) virtual classtype* Clone() { return  (classtype*)(cloned_node == 0 ? cloned_node = new classtype( *this ) : cloned_node);  }

class CTreeNode
{

public:

	CTreeNode( const string &nm );
    CTreeNode( const CTreeNode &mf ):printdiag( mf.printdiag ), objectname( mf.objectname ), unlocked( true ), notvalid( true ), cloned_node( 0 ) {  }
	virtual ~CTreeNode(void);

	virtual CTreeNode* Clone()=0;

    CTreeNode* CloneTree(bool clear = true );
	void GetStructure( vector< CTreeNode* > &v ) const;
	const string& GetObjectName() const { return objectname; }
	void SetDescription( const string ds ) { description = ds; }
	const string& GetDescription( ) const { return description; }

	void UnlockThis() { unlocked = true; }
	void ValidateThis()	{ notvalid = false;	}
	void InvalidateThis() { notvalid = true; }

    void UnlockStructure();
	void InvalidateStructure();
	void CreateUsedByVector( );
	void CloneReset();

	vector< CTreeNode* >& GetUsedByVector() { return usedby; }
	bool printdiag;
    void PrintStructure( ostream &out, unsigned level = 0 );

protected:

	vector< CTreeNode* > depends, usedby;
	CTreeNode *cloned_node;
	const string  objectname;
	string description;
    bool notvalid, unlocked;

	void InvalidateUsedByObjects();
	void UpdateUsedByVector( vector< CTreeNode* > uv );
	CTreeNode* AddNode( CTreeNode *ob );

};

void DeleteTree( CTreeNode *nd );
