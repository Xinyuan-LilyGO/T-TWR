<h1 align = "center">🌟 LILYGO T-TWR 🌟</h1>

## **English | [中文](README_CN.MD)**

- **TWR 1.x version** please [check here](https://t-twr.readthedocs.io/en/latest/)

------------


###  The following content explains the precautions and solutions to problems of **TWR Plus 2.0**

## 1️⃣ PlatformIO Quick Start

1. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
2. Search for the `PlatformIO` plugin in the `VisualStudioCode` extension and install it.
3. After the installation is complete, you need to restart `VisualStudioCode`
4. After restarting `VisualStudioCode`, select `File` in the upper left corner of `VisualStudioCode` -> `Open Folder` -> select the `T-TWR` directory
5. Click on the `platformio.ini` file, and in the `platformio` column, cancel the sample line that needs to be used, please make sure that only one line is valid
6. Click the (✔) symbol in the lower left corner to compile
7. Connect the board to the computer USB
8. Click (→) to upload firmware
9. Click (plug symbol) to monitor serial output


## 2️⃣ Arduino IDE Quick Start

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Install [Arduino ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/) 
3. Copy all the folders in the `T-TWR/lib` directory to `<C:\Users\UserName\Documents\Arduino\libraries>`. If there is no `libraries` directory, please create a new one. Please note that you are not copying the `lib` directory, but copying the folders in the lib directory
4. Open ArduinoIDE -> Tools
   - Board -> ESP32S3 Dev Module
   - USB CDC On Boot -> Enable  ## Note that you need to change Enable to Disable when USB is not connected, so USB CDC will not prevent the board from starting
   - CPU Frequency -> 240MHz
   - USB DFU On Boot -> Disable
   - Flash Mode -> QIO 80MHz
   - Flash Size -> 16MB(128Mb)
   - USB Firmware MSC On Boot -> Disable
   - PSRAM -> OPI PSRAM
   - Partition Scheme -> 16M Flash(3MB APP/9.9MB FATFS)
   - USB Mode -> Hardware CDC and JIAG
   - Upload Mode -> UART0/Hardware CDC
   - Upload Speed -> 921600
5. Insert USB into the PC and click Upload <If the upload fails, View the FAQ below>


## 3️⃣ FAQ:

1. ⚠⚠⚠ **Precautions (special matters)**:
    1. The **glue stick (RF) antenna** must be connected. If the antenna is not connected, the RF module may be damaged, and the PMU will automatically turn off the power output
    2. A separate USB power supply may not meet the power supply requirements, please connect the battery to use
 
 2. ⚠ **Unable to download the program, the error is shown below.**
   
    ```shell
    Flash: [====      ]  35.7% (used 467813 bytes from 1310720 bytes)
    Configuring upload protocol...
    AVAILABLE: cmsis-dap, esp-bridge, esp-builtin, esp-prog, espota, esptool, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa
    CURRENT: upload_protocol = esptool
    Looking for upload port...
    Auto-detected: COM236
    Uploading .pio\build\uhf_band\firmware.bin
    esptool.py v4.5
    Serial port COM236
    Connecting...

    A serial exception error occurred: ClearCommError failed (PermissionError(13, 'The device does not recognize the command.', None, 22))
    Note: This error originates from pySerial. It is likely not a problem with esptool, but with the hardware connection or drivers.
    For troubleshooting steps visit: https://docs.espressif.com/projects/esptool/en/latest/troubleshooting.html
    *** [upload] Error 1
    ================================================================================================================ [FAILED] Took 8.75 seconds ================================================================================================================

    ```
    > Solution
      - Press the **PWR** button for one second to make sure the TWR is powered on
      - Press and hold the **BOOT** button (without releasing it), then press the **RST** button, then release the **RST** button, and finally release the **BOOT** button
      - Click the upload button in the IDE and wait for the upload to complete
      - Press the **RST** button to exit the download mode
      
2. ⚠ **Can I change the voltage of the peripheral?**
   
     1. Not recommended, changing the peripheral voltage may cause abnormal operation, please do not change the default voltage setting of the peripheral, please refer to the voltage setting in the sample program.

3. ⚠ **Why can't I test the charging current to reach the set current?**
          
    1. If you use a USBC cable with low quality or too large or too long wire resistance, there will be a large internal resistance, and there is no rated 5V input on the board, so the PMU will think that the power supply voltage is insufficient, so the rated charging current cannot be reached. ,The solution is to replace the high-quality USBC wire, reduce the length of the wire to meet the requirements of the PMU input voltage
    2. You can measure the voltage at both ends of the figure below to see if it can meet the 5V input voltage required by the PMU
      <img width="320"  src=docs/_static/input-voltage.jpg>
       
   1. Please note that if the charging current is set to 1A, please install a suitable heat sink above the PMU to reduce the risk of heat and damage caused by overheating.

4.  ⚠  When you think there is a problem with the board, you can try to burn our [factory firmware](./firmware/README.MD) for testing, you can first rule out whether it is a hardware problem


5. ⚠ **There is a microphone port on the bottom of the board, do I need to install a microphone?**
      1. No, there is already a condenser microphone on board. If you need to use a wired microphone, please remove the board microphone. Please note that **LilyGo will not be responsible for the damage and abnormality of the board caused by unauthorized disassembly of the microphone**

 6. ⚠ **I don’t have a **21700 battery**, can I replace the battery holder with a **18650** battery holder?**
      1. The board is compatible with **21700** and **18650** battery holders, if you have the ability to replace it, please note that **LilyGo does not bear the risk of replacing the battery holder without authorization, damage and Brought to work abnormally**

 7. ⚠ **Can't turn on automatically after connecting the battery?**
      1. Connect the battery separately, you need to press the **PWR button** for one second, the board will start the action, press and hold the **PWR** button for 6 seconds to shut down, the shutdown time can be set by software

8. TWR currently has two modes before leaving the factory, one is flashed with regular firmware and uses the esp built-in boot, and the other uses TinyUF2 as the boot program. For novices, please [see here](./firmware/README.MD)

9. How to enter TinyUF2 boot mode?
   * Before entering tinyUF2, the boot program must have been flashed. If it has not been written, [please see here](./firmware/README.MD)
   * Press the RST button
   * After pressing the RST button for one second, press the BOOT button. The disk will be ejected on the computer.











