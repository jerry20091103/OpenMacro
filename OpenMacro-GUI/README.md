# OpenMacro-GUI


## Getting started
* Open `CMakeLists.txt` in QtCreator, using the "Open Project" button.

## Debugging
* In the top menu bar, select debug options from the Debug menu.
    * These buttons are connected to slots in `App/debugmenu.h`.
    * For serial port testing, change the port name `debugPortName` defined in `App/debugmenu.cpp` to the name of the port your USB device is connected to.
* Remember that you can also see the console in Qt Creator!

## Project Structure
* `App`: Contains GUI-related logic, as well as the `.ui` file that defines the UI layouts.
* `Core`: Contains OpenMacro data structures and functions.