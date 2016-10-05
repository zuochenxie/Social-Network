#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "BTree.h"

using namespace std;

int main()
{
	BTree *tree = new BTree();

	BTreeElement *e0 = new BTreeElement("Yang", 0);
	BTreeElement *e1 = new BTreeElement("Charles", 1);
	BTreeElement *e2 = new BTreeElement("Anna", 2);

	BTreeElement *e3 = new BTreeElement("Brian", 3);
	BTreeElement *e4 = new BTreeElement("David", 4);
	BTreeElement *e5 = new BTreeElement("Marcus", 5);

	BTreeElement *e6 = new BTreeElement("Isaiah", 6);
	BTreeElement *e7 = new BTreeElement("Alex", 7);
	BTreeElement *e8 = new BTreeElement("Jon", 8);

	tree->insertElement(e0);
	tree->insertElement(e1);
	tree->insertElement(e2);
	tree->insertElement(e3);
	tree->insertElement(e4);
	tree->insertElement(e5);
	tree->insertElement(e6);
	tree->insertElement(e7);
	tree->insertElement(e8);

	tree->printElements();

	vector<int> queryResult = tree->rangeQuery("Brian", "Jon");
	for(int i=0; i<queryResult.size(); i++) {
		printf("%d, ", queryResult[i]);
	}
	printf("\n");
	return 0;
}
