#ifndef _BST_H_
#define _BST_H_


public class BSTNode<T> {
	public:
	BSTNode(void);
	
	T key;
	(void *)pData;
	int iLeft;
	int iRight;
	int iParent;
};
	
	
public class BST<T> {
	public:
	BST(void);
	~BST(void);
	
	void Insert(T key, (void *)pData);
	int Search(T key, int searchFlags, T *pkey, (void **)ppData);
	int SearchCeiling(T key, int searchFlags, T *pkey, (void **)ppData);
	int SearchFloor(T key, int searchFlags, T *pkey, (void **)ppData);
	
	private:	
	int size;
	int cElements;
	BSTNode<T> *pItems;
	
	void resize(void);
};

#endif