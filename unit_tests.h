/*
 * Brianna Taborda (btabor01)
 * 
 * Unit testing to ensure the funtionality and accuracy of Sort.
*/

#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include "DataSort.h"
#include "SortChunk.h"
#include "MergeChunk.h"

/* 
*   DataSort Tests
*/

//tests that the random ints are being produced correctly
void generateRand_test() {
    DataSort datasort;
    size_t fullSize = 10;

    std::vector<int> test = datasort.generateRand(fullSize);
    for (int i = 0; i < 10; i++) {
        cout << test[i] << endl; //prints out each digit
    }

    assert(test.size() == fullSize); //asserts the sizes are equal
}

//tests that writeFile works properly
void writeFile_test() {
    DataSort datasort;
    std::string fileName = "test_output.bin";
    std::vector<int> test = datasort.generateRand(10); //create vector

    datasort.writeFile(fileName, test); //write to binary file
    std::vector<int> readTest = datasort.readBinaryFile(fileName); //read it into a diff vector

    assert(test == readTest);
}

//tests that readChunk works properly
void readChunk_test() {
    DataSort datasort;
    std::string fileName = "test_chunk.bin";
    std::vector<int> test = datasort.generateRand(10);

    //write test data to binary file
    datasort.writeFile(fileName, test);

    //read the binary file and populate a different vector
    std::vector<int> totalChunk = datasort.readBinaryFile(fileName);
    assert(totalChunk == test);

    //open file
    std::ifstream inFile(fileName, std::ios::binary);
    if (!inFile.is_open()){
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    //read in size header
    size_t size;
    inFile.read(reinterpret_cast<char*>(&size), sizeof(size));

    int chunkSize = 2;
    std::vector<int> combinedChunk;

    while (true) { //changed from eof to ensure last chunk gets read
        std::vector<int> chunk = datasort.readChunk(inFile, chunkSize);
        if (chunk.empty()) {
            break; //if no data is read, then exit
        }
        combinedChunk.insert(combinedChunk.end(), chunk.begin(), chunk.end());
    }
    inFile.close();
    assert(combinedChunk == totalChunk);

}

//tests writeChunkFile
void writeChunkFile_test() {
    DataSort datasort;
    int chunkSize = 2;
    std::vector<int> test = datasort.generateRand(10);
    datasort.writeChunkFile(test, chunkSize); //write data to a binary file

    int chunkNum = test.size()/chunkSize; //numChunk = # of files

    std::vector<int> combinedChunk;
    for (int i = 0; i < chunkNum; i++) {
        //creating separate files and reading them for a different vector
        std::string chunkFileName = "data_" + std::to_string(i) + ".bin";
        std::vector<int> chunk = datasort.readBinaryFile(chunkFileName);

        //expected chunk
        int start = i * chunkSize;
        int end = (i + 1) * chunkSize;
        std::vector<int> expChunk(test.begin() + start, test.begin() + end);

        assert(chunk == expChunk);

        //making sure the combined chunks are the same as what we started w/
        combinedChunk.insert(combinedChunk.end(), chunk.begin(), chunk.end());
    }
    assert(test == combinedChunk);
}

//tests that verifySort works
void verifySort_test() {
    DataSort datasort;

    //writes a binary file that is in ascending order
    std::vector<int> testTrue = {1,2,3,4,5};
    std::string fileTrue = "fileTrue";
    datasort.writeFile(fileTrue, testTrue);

    //writes a binary file NOT in ascending order
    std::vector<int> testFalse = {3,4,5,2,1};
    std::string fileFalse = "fileFalse";
    datasort.writeFile(fileFalse, testFalse);

    assert(datasort.verifySort(fileTrue) == true);
    assert(datasort.verifySort(fileFalse) == false);
}

/*
*   Sort Chunk Tests
*/

//tests insertionSort
void insertionSort_test() {
    SortChunk sortchunk;
    DataSort datasort;
    std::vector<int> test = datasort.generateRand(10);
    cout << "Before Sorting" << endl;
    for (int i = 0; i < 10; i++) {
        cout << test[i] << endl; //prints out each digit
    }

    sortchunk.insertionSort(test);
    cout << "After Sorting" << endl;
    for (int i = 0; i < 10; i++) {
        cout << test[i] << endl; //prints out each digit
    }

    std::string fileName = "insertFile";
    datasort.writeFile(fileName, test);
    assert(datasort.verifySort(fileName) == true);
}

// //tests readAndWrite for an unsorted chunk, a sorted chunk, and a descending chunk
void readAndWrite_test() {
    SortChunk sortchunk;
    DataSort datasort;

    std::vector<int> test = datasort.generateRand(10);
    std::string fileName = "insertFile";
    datasort.writeFile(fileName, test);
    sortchunk.readAndWrite(fileName);

    std::vector<int> testSorted = {1,2,3,4,5,6,7,8,9,10};
    std::string fileName1 = "insertFileSort";
    datasort.writeFile(fileName1, testSorted);
    sortchunk.readAndWrite(fileName1);

    std::vector<int> testDescending = {10,9,8,7,6,5,4,3,2,1};
    std::string fileName2 = "insertFileDes";
    datasort.writeFile(fileName2, testDescending);
    sortchunk.readAndWrite(fileName2);

    assert(datasort.verifySort(fileName) == true);
    assert(datasort.verifySort(fileName1) == true);
    assert(datasort.verifySort(fileName2) == true);
}

//tests exception for readAndWrite is thrown on a chunk with more than 1000 ints
//exception was thrown, commenting out for ease of other tests
void readAndWrite_over1000_test() {
    SortChunk sortchunk;
    DataSort datasort;
    std::vector<int> test = datasort.generateRand(1005);
    std::string fileName = "insertFile";
    datasort.writeFile(fileName, test);
    sortchunk.readAndWrite(fileName);
}

/*
*   MergeChunk tests
*/

/*                             updateBuffer Tests                             */

//tests updateBuffer
void updateBuffer_test() {
    DataSort datasort;
    SortChunk sortchunk;
    MergeChunk mergechunk;

    //create test chunk and buffer
    std::vector<int> test = datasort.generateRand(150);
    std::vector<int> buffer(50);

    //write test chunk to a file and read the contents into a different file
    std::string testFile = "inputFile";
    datasort.writeFile(testFile, test);
    std::vector<int> testCheck = datasort.readBinaryFile(testFile);

    //open the binary file and call the updated buffer
    std::ifstream inFile(testFile, std::ios::binary);
        if (!inFile.is_open()) {
            cerr << "Error: Could not open file " << testFile << endl;
            return;
        }
    //size_t numRead = mergechunk.updateBuffer(inFile, buffer);
    mergechunk.updateBuffer(inFile, buffer);
    assert(testCheck == test);
    //assert(numRead == 100);
    for (int i = 0; i < 50; i++) { //doesnt work when BUFFERSIZE is not 50
        assert(buffer[i] == test[i]);
    }
    inFile.close();
}

//tests updateBuffer when there is less data in vector than buffer size
void updateBuffer_fewer_test() {
    DataSort datasort;
    SortChunk sortchunk;
    MergeChunk mergechunk;

    //create test chunk and buffer
    std::vector<int> test = {3,1,2};
    std::vector<int> buffer(100);

    //write test chunk to a file and read the contents into a different file
    std::string testFile = "inputFile";
    datasort.writeFile(testFile, test);
    std::vector<int> testCheck = datasort.readBinaryFile(testFile);

    //open the binary file and call the updated buffer
    std::ifstream inFile(testFile, std::ios::binary);
        if (!inFile.is_open()) {
            cerr << "Error: Could not open file " << testFile << endl;
            return;
        }
    size_t numRead = mergechunk.updateBuffer(inFile, buffer);

    assert(numRead == 3);
    for (int i = 0; i < 3; i++) {
        assert(buffer[i] == test[i]);
    }
    inFile.close();
}

//tests updateBuffer when there is equal data to buffer size
void updateBuffer_equal_test() {
    DataSort datasort;
    SortChunk sortchunk;
    MergeChunk mergechunk;

    //create test chunk and buffer
    std::vector<int> test = datasort.generateRand(100);
    std::vector<int> buffer(100);

    //write test chunk to a file and read the contents into a different file
    std::string testFile = "inputFile";
    datasort.writeFile(testFile, test);
    std::vector<int> testCheck = datasort.readBinaryFile(testFile);

    //open the binary file and call the updated buffer
    std::ifstream inFile(testFile, std::ios::binary);
        if (!inFile.is_open()) {
            cerr << "Error: Could not open file " << testFile << endl;
            return;
        }
    size_t numRead = mergechunk.updateBuffer(inFile, buffer);

    assert(numRead == 100);
    for (int i = 0; i < 100; i++) {
        assert(buffer[i] == test[i]);
    }

    inFile.close();
}

//tests updateBuffer properly refills the buffer
void updateBuffer_refill_test() {
    DataSort datasort;
    SortChunk sortchunk;
    MergeChunk mergechunk;
    std::vector<int> buffer(5);
    std::vector<int> expFirst = {1,2,3,4,5};
    std::vector<int> expSecond = {6,7,8,9,10};
    std::vector<int> expThird = {11,12,13,14,15};
    std::vector<int> test = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

    std::string testFile = "testFile";
    datasort.writeFile(testFile, test);
    std::ifstream inFile(testFile, std::ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open file " << testFile<< endl;
        return;
    }    

    mergechunk.updateBuffer(inFile, buffer);
    //assert first buffer works correctly
    for (int i = 0; i < 5; i++) {
        assert(buffer[i] == expFirst[i]);
    }

    mergechunk.updateBuffer(inFile, buffer);
    //assert second buffer works correctly
    for (int i = 0; i < 5; i++) {
        assert(buffer[i] == expSecond[i]);
    }

    mergechunk.updateBuffer(inFile, buffer);
    //assert third buffer works correctly
    for (int i = 0; i < 5; i++) {
        assert(buffer[i] == expThird[i]);
    }

    inFile.close();
}

/*                                 merge Tests                                */

//tests merge function
void merge_test() {
    DataSort datasort;
    //SortChunk sortchunk;
    MergeChunk mergechunk;
    std::string outputFile = "outputFile";

    std::vector<int> first = {1,3,5,7,11};
    std::vector<int> second = {2,6,8,9,10};
    std::vector<int> third = {4,12,13,14,15};

    std::string test1 = "test1";
    std::string test2 = "test2";
    std::string test3 = "test3";

    datasort.writeFile(test1, first);
    datasort.writeFile(test2, second);
    datasort.writeFile(test3, third);

    std::vector<std::string> inputFiles = {test1, test2, test3};
    mergechunk.merge(inputFiles, outputFile);

    std::ifstream inFile(outputFile, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open output file for reading!" << std::endl;
        return;
    }

    size_t size;
    inFile.read(reinterpret_cast<char*>(&size), sizeof(size));  // Read size header
    std::vector<int> mergedData(size);
    inFile.read(reinterpret_cast<char*>(mergedData.data()), size * sizeof(int));  // Read merged data



    inFile.close();

    cout << "Merged Data: " << endl;
    for (size_t i = 0; i < mergedData.size(); i++) {
        cout << mergedData[i] << endl;
    }
}

//another test for merging
void merge_test2() {
    DataSort datasort;
    MergeChunk mergechunk;
    std::string outputFile = "outputFile";

    std::vector<int> first = {1,2,3,4,5,6,7,8,9,10};
    std::vector<int> second = {11,13,17,21,25,26,28,30,32,34};
    std::vector<int> third = {12,14,15,16,23,27,29,33,37,45};
    std::vector<int> fourth = {18,19,20,22,28,31,35,41,49,104};

    std::string test1 = "test1";
    std::string test2 = "test2";
    std::string test3 = "test3";

    datasort.writeFile(test1, first);
    datasort.writeFile(test2, second);
    datasort.writeFile(test3, third);

    std::vector<std::string> inputFiles = {test1, test2, test3};
    mergechunk.merge(inputFiles, outputFile);

    std::ifstream inFile(outputFile, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open output file for reading!" << std::endl;
        return;
    }

    size_t size;
    inFile.read(reinterpret_cast<char*>(&size), sizeof(size));  // Read size header
    std::vector<int> mergedData(size);
    inFile.read(reinterpret_cast<char*>(mergedData.data()), size * sizeof(int));  // Read merged data

    

    inFile.close();

    cout << "Merged Data: " << endl;
    for (size_t i = 0; i < mergedData.size(); i++) {
        cout << mergedData[i] << endl;
    }
}