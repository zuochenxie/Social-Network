#ifndef BTREE_H
#define BTREE_H

#include <string>
#include <vector>
#include <cstdio>
using namespace std;

#define M 5
#define L 3

// Class Declarations
class BTree;
class BTreeElement;
class BTreeNode;
class BTreeInternal;
class BTreeLeaf;

/*==============================================================================

	B TREE

==============================================================================*/
class BTree {
private:
	BTreeNode* pRoot;
public:
	BTree();
	void setRoot(BTreeNode* r) {
		pRoot = r;
	}
	BTreeLeaf* getFirstLeaf();
	BTreeLeaf* getLeaf(string key);

	void insertElement(BTreeElement* e);
	BTreeElement* getElement(string key);

	vector<int> rangeQuery(string start, string finish);
	void printElements();
};

/*==============================================================================

	B TREE ELEMENT

==============================================================================*/
class BTreeElement {
private:
	string key;
	int index;
	BTreeElement* pNextElement;
public:
	BTreeElement();
	BTreeElement(string key, int index);
	string getKey() { return key; }
	int getIndex() { return index; }
	BTreeElement* getNextElement() { return pNextElement; }

	void setNextElement(BTreeElement* e) { pNextElement = e; }
	bool isValid() { return key != ""; }

	void print();
};

/*==============================================================================

	B TREE NODE

==============================================================================*/
class BTreeNode {
protected:
	BTree* pTree;	// Used to set BTree's new root.
	BTreeInternal* pParentNode;

	virtual void split() = 0;
public:
	BTreeNode(BTree* pTree, BTreeInternal* pParentNode);

	void setParent(BTreeInternal *pParentNode) { this->pParentNode = pParentNode; }

	virtual string getKey() = 0;
	virtual BTreeLeaf* getFirstLeaf() = 0;
	virtual BTreeLeaf* getLeaf(string key) = 0;
};

/*==============================================================================

	B TREE INTERNAL

==============================================================================*/
class BTreeInternal: public BTreeNode {
private:
	const static int maxElements = M;
	vector<string> keys;
	vector<BTreeNode*> pChildNodes;

	void split();
	BTreeNode* findChildNode(string key);

public:
	BTreeInternal(BTree* pTree, BTreeInternal* pParentNode);
	BTreeInternal(BTree* pTree, BTreeInternal* pParentNode,
		BTreeNode* pChild1, BTreeNode* pChild2);

	string getKey();
	BTreeLeaf* getFirstLeaf();
	BTreeLeaf* getLeaf(string key);

	void addChildNode(BTreeNode* pChild);
	void deleteChildNode(int index);

};

/*==============================================================================

	B TREE LEAF

==============================================================================*/
class BTreeLeaf: public BTreeNode {
private:
	const static int maxElements = L;
	vector<BTreeElement*> pElements;
	BTreeLeaf* pNextLeaf;

	void split();

public:
	BTreeLeaf(BTree* pTree, BTreeInternal* pParentNode);

	string getKey();
	BTreeLeaf* getFirstLeaf() { return this; }
	BTreeLeaf* getLeaf(string key) { return this; }

	BTreeElement* getElement(string key);
	vector<BTreeElement*> getElements() { return pElements; }
	void insertElement(BTreeElement* element);

	void printElements();

	BTreeLeaf* getNextLeaf() { return pNextLeaf; }
	void setNextLeaf(BTreeLeaf* pLeaf) { pNextLeaf = pLeaf; }
};

#endif // BTREE_H
