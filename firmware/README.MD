
# 1️⃣ Regular firmware

| FileName        | Firmware                      |
| --------------- | ----------------------------- |
| twr_2.0_uhf.bin | [firmware](./twr_2.0_uhf.bin) |
| twr_2.0_vhf.bin | [firmware](./twr_2.0_vhf.bin) |

## How falsh *.bin file ?

1. [Win10 Flash method](https://github.com/Xinyuan-LilyGO/LilyGo-Document/blob/master/WinFlashBin.md)
2. [Mac & Liunx Flash method](https://github.com/Xinyuan-LilyGO/LilyGo-Document/blob/master/MacFlashBin.md)

# 2️⃣ TinyUF2 firmware

| FileName               | Firmware                             |
| ---------------------- | ------------------------------------ |
| tinyuf2_bootloader.bin | [firmware](./tinyuf2_bootloader.bin) |
| uhf_firmware.uf2       | [firmware](./uhf_firmware.uf2)       |
| vhf_firmware.uf2       | [firmware](./vhf_firmware.uf2)       |

* To use TinyUF2, you need to burn tinyuf2_bootloader.bin first. After the burning is completed, a disk will pop up. Drag firmware.uf2 into the disk.

## How to use ArduinoIDE to flash firmware again?

* After using TinyUF2, a virtual port will be generated and cannot be written directly using ArduinoIDE. The TWR must be put into download mode before the actual physical port will be displayed.
  
## How to put TWR into download mode

* Press the **PWR** button for one second to make sure the TWR is powered on
* Press and hold the **BOOT** button (without releasing it), then press the **RST** button, then release the **RST** button, and finally release the **BOOT** button
* Click the upload button in the IDE and wait for the upload to complete
* Press the **RST** button to exit the download mode



