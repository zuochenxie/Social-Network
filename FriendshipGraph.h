//
//  FriendshipGraph.h
//  130A Project
//
//  Created by Xie Zuochen on 2/27/16.
//  Copyright (c) 2016 Xie Zuochen. All rights reserved.
//

#ifndef ___30A_Project__FriendshipGraph__
#define ___30A_Project__FriendshipGraph__

#include <stdio.h>
#include <string>
#include <iostream>
#define TABLE_SIZE 211

struct Node {
    std::string x;
    Node *next;
};

enum RelEnum {
	INVALID = -1,
	DATING,
	MARRIED,
	PARENT,
	CHILD,
	SIBLING
};

class RelHelper {
public:
	static std::string getString(RelEnum e) {
		std::string s;
		switch(e) {
			case DATING: s = "Dating";
			break;
			case MARRIED: s = "Married";
			break;
			case PARENT: s = "Parent";
			break;
			case CHILD: s = "Child";
			break;
			case SIBLING: s = "Sibling";
			break;
			default: s = "Invalid";
			break;
		}
		return s;
	}
	static RelEnum getEnum(std::string s) {
		RelEnum r;
		if(s == "Dating") r = DATING;
		else if(s == "Married") r = MARRIED;
		else if(s == "Parent") r = PARENT;
		else if(s == "Child") r = CHILD;
		else if(s == "Sibling") r = SIBLING;
		else r = INVALID;
		return r;
	}
	static RelEnum getRelPair(RelEnum e) {
		RelEnum p;
		switch(e) {
			case DATING: p = DATING;
			break;
			case MARRIED: p = MARRIED;
			break;
			case PARENT: p = CHILD;
			break;
			case CHILD: p = PARENT;
			break;
			case SIBLING: p = SIBLING;
			break;
			default: p = INVALID;
			break;
		}
		return p;
	}
};

struct RelNode {
	RelEnum rel;
	std::string name;
	RelNode *next;
};

class FriendsLinkedList {
public:
    FriendsLinkedList()
    {
        head = NULL;
		relHead = NULL;
    }

    void addFriend(std::string name)
    {
        Node *n = new Node();
        n -> x = name;
        n -> next = head;
        head = n;
    }
	void addRel(std::string name, RelEnum rel)
	{
		RelNode *r = new RelNode();
        r -> name = name;
		r -> rel = rel;
        r -> next = relHead;
        relHead = r;
	}

    void printList()
    {
        Node *temp = head;
		std::cout << "Friends: ";
		if(temp == NULL) {
			std::cout << "N/A... :(\n";
			return;
		}
		while (temp != NULL) {
			std::cout << temp -> x << ",";
			temp = temp -> next;
		}
		std::cout << std::endl;
    }

	void printRelList()
	{
		RelNode *temp = relHead;
		std::cout << "Relationships: ";
		if(temp == NULL) {
			std::cout << "N/A\n";
			return;
		}
		while (temp != NULL) {
			std::cout << "[" << RelHelper::getString(temp->rel) << ", " << temp->name << "], ";
			temp = temp -> next;
		}
		std::cout << std::endl;
	}

    int countList()
    {
        Node *temp = head;
        int count = 0;
        while (temp != NULL)
        {
            count++;
            temp = temp -> next;
        }
        return count;
    }

    std::string getFriend(int num)
    {
        Node *temp = head;
        for (int i = 0; i < num; i++)
        {
            temp = temp -> next;
        }
            return temp -> x;
    }

    Node* getHead()
    {
	return head;
    }
	RelNode* getRelHead() {
		return relHead;
	}

    bool checkFriend(std::string name)
    {
        Node* tmp = head;
        while (tmp != NULL)
        {
            if (tmp -> x == name)
            {
                return true;
            }
            tmp = tmp -> next;
        }
        return false;
    }
	bool checkRel(std::string name)
    {
		RelNode* tmp = relHead;
        while (tmp != NULL)
        {
            if (tmp -> name == name)
            {
                return true;
            }
            tmp = tmp -> next;
        }
        return false;
    }
	// Extra Feature
	void deleteFriend(std::string name)
	{
	    Node* tmp = head;
		if(tmp->x == name) {
			head = tmp->next;
			return;
		}
	    while (tmp->next != NULL)
	    {
	        if (tmp -> next -> x == name)
	        {
	            tmp -> next = tmp -> next -> next;
	            break;
	        }
	        else
	        {
	            tmp = tmp -> next;
	        }
	    }
	}
	// Extra Feature
	void deleteRel(std::string name)
	{
	    RelNode* tmp = relHead;
		if(tmp->name == name) {
			relHead = tmp->next;
			return;
		}
	    while (tmp->next != NULL)
	    {
	        if (tmp->next->name == name)
	        {
	            tmp->next = tmp->next->next;
                break;
	        }
	        else
	        {
            	tmp = tmp->next;
            }
        }
    }

private:
    Node *head;
	RelNode *relHead;
};


class GraphNode {
public:
    std::string name;
    FriendsLinkedList l;
    int index;
    GraphNode()
    {
        this -> name = "";
        this -> l = FriendsLinkedList();
        this -> index = -1;
    }
    ~GraphNode(){}
    GraphNode(std::string name, FriendsLinkedList l, int index)
    {
        this -> name = name;
        this -> l = l;
        this -> index = index;
    }
};


class FriendshipGraph {
private:
    int hashvalue;
    GraphNode *array;
	int count[TABLE_SIZE]; //Extra Feature
   	int freq[3]; //Extra Feature
public:
    FriendshipGraph()
    {
        array = new GraphNode[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            array[i].name = "";
        }
		// Extra Feature
		for (int i = 0; i < TABLE_SIZE; i++)
		{
		count[i] = 0;
		}
    }
    ~FriendshipGraph(){}
    int hash(std::string str, int seed = 0);
    void insert(GraphNode n);
    int getHash(std::string name);
    int getIndex(std::string name);
    int countFriends(std::string name);
    void printFriendList(std::string name);
    void print();
    void addFriendList(std::string name1, std::string name2);
    GraphNode getGraphNode(std::string name);
	// Extra features
   	void unFriend(std::string name1, std::string name2);
   	void mutualFriends(std::string namelist, std::string name2);
   	void CountMutualFriends(std::string namelist, std::string name2);
   	void SortCount();
   	int* getFreq();
   	int* getCount();
	void clearFreqCountArrays();

	void printRelList(std::string name);
	void addRelList(std::string name1, std::string name2, std::string relationship);
	void unRelationship(std::string name1, std::string name2);
};



#endif /* defined(___30A_Project__FriendshipGraph__) */
