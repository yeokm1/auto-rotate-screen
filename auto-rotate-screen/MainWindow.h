#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QLabel>
#include "Logic.h"
#include "HelpDialog.h"

namespace Ui {
class MainWindow;
}

class Logic;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow* ui;
    Logic* logic;

    HelpDialog * helpBox = new HelpDialog();

    QSystemTrayIcon * trayIcon = nullptr;
    QMenu * rotateToSubMenu = nullptr;

    QLabel * calStatusLabel = nullptr;
    QLabel * apsStatusLabel = nullptr;
    QLabel * postRotateStatusLabel = nullptr;

    void refreshTextLabelsOnly();
    void refreshVariableTrayMenuLabels();
    void closeEvent(QCloseEvent * event);
    void changeEvent(QEvent * e);

    QSystemTrayIcon * createTrayIcon();
    QMenu * createTrayMenu();

    bool isSystemTrayAvailable;


    void keyPressEvent(QKeyEvent *e);


    
public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();
    void setRefToLogicAndInitUI(Logic * logicRef);
    void updateUIVariableElements();




public slots:
    void updateStatusbarAndTrayMenuLabels(QString);
    


private slots:
    void on_Orient0_clicked();
    void on_Orient1_clicked();
    void on_Orient2_clicked();
    void on_Orient3_clicked();
    void on_SensorPollRateSetting_valueChanged(int);
    void on_ScreenRotationDelaySetting_valueChanged(int);
    bool on_SaveSettingsButton_clicked();


    void toggleOrientEnable(int, int);

    void onExitMenuClicked();
    void on_AutoRotationButton_clicked();
    void on_HelpButton_clicked();
    void on_CalibrationButton_clicked();


    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void on_RotationMethodOSRadio_clicked();
    void on_RotationMethodKeyBShortRadio_clicked();
};

#endif // MAINWINDOW_H


