.. include:: links.ref

************
Get Started
************

This document is intended to guide users to build a software environment for T-TWR hardware development.

Introduction
=============

T-TWR has the following hardware resources:

    * ESP32-S3-WROOM-1-N16R8
    * OLED display (128*64)
    * SA868 Embedded small size wailie talkie module
    * TP4054 battery charging chip
    * 18650 battery holder
    * Encoder

What You Need
==============

Hardware:

    * T-TWR
    * Computer running Windows, Linux, or macOS

Software:

    * arduino
    * platformio

Install prerequisites
======================

Please complete the installation of the tool first. The specific steps are as follows:

+-------------------+-------------------+
| |arduino-logo|    | |platformio-logo| |
+-------------------+-------------------+
| |arduino-ide|_    | |platformio-ide|_ |
+-------------------+-------------------+

.. |arduino-logo| image:: ../_static/arduino-logo.png
    :target: arduino-ide_

.. |platformio-logo| image:: ../_static/platformio-logo.png
    :target: platformio-ide_

Install dependent libraries
============================

T-TWR supports the following versions of arduino-esp32:

    * |arduino-esp32|_

T-TWR needs the support of the following dependent libraries, users can use the library management to install:

    * |RotaryEncoder|_
    * |OneButton|_
    * |U8g2_Arduino|_
    * |ArduinoJson|_

.. warning::

    If there is a compilation error, please check whether the above dependent library versions are consistent.

Examples
========

* :ref:`AT Debug <at-debug>`
* :ref:`Walkie Talkie <walkie-talkie>`

Pinout
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
BUTTON_PTT_PIN  38   transmit/receive button
--------------- ---- ---------------------------------------------------------------------------------------------------
BUTTON_UP_PIN   3    volume+
--------------- ---- ---------------------------------------------------------------------------------------------------
BUTTON_DOWN_PIN 0    volume1
--------------- ---- ---------------------------------------------------------------------------------------------------
ENCODER_A_PIN   9    Encoder Pin1
--------------- ---- ---------------------------------------------------------------------------------------------------
ENCODER_B_PIN   5    Encoder Pin2
--------------- ---- ---------------------------------------------------------------------------------------------------
ENCODER_OK_PIN  7    Encoder OK button
--------------- ---- ---------------------------------------------------------------------------------------------------
BATTERY_ADC_PIN 6    Battery voltage detection pin
--------------- ---- ---------------------------------------------------------------------------------------------------
OLED_POWER_PIN  21   OLED screen power control pin
--------------- ---- ---------------------------------------------------------------------------------------------------
LED_PIN         1    User-Defined Indicators
=============== ==== ===================================================================================================

.. include:: datasheet.inc

Troubleshooting
================

If something goes wrong, first see :ref:`Troubleshooting <troubleshooting>`. If :ref:`Troubleshooting <troubleshooting>` doesn't cover the issue you're having, please get technical support via |github-issue|_.
