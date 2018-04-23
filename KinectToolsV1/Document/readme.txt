To make this work do the following

* Install Kinect SDK 1.8 [KinectSDK-v1.8-Setup.exe]
* Install Kinect Developer Toolkit 1.8 [KinectDeveloperToolkit-v1.8.0-Setup.exe]
* Install Visual Studio 2010 (express)

* The following libraries should be linked 

(Copy the following to the Linker->Input->Additional dependencies)

-------------------------
kernel32.lib
user32.lib
winspool.lib
comdlg32.lib
advapi32.lib
shell32.lib
ole32.lib
oleaut32.lib
uuid.lib
odbc32.lib
odbccp32.lib
Kinect10.lib
-------------------------

* For the class which uses any Kinect functions via NUI the windows.h should be also included

* Sometimes Kinect v1 (xbox 360 version) becomes not recognized for some reason. 
In this case you need to reboot the PC. Should work...

* LIBRARIES USED (should be stored in lib)
Use x64 libraries!
--- Free glut
--- Kinect SDK 1.8
--- Opencv 3.4.1 (use vc15 libraries)