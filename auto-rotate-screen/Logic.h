#ifndef LOGIC_H
#define LOGIC_H

#include <iostream>
#include <chrono>
#include <QObject>
#include <QTimer>
#include <QTableWidgetItem>

#include "MainWindow.h"
#include "Settings.h"

#include "WinOSApsSensor.h"
#include "WinOSFunctions.h"
#include "DiskStoring.h"
#include "OrientSettings.h"


class MainWindow;


using std::string;
using namespace std::chrono;

class Logic : public QObject
{

    Q_OBJECT

signals:
    void signalToStatusBar(QString);

public:

    enum settingTypes {minX, maxX, minY, maxY};
    enum rotationMethod{Rotation_W32, Rotation_KeyBSC};

    Logic(int argc, char* argv[], WinOSFunctions* osFunctRef, MainWindow* mWin);
    bool isSensorDetected(){ return sensor_detected;}
    bool isCalibrated(){ return calibrated;}

    bool isAutoRotationEnabled(){ return auto_rotation_status;}
    bool isCalibratingNow(){return calibration_in_progress;}
    bool isDataSaved(){return saved_data.isSettingsSaved();}


    bool shdShowMainWindow(){return toShowMainWindow;}

    void rotateToLandscape();
    void rotateToPortrait();
    void rotateToLandscapeFlipped();
    void rotateToPortraitFlipped();

    int obtainPollRate(){return saved_data.getPollRate();}
    int obtainDelayRate(){return saved_data.getScreenDelayRate();}
    int obtainRotationMethod(){return saved_data.getRotationMethod();}

    void setPollRate(int newPollRate);
    void setDelayRate(int newDelayRate);
    void setRotationMethod(rotationMethod);

    bool isOrientInUse(int orient);
    int obtainCurrentOrient();

    int getTableElements(int orient, settingTypes typeWanted);
    int getCalibElements(settingTypes typeWanted);

    bool toggleAutoRotationStatus();
    bool toggleCalibrationStatus();

    void toggleEnableForOrient(int);

    int calibratingThisOrient(){ return nowCalibratingOrient;}

    int obtainPostRotationAttemptStatus(){return postRotationAttemptStatus;}


    void setDataFlagToSaveWithoutSaving(){ saved_data.setDataSaveStatus(true);}
    bool saveSettings();

    void stopAutoRotation(){auto_rotation_status = false;}



private:
    WinOSFunctions* osFunctions;
    MainWindow* mWindow;
    WinOSApsSensor sensorlink;
    DiskStoring fileLink;

    Settings saved_data;

    bool sensor_detected = true;
    bool auto_rotation_status = true;
    bool calibration_in_progress = false;
    bool calibrated = true;

    bool toShowMainWindow = true;

    int nowCalibratingOrient = -1;

    int postRotationAttemptStatus = 0;

    OrientSettings calibrationProcess;

    QTimer* timerPoll;

    steady_clock::time_point timeChangeDetected;
    int newOrientAtTime;


    string generateStatusText();

    void getStatusTextSlot();
    void autoRotate();
    void calibrateInProgress();

    string generateOverlapRangeString(int);

    void postRotationAttemptErrorMessage(int);

    void translateRotateOrientIntegersIntoRightMethodCalls(int orient);

    void restartPollRateTimer(int pollRate);



public slots:
    void onPollRateHit();







};

#endif // LOGIC_H
