# OpenMacro
A extensible macro device and password manager built with Arduino.

## About this repository

This repository contains three subprojects:
* **OpenMacro-Arduino:** A macropad program designed to be run on an Arduino Micro board.
* **OpenMacro-GUI:** A Qt application that can be used to configure an OpenMacro device. You will need the Arduino USB driver to detect the Arduino board.
* **OpenMacro-PCB:** A KiCad project that contains the layouts of a complete OpenMacro device.

Note that the OpenMacro-GUI project has a dependency to the OpenMacro-Arduino project's `Keycodes.h`.
