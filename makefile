Main: Main.o BTree.o FriendshipGraph.o BTree.h FriendshipGraph.h
	g++ -w -o Main Main.o BTree.o FriendshipGraph.o

Main.o: Main.cpp
	g++ -w -c Main.cpp

BTree.o: BTree.cpp BTree.h
	g++ -w -c BTree.cpp

FriendshipGraph.o: FriendshipGraph.cpp FriendshipGraph.h
	g++ -w -c FriendshipGraph.cpp

clean:
	rm *.o
