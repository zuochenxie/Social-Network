#include <string>
#include <vector>
#include <cmath>
#include "BTree.h"

using namespace std;

/*==============================================================================

	B TREE

==============================================================================*/
BTree::BTree() {
	pRoot = NULL;
}

void BTree::insertElement(BTreeElement* pElement) {
	if(pRoot == NULL) {
		pRoot = new BTreeLeaf(this, NULL);
	}
	string key = pElement->getKey();
	pRoot->getLeaf(key)->insertElement(pElement);
}

BTreeLeaf* BTree::getFirstLeaf() {
	return pRoot->getFirstLeaf();
}

BTreeLeaf* BTree::getLeaf(string key) {
	return pRoot->getLeaf(key);
}

BTreeElement* BTree::getElement(string key) {
	return pRoot->getLeaf(key)->getElement(key);
}

vector<int> BTree::rangeQuery(string start, string finish) {
	vector<int> resultVector;
	BTreeLeaf* pLeaf = getLeaf(start);
	BTreeLeaf* pLeafEnd = getLeaf(finish)->getNextLeaf();
	vector<BTreeElement*> pElements;

	while(pLeaf != pLeafEnd) {
		pElements = pLeaf->getElements();
		for(int i=0; i < pElements.size(); i++) {
			string key = pElements[i]->getKey();
			int index = pElements[i]->getIndex();
			if((key.compare(start) >= 0) && (key.compare(finish) <= 0)) {
				resultVector.push_back(index);
			}
		}
		pLeaf = pLeaf->getNextLeaf();
	}
	return resultVector;
}

void BTree::printElements() {
	BTreeLeaf* pLeaf = pRoot->getFirstLeaf();
	int i=0;
	while(pLeaf != NULL) {
		printf("{LEAF NO.%02d}\n", i);
		pLeaf->printElements();
		pLeaf = pLeaf->getNextLeaf();
		printf("\n");
		i++;
	}
}

/*==============================================================================

	B TREE ELEMENT

==============================================================================*/
BTreeElement::BTreeElement() {
	key = "";
	index = -1;
}

BTreeElement::BTreeElement(string key, int index) {
	this->key = key;
	this->index = index;
}

void BTreeElement::print() {
	printf("%s, %d\n", key.c_str(), index);
}

/*==============================================================================

	B TREE NODE

==============================================================================*/
BTreeNode::BTreeNode(BTree* pTree, BTreeInternal* pParentNode) {
	this->pTree = pTree;
	this->pParentNode = pParentNode;
	if(pParentNode == NULL) {
		pTree->setRoot(this);
	}
}
/*==============================================================================

	B TREE INTERNAL

==============================================================================*/
BTreeInternal::BTreeInternal(BTree* pTree, BTreeInternal* pParentNode)
	: BTreeNode(pTree, pParentNode) {

}

BTreeInternal::BTreeInternal(BTree* pTree, BTreeInternal* pParentNode,
	BTreeNode* pChild1, BTreeNode* pChild2)
	: BTreeNode(pTree, pParentNode) {

	addChildNode(pChild1);
	addChildNode(pChild2);
}

void BTreeInternal::split() {
	// If parent node doen't exist, create one!
	if(pParentNode == NULL) {
		pParentNode = new BTreeInternal(pTree, NULL);
		pParentNode->addChildNode(this);
	}

	BTreeInternal* pNewSibling = new BTreeInternal(pTree, pParentNode);

	int size = pChildNodes.size();
	int midIndex = ceil(size/2.0);

	// Copy half the pChildNodes over to the new Node.
	for(int i=midIndex; i < size; i++) {
		pNewSibling->addChildNode(pChildNodes[i]);
	}
	// Delete the copied Nodes from the current Node.
	for(int i=midIndex; i < size; i++) {
		deleteChildNode(midIndex);
	}

	// Add new sibling to parent node
	pParentNode->addChildNode(pNewSibling);
}

BTreeNode* BTreeInternal::findChildNode(string key) {
	int i;
	for(i=0; i<keys.size(); i++) {
		if(key.compare(keys[i]) < 0) break;
	}
	return pChildNodes[i];
}

void BTreeInternal::addChildNode(BTreeNode* pChild) {
	pChild->setParent(this);

	// Check for case where this node is a new node.
	if(pChildNodes.size() == 0) {
		// Don't add a key.
		pChildNodes.insert(pChildNodes.begin(), pChild);
		return;
	}

	// Insert key into key vector.
	int i;
	for(i=0; i<keys.size(); i++) {
		if(pChild->getKey().compare(keys[i]) < 0) break;
	}
	keys.insert(keys.begin()+i, pChild->getKey());

	// Insert pChild into pChildNodes vector.
	pChildNodes.insert(pChildNodes.begin()+i+1, pChild);

	// If size is over maxElements, split the node.
	if(pChildNodes.size() > maxElements) {
		split();
	}
}

void BTreeInternal::deleteChildNode(int index) {
	// Delete key.
	if(index == 0)
		keys.erase(keys.begin());
	else
		keys.erase(keys.begin()+index-1);

	// Delete child node.
	pChildNodes.erase(pChildNodes.begin()+index);
}

string BTreeInternal::getKey() {
	return pChildNodes[0]->getKey();
}

BTreeLeaf* BTreeInternal::getFirstLeaf() {
	return pChildNodes[0]->getFirstLeaf();
}

BTreeLeaf* BTreeInternal::getLeaf(string key) {
	return findChildNode(key)->getLeaf(key);
}

/*==============================================================================

	B TREE LEAF

==============================================================================*/
BTreeLeaf::BTreeLeaf(BTree* pTree, BTreeInternal* pParentNode)
 	: BTreeNode(pTree, pParentNode) {
	pNextLeaf = NULL;
}

void BTreeLeaf::split() {
	// If parent node doen't exist, create one!
	if(pParentNode == NULL) {
		pParentNode = new BTreeInternal(pTree, NULL);
		pParentNode->addChildNode(this);
	}

	// Create new sibling node.
	BTreeLeaf* pNewSibling = new BTreeLeaf(pTree, pParentNode);
	pNewSibling->setNextLeaf(this->pNextLeaf);
	this->setNextLeaf(pNewSibling);

	int size = pElements.size();
	int midIndex = ceil(size/2.0);

	// Copy half the elements over to the new Leaf.
	for(int i=midIndex; i < size; i++) {
		pNewSibling->insertElement(pElements[i]);
	}
	// Erase the copied elements from the current Leaf.
	for(int i=midIndex; i < size; i++) {
		pElements.erase(pElements.begin()+midIndex);
	}

	// Add new sibling to parent node
	pParentNode->addChildNode(pNewSibling);
}
BTreeElement* BTreeLeaf::getElement(string key) {
	for(int i=0; i<pElements.size(); i++) {
		if(key.compare(pElements[i]->getKey()) == 0) {
			return pElements[i];
		}
	}
	// Return invalid BTreeElement
	return new BTreeElement();
}
void BTreeLeaf::insertElement(BTreeElement* element) {
	string key = element->getKey();
	int i;
	for(i=0; i<pElements.size(); i++) {
		if(element->getKey().compare(pElements[i]->getKey()) < 0) break;
	}
	pElements.insert(pElements.begin()+i, element);

	// If size is over maxElements, split the leaf
	if(pElements.size() > maxElements) {
		split();
	}
}
string BTreeLeaf::getKey() {
	return pElements[0]->getKey();
}

void BTreeLeaf::printElements() {
	for(int i=0; i<pElements.size(); i++) {
		pElements[i]->print();
	}
}
