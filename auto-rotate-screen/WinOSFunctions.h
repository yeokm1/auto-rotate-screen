#ifndef WIN_OS_FUNCTIONS_H
#define WIN_OS_FUNCTIONS_H
#define _WIN32_WINNT 0x0500
#include <windows.h>

class WinOSFunctions
{

static const DWORD LANDSCAPE         = 0;
static const DWORD PORTRAIT          = 1;
static const DWORD LANDSCAPE_FLIPPED = 2;
static const DWORD PORTRAIT_FLIPPED  = 3;

static const DWORD I_NUM_MODE = ENUM_CURRENT_SETTINGS;

private:
    HANDLE hMutex;



public:
    WinOSFunctions(){;}
    enum orientationType{landscape, portrait, landscape_flipped, portrait_flipped};



    void startProgramLock();
    void endProgramLock();
    bool isThereMultipleInstances();
    int rotateTo(int);

    int getCurrentWidth();
    int getCurrentHeight();
    int getCurrentOrientation();

     int rotateToUsingKeyboardShortcut(int);
};


#endif // WIN_OS_FUNCTIONS_H
