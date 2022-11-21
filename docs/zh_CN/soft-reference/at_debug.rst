.. include:: ../links.ref
.. _at-debug:

*********
AT Debug
*********

用于调试SA868的通讯协议。

esp32s3作为串口转发器，PC通过串口工具，将SA868的指令发送到esp32s3，esp32s3转发到SA868。
同理，SA868的应答指令发送到esp32s3，然后转发到PC。

Arduino
========

推荐使用以下配置：

.. image:: ../../_static/arduino-setting.jpg

PlatformIO
===========

请参考 |platformio-using|_。

烧录
=====

T-TWR 通过 USB 链接到电脑。先按住 **音量键-**，再轻按复位键，最后松开 **音量键-**，即可进入到下载模式。

引脚定义
========

=============== ==== ===================================================================================================
Name            Pin  Note
--------------- ---- ---------------------------------------------------------------------------------------------------
SA868_TX_PIN    47   Serial transmit data pin
--------------- ---- ---------------------------------------------------------------------------------------------------
SA868_RX_PIN    48   Serial receive data pin
--------------- ---- ---------------------------------------------------------------------------------------------------
SA868_PTT_PIN   41   SA868 Transmitting/receiving control, "0" force the module to enter TX state; and "1" to Rx state.
--------------- ---- ---------------------------------------------------------------------------------------------------
SA868_PD_PIN    40   Power Down control, "0" for power down mode; "1" for the normal work
--------------- ---- ---------------------------------------------------------------------------------------------------
SA868_RF_PIN    39   high/low output power control; Leave open for high output power, low level to low output power.
--------------- ---- ---------------------------------------------------------------------------------------------------
BUTTON_PTT_PIN  38   发射/接收按钮
=============== ==== ===================================================================================================

数据手册
=========

* `ESP32-S3-WROOM-1-N16R8`_ (Datasheet)
* `SA868`_ (Datasheet)

.. _ESP32-S3-WROOM-1-N16R8: https://www.espressif.com.cn/sites/default/files/documentation/esp32-s3-wroom-1_wroom-1u_datasheet_cn.pdf
.. _SA868: https://github.com/Xinyuan-LilyGO/T-TWR/blob/master/datasheet/SA868%202W%E5%B5%8C%E5%85%A5%E5%BC%8F%E5%B0%8F%E4%BD%93%E7%A7%AF%E5%AF%B9%E8%AE%B2%E6%9C%BA%E6%A8%A1%E5%9D%97V1.2.pdf

Troubleshooting
================

如果出现故障，请先查阅 :ref:`故障排查 <troubleshooting>`。如果 :ref:`故障排查 <troubleshooting>` 不存在你遇到的问题，请通过 |github-issue|_ 获得技术支持。
