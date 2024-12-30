/*
 * SortChunk.cpp
 * Brianna Taborda (btabor01)
 * 
 * This the implementation of the SortChunk class which sorts data in a given
 * binary file. verifySort is a helper function that opens a binary file, reads
 * in the data, and returns true if the data is sorted and false if it's not.
 * insertionSort is another helper function that takes in a chunk of data,
 * and performs the insertion sorting algorithm on the integers in the chunk.
 * readAndWrite takes in a binary file, prints out whether the file is sorted or
 * not, reads the data into a vector of ints and calls insertionSort on it.
 * It then writed the sorted chunk back to the same binary file, overwriting the
 * unsorted data from before.
*/

#include "SortChunk.h"

/*
 * name:      readAndWrite
 * purpose:   reads in a binary file, sorts it, and writes the sorted data back
 * arguments: a binary file
 * returns:   none
 * effects:   sorts a binary file
*/
void SortChunk::readAndWrite(std::string &fileName) {
    DataSort datasort;
    std::vector<int> chunkFile = datasort.readBinaryFile(fileName); //read file
    if (chunkFile.size() > 1000) {
        throw std::runtime_error("Chunk exceeds limit of 1,000 integers.");
    }

    //verify if data is sorted before sorting
    bool before = verifySort(fileName);
    cout << "File " << fileName << " is sorted: " << std::boolalpha << before << endl;

    //sort the ints and write sorted data back to file
    insertionSort(chunkFile); 
    datasort.writeFile(fileName, chunkFile);
    cout << "File " << fileName << " contains " << chunkFile.size() << " elements." << endl;

    //verify data is sorted and print out
    bool after = verifySort(fileName);
    cout << "File " << fileName << " is sorted: " << std::boolalpha << after << endl;
}

/*
 * name:      insertionSort
 * purpose:   recursively sort the chunk by splitting it
 * arguments: a chunk, two ints
 * returns:   none
 * effects:   sorts a chunk
*/
void SortChunk::insertionSort(std::vector<int>& data) {
    for (size_t i = 1; i < data.size(); i++) {
        int key = data[i];
        int j = i - 1;

        //moves elements greater than key one position ahead
        while (j >= 0 and data[j] > key) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

/*
 * name:      verifySort
 * purpose:   verifies if a file has been sorted properly
 * arguments: the name of the file to be read
 * returns:   none
 * effects:   none
*/
bool SortChunk::verifySort(std::string& fileName) {
    std::ifstream inFile(fileName, std::ios::binary);
    if (!inFile.is_open()) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    //read in the size
    size_t size;
    inFile.read(reinterpret_cast<char*>(&size), sizeof(size));

    //set indexes for the binary file and read in those positions
    int curr, prev;
    inFile.read(reinterpret_cast<char*>(&prev), sizeof(int));

    //checks if the ints in the binary file are in ascending order
    for (size_t i = 0; i < size; i++) {
        inFile.read(reinterpret_cast<char*>(&curr), sizeof(int)); //changes curr to next value everytime
        if (prev > curr) {
            inFile.close();
            return false;
        }
    }
    inFile.close();
    return true;
}
