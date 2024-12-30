/*
 *  DataSort.cpp
 *  Brianna Taborda
 *
 * This is the implementation of the DataSort class and handles operations 
 * related to reading and writing binary files, generating random data, 
 * verifying the sorting of data, and managing file chunks. generateRand returns
 * a vector of ints (a chunk) where the order is shuffled and is maintained in a
 * range from 1 to the size given. writeChunkFile writes each chunk to its own
 * binary file and uses writeFile as a helper function for modularity.
 * readBinaryFile reads in binary files and converts the binary numbers into  
 * ints and returns a vector of those ints and was used extensively in unit 
 * testing for that reason.
*/

#include "DataSort.h"

/*
 * name:      writeChunkFile
 * purpose:   writes data to file
 * arguments: a vector of ints (all chunks) and the size of each chunk
 * returns:   none
 * effects:   writes chunks to its own file
*/
void DataSort::writeChunkFile(std::vector<int> &data, int chunkSize) {
    int chunkNum = (data.size())/chunkSize; //total # of chunks
    
    //write each chunk to its individual file
    for (int i = 0; i < chunkNum; i++) {
        //calculate indices of the chunk
        int start = i*chunkSize;
        int end = (i +1) * chunkSize;
        std::vector<int> chunk(data.begin() + start, data.begin() + end);

        std::string fileName = "data_" + std::to_string(i) + ".bin";
        cout << "Writing data to data_"<< std::to_string(i) << ".bin" << endl;
        writeFile(fileName, chunk);
    }
}

/*
 * name:      writeFile
 * purpose:   helper for writeChunkFile
 * arguments: name of the file to work with and a 'chunk' of data of any size
 * returns:   none
 * effects:   none
*/
void DataSort::writeFile(std::string& fileName, std::vector<int>& data) {
    
    //create binary file, open in binary mode, and check that it's open
    std::ofstream outfile(fileName, std::ios::binary);
    if (!outfile.is_open()) {
        std::runtime_error("Error: Could not open" + fileName);
    }

    size_t size = data.size();

    //write size of file first and then write the ints
    outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
    outfile.write(reinterpret_cast<const char*>(data.data()), size * sizeof(int));

    outfile.close();
}

/*
 * name:      generateRand
 * purpose:   Generate a vector of shuffled integers of fullSize
 * arguments: Int representing the full size value inputted
 * returns:   a vector of shuffled integers
 * effects:   none
*/
std::vector<int> DataSort::generateRand(int fullSize) {
    std::vector<int> data(fullSize); //initializes the vector of size fullSize
    std::iota(data.begin(), data.end(), 1); //fills the vector with sequential ints

    random_device rd;
    mt19937 g(rd());
    shuffle(data.begin(), data.end(), g); //shuffles the ints in the vector

    return data;
}

/*
 * name:      readBinaryFile
 * purpose:   helper for reading data from an open binary file, testing
 * arguments: the name of an open binaryfile
 * returns:   a vector of ints containing the data from the binary file
 * effects:   reads the chunk from the binary file and puts in a vector of ints
*/
std::vector<int> DataSort::readBinaryFile(const std::string& fileName) {
    std::ifstream inFile(fileName, std::ios::binary);
    if (!inFile.is_open()) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    //reading in the size
    size_t size;
    inFile.read(reinterpret_cast<char*>(&size), sizeof(size));

    //read the integers into a vector
    std::vector<int> data(size);
    inFile.read(reinterpret_cast<char*>(data.data()), size * sizeof(int));

    inFile.close();
    return data;
}