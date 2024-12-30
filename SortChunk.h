/*
 * SortChunk.cpp
 * Brianna Taborda (btabor01)
 * 
 * The purpsoe of this class is to sort the data in a binary file.
*/

#ifndef _SORTCHUNK_H_
#define _SORTCHUNK_H_

#include "DataSort.h"
#include <iomanip>

class SortChunk 
{
public:
    void readAndWrite(std::string& fileName);
    bool verifySort(std::string& fileName); //checks if a file is sorted
private:
    void insertionSort(std::vector<int>& data);
};
#endif