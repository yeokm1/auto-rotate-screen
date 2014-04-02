Auto Rotate Screen using APS
==================

Originally posted on Sourceforge  http://sourceforge.net/projects/autoscreenrotat/

![Screen](https://raw.github.com/yeokm1/auto-rotate-screen/master/screenshots/ARS%20v41.PNG)


Using the sensor data provided by the Active Protection System, the program will auto rotate the screen to the desired orientation.

Although IBM/Lenovo has an official software that does exactly this, it does not work for the non-tablet versions. This program differs from the official one in that it allows the user to customise the exact laptop orientation for the rotation to kick in. You can also individually select which orientation to activate or deactivate.

This project was conceived by my need to have this function when I added a touchscreen to my non-tablet X60.

For non-Thinkpad machines, this tool can work as a regular screen rotation shortcut on the tray. See readme file for more details.

Requirements: Thinkpad Active Protection System driver. Windows XP or above. (Both 32 and 64 bit supported)

Important!
This software is still new. Problems such as failure to rotate or blank screens may occur on certain systems. If you experience any issues, I appreciate it if you can drop me an email or create an issue. Do include your machine type, operating system version, graphics card model and graphics driver version.

------------------------------

Calibration Instructions:

1. Turn off AutoRotation if it is still enabled.

2. Choose the first of the 4 display orientation position by selecting any of the 4 buttons.

3. Move your laptop roughly into the position you desire for that particular display orientation.

4. Click the Calibrate button.

5. Tilt your machine side to side, forwards and backwards to obtain the full range of position you desire for the program to switch into that display orientation.

6. Click the Calibrate button again to stop. Click Yes if you are satisfied with the calibrated result.

7. Repeat the above steps for the other 3 display orientation positions.

8. You can save your settings into the configuration file ars-config.ini (stored in the same directory) at any point in time.

9. Enable AutoRotation and test your results. You can recalibrate for just a particular display orientation if necessary.

If there is any abnormal behavoir, press the ESC key to disable AutoRotation.

------------------------------

Rate Settings:

Sensor Poll Rate: Defines how often AutoRotateScreen will poll the sensor for new accelerometer data.

Screen Delay Rate: How long AutoRotateScreen will wait for before rotating to the new display orientation

------------------------------

Rotation Method Settings:

Native OS: Uses the Win32 API ChangeDisplaySettings() to rotate the screen

Keyboard shortcut: AutoRotateScreen will issue the keystrokes to rotate the screen

Orient 0: Ctrl+ALT+UP

Orient 1: Ctrl+ALT+LEFT

Orient 2: Ctrl+ALT+RIGHT

Orient 3: Ctrl+ALT+DOWN

See limitation 3 below.

------------------------------
Limitations:

1. It is possible to set overlapping ranges during the calibration. Should your machine be within an overlapped range, the screen orientation will switch back and forth between the affected orientations. Just take care to avoid this issue during calibration. My program will issue a warning, but it is up to you to decide whether to still accept the calibration.

2. The APS sensor is only a 2-axis device. This means that 2 laptop positions can have the same x and y values, eg. flat up, and directly upside down. Again, there's is no way for me to program around this. Just be aware when calibrating.

3. On some systems, the graphics card driver may not support the native Win32 Rotation. In that case, you can try the keyboard shortcut method. AutoRotateScreen will issue the keystrokes listed above.

This keystrokes are typical Intel graphics driver screen rotation hotkey hotkeys. If it fails, you can pair AutoRotateScreen with this program iRotate. http://www.entechtaiwan.com/util/irotate.shtm

It interfaces natively with the graphics driver to rotate the screen. It accepts the keyboard shortcuts above and then rotate.

------------------------------

For non-Thinkpad machines:

This program can will simply act as a screen orientation shortcut on the system tray. Several warning messages will show during launch. Use the commandline arguments given below to suppress those warnings. Once suppressed, one can use the tray menu to select the desired screen rotation on the fly.


Optional: You can add this program to your Startup folder or Task Scheduler if u want it to start automatically upon every login. Once calibrated, the program will start minimised to the tray. See the link for more information
http://windows.microsoft.com/en-us/windows7/Schedule-a-task

------------------------------

CommandLine Arguments:

/s : To suppress missing sensor.dll warning

/r : To suppress missing/corrupted ars-config.ini file warning

/m : Always start minimised to tray area. (Useful for non-calibrated/non-Thinkpads)

/a : Disable Auto Rotation on program start (default is enabled if program is calibrated)


------------------------------

ars-config.ini format:

Orient0: MinX MaxX MinY MaxY enable

.

.

.

RotationMethod PollRate ScreenDelayRate

To reset program, just delete this file and start calibration from scratch.

------------------------------

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

------------------------------

Credits:
1. Programming Windows 5th Edition by Charles Petzold
http://www.charlespetzold.com/pw5/

2. Numerous MSDN guides

3. Actions transform rotate Icon
Oxygen Team
http://www.iconarchive.com/show/oxygen-icons-by-oxygen-icons.org/Actions-transform-rotate-icon.html

4. Codeproject.com for Display Orientation guides and other etc
http://www.codeproject.com/Articles/36664/Changing-Display-Settings-Programmatically

5. Pulling Data from Thinkpad APS Sensor (CONG NGUYEN)
https://flashandrc.wordpress.com/2012/02/12/pulling-data-from-thinkpad-aps-sensor-using-c/

6. ThinkWiki page on the APS
http://www.thinkwiki.org/wiki/Active_Protection_System

7. Finally the National University of Singapore module CS1010 (lecturer Yung-Hsiang Lu ) for teaching me the basics of C Programming.

If I miss out any credits, please feel free to inform me Yeo Kheng Meng at yeokm1@yahoo.com.sg


------------------------------

Last Build Date: 2 Jan 2013

Compiler: MinGW GCC 4.7.2 with QT Creator 2.6.1

Language: C++11 with QT 4.8.4


Copyright 2012,2013 Yeo Kheng Meng (yeokm1@yahoo.com.sg)

Licensed under the MIT license
