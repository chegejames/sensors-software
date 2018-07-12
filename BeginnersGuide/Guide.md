# Beginner's Guide
![NodeMCU](img/NodeMCU-v2.jpg)
Instead of programming the ESP8266 or NodeMCU with the terminal, the Arduino IDE can be used. This tutorial shows the steps to prepare the Arduino IDE for the ESP8266.

## Identify the NodeMCU
Turn over NodeMCU and see if CP2102 or CH340G drivers need to be installed.
Only for Windows and Mac, Linux does not need drivers.

#### NodeMCU v2
CP210x USB to UART Bridge driver
[Mac / Win Driver](https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx)

#### NodeMCU v3
CH340G Bridge driver
[Driver for Mac](http://www.wch.cn/download/CH341SER_MAC_ZIP.html)
[Drivers for Windows](http://www.arduined.eu/files/CH341SER.zip)

After installing the driver, restart the computer.

Note for MacOS 10.9 or later: If an unsigned driver is installed, it must be explicitly allowed in MacOS. A good description can be found at [tzapu.com] (https://tzapu.com/making-ch340-ch341-serial-adapters-work-under-el-capitan-os-x/)

## Install Board Manager
Download & install the Arduino IDE:
https://www.arduino.cc/en/Main/Software

In the settings in the**Additional Board Manager URLs**field insert this URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json

![Settings](img/Einstellungen.png)

In**Tools**→**Board**→**Board Manager**search for the ESP8266 package and install.

![Board Manager](img/Boardmanager.png)

## ESP8266 / NodeMCU settings
After restarting the Arduino IDE in **Tools** → **Board**, select the appropriate board.
- NodeMCU 0.9 (ESP-12 modules) **for NodeMCU v1**
- NodeMCU 1.0 (ESP-12E modules) **for NodeMCU v2 and v3**

![NodeMCU Settings](img/NodeMCU.jpg)

## Select port
Select Port**Tools**→**Port**.
For the CP2102, this would be the**/ dev / cu.SLAB_USBTOUART**on the Mac, for the CH340G it's the port**/ dev / cu.wchusbserialXXXXXXXX**on the Windows COM3. If this is not the right one then just pick the next one in the list.

## Copy code to the NodeMCU
Insert the latest code from GitHub into the Arduino IDE.
*Tip: to copy the code better just click on the **RAW** button*
https://raw.githubusercontent.com/CodeForAfricaLabs/sensors-software/develop/airrohr-firmware/airrohr-firmware.ino

#### Set wifi
![Wifi Settings](img/Wifi.jpg)

in the**WiFi declaration**enter the SSID (name of the WLAN) and the corresponding password.

#### Flash NodeMCU
Click on the → or in the menu *Sketch* → *Upload*.

![Dubbing the code](img/Uberspielen.jpg)

## Check
**Tools** → **Enable Serial Montior** so that you can view the current data locally via USB interface.
Now it can check if it leads to errors at one place, e.g. NodeMCU can not log in to the wlan, can not upload the data, ...

![Check](img/Uberspielen.jpg)