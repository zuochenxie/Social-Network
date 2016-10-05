//
//  Main2.cpp
//  130A Project
//
//  Created by Xie Zuochen on 2/28/16.
//  Copyright (c) 2016 Xie Zuochen. All rights reserved.
//

#include "FriendshipGraph.h"
#include "BTree.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>
using namespace std;

FriendshipGraph f;
BTree *pTree;

void printUserInfo(int index);
void printUserInfo(string name);
void printUserInfoWithFriends(int index);
void printUserInfoWithFriends(string name);
void printUserInfoAll(int index);
void printUserInfoAll(string name);
string getName(int index);
string getMultiWord();

vector<string> split(string str, char delimiter)
{
    vector<string> internal;
    stringstream ss(str);       // turn the string into a stream.
    string tok;

    while(getline(ss, tok, delimiter))
    {
        internal.push_back(tok);
    }
    return internal;
}

int main()
{
    f = FriendshipGraph();
	pTree = new BTree();
    ifstream file;
    file.open("FinalProjectTestCases/Generated1.txt", ios::in);

    int index = 0;
    FILE *Data;

    if(!file)
    {
        cerr << "File not found" << std::endl;
		return -1;
    }
    else
    {
		Data = fopen("Output.txt", "w");
		string line;
        while (std::getline(file, line))
        {
            /* Insert to Hashtable */
            FriendsLinkedList l = FriendsLinkedList();
            vector<string> words = split(line, ',');
            for (int i = 3; i < words.size(); i++)
            {
                l.addFriend(words[i]);
            }
            GraphNode tmp(words[0], l, index);
            f.insert(tmp);


            /* Insert to B-Tree */
			BTreeElement *e = new BTreeElement(words[0], index);
			pTree->insertElement(e);

            /* Generate Data File */

            fseek (Data, index*53, SEEK_SET);
            fputs(words[0].c_str(), Data);
            fseek (Data, index*53+20, SEEK_SET);
            fputs(words[1].c_str(), Data);
            fseek(Data, index*53+23, SEEK_SET);
            fputs(words[2].c_str(), Data);

            index++;
        }
        fclose(Data);
        std::cout << "Data File successfully Generated" << std::endl;
    }

    string str;
    while (true)
    {
        cout << "Please Type Command" << endl;
        cin >> str;
		// To lower case.
		transform(str.begin(), str.end(), str.begin(), ::tolower);

        if (cin.eof())
        {
            return 0;
        }
        if(str.compare("exit")==0)
        {
            return 0;
        }
		else if(str.compare("help")==0)
        {
            cout << "Commands:" << endl;
			cout << "insert [name] [age] [\"occupation name\"]" << endl;
			cout << "addfriend [name1] [name2]" << endl;
			cout << "listfriendsinfo [name]" << endl;
			cout << "listinfo [name1] [name2]" << endl;
			cout << "userinfo [name]" << endl;
			cout << "unfriend [name1] [name2]" << endl;
			cout << "showmutualfriends [name1] [name2]" << endl;
			cout << "friendrecommendation [name]" << endl;
			cout << "addrelationship [name1] [name2] [relationship]" << endl;
			cout << "unrelationship [name1] [name2]" << endl;
        }
		else if(str.compare("insert")==0)
        {
			vector<string> multiWord;

			Data = fopen("Output.txt", "r+");
			string name;
            string age;
            string occupation;
            FriendsLinkedList l = FriendsLinkedList();
            cout << "Type Name, Age, and Occupation" << endl;
			cout << "Please put quotation marks around multi-word entries" << endl;
			cin >> name;
			cin >> age;
			occupation = getMultiWord();

			// Make sure that strings don't overwrite other entries
			if(name.size() > 20) name.resize(20);
			if(age.size() > 3) age.resize(3);
			if(occupation.size() > 30) occupation.resize(30);

            fseek (Data, index*53, SEEK_SET);
            fputs(name.c_str(), Data);
            fseek (Data, index*53+20, SEEK_SET);
            fputs(age.c_str(), Data);
            fseek(Data, index*53+23, SEEK_SET);
            fputs(occupation.c_str(), Data);
            GraphNode tmp(name, l, index);
            f.insert (tmp);

			BTreeElement *e = new BTreeElement(name, index);
			pTree->insertElement(e);

			index++;
			fclose(Data);
	        std::cout << "Successfully Inserted" << std::endl;
        }
        else if(str.compare("addfriend")==0)
        {
            string name1;
            string name2;
            cin >> name1 >> name2;
            f.addFriendList(name1, name2);
        }
        else if(str.compare("printall")==0)
        {
			for(int i=0; i<index; i++) {
				printUserInfoAll(i);
			}
        }
        else if(str.compare("listfriendsinfo")==0)
        {
            string name;
            cout << "Type Name" << endl;
            cin >> name;
            for (int i = 0; i < f.countFriends(name); i++)
            {
                int userIndex = f.getIndex(f.getGraphNode(name).l.getFriend(i));
				printUserInfo(userIndex);
            }
        }
        else if (str.compare("listinfo")==0)
        {
			string name1;
            string name2;
            cin >> name1 >> name2;
			if(name1.compare(name2) > 0) swap(name1, name2);

			vector<int> indicies = pTree->rangeQuery(name1, name2);
			for(int i=0; i<indicies.size(); i++) {
				printUserInfoWithFriends(indicies[i]);
			}
        }
		else if (str.compare("userinfo")==0)
		{
			string name;
			cin >> name;
			printUserInfoWithFriends(name);
		}
// Extra Feature 1

		else if(str.compare("unfriend")==0)
		{
		    string name1;
		    string name2;
		    cin >> name1 >> name2;
		    f.unFriend(name1, name2);
		}

// Extra Feature 2

		else if(str.compare("showmutualfriends")==0)
		{
		    string name1;
		    string name2;
		    cin >> name1 >> name2;
		    for (int i = 0; i < f.countFriends(name1); i++)
		    {
		        f.mutualFriends(f.getGraphNode(name1).l.getFriend(i), name2);
		    }
			std::cout << std::endl;
		}

// Extra Feature 3

        else if(str.compare("friendrecommendation")==0)
        {
            string name;
            cin >> name;
			f.clearFreqCountArrays();
			for (int i = 0; i < f.countFriends(name); i++)
            {
                for (int j = 0; j < index; j++)
                {
                    if (getName(j) == name)
                    {
                        ;
                    }
                    else
                    {
                        f.CountMutualFriends(f.getGraphNode(name).l.getFriend(i), getName(j));
                    }
                }
            }
            f.SortCount();

			if (f.getFreq()[0] != -1)
				cout << getName(f.getFreq()[0]) << " has " << f.getCount()[f.getFreq()[0]] << " mutual friends." << endl;
			if (f.getFreq()[1] != -1)
				cout << getName(f.getFreq()[1]) << " has " << f.getCount()[f.getFreq()[1]] << " mutual friends." << endl;
			if (f.getFreq()[2] != -1)
				cout << getName(f.getFreq()[2]) << " has " << f.getCount()[f.getFreq()[2]] << " mutual friends." << endl;
            if (f.getFreq()[0] == -1 && f.getFreq()[1] == -1 && f.getFreq()[2] == -1)
                cout << "No Recommendations" << endl;
        }
// Extra Feature 4
		else if(str.compare("addrelationship")==0)
		{
			cout << "Valid relationships:" << endl;
			cout << "Dating, Married, Parent, Child, Sibling" << endl;

			string name1;
			string name2;
			string relationship;
			cin >> name1 >> name2 >> relationship;
			f.addRelList(name1, name2, relationship);
		}
		else if(str.compare("unrelationship")==0)
		{
			string name1;
			string name2;
			cin >> name1 >> name2;
			f.unRelationship(name1, name2);
		}
        else
        {
            cin.clear();
            cout << "Incorrect Command" << endl;
            //return 0;
        }
    }
}

void printUserInfo(int index) {
	if(index == -1) return;
	char name[21];
	char age[4];
	char occupation[31];
	// Access file to get user info
	FILE *Data = fopen("Output.txt", "r");
	fseek(Data, index*53, SEEK_SET);
	fgets(name, 20, Data);
	fseek(Data, index*53+20, SEEK_SET);
	fgets(age, 3, Data);
	fseek(Data, index*53+23, SEEK_SET);
	fgets(occupation, 30, Data);
	fclose(Data);
	printf("%d: %s, %s, %s\n", index, name, age, occupation);
}

void printUserInfo(string name) {
	int index = pTree->getElement(name)->getIndex();
	printUserInfo(index);
}

void printUserInfoWithFriends(int index) {
	if(index == -1) return;
	string name = getName(index);
	printUserInfoWithFriends(name);
}

void printUserInfoWithFriends(string name) {
	printUserInfo(name);
	f.printFriendList(name);
}

void printUserInfoAll(int index) {
	if(index == -1) return;
	string name = getName(index);
	printUserInfoAll(name);
}

void printUserInfoAll(string name) {
	printUserInfo(name);
	f.printFriendList(name);
	f.printRelList(name);
}

string getName(int index) {
	char name[21];
	FILE *Data = fopen("Output.txt", "r");
	fseek(Data, index*53, SEEK_SET);
	fgets(name, 20, Data);
	fclose(Data);
	return name;
}

string getMultiWord() {
	string temp;
	bool multiStarted = false;
	vector<string> multiWord;
	while(true) {
		cin >> temp;
		char firstChar = temp[0];
		char lastChar = temp[temp.size()-1];
		if(firstChar == '\"' || firstChar == '\'') {
			multiStarted = true;
			temp = temp.substr(1);
		}
		if(lastChar == '\"' || lastChar == '\'') {
			multiStarted = false;
			// dump strings in multiWord into the temp string
			temp = temp.substr(0, temp.size()-1);
			while(!multiWord.empty()) {
				temp = multiWord.back() + " " + temp;
				multiWord.pop_back();
			}
		}
		if(multiStarted) {
			multiWord.push_back(temp);
		} else {
			return temp;
		}
	}
}
