#include <QMessageBox>

#include "Logic.h"
#include <stdio.h>


using std::string;
using std::ostringstream;

Logic::Logic(int argc, char* argv[], WinOSFunctions* osFunctRef, MainWindow* mWin)
{
    osFunctions = osFunctRef;
    mWindow = mWin;

    bool cmdline_s = false;
    bool cmdline_r = false;
    bool cmdline_m = false;


    for (int i = 0; i < argc; ++i) {
        string tempArg(argv[i]);
        if(tempArg.find("/s") != string::npos) cmdline_s = true;
        if(tempArg.find("/r") != string::npos) cmdline_r = true;
        if(tempArg.find("/m") != string::npos) cmdline_m = true;
        if(tempArg.find("/a") != string::npos) auto_rotation_status = false;
    }

    if(!sensorlink.isSensorDllDetected())
    {
        if(cmdline_s == false) {
            QMessageBox::critical(nullptr, "Cannot find APS library", "sensor.dll was not found. "
                                  "Please ensure that the Active Protection System software is installed. "
                                  "You can use the /s parameter to suppress this message." );
        }
        sensor_detected = false;
        auto_rotation_status = false;

    } else {
        sensorlink.initialiseSensorReading();
    }


    if(!fileLink.readFileIfExists(&saved_data))
    {
        if(cmdline_r == false) {
           QMessageBox::warning(nullptr, "Missing or corrupted configuration file",
                                "First launch or corrupted ars-config.ini configuration file in current directory, "
                                "please calibrate the program before use. You can use the /r parameter to suppress this message." );
        }
        calibrated = false;
        auto_rotation_status = false;

    }


    toShowMainWindow = (calibrated == false) && (cmdline_m == false);

    timerPoll = new QTimer(this);

    QObject::connect(timerPoll, SIGNAL(timeout()), this, SLOT(onPollRateHit()));

    QObject::connect(this, SIGNAL(signalToStatusBar(QString)), mWindow, SLOT(updateStatusbarAndTrayMenuLabels(QString)));


    saved_data.setDataSaveStatus(true);

    restartPollRateTimer(saved_data.getPollRate());


    timeChangeDetected = steady_clock::now();
    newOrientAtTime = osFunctions->getCurrentOrientation();
}

void Logic::restartPollRateTimer(int pollRate){
   //QT timer will auto stop and start timer with new Poll Rate
    timerPoll->start(pollRate);
}

bool Logic::toggleCalibrationStatus(){
    if((sensor_detected == false) || auto_rotation_status){
        return false;
    }

    if(calibration_in_progress){

        string overlapStatusMessage = generateOverlapRangeString(nowCalibratingOrient);
        if(!overlapStatusMessage.empty()){
            QMessageBox::warning(nullptr, "Current calibration data overlaps with other orientation", QString::fromStdString(overlapStatusMessage));
        }

        char temp[400] = {0};
        sprintf(temp, "Do you want to use the new calibration values?\n"
                "New Values for Orient %d:\nMinX = %03d, MaxX = %03d, MinY = %03d, MaxY = %03d\n",
                nowCalibratingOrient,
                calibrationProcess.min_x, calibrationProcess.max_x, calibrationProcess.min_y, calibrationProcess.max_y);

        string calibConfirmation(temp);

        int response = QMessageBox::question(nullptr, "Use new settings?", QString::fromStdString(calibConfirmation), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if(response == QMessageBox::Yes){
            saved_data.setCalibSettings(nowCalibratingOrient, calibrationProcess.min_x, calibrationProcess.max_x, calibrationProcess.min_y, calibrationProcess.max_y);
            calibrated = true;
        }
    } else {
        nowCalibratingOrient = osFunctions->getCurrentOrientation();
        calibrationProcess.reset();
    }


    calibration_in_progress  = !calibration_in_progress;
    return true;
}


//return nullptr if no overlap
string Logic::generateOverlapRangeString(int curr_orient){

    bool anyOverlap = false;
    string overlapstring = "Overlapping range(s)\n";

    for(int orient = 0; orient < 4; orient++)
    {
        int x_min_overlap = -1;
        int x_max_overlap = -1;
        int y_min_overlap = -1;
        int y_max_overlap = -1;

        bool xoverlap = false;
        bool yoverlap = false;

        //Ignore case of current orientation
        if(orient == curr_orient) {
            continue;
        }

        //X overlap

        //Do not consider non-calibrated orientation
        OrientSettings forOrientToBeChecked = saved_data.getCalibSettings(orient);

        if(forOrientToBeChecked.calibrated)
        {

            if((forOrientToBeChecked.min_x  <= calibrationProcess.min_x) && (calibrationProcess.min_x <= forOrientToBeChecked.max_x))
            {
                xoverlap = true;
                x_min_overlap = calibrationProcess.min_x;
            }

            if((calibrationProcess.min_x <= forOrientToBeChecked.min_x) && ( forOrientToBeChecked.min_x <= calibrationProcess.max_x))
            {
                xoverlap = true;
                x_min_overlap = forOrientToBeChecked.min_x;
            }


            if((forOrientToBeChecked.min_x  <= calibrationProcess.max_x) && (calibrationProcess.max_x <= forOrientToBeChecked.max_x))
            {
                xoverlap = true;
                x_max_overlap = calibrationProcess.max_x;
            }

            if((calibrationProcess.min_x <= forOrientToBeChecked.max_x) && ( forOrientToBeChecked.max_x <= calibrationProcess.max_x))
            {
                xoverlap = true;
                x_max_overlap = forOrientToBeChecked.max_x;
            }


            //Y overlap
            if((forOrientToBeChecked.min_y  <= calibrationProcess.min_y) && (calibrationProcess.min_y <= forOrientToBeChecked.max_y))
            {
                yoverlap = true;
                y_min_overlap = calibrationProcess.min_y;
            }

            if((calibrationProcess.min_y <= forOrientToBeChecked.min_y) && ( forOrientToBeChecked.min_y <= calibrationProcess.max_y))
            {
                yoverlap = true;
                y_min_overlap = forOrientToBeChecked.min_y;
            }


            if((forOrientToBeChecked.min_y  <= calibrationProcess.max_y) && (calibrationProcess.max_y <= forOrientToBeChecked.max_y))
            {
                yoverlap = true;
                y_max_overlap = calibrationProcess.max_y;
            }

            if((calibrationProcess.min_y <= forOrientToBeChecked.max_y) && ( forOrientToBeChecked.max_y <= calibrationProcess.max_y))
            {
                yoverlap = true;
                y_max_overlap = forOrientToBeChecked.max_y;
            }
        }

        if (xoverlap && yoverlap)
        {
           char temp[50] = {0};
           sprintf(temp, "Orient %d: x(%03d to %03d), y(%03d to %03d)\n", orient, x_min_overlap, x_max_overlap, y_min_overlap, y_max_overlap);
           overlapstring += temp;
           anyOverlap = true;
        }
    }

    if(anyOverlap){
        return overlapstring;
    } else {
        //return string of 0 length to signify no overlap
        return "";
    }
}

bool Logic::toggleAutoRotationStatus(){
    if((calibrated == false) || (sensor_detected == false) || calibration_in_progress ){
        return false;
    }

    auto_rotation_status = !auto_rotation_status;
    return true;
}

void Logic::toggleEnableForOrient(int orient){
    if((sensor_detected == false) || calibration_in_progress){
            return;
    }

    if((orient < 0) || (orient > 4)) {
        return;
    }
    saved_data.toggleOrientEnable(orient);
}


bool Logic::saveSettings(){
    return fileLink.saveFile(&saved_data);
}

int Logic::obtainCurrentOrient(){
    return osFunctions->getCurrentOrientation();
}

bool Logic::isOrientInUse(int orient){
    OrientSettings orientSet = saved_data.getCalibSettings(orient);
    if(orientSet.in_use){
        return true;
    } else {
        return false;
    }

}

int Logic::getTableElements(int orient, settingTypes typeWanted){
    OrientSettings orientSet = saved_data.getCalibSettings(orient);

    if(typeWanted == minX){
        return orientSet.min_x;
    }

    if(typeWanted == maxX){
        return orientSet.max_x;
    }

    if(typeWanted == minY){
       return orientSet.min_y;
    }

    if(typeWanted == maxY){
        return orientSet.max_y;
    }

    return -1;
}

int Logic::getCalibElements(settingTypes typeWanted){

    if(typeWanted == minX){
        return calibrationProcess.min_x;
    }

    if(typeWanted == maxX){
        return calibrationProcess.max_x;
    }

    if(typeWanted == minY){
       return calibrationProcess.min_y;
    }

    if(typeWanted == maxY){
        return calibrationProcess.max_y;
    }

    return -1;
}

string Logic::generateStatusText()
{
    stringstream textStream;

    SensorData currAccData = sensorlink.getSensorData();

    textStream << "Sensor ";

    textStream << currAccData.x;
    textStream << ", ";

    textStream << currAccData.y;
    textStream << ", ";

    textStream << currAccData.temp;

    //This is the ° symbol, cannot use ° directly as will have extra Â character
    textStream << "\xB0";

    textStream << "C, ";

    textStream << currAccData.status;
    textStream << ", ";


    textStream << currAccData.statusMessage;
    textStream << ".     Display: ";


    textStream << osFunctions->getCurrentWidth();
    textStream << " x ";

    textStream << osFunctions->getCurrentHeight();
    textStream << ", Orient ";

    textStream << osFunctions->getCurrentOrientation();
    textStream << ".     Pollrate: ";

    textStream << saved_data.getPollRate();
    textStream << "ms, DelayRate: ";


    textStream << saved_data.getScreenDelayRate();
    textStream << "ms";

    string text = textStream.str();
    return text;

}

void Logic::onPollRateHit(){

    autoRotate();
    calibrateInProgress();
    getStatusTextSlot();
}

void Logic::getStatusTextSlot(){
    string statusString = generateStatusText();
    QString statusQString(statusString.c_str());
    emit signalToStatusBar(statusQString);

}


void Logic::autoRotate(){

    if(!auto_rotation_status){
        return;
    }

    SensorData reading = sensorlink.getSensorData();
    int currentX = reading.x;
    int currentY = reading.y;


    //Loop through all the orientation settings
    for(int orient = 0; orient < 4; orient++)
    {
        OrientSettings orientSet = saved_data.getCalibSettings(orient);

        milliseconds elapsed = duration_cast<milliseconds>(steady_clock::now() - timeChangeDetected);

        bool elapsedLongEnough = (elapsed.count() > saved_data.getScreenDelayRate());

        if( (orientSet.min_x  <= currentX) && (currentX <= orientSet.max_x) &&
                (orientSet.min_y  <= currentY) && (currentY <= orientSet.max_y) &&
                (orientSet.in_use == true) )
        {

            //Rotate to this orientation only when the computer has been there for at least the screen delay
            if(elapsedLongEnough && (orient == newOrientAtTime))
            {
                translateRotateOrientIntegersIntoRightMethodCalls(orient);
            }
            else if(newOrientAtTime != orient)
            {
                timeChangeDetected = steady_clock::now();
                newOrientAtTime = orient;
            }
            else if(elapsedLongEnough && (orient == osFunctions->getCurrentOrientation()))
            {
                //Reset orientation should the previous orientaion value be temporary
                newOrientAtTime = osFunctions->getCurrentOrientation();
            }
        }
    }

    mWindow->updateUIVariableElements();
}

void Logic::calibrateInProgress()
{

    if(!calibration_in_progress) {
        return;
    }

    SensorData reading = sensorlink.getSensorData();
    int currentX = reading.x;
    int currentY = reading.y;


    if( currentX < calibrationProcess.min_x) calibrationProcess.min_x = currentX;
    if( currentX > calibrationProcess.max_x) calibrationProcess.max_x = currentX;

    if( currentY < calibrationProcess.min_y) calibrationProcess.min_y = currentY;
    if( currentY > calibrationProcess.max_y) calibrationProcess.max_y = currentY;

    mWindow->updateUIVariableElements();
}

void Logic::translateRotateOrientIntegersIntoRightMethodCalls(int orient)
{
    switch(orient){
        case 0:
            rotateToLandscape();
            break;
        case 1:
            rotateToPortrait();
            break;
        case 2:
            rotateToLandscapeFlipped();
            break;
        case 3:
            rotateToPortraitFlipped();
            break;
       default:
            rotateToLandscape();
            break;
    }
}

void Logic::rotateToLandscape()
{
    if(calibration_in_progress){
        return;
    }
    if(saved_data.getRotationMethod() == Rotation_KeyBSC){
        postRotationAttemptStatus = osFunctions->rotateToUsingKeyboardShortcut(osFunctions->landscape);
    } else {
        postRotationAttemptStatus = osFunctions->rotateTo(osFunctions->landscape);
    }

    postRotationAttemptErrorMessage(postRotationAttemptStatus);

    mWindow->updateUIVariableElements();
}

void Logic::rotateToPortrait()
{
    if(calibration_in_progress){
        return;
    }
    if(saved_data.getRotationMethod() == Rotation_KeyBSC){
        postRotationAttemptStatus = osFunctions->rotateToUsingKeyboardShortcut(osFunctions->portrait);
    } else {
        postRotationAttemptStatus = osFunctions->rotateTo(osFunctions->portrait);
    }

    postRotationAttemptErrorMessage(postRotationAttemptStatus);

    mWindow->updateUIVariableElements();
}

void Logic::rotateToLandscapeFlipped()
{
    if(calibration_in_progress){
        return;
    }
    if(saved_data.getRotationMethod() == Rotation_KeyBSC){
        postRotationAttemptStatus = osFunctions->rotateToUsingKeyboardShortcut(osFunctions->landscape_flipped);
    } else {
        postRotationAttemptStatus = osFunctions->rotateTo(osFunctions->landscape_flipped);
    }
    postRotationAttemptErrorMessage(postRotationAttemptStatus);
    mWindow->updateUIVariableElements();
}

void Logic::rotateToPortraitFlipped()
{
    if(calibration_in_progress){
        return;
    }
    if(saved_data.getRotationMethod() == Rotation_KeyBSC){
        postRotationAttemptStatus = osFunctions->rotateToUsingKeyboardShortcut(osFunctions->portrait_flipped);
    } else {
        postRotationAttemptStatus = osFunctions->rotateTo(osFunctions->portrait_flipped);
    }

    postRotationAttemptErrorMessage(postRotationAttemptStatus);
    mWindow->updateUIVariableElements();
}


void Logic::postRotationAttemptErrorMessage(int errorValue)
{
    if(errorValue == 0){
        return;
    }
    QMessageBox::critical(nullptr, "Cannot rotate screen", "Your display driver apparently does not support native Win32 screen rotation."
                          "You can try selecting the keyboard shortcut method to see if it works. Consult readme.txt for more details."
                          "If both fail, please send me an email to inform me.");
}


void Logic::setPollRate(int newPollRate){
    if(!sensor_detected){
        return;
    }

    saved_data.setPollRate(newPollRate);
    restartPollRateTimer(newPollRate);
}

void Logic::setDelayRate(int newDelayRate){
    if(!sensor_detected){
        return;
    }

    saved_data.setDelayRate(newDelayRate);
}

void Logic::setRotationMethod(rotationMethod method){
    if(method == Rotation_W32){
        saved_data.setRotationMethod(saved_data.ROTATION_METHOD_W32);
    } else if(method == Rotation_KeyBSC){
        saved_data.setRotationMethod(saved_data.ROTATION_METHOD_KEYB_SHORTCUT);
    } else {
        saved_data.setRotationMethod(saved_data.ROTATION_METHOD_W32);
    }
}

