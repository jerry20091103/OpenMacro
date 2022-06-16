# OpenMacro-GUI


## Getting started
* Open `CMakeLists.txt` in QtCreator, using the "Open Project" button. **Qt 6.3.0 is needed.**

## Debugging
* In the top menu bar, select debug options from the Debug menu.
    * These buttons are connected to slots in `App/debugmenu.h`.
    * For serial port testing, change the port name `debugPortName` defined in `App/debugmenu.cpp` to the name of the port your USB device is connected to.
* Remember that you can also see the console in Qt Creator!

## Project structure
* `App`: Contains GUI-related logic, as well as the `.ui` file that defines the UI layouts.
* `Core`: Contains OpenMacro data structures and functions.
* `commons.h`: Manages external includes.

## How to build the executable
* After building the CMake project from inside Qt Creator, you will need to add the following Qt `.dll` files to the folder containing `OpenMacro-GUI.exe`, which can be found under `Qt/6.3.0/mingw_64/bin`.
   * `libgcc_s_seh-1.dll`
   * `libstdc++-6.dll`
   * `libwinpthread-1.dll`
   * `Qt6Core.dll`
   * `Qt6Gui.dll`
   * `Qt6SerialPort.dll`
   * `Qt6Widgets.dll`
* You will also need to add `Qt/6.3.0/mingw_64/plugins/platforms` to the same folder.
