# RLC Programming

This is a guide to get you started with programming the RLC.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine.

### Prerequisites

- You will need Arduino IDE version 1.6.5 (Must be this version). We recommend using a portable version of the arduino IDE.
- You will need to install our version of Ardublock.
- you will need to install the esp32 board packages.

### installing prerequisites

#### easy way out

To avoid the complicated steps bellow you can download our pre-setup enviorenment from this repository, available here and in releases.

#### installing Arduino IDE

These instructions will help you install a portable version of Arduino IDE 1.6.5

- Download Version 1.6.5 on the official Arduino page <a href="https://www.arduino.cc/en/Main/OldSoftwareReleases#previous">Arduino</a> according to your OS.

![Alt text](https://github.com/gimbas/rlc/blob/master/documentation/assets/Download%20Version%201.6.5.png?raw=true "Version 1.6.5")

- Extract the Downloaded file to wherever you want.

- Inside the folder you just extracted create a subfolder named "portable".

- Now you should be able to run the executable "arduino".

#### installing Ardublock

- Install the Arduino IDE if it is not already installed. instructions above.

- Download ardublock-all.jar from the releases page in this repository.

- Inside the Arduino installation Folder (the one you extracted before) you should be able to see a folder named "tools", open it.

- Create a folder named "ArduBlockTool" WARNING: THIS IS CASE SENSITVE.

- Inside the folder you just created create a new one named "tool".

- Copy ardublock-all.jar (the file you downloaded) to the folder you just created (tool).

- Now open the Arduino IDE and in the tab "Tools" you should see an option "ArduBlock" if everything went well.

#### Installing the esp32 board packages

- Download the <a href="https://github.com/espressif/arduino-esp32">arduino-esp32 core</a>

- Inside the Arduino installation Folder you should be able to see a folder named "hardware", open it

- Create a folder named "espressif"

- extract the arduino-esp32 core rename it to "esp32" and move it to this new folder

- now inside esp32/tools if you are in Windows double-click get.exe

- if you are in linux run th following in terminal (right click > open terminal here)
```
python3 get.py
```

## Running

- Open th arduino IDE you setup above.

- Go to the Tab "Tools" > "Board: ***" and select ESP32 Dev Module.

- Go to the Tab "Tools" > "Port" and select the port your board is connected to.

TODO: explain how to select the correct port.

- Now open Ardublock: Go to the Tab "Tools" and click "Ardublock".

- You can now program by dragging blocks from the selection to the left and conecting them together. you can look at our examples to get you started.

- To run your program click th "Upload" Button.

TODO: Block programming tutorial

TODO: add examples
