/**
 * @file      SA868_ESPSendAudio_Example.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-03-22
 *
 */

/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include "config.h"
#include "sa868.h"
#include <Wire.h>
#include <Arduino.h>
#define XPOWERS_CHIP_AXP2101
#include <XPowersLib.h>
#include "utilities.h"
/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/
SA868 sa868(Serial1, SA868_PTT_PIN, SA868_PD_PIN, SA868_RF_PIN);
XPowersPMU PMU;

/******************************************************************************/
/***        exported functions                                              ***/
/******************************************************************************/

void setupPower()
{
    bool result = PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, I2C_SDA, I2C_SCL);
    if (result == false) {
        while (1) {
            Serial.println("PMU is not online...");
            delay(500);
        }
    }

    //! DC1 ESP32S3 Core VDD , Don't change
    // PMU.setDC1Voltage(3300);

    //! DC3 Radio VDD , Don't change
    PMU.setDC3Voltage(3400);

    //! ALDO2 MICRO TF Card VDD, Don't change
    PMU.setALDO2Voltage(3300);

    //! ALDO4 GNSS VDD, Don't change
    PMU.setALDO4Voltage(3300);

    //! BLDO1 MIC VDD, Don't change
    PMU.setBLDO1Voltage(3300);


    //! The following supply voltages can be controlled by the user
    // DC5 IMAX=2A
    // 1200mV
    // 1400~3700mV,100mV/step,24steps
    PMU.setDC5Voltage(3300);

    //ALDO1 IMAX=300mA
    //500~3500mV, 100mV/step,31steps
    PMU.setALDO1Voltage(3300);

    //ALDO3 IMAX=300mA
    //500~3500mV, 100mV/step,31steps
    PMU.setALDO3Voltage(3300);

    //BLDO2 IMAX=300mA
    //500~3500mV, 100mV/step,31steps
    PMU.setBLDO2Voltage(3300);

    //! END

    // Turn on the power that needs to be used
    PMU.enableDC3();
    PMU.enableDC5();
    PMU.enableALDO1();
    PMU.enableALDO2();
    PMU.enableALDO3();
    PMU.enableALDO4();
    PMU.enableBLDO1();
    PMU.enableBLDO2();

}

void playScale(uint8_t pin, uint8_t channel)
{
    uint8_t octave = 4;
    ledcAttachPin(pin, channel);
    ledcWriteNote(channel, NOTE_C, octave);
    delay(500);
    ledcWriteNote(channel, NOTE_D, octave);
    delay(500);
    ledcWriteNote(channel, NOTE_E, octave);
    delay(500);
    ledcWriteNote(channel, NOTE_F, octave);
    delay(500);
    ledcWriteNote(channel, NOTE_G, octave);
    delay(500);
    ledcWriteNote(channel, NOTE_A, octave);
    delay(500);
    ledcWriteNote(channel, NOTE_B, octave);
    delay(500);
    ledcDetachPin(pin);
}


void setup()
{
    Serial.begin(115200);

    pinMode(MIC_CTRL_PIN, OUTPUT);
    digitalWrite(MIC_CTRL_PIN, LOW);

    setupPower();

    // Radio
    Serial1.begin(9600, SERIAL_8N1, SA868_RX_PIN, SA868_TX_PIN);

    sa868.begin();

    sa868.setVolume(5);

    //set radio to send status
    sa868.transmit();

    //Switch the audio source to ESP32S3
    digitalWrite(MIC_CTRL_PIN, HIGH);

}

void loop()
{
    // Send the PWM signal output by ESP32S3 to Radio
    playScale(ESP2SA868_MIC, 0);
    delay(3000);
}


