#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <string>

using std::string;

class SensorData
{
public:
    SensorData(){;}

    short x = 0;
    short y = 0;
    int status = -1;
    int temp = -1;

    string statusMessage = "No Sensor";

};

#endif // SENSORDATA_H
