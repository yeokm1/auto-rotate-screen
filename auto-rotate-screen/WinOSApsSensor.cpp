#include "WinOSApsSensor.h"

WinOSApsSensor::WinOSApsSensor()
{
}


bool WinOSApsSensor::isSensorDllDetected() {
    if(sensorlib == nullptr) {
        return false;
    } else {
        return true;
    }
}


//sensorlib must not be NULL
void WinOSApsSensor::initialiseSensorReading() {


    getData = (ShockproofGetAccelerometerData) GetProcAddress(sensorlib, "ShockproofGetAccelerometerData");
    getData(&inputdata);
}

SensorData WinOSApsSensor::getSensorData(){

    SensorData sd;
    if(isSensorDllDetected() == false) {
        return sd;
    }

    getData(&inputdata);



    sd.x = inputdata.x;
    sd.y = inputdata.y;
    sd.temp = (int) inputdata.temp;
    sd.status = inputdata.status;
    sd.statusMessage = translateStatus(inputdata.status);

    return sd;
}

string WinOSApsSensor::translateStatus(int statusNum)
{
    if((0 <= statusNum) && (statusNum <= 4) ) {
        return "HDD Running";
    } else if((8 <= statusNum) && (statusNum <= 9)) {
        return "HDD Stopped";
    } else if(statusNum == 10){
        return "APS Disabled";
    } else if(statusNum == 13) {
        return "Auto Ignore";
    }

    return "Unknown Status Number";
}
