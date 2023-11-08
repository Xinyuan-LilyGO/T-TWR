


<h1 align = "center">✨ LILYGO T-TWR  ✨</h1>

## **[English](README.MD) | 中文**


- **TWR 1.x 版本** 请 [查看这里](https://t-twr.readthedocs.io/en/latest/)

------------


###  以下说明仅针对 **TWR Plus 2.0**


## 1️⃣ PlatformIO Quick Start

1. 安装[VisualStudioCode](https://code.visualstudio.com/) 和 [Python](https://www.python.org/)
2. 在`VisualStudioCode`扩展中搜索`PlatformIO`插件并安装.
3. 安装完成后需要将`VisualStudioCode`重新启动
4. 重新开启`VisualStudioCode`后,选择 `VisualStudioCode` 左上角的 `文件` -> `打开文件夹` ->选择 `T-TWR` 目录
5. 点击 `platformio.ini` 文件,在`platformio` 栏目中取消需要使用的示例行,请确保仅仅一行有效
6. 点击左下角的（✔）符号编译
7. 将板子与电脑USB进行连接
8. 点击（→）上传固件
9. 点击 (插头符号) 监视串行输出


## 2️⃣ Arduino IDE Quick Start

1. 安装 [ArduinoIDE](https://www.arduino.cc/en/software)
2. 安装 [Arduino ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/) 
3. 将 `T-TWR/lib` 目录内的所有文件夹拷贝到`<C:\Users\UserName\Documents\Arduino\libraries>`,如果没有`libraries`目录,请新建,请注意,不是拷贝`lib`目录,而是拷贝lib目录里面的文件夹
4. 打开ArduinoIDE -> Tools 
   - Board -> ESP32S3 Dev Module
   - USB CDC On Boot -> Enable   # 注意，在不连接USB的时候你需要将Enable改为Disable，这样USB CDC 不会阻止板子的启动
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
5. 插入USB到PC,点击上传<如果无法顺利上传,请查看下方常见问题>

## 3️⃣ 常见问题:

1. ⚠⚠⚠ 注意事项(特别事项):
   1. 必须连接 **胶棒(RF)天线** ，如果不连接天线使用，有可能损坏射频模块，并且PMU会自动关闭电源输出
   2. 单独USB进行供电可能无法满足供电需求,请连接电池使用
   
2. ⚠ **无法下载程序,错误如下面所示.**
   
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
    > 解决办法
     - 按压一秒 **PWR** 按键，确保TWR已经开机
     - 按住**BOOT**按键(不松开)，然后按下**RST**按键，再放开**RST**按键，最后放开**BOOT**按键
     - 点击IDE中的上传按钮,等待上传完毕
     - 按下**RST**按键，退出下载模式

3. ⚠ **我可以更改外设的电压吗 ？**

       1. 不推荐,更改外设电压可能导致工作异常,请不要更改外设的默认设置电压,请参照示例程序中的电压设定.
 
 4. ⚠**为什么我测试充电电流达不到所设定的电流?**
      1. 使用劣质或者线径电阻过大过长的USBC线会有较大的内阻，到达板子上已经没有额定的5V输入了,所以PMU会认为供电电压不足,所以达不到额定的充电电流,解决办法是更换优质的USBC线材，减量减少线的长度,以满足PMU输入电压的要求
      2. 可以测量下图两端的电压查看是否能够达到PMU要求的5V输入电压
         <img width="320"  src=docs/_static/input-voltage.jpg>
     1. 请注意,如果设定充电电流为1A，请在PMU上方安装合适的散热器以减少热量和过热导致的损坏风险.

 5. ⚠ **当你认为板子有问题时,你可以尝试烧录我们的 [出厂固件](./firmware/README.MD) 进行测试,可以先排除是否是硬件问题**

 6. ⚠ 板子底部有个麦克风的接口，我需要安装麦克风吗 ？
     1. 不需要,板载已经有一个电容麦克风了，如果你需要使用连线麦克风，请移除板子麦克风，请注意,**LilyGo 不承担私自拆卸麦克风的导致板子的损坏和异常**

 7. ⚠ 我没有**21700电池**，我可以将电池座更换为 **18650**电池座吗？
     1. 板子兼容 **21700** 和 **18650** 型号的电池座,如果你有能力进行更换是可以的,请注意,**LilyGo 不承担私自更换电池座所带来的风险,损坏和带来的工作异常**

 8. ⚠ **接入电池后无法自动开机?**
     1. 单独接入电池，需要按下 **PWR按键** 一秒，板子才会进行开机的动作,长按**PWR**按键6秒将进行关机,关机时间可以通过软件设定


