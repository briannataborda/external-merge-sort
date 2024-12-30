/*
 * Sort.cpp
 * Brianna Taborda (btabor01)
 *
 * This is the implementation of the Sort class which acts as a sort of master
 * class for the rest of the classes in this program. createChunks uses methods
 * from the DataSort class in order to generate the data given as input and
 * write it to the appropriate number of binary files. sortChunks uses methods
 * from the SortChunk class which then opens the same binary files that were
 * created in createChunks, and sort the data in it and writes it back to them.
 * mergeChunks uses methods from the MergeChunk class where it creates the
 * vector of binary files and then merges their data and writes it to a
 * specified output file. doSort calls all of these function and contains the
 * relevant cout statements to show if the data in the merged output file is
 * sorted. 
*/

#include "Sort.h"

using namespace std;

/*
 * name:      doSort
 * purpose:   performs all operations needed to do external merge sort
 * arguments: two ints representing the number of ints and number of files
 * returns:   none
 * effects:   performs external merge sort
*/
void Sort::doSort(int fullSize, int numFiles) {    
    std::string outputFile = "mergedData.bin";
    
    createChunks(fullSize, numFiles);
    sortChunks(numFiles);
    mergeChunks(numFiles, outputFile);

    SortChunk sortchunk;
    bool merge = sortchunk.verifySort(outputFile);
    cout << "File mergedData.bin is sorted: " << std::boolalpha << merge << endl;
}

/*
 * name:      createChunks
 * purpose:   generates the data and writes it to binary files
 * arguments: two ints representing the number of ints and number of files
 * returns:   none
 * effects:   none
*/
void Sort::createChunks(int fullSize, int numFiles) {
    DataSort datasort;
    int chunkSize = fullSize/numFiles;
    std::vector<int> data = datasort.generateRand(fullSize);
    datasort.writeChunkFile(data, chunkSize); //prints out "Writing to ..."
}

/*
 * name:      sortChunks
 * purpose:   sorts the data in each file
 * arguments: an int representing the number of files
 * returns:   none
 * effects:   sorts the data in each file
*/
void Sort::sortChunks(int numFiles) {
    SortChunk sortchunk;
    for (int i = 0; i < numFiles; ++i) {
        std::string fileName = "data_" + std::to_string(i) + ".bin";
        sortchunk.readAndWrite(fileName);
    }
}

/*
 * name:      mergeChunks
 * purpose:   merges the chunks to one output file
 * arguments: an int representing the number of files and the output filename
 * returns:   none
 * effects:   same as purpose
*/
void Sort::mergeChunks(int numFiles, std::string& outputFile) {
    MergeChunk mergechunk;
    std::vector<std::string> inputFiles;

    // Create the list of sorted files to be merged
    for (int i = 0; i < numFiles; ++i) {
        inputFiles.push_back("data_" + std::to_string(i) + ".bin");
    }

    // Merge the sorted files into the final output file
    mergechunk.merge(inputFiles, outputFile);
}