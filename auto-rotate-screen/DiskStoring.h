#ifndef DISKSTORING_H
#define DISKSTORING_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Settings.h"
#include "constants.h"


using std::string;
using std::vector;

using std::ifstream;
using std::ofstream;
using std::stringstream;

using std::getline;
using std::endl;

class DiskStoring
{


private:
    const string filename = FILE_NAME;
    vector<int> splitString(string input, int numValues);



public:
    DiskStoring();
    bool readFileIfExists(Settings *toBeStored);
    bool saveFile(Settings *toBeStored);

};

#endif // DISKSTORING_H
