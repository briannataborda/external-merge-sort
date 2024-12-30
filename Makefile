#########################################################
#                    PROJECT 1: Sort                    #
#########################################################

MAKEFLAGS += -L

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3

sort: main.o Sort.o DataSort.o SortChunk.o MergeChunk.o
	$(CXX) $(CXXFLAGS) -o sort $^

DataSort.o: DataSort.h DataSort.cpp
	$(CXX) $(CXXFLAGS) -c DataSort.cpp

SortChunk.o: SortChunk.h SortChunk.cpp DataSort.h
	$(CXX) $(CXXFLAGS) -c SortChunk.cpp

MergeChunk.o: MergeChunk.h MergeChunk.cpp SortChunk.h DataSort.h
	$(CXX) $(CXXFLAGS) -c MergeChunk.cpp

Sort.o: Sort.h Sort.cpp MergeChunk.h SortChunk.h DataSort.h
	$(CXX) $(CXXFLAGS) -c Sort.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

#The below rule will be used by unit tests
unit_test: unit_test_driver.o DataSort.o SortChunk.o MergeChunk.o
	$(CXX) $(CXXFLAGS) $^

clean:
	rm *.o *~ a.out
