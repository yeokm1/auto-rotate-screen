#ifndef STOREDSETTINGS_H
#define STOREDSETTINGS_H
#include "OrientSettings.h"


class Settings
{

static const int POLL_RATE_DEF_RATE = 100;
static const int ROTATION_DELAY_DEF_RATE = 1500;


public:

    const int ROTATION_METHOD_W32 = 0;
    const int ROTATION_METHOD_KEYB_SHORTCUT = 1;


    Settings();



    void setInitialSettings(int orient, int minX, int maxX, int minY, int maxY, bool enabled);

    void setCalibSettings(int orient, int minX, int maxX, int minY, int maxY);
    void toggleOrientEnable(int orient);

    void setPollRate(int newRotationRate){sensorPollRate = newRotationRate; isSaved = false;}
    void setDelayRate(int newDelayRate){screenRotationDelay = newDelayRate; isSaved = false;}
    void setRotationMethod(int newRotationMethod){rotationMethod = newRotationMethod; isSaved = false;}


    int getPollRate(){return sensorPollRate;}
    int getScreenDelayRate(){return screenRotationDelay;}

    int getRotationMethod(){return rotationMethod;}


    OrientSettings getCalibSettings(int orient){return data[orient];}
    void reset();
    void setDataSaveStatus(bool newStatus){isSaved = newStatus;}
    bool isSettingsSaved(){return isSaved;}


private:

    bool isSaved = true;
    int sensorPollRate;
    int screenRotationDelay;
    int rotationMethod;
    OrientSettings data[4];

};

#endif // STOREDSETTINGS_H
