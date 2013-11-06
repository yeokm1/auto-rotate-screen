/*
    Copyright 2012,2013 Yeo Kheng Meng

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/*
Changelog:
v0.1 (8 June 2012)
Initial release

v0.2 (8 June 2012)
Added commandline argument to allow disable auto rotation on program start
Added commandline argument to customise polling rate

v0.3 (24 June 2012)
Left click on tray icon also brings up program
Reduced confusion on Auto-Rotation option in tray menu
Prevent multiple instances of the same program
Range of poll rate limited from 20ms to 90000000ms
Allow individual display orientation to be deselected
Improve screen orientation delay algorithm
Press ESC key to disable autorotation (during abnormal behavior)
Warn if overlapping ranges encountered

v0.31 (27 June 2012)
Corrected bug that enables to save config data even for non-APS enabled machines
Adjusted custom message values to make all above 1024

v0.4 (1 Jan 2013)
Ported to C++11 and QT 4.8.4
Added easier adjustments of poll/delay rate
2 ways of rotating, using native Win32 APIs or keyboard shortcuts

v0.41 (2 Jan 2013)
Allow change of poll rate on the fly
Inform user of next launch minimise to system tray after first successful calibration
Renamed Win32 rotation label to OS rotation
Added Settings help to About/Help
Corrected bug where during calibrating shows the wrong calibration orientation value

*/


#include <iostream>
#include <QApplication>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QtPlugin>

#include "WinOSFunctions.h"
#include "Logic.h"
#include "MainWindow.h"

Q_IMPORT_PLUGIN(qico) //To statically link to QT's icon library


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WinOSFunctions osFunct;
    osFunct.startProgramLock();

    if (osFunct.isThereMultipleInstances())
    {
        QMessageBox::critical(nullptr, "Multiple instances!", "AutoRotateScreen is already running" );
        return -1;
    }

    MainWindow mWindow;
    Logic logic(argc, argv, &osFunct, &mWindow);


    mWindow.setRefToLogicAndInitUI(&logic);



    //Start hidden if everything is in order, else open main window
    if(logic.shdShowMainWindow())
    {
        mWindow.show();
    }


    int returnValue = a.exec();
    osFunct.endProgramLock();

    return returnValue;
}
