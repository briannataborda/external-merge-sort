/*
 * main.cpp
 * Brianna Taborda (btabor01)
 *
 * Main.cpp opens and closes the files, and checks for any errors when opening 
 * the files and throws them when needed. It calls doSort from the Sort class
 * which then performs all of the operations necessary. 
 */

#include <iostream>
#include "Sort.h"
using namespace std;

/*
 * name:      main
 * purpose:   starts the program
 * arguments: the files provided by the user (int argc and char *argv[])
 * returns:   an integer
 * effects:   calls Sort
*/
int main(int argc, char *argv[])
{
    if (argc != 3) {
            throw std::runtime_error("Usage: ./sort <FULL_SIZE> <NUM_FILES>");
        }
    Sort sorter;
    int fullSize = std::stoi(argv[1]);
    int numFiles = std::stoi(argv[2]);

    if ((numFiles == 0) or (fullSize % numFiles != 0)) {
        throw std::runtime_error("Usage: ./sort <FULL_SIZE> <NUM_FILES>");
    }

    sorter.doSort(fullSize, numFiles);
    return 0;
}