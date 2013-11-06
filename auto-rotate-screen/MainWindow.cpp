#include <QTableWidgetItem>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "constants.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->StatusBar->setStyleSheet("QStatusBar { background-color: white; }");

    calStatusLabel = new QLabel("NIL");
    apsStatusLabel = new QLabel("NIL");
    postRotateStatusLabel = new QLabel("NIL");

    ui->StatusBar->addPermanentWidget(postRotateStatusLabel);
    ui->StatusBar->addPermanentWidget(calStatusLabel);
    ui->StatusBar->addPermanentWidget(apsStatusLabel);

}

MainWindow::~MainWindow()
{
    helpBox->hide();
    delete helpBox;

    delete calStatusLabel;
    delete apsStatusLabel;
    delete postRotateStatusLabel;

    delete rotateToSubMenu;
    delete trayIcon;
    delete ui;

}

void MainWindow::setRefToLogicAndInitUI(Logic* logicRef){
    logic = logicRef;

    isSystemTrayAvailable = QSystemTrayIcon::isSystemTrayAvailable();
    if(isSystemTrayAvailable){
        trayIcon = createTrayIcon();
        trayIcon->show();

        connect(trayIcon ,SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    }


    updateUIVariableElements();
    //Set this flag as the initial setting of the delay/poll rate spinbox will trigger a false unsaved positive
    logic->setDataFlagToSaveWithoutSaving();
    refreshTextLabelsOnly();
    QObject::connect(ui->DataTable, SIGNAL(cellClicked(int,int)), this, SLOT(toggleOrientEnable(int, int)));
}

QSystemTrayIcon * MainWindow::createTrayIcon(){
    QSystemTrayIcon * trayIcon = new QSystemTrayIcon;

    trayIcon->setIcon(QIcon(":/resource/icon/Actions-transform-rotate.ico"));

    trayIcon->setToolTip(QString(PROGRAM_NAME));

    trayIcon->setContextMenu(createTrayMenu());


    return trayIcon;
}



QMenu * MainWindow::createTrayMenu(){
    QMenu * trayMenu = new QMenu();

    QAction *openMainWindow = new QAction(QString("Open AutoRotateScreen"), trayMenu);
    connect(openMainWindow, SIGNAL(triggered()), this, SLOT(showNormal()));
    trayMenu->addAction(openMainWindow);


    QMenu * rotateToMenu = new QMenu("Rotate To", trayMenu);
    //This \xB0 is the ° symbol, cannot use ° directly as will have extra Â character
    QAction *rotateToLandscape = new QAction(QString("Orient 0 (0\xB0) Landscape"),rotateToMenu);
    rotateToLandscape->setCheckable(true);
    connect(rotateToLandscape, SIGNAL(triggered()), this, SLOT(on_Orient0_clicked()));
    rotateToMenu->addAction(rotateToLandscape);

    QAction *rotateToPortrait = new QAction(QString("Orient 1 (90\xB0) Portrait"),rotateToMenu);
    rotateToPortrait->setCheckable(true);
    connect(rotateToPortrait, SIGNAL(triggered()), this, SLOT(on_Orient1_clicked()));
    rotateToMenu->addAction(rotateToPortrait);

    QAction *rotateToLandscapeFlipped = new QAction(QString("Orient 2 (180\xB0) Landscape Flipped"),rotateToMenu);
    rotateToLandscapeFlipped->setCheckable(true);
    connect(rotateToLandscapeFlipped, SIGNAL(triggered()), this, SLOT(on_Orient2_clicked()));
    rotateToMenu->addAction(rotateToLandscapeFlipped);

    QAction *rotateToPortraitFlipped = new QAction(QString("Orient 3 (270\xB0) Portrait Flipped"),rotateToMenu);
    rotateToPortraitFlipped->setCheckable(true);
    connect(rotateToPortraitFlipped, SIGNAL(triggered()), this, SLOT(on_Orient3_clicked()));
    rotateToMenu->addAction(rotateToPortraitFlipped);

    rotateToSubMenu = rotateToMenu;
    trayMenu->addMenu(rotateToMenu);

    QAction *toggleAutoRotationStatus = new QAction(QString("Turn ON/OFF AutoRotation (currently Nill)"), trayMenu);
    toggleAutoRotationStatus->setCheckable(true);
    connect(toggleAutoRotationStatus, SIGNAL(triggered()), this, SLOT(on_AutoRotationButton_clicked()));
    trayMenu->addAction(toggleAutoRotationStatus);

    QAction *aboutHelp = new QAction(QString("About/Help"), trayMenu);
    connect(aboutHelp, SIGNAL(triggered()), this, SLOT(on_HelpButton_clicked()));
    trayMenu->addAction(aboutHelp);


    QAction *exit = new QAction(QString("Exit"), trayMenu);
    connect(exit, SIGNAL(triggered()), this, SLOT(onExitMenuClicked()));
    trayMenu->addAction(exit);

    return trayMenu;
}

void MainWindow::onExitMenuClicked(){
    this->showNormal();
    this->close();
}

void MainWindow::updateUIVariableElements(){
    //Assume TableItems in table have been initialised. By clicking on all the cells in QT Creator Forms. QT creator properties text must be bold

    for(int i = 0; i < 4; i++){

        if(logic->isOrientInUse(i)){
            ui->DataTable->item(i, 0)->setCheckState(Qt::Checked);
        } else {
            ui->DataTable->item(i, 0)->setCheckState(Qt::Unchecked);
        }

        ui->DataTable->item(i,1)->setText(QString::number(logic->getTableElements(i, logic->minX)));
        ui->DataTable->item(i,2)->setText(QString::number(logic->getTableElements(i, logic->maxX)));
        ui->DataTable->item(i,3)->setText(QString::number(logic->getTableElements(i, logic->minY)));
        ui->DataTable->item(i,4)->setText(QString::number(logic->getTableElements(i, logic->maxY)));

    }


    if(logic->isCalibratingNow()){
        ui->DataTable->item(4,0)->setText(QString::number(logic->calibratingThisOrient()));

        ui->DataTable->item(4,1)->setText(QString::number(logic->getCalibElements(logic->minX)));
        ui->DataTable->item(4,2)->setText(QString::number(logic->getCalibElements(logic->maxX)));
        ui->DataTable->item(4,3)->setText(QString::number(logic->getCalibElements(logic->minY)));
        ui->DataTable->item(4,4)->setText(QString::number(logic->getCalibElements(logic->maxY)));

        ui->DataTable->selectRow(4); //To highlight the row

    } else {
        ui->DataTable->item(4,0)->setText(QString("-"));
        ui->DataTable->item(4,1)->setText(QString("-"));
        ui->DataTable->item(4,2)->setText(QString("-"));
        ui->DataTable->item(4,3)->setText(QString("-"));
        ui->DataTable->item(4,4)->setText(QString("-"));
    }

    ui->SensorPollRateSetting->setValue(logic->obtainPollRate());
    ui->ScreenRotationDelaySetting->setValue(logic->obtainDelayRate());


    int rotationMethod = logic->obtainRotationMethod();

    //rotationMethod will return 0 if OS radio
    if(rotationMethod == 1){
        ui->RotationMethodKeyBShortRadio->setChecked(true);
    } else {
        ui->RotationMethodOSRadio->setChecked(true);
    }

    refreshTextLabelsOnly();
    refreshVariableTrayMenuLabels();


}

void MainWindow::refreshTextLabelsOnly(){

    if(logic->isDataSaved()){

        ui->DataSaveStatus->setText("Unsaved Data: No");
    } else {
        ui->DataSaveStatus->setText("Unsaved Data: Yes");
    }

    if(logic->isAutoRotationEnabled()){
        ui->AutoRotationStatus->setText("Auto Rotation Status: ON");
    } else {
        ui->AutoRotationStatus->setText("Auto Rotation Status: OFF");
    }

    if(logic->isCalibratingNow()){
        ui->CalibrationStatus->setText("Calibration-in-progress: Yes");
    } else {
        ui->CalibrationStatus->setText("Calibration-in-progress: No");
    }
}

void MainWindow::refreshVariableTrayMenuLabels(){

    QAction * autoRotateAction = trayIcon->contextMenu()->actions().at(2);

    if(logic->isAutoRotationEnabled()){
        autoRotateAction->setText("Turn OFF AutoRotation (currently ON)");
        autoRotateAction->setChecked(true);
    } else {
        autoRotateAction->setText("Turn ON AutoRotation (currently OFF)");
        autoRotateAction->setChecked(false);
    }

    int currentOrient = logic->obtainCurrentOrient();

    for(int orient = 0; orient < 4; orient++){
        QAction * orientAction = rotateToSubMenu->actions().at(orient);
        if(currentOrient == orient){
            orientAction->setChecked(true);
        } else {
            orientAction->setChecked(false);
        }

    }



}


//To hide the main window on minimise
void MainWindow::changeEvent(QEvent * hopefullyItsMinimiseEvent )
{
    if(isSystemTrayAvailable == false) return;

    if (hopefullyItsMinimiseEvent->type() == QEvent::WindowStateChange){

        if (isMinimized() == true) {

            //Use the following code as this.hide() does not work, leaves a blank window
            QTimer::singleShot(0, this, SLOT(hide()));
            hopefullyItsMinimiseEvent->ignore();
            return;
    }
}

//This function is for use by above
QMainWindow::changeEvent(hopefullyItsMinimiseEvent);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    helpBox->hide();


    int confirmExit = QMessageBox::question(this, "Exit AutoRotateScreen?",
                                            "This will exit AutoRotateScreen. To keep it running, click the minimise button to minimise to System Tray. "
                                            "Confirm Exit?",
                                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if(confirmExit == QMessageBox::No){
        event->ignore();
        return;
    }


    if(logic->isDataSaved()){
        event->accept();

    } else {
        int answer = QMessageBox::question(this, "Unsaved data", "You have not saved your data, save before exit?",
                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                              QMessageBox::Cancel);

        switch(answer){
            case(QMessageBox::Yes): {

                if(on_SaveSettingsButton_clicked()){
                    event->accept();
                } else {
                    int forceExit = QMessageBox::question(this, "Still exit?", "Data cannot be saved, confirm exit?",
                                          QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

                    if(forceExit == QMessageBox::Yes){
                        event->accept();
                    } else if (forceExit == QMessageBox::No){
                        event->ignore();
                    } else {
                        event->ignore();
                    }
                }
            }
                break;

            case(QMessageBox::Cancel):
                event->ignore();
                break;

            case(QMessageBox::No):
                //Fallthrough
            default:
                event->accept();
        }

    }


}

void MainWindow::toggleOrientEnable(int orient, int position){
    if(position != 0) return;
    if((orient < 0) || (orient > 4)) return;

    logic->toggleEnableForOrient(orient);
    updateUIVariableElements();
}


void MainWindow::updateStatusbarAndTrayMenuLabels(QString statusText)
{
    ui->StatusBar->showMessage(statusText, 0);

    int postRotateValue = logic->obtainPostRotationAttemptStatus();
    if(postRotateValue == 0){
        postRotateStatusLabel->setText("");
    } else {
        postRotateStatusLabel->setText(QString::number(postRotateValue));
    }

    if(logic->isSensorDetected()){
        apsStatusLabel->setText("APS");
    } else {
        apsStatusLabel->setText("!APS");
    }


    if(logic->isCalibrated()){
        calStatusLabel->setText("CAL");
    } else {
        calStatusLabel->setText("!CAL");
    }

    refreshVariableTrayMenuLabels();
}

void MainWindow::on_SensorPollRateSetting_valueChanged(int newPollRate)
{
    logic->setPollRate(newPollRate);
    updateUIVariableElements();
}

void MainWindow::on_ScreenRotationDelaySetting_valueChanged(int newDelayRate)
{
    logic->setDelayRate(newDelayRate);
    updateUIVariableElements();
}



//Assume logic will force a screen up update
void MainWindow::on_Orient0_clicked()
{
    logic->rotateToLandscape();
    updateUIVariableElements();
}

void MainWindow::on_Orient1_clicked()
{
    logic->rotateToPortrait();
    updateUIVariableElements();
}


void MainWindow::on_Orient2_clicked()
{
    logic->rotateToLandscapeFlipped();
    updateUIVariableElements();
}

void MainWindow::on_Orient3_clicked()
{
    logic->rotateToPortraitFlipped();
    updateUIVariableElements();
}

bool MainWindow::on_SaveSettingsButton_clicked()
{
    if(!logic->saveSettings()){
        QMessageBox::warning(this, "Data cannot be saved!", "Unable to save data, please check file permissions");
        updateUIVariableElements();
        return false;
    } else {
        QMessageBox::information(this, "Data saved!", "Data saved successfully to ars-config.ini!");
        updateUIVariableElements();
        return true;
    }
}

void MainWindow::on_AutoRotationButton_clicked()
{
    if(!logic->toggleAutoRotationStatus()) {
        QMessageBox::warning(this, "Cannot change AutoRotateSetting", "Please ensure AutoRotateScreen is calibrated and not calibrating in progress.");
    }

    updateUIVariableElements();
}


void MainWindow::on_HelpButton_clicked()
{
    helpBox->show();
}

void MainWindow::on_CalibrationButton_clicked()
{
    bool beforeCalibInProgressStatus = logic->isCalibratingNow();
    bool isCalibratedBefore = logic->isCalibrated();


    if(!logic->toggleCalibrationStatus()){
       QMessageBox::warning(this, "Cannot start calibration", "Please ensure APS driver is installed and AutoRotation is off.");
    }

    bool afterCalibInProgressStatus = logic->isCalibratingNow();
    bool isCalibratedAfter = logic->isCalibrated();


    //ONly show the message after calibrating sucessfully for the first time
    if((isCalibratedBefore == false) && isCalibratedAfter && beforeCalibInProgressStatus && (afterCalibInProgressStatus == false)){
        QMessageBox::information(this,"AutoRotateScreen is now calibrated",
                                 "Once you save settings, on next launch, AutoRotateScreen will start minimised to the System Tray.");
    }

    updateUIVariableElements();
}

void MainWindow::on_RotationMethodOSRadio_clicked()
{
    logic->setRotationMethod(logic->Rotation_W32);
    updateUIVariableElements();
}

void MainWindow::on_RotationMethodKeyBShortRadio_clicked()
{
    logic->setRotationMethod(logic->Rotation_KeyBSC);
    updateUIVariableElements();
}


void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason){
    if(reason == QSystemTrayIcon::DoubleClick){
        this->showNormal();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
   if (e->key() == Qt::Key_Escape) {
       logic->stopAutoRotation();
   }
   updateUIVariableElements();
}
