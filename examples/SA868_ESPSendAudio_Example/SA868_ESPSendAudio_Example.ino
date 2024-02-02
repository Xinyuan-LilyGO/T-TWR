/**
 * @file      SA868_ESPSendAudio_Example.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-01-05
 *
 */
#include "LilyGo_TWR.h"

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
    bool rlst = false;

    Serial.begin(115200);

    //* Initializing PMU is related to other peripherals
    //* Automatically detect the revision version through GPIO detection.
    //* If GPIO2 has been externally connected to other devices, the automatic detection may not work properly.
    //* Please initialize with the version specified below.
    //* Rev2.1 is not affected by GPIO2 because GPIO2 is not exposed in Rev2.1
    twr.begin();

    //* If GPIO2 is already connected to other devices, please initialize it with the specified version.
    // twr.begin(LILYGO_TWR_REV2_0);

    if (twr.getVersion() == TWRClass::TWR_REV2V1) {
        Serial.println("Detection using TWR Rev2.1");

        // Initialize SA868
        radio.setPins(SA868_PTT_PIN, SA868_PD_PIN);
        rlst = radio.begin(RadioSerial, twr.getBandDefinition());

    } else {

        Serial.println("Detection using TWR Rev2.0");

        //* Rev2.0 cannot determine whether it is UHF or VHF, and can only specify the initialization BAND.

        //* Designated as UHF
        //  rlst = radio.begin(RadioSerial, SA8X8_UHF);

        //* Designated as VHF
        radio.setPins(SA868_PTT_PIN, SA868_PD_PIN, SA868_RF_PIN);
        rlst = radio.begin(RadioSerial, SA8X8_VHF);

    }

    if (!rlst) {
        while (1) {
            Serial.println("SA868 is not online !");
            delay(1000);
        }
    }

    //* Microphone will be routed to ESP ADC IO15 and the SA868 audio input will be routed to ESP IO18
    twr.routingMicrophoneChannel(TWRClass::TWR_MIC_TO_ESP);

    // Start transfer
    radio.transmit();
}

void loop()
{
    // Send the PWM signal output by ESP32S3 to Radio
    playScale(ESP2SA868_MIC, 0);
    delay(3000);
}


