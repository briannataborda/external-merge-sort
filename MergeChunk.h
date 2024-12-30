/*
 * MergeChunk.h
 * Brianna Taborda (btabor01)
 * 
 * The purpose of this class is to merge sorted binary files and produce a
 * master sorted file containing all of the ints in these files.
*/

#ifndef _MERGECHUNK_H_
#define _MERGECHUNK_H_

#include "SortChunk.h"
#include <queue>

struct ChunkElement { //will be used in the priority queue for deciding
    int value;
    size_t fileIndex;

    bool operator>(const ChunkElement& other) const { 
        return (value > other.value); 
    }
};

class MergeChunk 
{
public:
    void merge(std::vector<std::string>& inputFiles, std::string& outputFile);
private:
    void updateBuffer(std::ifstream& file, std::vector<int>& buffer, int &sizeRead);
    std::vector<int> readChunk(std::ifstream& ifstream, int chunkSize); //helper for reading a chunk of data 
    const size_t BUFFERSIZE = 100;
};
#endif