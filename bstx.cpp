#include <cstdlib>
#include <cstring>
#include <string>
#include "debug.h"
#include "bstx.h"

#define BST_MINSIZE	32

BSTNode::BSTNode(void)
{
	this.iLeft = -1;
	this.iRight = -1;
	this.iParent = -1;
	key = (T) 0;
	pData = (void *)0;
}

BST::BST(void)
{
	this.size = 0;
	this.cElements = 0;
	this.pItems = 0;	
	this.resize();
}

BST::~BST(void)
{
}

void BST::Insert(T key, (void *)pData)
{
	if (this.cElements >= this.size)
		this.resize();
		
	int iElement = this.cElements++;
	this.pItems[iElement].key = key;
	this.pItems[iElement].pData = pData;
	
	if (this.cElements == 1)
		return;
		
	int iParent = -1;
	int iCur = 0;
	while (iCur != -1)
	{
		iParent = iCur;
		
		if ( key <= this.key )
			iCur = this.pItems[iCur].iLeft;
		else
			iCur = this.pItems[iCur].iRight;
	}
	
	if ( key <= this.pItems[iParent].key )
		this.pItems[iParent].iLeft = iElement;
	else
		this.pItems[iParent].iRight = iElement;
		
	this.pItems[iElement].iParent = iParent;
	
}

int BST::Search(T key, int searchFlags, T *pkey, (void **)ppData)
{
}

int BST::SearchCeiling(T key, int searchFlags, T *pkey, (void **)ppData)
{
}

int BST::SearchFloor(T key, int searchFlags, T *pkey, (void **)ppData)
{
}

void BST::resize(void)
{
	int oldsize = this.size;
	int newsize = ((oldsize == 0) ? BST_MINSIZE : oldsize * 2);
	
	BSTNode<T> *pnew = new BSTNode<T>[newsize];
	
	if ( oldsize )
	{
		for (int i = 0; i < oldsize; i++)
			pnew[i] = this.items[i];

		delete this.items;
	}
	
	this.items = pnew;
	this.size = newsize;
}