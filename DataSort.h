/*
 * DataSort.h
 * Brianna Taborda (btabor01)
 * 
 * The purpose of this class is to deal with binary files such as managing file
 * chunks, reading, and writing. 
*/

#ifndef _DATASORT_H_
#define _DATASORT_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <bits/stdc++.h>
using namespace std;

class DataSort
{
public:
    std::vector<int> generateRand(int fullSize); //generates and shuffles vector of ints
    void writeChunkFile(std::vector<int> &data, int chunkNum); //writes sorted data back to file
    std::vector<int> readBinaryFile(const std::string& fileName); //reads in a binary file
    void writeFile(std::string& fileName, std::vector<int>& data); //helper for writing to a file  
};
#endif