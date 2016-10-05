//
//  FriendshipGraph.cpp
//  130A Project
//
//  Created by Xie Zuochen on 2/27/16.
//  Copyright (c) 2016 Xie Zuochen. All rights reserved.
//

#include "FriendshipGraph.h"

int FriendshipGraph::hash(std::string str, int seed )
{
    int hash = seed;
    for (int i = 0; i < str.length(); i++)
    {
        hash = (hash * 101 + str[i]) % TABLE_SIZE;
    }
    return hash;
}

int FriendshipGraph::getHash(std::string name)
{
    int i = hash(name, 0);
    bool found = false;
    do
    {
        if (array[i].name == name)
        {
            found = true;
            break;
        }
        i++;
        if (i == TABLE_SIZE)
        {
            i = 0;
        }
    }
    while (i != hash(name, 0));
    if(found)
    {
        return i;
    }
    else
    {
        std::cout << "Name Not Found" << std::endl;
        return -1;
    }
}

void FriendshipGraph::insert(GraphNode n)
{
    bool found = false;
    int i = 0;
    while ( i < TABLE_SIZE)
    {
        if (array[i].name == n.name)
        {
            found = true;
            break;
        }
        i++;
    }
    if (found == true)
    {
        std::cout << "Name already present" << std::endl;
        return;
    }
    hashvalue = hash(n.name, 0);
    if (array[hashvalue].name == "")
    {
        array[hashvalue] = n;
    }
    else
    {
        while (array[hashvalue].name != "")
        {
            hashvalue++;
            if (hashvalue == TABLE_SIZE)
            {
                hashvalue = 0;
            }
        }
        array[hashvalue] = n;
    }
}

int FriendshipGraph::getIndex(std::string name)
{
    return array[getHash(name)].index;
}

int FriendshipGraph::countFriends(std::string name)
{
    return array[getHash(name)].l.countList();
}

GraphNode FriendshipGraph::getGraphNode(std::string name)
{
	return array[getHash(name)];
}

void FriendshipGraph::printFriendList(std::string name)
{
	int index = getHash(name);
	if(index == -1) return;
	array[index].l.printList();
}

// Extra Feature 1
void FriendshipGraph::printRelList(std::string name)
{
	int index = getHash(name);
	if(index == -1) return;
	array[index].l.printRelList();
}

void FriendshipGraph::print()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (array[i].name == "")
        {
            ;
        }
        else
        {
        std::cout << array[i].name << "," << array[i].index << ",";
	    array[i].l.printList();
	    std::cout << std::endl;
        }
    }
}

void FriendshipGraph::addFriendList(std::string name1, std::string name2)
{
	int index1 = getHash(name1);
	int index2 = getHash(name2);
	if(index1 == -1 || index2 == -1) return;

	if (array[index1].l.checkFriend(name2))
    {
        std::cout << "Name2 Already Exist in Name1's List" << std::endl;
    }
    else
    {
        array[index1].l.addFriend(name2);
        std::cout << "Name2 Successfully inserted to Name1's List" << std::endl;
    }

    if (array[index2].l.checkFriend(name1))
    {
        std::cout << "Name1 Already Exist in Name2's List" << std::endl;
    }
    else
    {
        array[index2].l.addFriend(name1);
        std::cout << "Name1 Successfully inserted to Name2's List" << std::endl;
    }
}
// Extra Feature
void FriendshipGraph::addRelList(std::string name1, std::string name2, std::string relationship)
{
	int index1 = getHash(name1);
	int index2 = getHash(name2);
	if(index1 == -1 || index2 == -1) return;
	RelEnum e = RelHelper::getEnum(relationship);
	if(e == INVALID) {
		printf("Relationship string invalid!\n");
		return;
	}

	if (array[index1].l.checkFriend(name2)) {
		if (array[index1].l.checkRel(name2))
    	{
        	std::cout << "Updating Name1's relationship with Name2!" << std::endl;
			array[getHash(name1)].l.deleteRel(name2);
    	}
    	else
    	{
        	std::cout << "Name2 Successfully inserted to Name1's Relationship List" << std::endl;
    	}
		array[index1].l.addRel(name2, e);
	} else {
		printf("Name1 is not yet friends with Name2!\n");
	}

	if (array[index2].l.checkFriend(name1)) {
    	if (array[index2].l.checkRel(name1))
    	{
        	std::cout << "Updating Name2's relationship with Name1!" << std::endl;
			array[getHash(name2)].l.deleteRel(name1);
    	}
    	else
    	{
        	std::cout << "Name1 Successfully inserted to Name2's Relationship List" << std::endl;
    	}
		array[index2].l.addRel(name1, RelHelper::getRelPair(e));
	} else {
		printf("Name2 is not yet friends with Name1!\n");
	}
}

void FriendshipGraph::unRelationship(std::string name1, std::string name2)
{
    if (!array[getHash(name1)].l.checkRel(name2))
    {
        std::cout << "Name2 is not in Name1's Relationship List" << std::endl;
    }
    else
    {
        array[getHash(name1)].l.deleteRel(name2);
        std::cout << "Name2 Successfully deleted from Name1's Relationship List" << std::endl;
    }

    if (!array[getHash(name2)].l.checkRel(name1))
    {
        std::cout << "Name1 is not in Name2's Relationship List" << std::endl;
    }
    else
    {
        array[getHash(name2)].l.deleteRel(name1);
        std::cout << "Name1 Successfully deleted from Name2's Relationship List" << std::endl;
    }
}

// Extra Feature 1
void FriendshipGraph::unFriend(std::string name1, std::string name2)
{
	if(array[getHash(name1)].l.checkRel(name2)) {
		unRelationship(name1, name2);
	}
	
	if (!array[getHash(name1)].l.checkFriend(name2))
    {
        std::cout << "Name2 is not in Name1's List" << std::endl;
    }
    else
    {
        array[getHash(name1)].l.deleteFriend(name2);
        std::cout << "Name2 Successfully deleted from Name1's List" << std::endl;
    }

    if (!array[getHash(name2)].l.checkFriend(name1))
    {
        std::cout << "Name1 is not in Name2's List" << std::endl;
    }
    else
    {
        array[getHash(name2)].l.deleteFriend(name1);
        std::cout << "Name1 Successfully deleted from Name2's List" << std::endl;
    }
}

// Extra Feature 2
void FriendshipGraph::mutualFriends(std::string namelist, std::string name2)
{
    if(array[getHash(name2)].l.checkFriend(namelist))
    {
        std::cout << namelist << ", ";
    }
    else
    {
        return;
    }
}

// Extra Feature 3
void FriendshipGraph::CountMutualFriends(std::string namelist, std::string name2)
{
    if(array[getHash(namelist)].l.checkFriend(name2))
    {
        count[getIndex(name2)]++;
    }
    else
    {
        return;
    }
}

void FriendshipGraph::SortCount()
{
    for (int i = 0; i < 3; i++)
    {
        freq[i] = -1;
    }
    for (int i = 0;  i < TABLE_SIZE; i++)
    {
        if (freq[0] == -1 || count[i] >= count[freq[0]])
        {
            freq[2] = freq[1];
            freq[1] = freq[0];
            freq[0] = i;
        }
        else if ((freq[1] == -1) || (count[i] >= count[freq[1]]))
        {
            freq[2] = freq[1];
            freq[1] = i;
        }
        else if ((freq[2] == -1) || (count[i] >= count[freq[2]]))
        {
            freq[2] = i;
        }
    }
	if(getCount()[freq[0]] == 0) freq[0] = -1;
	if(getCount()[freq[1]] == 0) freq[1] = -1;
	if(getCount()[freq[2]] == 0) freq[2] = -1;
}

int* FriendshipGraph::getFreq()
{
    return freq;
}

int* FriendshipGraph::getCount()
{
    return count;
}

void FriendshipGraph::clearFreqCountArrays() {
	for(int i = 0; i < 3; i++) {
		freq[i] = -1;
	}
	for(int i = 0; i < TABLE_SIZE; i++) {
		count[i] = 0;
	}
}
