CXX = g++
CXXFLAGS = -Wall

proj4: JQTest.cpp JQueue.h JQueue.cpp
	$(CXX) $(CXXFLAGS) JQueue.h JQueue.cpp JQTest.cpp -o proj4

run:
	./proj4
clean:
	rm *.o*
	rm *~
val:
	valgrind ./proj4