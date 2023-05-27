<h1 align = "center">🌟 LILYGO T-TWR 🌟</h1>

## **English | [中文](README_CN.MD)**

- **TWR 1.x version** please [check here](https://t-twr.readthedocs.io/en/latest/)

------------


###  The following content explains the precautions and solutions to problems of **TWR Plus 2.0**

**TWR Plus 2.0 FAQ**:

1. ⚠ **Unable to download the program, the error is shown below.**
   
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
      - Press and hold the **BOOT** button (without releasing it), then press the **RST** button, then release the **RST** button, and finally release the **BOOT** button
      - Click the upload button in the IDE and wait for the upload to complete
      - Press the **RST** button to exit the download mode
      
2. ⚠ **Can I change the voltage of the peripheral?**
   
     1. Not recommended, changing the peripheral voltage may cause abnormal operation, please do not change the default voltage setting of the peripheral, please refer to the voltage setting in the sample program.

3. ⚠ **Why can't I test the charging current to reach the set current?**
          
    1. If you use a USBC cable with low quality or too large or too long wire resistance, there will be a large internal resistance, and there is no rated 5V input on the board, so the PMU will think that the power supply voltage is insufficient, so the rated charging current cannot be reached. ,The solution is to replace the high-quality USBC wire, reduce the length of the wire to meet the requirements of the PMU input voltage
    2. You can measure the voltage at both ends of the figure below to see if it can meet the 5V input voltage required by the PMU
       ![](./docs/_static/input-voltage.jpg)
       
   1. Please note that if the charging current is set to 1A, please install a suitable heat sink above the PMU to reduce the risk of heat and damage caused by overheating.

4.  ⚠  When you think there is a problem with the board, you can try to burn our [factory firmware](./firmware/twr-plus%202.0/README.MD) for testing, you can first rule out whether it is a hardware problem


5. ⚠ **There is a microphone port on the bottom of the board, do I need to install a microphone?**
      1. No, there is already a condenser microphone on board. If you need to use a wired microphone, please remove the board microphone. Please note that **LilyGo will not be responsible for the damage and abnormality of the board caused by unauthorized disassembly of the microphone**

 6. ⚠ **I don’t have a **21700 battery**, can I replace the battery holder with a **18650** battery holder?**
      1. The board is compatible with **21700** and **18650** battery holders, if you have the ability to replace it, please note that **LilyGo does not bear the risk of replacing the battery holder without authorization, damage and Brought to work abnormally**

 7. ⚠ **Can't turn on automatically after connecting the battery?**
      1. Connect the battery separately, you need to press the **PWR button** for one second, the board will start the action, press and hold the **PWR** button for 6 seconds to shut down, the shutdown time can be set by software















