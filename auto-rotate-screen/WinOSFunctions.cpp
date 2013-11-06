#include "WinOSFunctions.h"
#include "constants.h"

void WinOSFunctions::startProgramLock()
{
    hMutex = CreateMutex(nullptr, TRUE, MUTEX_NAME);
}

void WinOSFunctions::endProgramLock()
{
    CloseHandle(hMutex);
}

bool WinOSFunctions::isThereMultipleInstances(){
    return (hMutex != nullptr) && (GetLastError () == ERROR_ALREADY_EXISTS);
}


int WinOSFunctions::rotateTo(int orientTo){

    DWORD orientation = (DWORD) orientTo;

    DEVMODE tempdevmode;
    EnumDisplaySettings(nullptr, I_NUM_MODE, &tempdevmode);

    //Do not rotate if the given value is the same as the current screen orientation
    if(tempdevmode.dmDisplayOrientation == orientation) return 0;
    DWORD def_height;
    DWORD def_width;

    //Get base resolution settings
    if((tempdevmode.dmDisplayOrientation == LANDSCAPE ) || (tempdevmode.dmDisplayOrientation == LANDSCAPE_FLIPPED ))
    {
        def_height = tempdevmode.dmPelsHeight;
        def_width = tempdevmode.dmPelsWidth;
    }
    else
    {
        def_height = tempdevmode.dmPelsWidth;
        def_width = tempdevmode.dmPelsHeight;
    }

    //Rotate if necessary
    if((orientation == PORTRAIT) || (orientation == PORTRAIT_FLIPPED))
    {
        tempdevmode.dmPelsWidth = def_height;
        tempdevmode.dmPelsHeight = def_width;
    }
    else
    {
        tempdevmode.dmPelsWidth = def_width;
        tempdevmode.dmPelsHeight = def_height;

    }

    tempdevmode.dmDisplayOrientation = orientation;

    int rotationAttempt = (int) ChangeDisplaySettings(&tempdevmode, 0);
    return rotationAttempt;
}


//To simulate keypress, auto rotation done by another program
int WinOSFunctions::rotateToUsingKeyboardShortcut(int orientTo){

    DEVMODE tempdevmode;
    EnumDisplaySettings(nullptr, I_NUM_MODE, &tempdevmode);

    DWORD orientation = (DWORD) orientTo;
    //Do not rotate if the given value is the same as the current screen orientation

    if(tempdevmode.dmDisplayOrientation == orientation) return 0;

    INPUT keystrokes[6];

    for(int i = 0; i < 6; i++){
        keystrokes[i].type = INPUT_KEYBOARD;
        keystrokes[i].ki.wScan = 0; // hardware scan code for key
        keystrokes[i].ki.time = 0;
        keystrokes[i].ki.dwExtraInfo = 0;
    }


    keystrokes[0].ki.wVk = VK_CONTROL; //CTRL Press
    keystrokes[0].ki.dwFlags = 0;

    keystrokes[1].ki.wVk = VK_MENU; //ALT Press
    keystrokes[1].ki.dwFlags = 0;


    switch(orientTo){
        case 0:
            keystrokes[2].ki.wVk = VK_UP; //UP Press
            keystrokes[2].ki.dwFlags = 0;

            keystrokes[3].ki.wVk = VK_UP; //UP Release
            keystrokes[3].ki.dwFlags = KEYEVENTF_KEYUP;
            break;
        case 1:
            keystrokes[2].ki.wVk = VK_LEFT; //LEFT Press
            keystrokes[2].ki.dwFlags = 0;

            keystrokes[3].ki.wVk = VK_LEFT; //LEFT Release
            keystrokes[3].ki.dwFlags = KEYEVENTF_KEYUP;
            break;
        case 2:
            keystrokes[2].ki.wVk = VK_DOWN; //DOWN Press
            keystrokes[2].ki.dwFlags = 0;

            keystrokes[3].ki.wVk = VK_DOWN; //DOWN Release
            keystrokes[3].ki.dwFlags = KEYEVENTF_KEYUP;
            break;
        case 3:
            keystrokes[2].ki.wVk = VK_RIGHT; //RIGHT Press
            keystrokes[2].ki.dwFlags = 0;

            keystrokes[3].ki.wVk = VK_RIGHT; //RIGHT Release
            keystrokes[3].ki.dwFlags = KEYEVENTF_KEYUP;
            break;
        default: //Nothing done
            break;
    }

    keystrokes[4].ki.wVk = VK_MENU; //ALT Press
    keystrokes[4].ki.dwFlags = KEYEVENTF_KEYUP;

    keystrokes[5].ki.wVk = VK_CONTROL; //CTRL Press
    keystrokes[5].ki.dwFlags = KEYEVENTF_KEYUP;


    int result = SendInput(6, keystrokes, sizeof(INPUT));

    if(result == 6){
        return 0;
    } else {
        return result;
    }
}

//To simulate keypress, auto rotation done by another program
//Using the deprecated keybd_event function
//void WinOSFunctions::useKeyboardShortcutToRotate(int orient){

//    keybd_event(VK_CONTROL, 0x9d, 0 , 0); //CTRL Press
//    keybd_event(VK_MENU, 0xb8, 0 , 0); //ALT Press

//    switch(orient){
//        case 0:
//            keybd_event(VK_UP, 0xc8, 0 , 0); //UP Press
//            keybd_event(VK_UP, 0xc8, KEYEVENTF_KEYUP , 0); //UP Release
//            break;
//        case 1:
//            keybd_event(VK_LEFT, 0xcb, 0 , 0); //LEFT Press
//            keybd_event(VK_LEFT, 0xcb, KEYEVENTF_KEYUP , 0); //LEFT Release
//            break;
//        case 2:
//            keybd_event(VK_DOWN, 0xd0, 0 , 0); //DOWN Press
//            keybd_event(VK_DOWN, 0xd0, KEYEVENTF_KEYUP , 0); //DOWN Release
//            break;
//        case 3:
//            keybd_event(VK_RIGHT, 0xcd, 0 , 0); //RIGHT Press
//            keybd_event(VK_RIGHT, 0xcd, KEYEVENTF_KEYUP , 0); //RIGHT Release
//            break;
//        default: //Nothing done
//            break;
//    }

//    keybd_event(VK_MENU, 0xb8, KEYEVENTF_KEYUP , 0); //ALT Release
//    keybd_event(VK_CONTROL, 0x9d, KEYEVENTF_KEYUP , 0); //CTRL Release
//}


int WinOSFunctions::getCurrentWidth()
{
    DEVMODE tempdevmode;
    EnumDisplaySettings(nullptr, I_NUM_MODE, &tempdevmode);
    return (int) tempdevmode.dmPelsWidth;
}

int WinOSFunctions::getCurrentHeight()
{
    DEVMODE tempdevmode;
    EnumDisplaySettings(nullptr, I_NUM_MODE, &tempdevmode);
    return (int)tempdevmode.dmPelsHeight;
}

int WinOSFunctions::getCurrentOrientation()
{
    DEVMODE tempdevmode;
    EnumDisplaySettings(nullptr, I_NUM_MODE, &tempdevmode);
    return (int) tempdevmode.dmDisplayOrientation;
}
