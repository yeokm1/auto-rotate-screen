#include "Settings.h"
Settings::Settings()
{
    reset();
}


void Settings::setInitialSettings(int orient, int minX, int maxX, int minY, int maxY, bool enabled)
{
    data[orient].min_x = minX;
    data[orient].max_x = maxX;
    data[orient].min_y = minY;
    data[orient].max_y = maxY;
    data[orient].in_use = enabled;
    data[orient].calibrated = true;
}


void Settings::setCalibSettings(int orient, int minX, int maxX, int minY, int maxY){
    data[orient].min_x = minX;
    data[orient].max_x = maxX;
    data[orient].min_y = minY;
    data[orient].max_y = maxY;
    data[orient].calibrated = true;

    isSaved = false;
}

void Settings::toggleOrientEnable(int orient){
    data[orient].in_use = !data[orient].in_use;

    isSaved = false;
}

void Settings::reset() {
    sensorPollRate = POLL_RATE_DEF_RATE;
    screenRotationDelay = ROTATION_DELAY_DEF_RATE;

    for(int i = 0; i < 4; i++){
        data[i].reset();
    }

    isSaved = false;
    rotationMethod = ROTATION_METHOD_W32;
}


