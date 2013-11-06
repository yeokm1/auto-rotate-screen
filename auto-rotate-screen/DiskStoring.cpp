#include "DiskStoring.h"

DiskStoring::DiskStoring()
{
}


vector<int> DiskStoring::splitString(string input, int numValues) {
    stringstream stream(input);
    vector<int> values(numValues);



    for(int i = 0, temp = 0; i < numValues; i++){
        stream >> temp;

        if (stream.peek() == ' '){
           stream.ignore();
        }

        values[i] = temp;
    }


    return values;
}

bool DiskStoring::readFileIfExists(Settings *toBeStored)
{
   ifstream inputFile;
   inputFile.open(filename);

   if(!inputFile.is_open()) {
       return false;
   }
   string inputLine;

   for(int i = 0; i < 4; i++) {
       getline(inputFile, inputLine);
       vector<int> lineValues = splitString(inputLine, 5);

       bool orientEnable = (lineValues[4] != 0);
       toBeStored->setInitialSettings(i, lineValues[0], lineValues[1], lineValues[2], lineValues[3], orientEnable);

   }

   getline(inputFile, inputLine);
   vector<int> lineValues = splitString(inputLine, 3);
   toBeStored->setRotationMethod(lineValues[0]);
   toBeStored->setPollRate(lineValues[1]);
   toBeStored->setDelayRate(lineValues[2]);

   return true;
}


bool DiskStoring::saveFile(Settings *toBeStored) {
    ofstream outputFile;
    outputFile.open(filename);

    if(!outputFile.is_open()) {
        return false;
    }


    for(int orient = 0; orient < 4; orient++ ) {
        OrientSettings toBeSaved = toBeStored->getCalibSettings(orient);

        stringstream line;

        line << toBeSaved.min_x;
        line << " ";

        line << toBeSaved.max_x;
        line << " ";

        line << toBeSaved.min_y;
        line << " ";

        line << toBeSaved.max_y;
        line << " ";


        if(toBeSaved.in_use){
            line << 1;
        } else {
            line << 0;
        }

        string outputString = line.str();
        outputFile << outputString << endl;

    }

    outputFile << toBeStored->getRotationMethod();

    outputFile << " ";

    outputFile << toBeStored->getPollRate();

    outputFile << " ";

    outputFile << toBeStored->getScreenDelayRate();

    toBeStored->setDataSaveStatus(true);

    outputFile.close();

    return true;
}
