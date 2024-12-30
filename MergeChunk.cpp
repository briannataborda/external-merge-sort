/*
 * MergeChunk.cpp
 * Brianna Taborda (btabor01)
 * 
 * This is the implementation of the MergeChunk class which uses a priority
 * queue that implements a min heap in order to perform the sorting of these
 * multiple binary files. updateBuffer clears and updates the buffers that are
 * created in merge. This function makes sure to take into account to read the
 * size header of each file once in order to ensure that the file pointer is
 * accurate. It also uses the help of the function readChunk which reads
 * specified chunks of data from an open binary file. merge is where all the
 * magic happens and uses multiple vectors for different reasons. Essentially,
 * the function opens all input files, fills the buffer of each file and loads
 * the first (the smallest) element into the priority queue. In the priority
 * queue, these elements get sorted and then pushed onto a master vector of ints
 * which will be read to the master file. These buffers continuously fill as
 * they empty until they reach the end of their corresponding binary file. Once
 * all is done, the input files are all closed and the master vector of ints
 * gets written to the outputFile. 
*/

#include "MergeChunk.h"

/*
 * name:      merge
 * purpose:   merges all of the files together into one output file
 * arguments: a vector of input files and the output file
 * returns:   none
 * effects:   merges all of the files together into one output file
*/
void MergeChunk::merge(std::vector<string>& inputFiles, std::string& outputFile) {
    DataSort datasort;
    std::vector<int> mergedChunks; 
    std::vector<bool> filesProcessed(inputFiles.size(), false); //tracks if a file is fully read
    std::vector<int> isSizeRead(inputFiles.size(), 0); //tracks if a buffers size has been read

    //the greater operator ensures the priority queue behaves like a min-heap not max
    std::priority_queue<ChunkElement, std::vector<ChunkElement>, std::greater<ChunkElement>> minHeap;

    //creating file streams for each input file and vector of buffers
    std::vector<std::ifstream> inputFileStreams(inputFiles.size());
    std::vector<std::vector<int>> buffers(inputFiles.size());

    //open input files
    for (size_t i = 0; i < inputFiles.size(); i++) {
        inputFileStreams[i].open(inputFiles[i], std::ios::binary);
        if (!inputFileStreams[i].is_open()) {
            throw std::runtime_error("Error. Could not open: " + inputFiles[i]);
        }
        updateBuffer(inputFileStreams[i], buffers[i], isSizeRead[i]); //populate buffers

        //put first element of each file into min-heap and erase it from buffer
        if (!buffers[i].empty()) {
            minHeap.push(ChunkElement{buffers[i][0], i}); //ChunkElement from i-th file
            buffers[i].erase(buffers[i].begin());
        }
    }

    //while minHeap is not empty
    while (!minHeap.empty()) {
        //get smallest value, put into master chunk, and then pop off min heap
        ChunkElement min = minHeap.top();
        mergedChunks.push_back(min.value);
        minHeap.pop();

        //read next element from same file and push to minHeap
        size_t fileIndex = min.fileIndex;
        if (!buffers[fileIndex].empty()) {
            minHeap.push(ChunkElement{buffers[fileIndex][0], fileIndex}); 
            buffers[fileIndex].erase(buffers[fileIndex].begin());
        //otherwise update buffer and push to minHeap    
        } else {
            updateBuffer(inputFileStreams[fileIndex], buffers[fileIndex], isSizeRead[fileIndex]);
            //if buffer for file is empty
            if (!buffers[fileIndex].empty()) {
                minHeap.push(ChunkElement{buffers[fileIndex][0], fileIndex}); 
                buffers[fileIndex].erase(buffers[fileIndex].begin()); //remove the first element after pushing to heap
            }
            //buffer and file are empty
            if (buffers[fileIndex].empty() and inputFileStreams[fileIndex].eof()) {
                filesProcessed[fileIndex] = true;
            }
        }  
    }
    //close all file streams
        for (size_t i = 0; i < inputFileStreams.size(); i++) {
            inputFileStreams[i].close();
        }
    //write master sorted chunk to output file
    datasort.writeFile(outputFile, mergedChunks);
    cout << "File " << outputFile << " contains " << mergedChunks.size() << " elements." << endl;
}

/*
 * name:      updateBuffer
 * purpose:   will update the buffer for a file when called
 * arguments: a file, its corresponding buffer, an int value representing
 *            whether or not the size header for a buffer has been read
 * returns:   size_t representing the number of ints read from the file
 * effects:   updates the buffer
*/
void MergeChunk::updateBuffer(std::ifstream& file, std::vector<int>& buffer, int& sizeRead) {
    size_t size; //size header

    //ensure buffer is cleared
    buffer.clear();

    //read in size so it's not read as an int to the buffer but doesnt repeat
    if (sizeRead == 0) {
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        sizeRead = 1;
    }

    //fill buffer
    buffer = readChunk(file, BUFFERSIZE);
}

/*
 * name:      readChunk
 * purpose:   helper for reading chunks of data from an open binary file
 * arguments: an open binary file and the size of a chunk of data
 * returns:   a vector of ints from the binary file of size chunkSize
 * effects:   writes the chunk into a binary file
*/
std::vector<int> MergeChunk::readChunk(std::ifstream& ifstream, int chunkSize) {
    if (!ifstream.is_open()) {
        throw std::runtime_error("Error: File stream is not open");
    }

    std::vector<int> buffer(chunkSize); //create buffer

    //read in number of ints
    ifstream.read(reinterpret_cast<char*>(buffer.data()), chunkSize * sizeof(int));

    //takes into account that the last chunk is included 
    buffer.resize(ifstream.gcount() / sizeof(int));
    return buffer;
}