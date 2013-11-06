#ifndef WINOSAPSSENSOR_H
#define WINOSAPSSENSOR_H
#define _WIN32_WINNT 0x0500

#include <iostream>
#include <windows.h>
#include "SensorData.h"
using std::string;



class WinOSApsSensor
{

struct accdata_t {
    int status;
    short x;
    short y;

    short varx;//Except for temp, Here onwards is actually useless, just to avoid variable overflow
    short vary;
    char temp;

    short x0;
    short y0;

};


typedef void (__stdcall *ShockproofGetAccelerometerData)(accdata_t* accData);

//FOR APS outputdata

public:

    WinOSApsSensor();
    bool isSensorDllDetected();
    void initialiseSensorReading();
    SensorData getSensorData();


private:

    ShockproofGetAccelerometerData getData;
    string translateStatus(int statusNum);
    accdata_t inputdata = {0,0,0,0,0,0,0,0};
    HINSTANCE sensorlib = LoadLibrary(L"sensor.dll");

};



#endif // WINOSAPSSENSOR_H
