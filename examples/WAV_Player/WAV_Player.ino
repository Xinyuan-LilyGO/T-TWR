/**
 * @file      WAV_Player.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2024-03-05
 * @note      PDM playback wav example from [SoftRF](https://github.com/lyusupov/SoftRF/)
 *            ESP8266Audio is not a mainline branch and uses SoftRF modified libraries.
 */

#include <SD.h>
#include <driver/i2s.h>
// ESP8266Audio is not a mainline branch and uses SoftRF modified libraries.
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>
#include <AudioFileSourceSD.h>

#define DEBUG_PORT Serial    // Output debugging information
#include "LilyGo_TWR.h"

#define WAV_FILE_PATH           "/POST.wav"

AudioGeneratorWAV    *wav;
AudioFileSourceSD    *file;
AudioOutputI2S       *out;

void setup()
{
    bool rslt = false;

#ifdef DEBUG_PORT
    DEBUG_PORT.begin(115200);
#endif

    //* Initializing PMU is related to other peripherals
    //* Automatically detect the revision version through GPIO detection.
    //* If GPIO2 has been externally connected to other devices, the automatic detection may not work properly.
    //* Please initialize with the version specified below.
    //* Rev2.1 is not affected by GPIO2 because GPIO2 is not exposed in Rev2.1
    rslt = twr.begin();

    //* If GPIO2 is already connected to other devices, please initialize it with the specified version.
    //rslt =  twr.begin(LILYGO_TWR_REV2_0);

    while (!rslt) {
        DBG("PMU communication failed..");
        delay(1000);
    }


    //* Rev2.1 uses OLED to determine whether it is VHF or UHF.
    //* Rev2.0 cannot determine by device address.By default, the 0X3C device address is used.
    if (twr.getVersion() == TWRClass::TWR_REV2V1) {
        DBG("Detection using TWR Rev2.1");

        //* Initialize SA868
        radio.setPins(SA868_PTT_PIN, SA868_PD_PIN);
        rslt = radio.begin(RadioSerial, twr.getBandDefinition());

    } else {

        DBG("Detection using TWR Rev2.0");

        //* Rev2.0 cannot determine whether it is UHF or VHF, and can only specify the initialization BAND.
        //* Or modify it through the menu INFO -> BAND
        radio.setPins(SA868_PTT_PIN, SA868_PD_PIN, SA868_RF_PIN);

        //* Designated as UHF
        //  rslt = radio.begin(RadioSerial, SA8X8_UHF);

        //* Designated as VHF
        rslt = radio.begin(RadioSerial, SA8X8_VHF);

    }

    // If the display does not exist, it will block here
    while (!rslt) {
        DBG("SA8x8 communication failed, please use ATDebug to check whether the module responds normally..");
        delay(300);
    }

    // Initialize SD card
    DBG("Initialize SD card");
    if (SD.begin(SD_CS)) {
        uint8_t cardType = SD.cardType();
        DBG("SD_MMC Card Type: ");
        if (cardType == CARD_MMC) {
            DBG("MMC");
        } else if (cardType == CARD_SD) {
            DBG("SDSC");
        } else if (cardType == CARD_SDHC) {
            DBG("SDHC");
        } else {
            DBG("UNKNOWN");
        }
#ifdef DEBUG_PORT
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        uint32_t cardTotal = SD.totalBytes() / (1024 * 1024);
        uint32_t cardUsed = SD.usedBytes() / (1024 * 1024);
        DBG("SD Card Size:", cardSize, "MB");
        DBG("Total space:",  cardTotal, "MB");
        DBG("Used space:",   cardUsed, "MB");
#endif
    }


    if (!SD.exists(WAV_FILE_PATH)) {
        DBG("The file does not exist. Please make sure you place the Sketch/data/POST.wav in the root directory of the SD card.");
        return;
    }

    // Turn on the power amplifier
    twr.enableRadio();

    // Route the ESP32 PDM PIN to the power amplifier
    twr.routingSpeakerChannel(TWRClass::TWR_ESP_TO_SPK);


    DBG("Initialize Audio");
    wav  = new AudioGeneratorWAV();
    file = new AudioFileSourceSD(WAV_FILE_PATH);
    out  = new AudioOutputI2S(0, AudioOutputI2S::INTERNAL_PDM);

    out->SetPinout(I2S_PIN_NO_CHANGE, I2S_PIN_NO_CHANGE, ESP32_PWM_TONE, I2S_PIN_NO_CHANGE);

    out->SetOutputModeMono(true);

    out->SetGain(0.5);

    rslt =  wav->begin(file, out);

    if (!rslt) {
        DBG("wav begin failed.");
        return;
    }

    while (1) {
        if (!wav->loop()) {
            wav->stop();
            break;
        }
    }

    DBG("Done .");

}



void loop()
{
    delay(10);
}