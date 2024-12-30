/*
 * Sort.h
 * Brianna Taborda (btabor01)
 *
 * The purpose of this class is to use the functions of the classes DataSort,
 * SortChunk, and MergeChunk so that it can be called in main.cpp to allow for
 * more abstraction.
*/

#ifndef _SORT_H_
#define _SORT_H_

#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include "DataSort.h"
#include "MergeChunk.h"
#include "SortChunk.h"

class Sort
{
public:
    void doSort(int fullSize, int numFiles);
private:
    void createChunks(int fullSize, int numFiles);
    void sortChunks(int numFiles);
    void mergeChunks(int numFiles, std::string& outputFile);
};
#endif