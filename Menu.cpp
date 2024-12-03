#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <SortingAlgorithms.cpp>
using namespace std;

struct File {
    string fileName;
    string extension;
    int sizeKB;
    string category;
    string creationDate;
    string lastModifiedDate;
};
