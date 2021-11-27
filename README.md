# Decolorizer

If you have kids into coloring, this might be for you.
As they always want to color something that's already colored, we sometimes need to do some painful paintbrush work.
So here we try to do this automatically.

## Screenshots
![Program screenshot](/img/screenshot.png)
![Output result](/img/output.png)

##  Description
The code combines contours and lines detection using OpenCV.
The decoloring is done in several steps:
- Converts the original image to a gray image
- Applies blur to the resulting image 
- Creates a blank image with identical size.
- Performs line detection and outputs the result on the blank image
- Performs contour detection and draws the result on the previous image
The resulting image can be saved, either from the dialog or by using the "Show Image" button which presents an OpenCV widget with saving menu.
The interface supports also printing using the QT related libraries, but not tested thoroughly.

##  About the code
This is a simple mockup of a generic "Decolor" class and its usage from a QT application. No effort was spent on debugging, neither on making this a "nice" componenent. It works as intended so far, and it was my actual take on checking out what QT and OpenCV can do together, in a portable way.

## ToDo
The algorithms can be improved. Probably performing adding contrast first might ease the overall output.
Also, different settings for the line/contour detection will probably yield better results.

## Building
It should be noted that OpenCV was rebuilt with QT support (QT 6.2 currently) for both Linux and Windows.
On Windows, the build is done without OpenGL because on this version the QGlWidget is not available (it was deprecated a while ago).
The project itself was done using QT creator with cmake support (not qmake). Some cmake settings related to the windows build need to be changed (the path to the OpenCV install). On Linux, this should work without other settings.

## Package
The intention is to release the application as a zip archive or installer on Windows. No such release is intended for Linux. It was, though, tested on Ubuntu 21.10.