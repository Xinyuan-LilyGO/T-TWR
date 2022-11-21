.. _troubleshooting:

****************
Troubleshooting
****************

1. How to enter download mode?
================================

First press and hold the **Volume-** button, then lightly press the reset button,
and finally release the **Volume-** button to enter the download mode.

2. SA868 power problem
========================

The SA868 module supports low (1.6W) / high (1.8W) power; the power can be
selected by controlling the level of Pin#7 of SA868.
The program uses low power by default, if you choose high power, it will cause
the temperature of T-TWR to be too high, please use it with caution.

.. warning::

    When there is no program in esp32s3, Pin#39 of esp32s3 is floating,
    and the power of SA868 will be set to high power.
