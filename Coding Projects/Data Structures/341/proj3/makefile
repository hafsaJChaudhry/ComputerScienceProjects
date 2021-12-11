CXX = g++
CXXFLAGS = -Wall

proj3: test1.cpp CTree.h CTree.cpp
	$(CXX) $(CXXFLAGS) CTree.h CTree.cpp test1.cpp -o proj3


run:
	./proj3
clean:
	rm *.o*
	rm *~

valgrind:
	valgrind ./proj3