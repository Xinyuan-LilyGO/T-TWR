/**
 * @file      Factory.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-01-04
 * @note      Compatible with LilyGo TWR Rev2.0 and Rev2.1
 * * Currently, SA868 only supports NiceRF factory AT firmware and does not support OpenRTX firmware for the time being.
 * * Menu inspired by https://giltesa.com/2020/09/07/menu-grafico-para-pantalla-oled-en-arduino
 */

#include <U8g2lib.h>
#include <Rotary.h>
#include <TinyGPS++.h>
#include <WiFi.h>
#include <BLEServer.h>
#include <WiFiAP.h>
#include <SD.h>
#include "BLE.h"
#include "fix_fft.h"
#include <AceButton.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>
#include "Constants.h"
#include "WebIndex.h"


// #define DEBUG_PORT Serial    // Output debugging information
#include "LilyGo_TWR.h"

using namespace ace_button;

enum Button {
    ShortPress,
    LongPress,
    Unknown
};

#define DECLARE_DEMO(function)      void function(uint8_t menuSelect)
#define U8G2_HOR_ALIGN_CENTER(t)    ((u8g2.getDisplayWidth() - (u8g2.getUTF8Width(t))) / 2)
#define U8G2_HOR_ALIGN_RIGHT(t)     (u8g2.getDisplayWidth()  -  u8g2.getUTF8Width(t))


DECLARE_DEMO(demoLed);
DECLARE_DEMO(demoSpeaker);
DECLARE_DEMO(demoOled);
DECLARE_DEMO(demoButton);
DECLARE_DEMO(demoMic);
DECLARE_DEMO(demoAlarm);
DECLARE_DEMO(demoSensor);
DECLARE_DEMO(demoGPS);
DECLARE_DEMO(demoSDCard);
DECLARE_DEMO(demoPMU);
DECLARE_DEMO(demoTransFreq);
DECLARE_DEMO(demoRecvFreq);
DECLARE_DEMO(demoSquelchLevel);
DECLARE_DEMO(demoPowerLevel);
DECLARE_DEMO(demoFilter);
DECLARE_DEMO(demoBLE);
DECLARE_DEMO(demoWiFi);
DECLARE_DEMO(demoSetting);
DECLARE_DEMO(demoDevInfo);
DECLARE_DEMO(drawError);


uint32_t readRotary(uint32_t cur, uint32_t minOut, uint32_t maxOut, uint32_t steps = 1 );
void printMain();
void printPowerOFF();
Button readButton();

struct demo_struct {
    const char      *demo_item;
    const char      *demo_description;
    const uint8_t   demo_icon[32];
    void            (*demo_func)(uint8_t menuSelect);
} demo [] = {
    //* {
    //*         Title,
    //*         Description,
    //*         XBM ICON,
    //*         Callback,
    //* },
    {
        "LED",
        "PIXELS RGB LED",
        { 0x00, 0x00, 0xe0, 0x01, 0xe0, 0x01, 0x60, 0x3f, 0x7f, 0x7f, 0x7f, 0xc1, 0x60, 0x81, 0x60, 0x8f, 0x60, 0x8f, 0x60, 0x81, 0x7f, 0xc1, 0x7f, 0x7f, 0x60, 0x3f, 0xe0, 0x01, 0xe0, 0x01, 0x00, 0x00 },
        demoLed
    },
    {
        "SPEAKER",
        "PLAYING GAME MELODIES",
        { 0x00, 0x00, 0x80, 0x01, 0xc0, 0x01, 0xe0, 0x01, 0xb0, 0x11, 0x9e, 0x21, 0x8e, 0x45, 0x86, 0x49, 0x86, 0x49, 0x8e, 0x45, 0x9e, 0x21, 0xb0, 0x11, 0xe0, 0x01, 0xc0, 0x01, 0x80, 0x01, 0x00, 0x00 },
        demoSpeaker
    },
    {
        "OLED",
        "BRIGHTNESS SETTING",
        { 0x00, 0x00, 0x00, 0x00, 0xfe, 0x7f, 0xfe, 0x7f, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0xfe, 0x7f, 0xfe, 0x7f, 0x80, 0x01, 0x80, 0x01, 0xf0, 0x0f, 0xf0, 0x0f, 0x00, 0x00 },
        demoOled
    },
    {
        "BUTTON",
        "COUNTING BUTTON PRESSES",
        { 0x00, 0x00, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xfe, 0x7f, 0xfe, 0x7f, 0x06, 0x60, 0x06, 0x60, 0x36, 0x6c, 0xfe, 0x7f, 0xfe, 0x7f, 0x30, 0x0c, 0x30, 0x0c, 0x30, 0x0c, 0x00, 0x00 },
        demoButton
    },
    {
        "ALARM",
        "SEND ALERT",
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcf, 0x7b, 0xdb, 0xdb, 0x71, 0x8e, 0x63, 0x1e, 0x6f, 0x7e, 0x7e, 0xf6, 0x78, 0xc6, 0x71, 0x8e, 0xdb, 0xdb, 0xde, 0xf3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        demoAlarm
    },
    {
        "MICROPHONE",
        "MICROPHONE TEST",
        { 0x00, 0x00, 0xc0, 0x03, 0xe0, 0x07, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0x60, 0x06, 0xec, 0x37, 0xcc, 0x33, 0x1c, 0x38, 0xf8, 0x1f, 0xe0, 0x07, 0x80, 0x01, 0x80, 0x01, 0xe0, 0x07, 0xf0, 0x0f },
        demoMic
    },
    {
        "BME280 SENSOR",
        "EXTERNAL SENSOR",
        { 0xe0, 0x00, 0xf0, 0x01, 0x18, 0x03, 0x58, 0x7b, 0x58, 0x03, 0x58, 0x03, 0x58, 0x7b, 0x58, 0x03, 0x58, 0x03, 0x4c, 0x66, 0xe6, 0x0c, 0xf6, 0x0d, 0xe6, 0x6c, 0x0c, 0x06, 0xf8, 0x03, 0xf0, 0x01 },
        demoSensor
    },
    {
        "GPS",
        "LOCATION INFORMATION",
        {0xe0, 0x07, 0xf0, 0x0f, 0x38, 0x1c, 0xcc, 0x33, 0xec, 0x37, 0x6e, 0x76, 0x26, 0x64, 0x66, 0x66, 0xee, 0x77, 0xcc, 0x33, 0x18, 0x18, 0x38, 0x1c, 0x70, 0x0e, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01},
        demoGPS
    },
    {
        "STORAGE",
        "SD CARD INFORMATION",
        {0x00, 0x00, 0xc0, 0x1f, 0xe0, 0x3f, 0x70, 0x35, 0x78, 0x35, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xf8, 0x1f, 0x00, 0x00},
        demoSDCard
    },
    {
        "POWER",
        "POWER MANAGEMENT",
        {0x1e, 0x7a, 0x1f, 0xfb, 0x83, 0xc3, 0xc3, 0xc1, 0xe3, 0xc1, 0xa0, 0x01, 0x90, 0x1f, 0x18, 0x10, 0xfc, 0x09, 0x80, 0x0d, 0x80, 0x07, 0x83, 0xc2, 0x83, 0xc3, 0xc3, 0xc1, 0xdf, 0xf8, 0xde, 0x78},
        demoPMU
    },
    {
        "TRANS FREQ",
        "RADIO TRANS FREQ",
        {0x00, 0x00, 0x00, 0x0c, 0x00, 0x1c, 0x00, 0x3c, 0xc0, 0x7f, 0x30, 0xe0, 0x38, 0xe0, 0xfc, 0x77, 0xee, 0x3f, 0x06, 0x1c, 0x06, 0x0c, 0xfc, 0x03, 0xf8, 0x01, 0x30, 0x00, 0x20, 0x00, 0x00, 0x00},
        demoTransFreq
    },

    {
        "RECV FREQ",
        "RADIO RECV FREQ",
        {0x00, 0x00, 0x00, 0x0c, 0x00, 0x1c, 0x00, 0x3c, 0xc0, 0x7f, 0x30, 0xe0, 0x38, 0xe0, 0xfc, 0x77, 0xee, 0x3f, 0x06, 0x1c, 0x06, 0x0c, 0xfc, 0x03, 0xf8, 0x01, 0x30, 0x00, 0x20, 0x00, 0x00, 0x00},
        demoRecvFreq
    },

    {
        "SQUELCH LEVEL",
        "RADIO SQUELCH LEVEL",
        {0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x70, 0x3c, 0xf0, 0x7c, 0xf0, 0x7f, 0xf0, 0x1f, 0xf0, 0x3e, 0xfe, 0x3e, 0xfc, 0x3e, 0x7c, 0x1f, 0xfc, 0x0f, 0xf8, 0x01, 0xf0, 0x03, 0xc0, 0x03, 0x00, 0x00},
        demoSquelchLevel
    },
    {
        "POWER LEVEL",
        "RADIO POWER LEVEL",
        {0xf0, 0x0f, 0xf8, 0x1f, 0x1c, 0x18, 0x06, 0x40, 0x07, 0xc2, 0x03, 0xc7, 0x43, 0xc7, 0x63, 0xc7, 0xfb, 0xdf, 0xbb, 0xdd, 0x83, 0xc1, 0x87, 0xe0, 0x06, 0x60, 0x1c, 0x38, 0xf8, 0x1f, 0xf0, 0x0f},
        demoPowerLevel
    },
    {
        "FILTER",
        "RADIO FILTER",
        {0x00, 0x00, 0x00, 0x08, 0x00, 0x1e, 0xfe, 0x77, 0xfe, 0x7f, 0x00, 0x1c, 0x78, 0x00, 0xfe, 0x7f, 0xfe, 0x7f, 0x78, 0x00, 0x00, 0x1c, 0xfe, 0x7f, 0xfe, 0x7f, 0x00, 0x1e, 0x00, 0x08, 0x00, 0x00},
        demoFilter
    },
    {
        "BLUETOOTH",
        "BLUETOOTH CONFIGURE",
        {0x80, 0x01, 0x80, 0x03, 0x80, 0x07, 0x98, 0x0f, 0xb8, 0x1d, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0xc0, 0x03, 0xe0, 0x07, 0xf0, 0x0f, 0xb8, 0x1d, 0x98, 0x0f, 0x80, 0x07, 0x80, 0x03, 0x80, 0x01},
        demoBLE
    },
    {
        "WIFI",
        "WIFI CONFIGURE",
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0xfc, 0x3f, 0xfe, 0x7f, 0x1f, 0xf8, 0xe6, 0x67, 0xf0, 0x0f, 0xf8, 0x1f, 0x30, 0x0c, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00},
        demoWiFi
    },
    {
        "SETTING",
        "SYSTEM SETTING",
        {0x00, 0x00, 0xc0, 0x03, 0xc0, 0x03, 0xfc, 0x3f, 0x7c, 0x3e, 0xc6, 0x63, 0xee, 0x77, 0x6c, 0x36, 0x6c, 0x36, 0xee, 0x77, 0xc6, 0x63, 0x7c, 0x3e, 0xfc, 0x3f, 0xc0, 0x03, 0xc0, 0x03, 0x00, 0x00},
        demoSetting
    },
    {
        "INFO",
        "DEVICE INFO",
        {0xf0, 0x0f, 0xf8, 0x1f, 0x3c, 0x3c, 0x8e, 0x71, 0x87, 0xe1, 0x07, 0xe0, 0x03, 0xc0, 0x83, 0xc1, 0x83, 0xc1, 0x83, 0xc1, 0x87, 0xe1, 0x87, 0xe1, 0x8e, 0x61, 0x3c, 0x3c, 0xf8, 0x1f, 0xf0, 0x0f},
        demoDevInfo
    },
};

const uint8_t itemsMENU = COUNT(demo);

struct RotarySetting {
    uint32_t cur;
    uint32_t max;
    uint32_t min;
    uint32_t steps;
};

AsyncWebServer                      server(80);
U8G2_SH1106_128X64_NONAME_F_HW_I2C  u8g2(U8G2_R0, U8X8_PIN_NONE);
Rotary                              rotary = Rotary(ENCODER_A_PIN, ENCODER_B_PIN);
TinyGPSPlus                         gps;
Adafruit_BME280                     bme;
AceButton                           buttons[3];
Button                              state = Unknown;
Adafruit_NeoPixel                   strip = Adafruit_NeoPixel(1, PIXELS_PIN, NEO_GRB + NEO_KHZ800);
QueueHandle_t                       rotaryMsg;
QueueHandle_t                       rotarySetting;
TaskHandle_t                        rotaryHandler;
bool                                inMenu = true;
bool                                isBMEOnline = false;
const char                          *ssid = "T-TWR";
const char                          *password = "12345678";

const uint8_t                       buttonPins [] = {
    ENCODER_OK_PIN,
    BUTTON_PTT_PIN,
    BUTTON_DOWN_PIN
};

void endWeb()
{
    server.end();
    MDNS.end();
}

// Just for testing, no functionality
void setupWeb()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send(200, "text/html", web_html );
    });

    MDNS.begin("twr");

    MDNS.addService("http", "tcp", 80);

    server.begin();
}

void setRotaryValue(uint32_t cur, uint32_t min, uint32_t max, uint32_t steps)
{
    static RotarySetting rSetting = {0};
    rSetting.cur = cur;
    rSetting.max = max;
    rSetting.min = min;
    rSetting.steps = steps;
    xQueueOverwrite(rotarySetting, (void *)&rSetting);
}


void rotaryTask(void *p)
{
    RotarySetting setting = {0};
    uint32_t pos = 0;

    rotary.begin();

    while (1) {

        uint8_t result = rotary.process();

        if (xQueueReceive(rotarySetting, (void *)&setting, ( TickType_t ) 2) == pdPASS) {
            pos = setting.cur;
            xQueueOverwrite(rotaryMsg, (void *)&pos);
            continue;
        }

        if (result) {
            if (result == DIR_CW) {
                pos += setting.steps;
                DBG("Up");
            } else {
                if (pos != 0) {
                    pos -= setting.steps;
                }
                DBG("Down");
            }
            if (pos <= setting.min) {
                pos = setting.min;
            }
            if (pos >= setting.max) {
                pos = setting.max;
            }
            xQueueOverwrite(rotaryMsg, (void *)&pos);
        }
        delay(2);
    }
}

void handleEvent(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
    uint8_t id = button->getId();

    DBG(F("EventType: "), AceButton::eventName(eventType), F("; buttonState: "), buttonState, F("; ID: "), id);

    // rotary center button
    switch (id) {
    case 0: {
        switch (eventType) {
        case AceButton::kEventClicked:
            state = ShortPress;
            DBG("ShortPress");
            break;
        case AceButton::kEventLongPressed:
            state = LongPress;
            DBG("LongPress");
            break;
        default:
            break;
        }
    }
    break;

    // PTT button
    case 1:
        switch (eventType) {
        case AceButton::kEventPressed:
            DBG("transmit");
            strip.setPixelColor(0, strip.Color(255, 0, 0));
            strip.show();
            radio.transmit();
            break;
        case AceButton::kEventReleased:
            DBG("receive");
            strip.clear();
            strip.show();
            radio.receive();
            break;
        default:
            break;
        }
        break;

    // BOOT button
    case 2:
        switch (eventType) {
        case AceButton::kEventPressed:
            if (!inMenu) {
                state = LongPress;
                DBG("LongPress");
            } else {
                uint8_t v = radio.getVolume();
                radio.setVolume(--v);
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

bool setupSDCard()
{
    if (SD.begin(SD_CS, SPI)) {
        uint8_t cardType = SD.cardType();
        if (cardType != CARD_NONE) {
            return true;
        }
    }
    return false;
}

bool setupBME280()
{
    uint8_t slaveAddress = 0x00;
    Wire.beginTransmission(0x76);
    if (Wire.endTransmission() == 0) {
        slaveAddress = 0x76;
    }
    Wire.beginTransmission(0x77);
    if (Wire.endTransmission() == 0) {
        slaveAddress = 0x77;
    }
    if (slaveAddress == 0) {
        return false;
    }
    if (! bme.begin(slaveAddress, &Wire)) {
        DBG("Could not find a valid BME280 sensor, check wiring!");
        isBMEOnline = false;
        return false;
    }
    bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                    Adafruit_BME280::SAMPLING_X1, // temperature
                    Adafruit_BME280::SAMPLING_X1, // pressure
                    Adafruit_BME280::SAMPLING_X1, // humidity
                    Adafruit_BME280::FILTER_OFF   );
    isBMEOnline = true;
    return isBMEOnline;
}

void setupOLED(uint8_t addr)
{
    u8g2.setI2CAddress(addr << 1);
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setContrast(10);
    uint8_t b = 10;
    u8g2.firstPage();
    do {
        do {
            u8g2.setFont(u8g2_font_tenstamps_mu);
            u8g2.drawXBM(40, 0, 48, 48, xbmLOGO);
            u8g2.setCursor(22, 62);
            u8g2.print("LILYGO");
            u8g2.sendBuffer();
            u8g2.setContrast(b);
        } while (u8g2.nextPage());
        b += 10;
        delay(8);
    } while (b < twr.pdat.dispBrightness);
}

void setup()
{
    bool rslt = false;

#ifdef DEBUG_PORT
    DEBUG_PORT.begin(115200);
#endif

    // Create message queue
    rotaryMsg = xQueueCreate(1, sizeof(uint32_t));
    rotarySetting = xQueueCreate(1, sizeof(RotarySetting));

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

    // Initialize pixel lights
    strip.setBrightness(twr.pdat.pixelBrightness);
    strip.begin();
    strip.clear();
    strip.show();

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
        // rslt = radio.begin(RadioSerial, SA8X8_VHF);

        // If BAND is not specified, the default BAND is used, set through the menu.
        rslt = radio.begin(RadioSerial, SA8X8_UNKNOW);
    }

    // If the display does not exist, it will block here
    while (!rslt) {
        DBG("SA8x8 communication failed, please use ATDebug to check whether the module responds normally..");
        strip.setPixelColor(0, strip.Color(255, 0, 0));
        strip.show();
        delay(300);
        strip.clear();
        strip.show();
        delay(300);
    }

    // Initialize SD card
    if (setupSDCard()) {
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

    // Initialize the temperature and pressure sensor.
    // Note that this sensor is an external addition and is not integrated in TWR.
    setupBME280();

    // Initialize display
    setupOLED(twr.getOLEDAddress());

    delay(3000);

    // Initialize button
    for (uint8_t i = 0; i < COUNT(buttonPins); i++) {
        pinMode(buttonPins[i], INPUT_PULLUP);
        buttons[i].init(buttonPins[i], HIGH, i);
    }
    ButtonConfig *buttonConfig = ButtonConfig::getSystemButtonConfig();
    buttonConfig->setEventHandler(handleEvent);
    buttonConfig->setFeature(ButtonConfig::kFeatureClick);
    buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
    buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
    buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);

    // Disable the PMU long press shutdown function.
    twr.enablePowerOff(false);

    // Added display of shutdown animation after long pressing the PWR button
    twr.attachLongPressed([]() {
        printPowerOFF();
    });

    // Add a callback function after pressing the PWR button
    twr.attachClick([]() {
        if (inMenu) {
            uint8_t v = radio.getVolume();
            radio.setVolume(++v);
        }
    });

    // Create a rotary encoder processing task
    xTaskCreate(rotaryTask, "rotary", 10 * 1024, NULL, 10, &rotaryHandler);

}


void loop()
{
    static uint8_t menuSelect     = 0;
    static uint8_t prevMenuSelect = menuSelect;
    Button btnPressed;

    inMenu = true;
    do {
        btnPressed = readButton();
        printMain();

        // DeepSleep test , About 900uA
        if (btnPressed == LongPress) {
            DBG("DeepSleep....");
            printDeepSleep();

            strip.clear();
            strip.show();

            radio.sleep();

            twr.sleep();

            esp_sleep_enable_ext1_wakeup(_BV(BUTTON_PTT_PIN), ESP_EXT1_WAKEUP_ALL_LOW);

            esp_deep_sleep_start();
        }

    } while ( btnPressed != ShortPress );

    inMenu = false;
    uint8_t lastItem = 0;

    if (twr.isEnableBeep()) {
        twr.routingSpeakerChannel(TWRClass::TWR_ESP_TO_SPK);
    }

    while (1) {
        do {
            menuSelect  = readRotary(lastItem, 0, itemsMENU - 1);
            btnPressed = readButton();
            if (btnPressed == LongPress) {
                menuSelect = 0;
                break;
            }
            if ( menuSelect != prevMenuSelect ) {
                prevMenuSelect = menuSelect;
                beep();
            }
            printMenu(menuSelect);

        } while ( btnPressed != ShortPress );

        beep();

        if (btnPressed == ShortPress) {
            demo[menuSelect].demo_func(menuSelect);
            lastItem = menuSelect;
        } else {
            break;
        }
    }
    menuSelect = 0;
    if (twr.isEnableBeep()) {
        twr.routingSpeakerChannel(TWRClass::TWR_RADIO_TO_SPK);
    }
}

void printPowerOFF()
{
    int b = twr.pdat.dispBrightness;
    do {
        u8g2.firstPage();
        do {
            u8g2.setFont(u8g2_font_tenstamps_mu);
            u8g2.drawXBM(40, 0, 48, 48, xbmOFF);
            u8g2.setCursor(5, 59);
            u8g2.print("POWEROFF");
            u8g2.setContrast(b);
        } while ( u8g2.nextPage() );
        b -= 10;
        delay(8);
    } while (b >= 0);
    twr.shutdown();
}

void printDeepSleep()
{
    int b = twr.pdat.dispBrightness;
    do {
        u8g2.firstPage();
        do {
            u8g2.setFont(u8g2_font_tenstamps_mu);
            u8g2.drawXBM(40, 0, 48, 48, xbmOFF);
            u8g2.setCursor(32, 59);
            u8g2.print("SLEEP");
            u8g2.setContrast(b);
        } while ( u8g2.nextPage() );
        b -= 10;
        delay(8);
    } while (b >= 0);

    u8g2.setPowerSave(true);
}

void printMain()
{
    uint8_t rssi_x = 95;
    uint8_t rssi_y = 12;
    uint8_t bat_x = 110;
    uint8_t bat_y = 12;
    static int rssi = 0;
    static uint32_t check_interval = 0;
    int percentage = 0;
    static bool  showStrip = false;

    u8g2.firstPage();
    do {
        drawFrame();

        bool isTransmit = radio.isTransmit();
        // Draw RSSI
        if (!isTransmit) {
            if (millis() > check_interval) {
                rssi = radio.getRSSI();
                check_interval = millis() + 1000;
            }
        }

        percentage = twr.getBatteryPercent();

        u8g2.setFont(u8g2_font_siji_t_6x10);

        if (rssi >= 80) {
            u8g2.drawGlyph(rssi_x, rssi_y, 0xe261);
        } else if (rssi >= 40) {
            u8g2.drawGlyph(rssi_x, rssi_y, 0xe260);
        } else if (rssi >= 20) {
            u8g2.drawGlyph(rssi_x, rssi_y, 0xe25f);
        } else if (rssi >= 10) {
            u8g2.drawGlyph(rssi_x, rssi_y, 0xe25e);
        } else {
            u8g2.drawGlyph(rssi_x, rssi_y, 0xe25d);
        }

        // Battery
        if (twr.isCharging()) {
            u8g2.drawGlyph(bat_x, bat_y, 0xe239);
        } else if (percentage < 0) {
            u8g2.drawGlyph(bat_x, bat_y, 0xe242);
        } else if (percentage >= 80) {
            u8g2.drawGlyph(bat_x, bat_y, 0xe251);
        } else if (percentage >= 40) {
            u8g2.drawGlyph(bat_x, bat_y, 0xe250);
        } else if (percentage >= 20) {
            u8g2.drawGlyph(bat_x, bat_y, 0xe24f);
        } else if (percentage >= 10) {
            u8g2.drawGlyph(bat_x, bat_y, 0xe24e);
        } else {
            u8g2.drawGlyph(bat_x, bat_y, 0xe24d);
        }

        // GPS
        u8g2.drawGlyph(80, 12, 0xe02c);

        int start_pos = 70;
        // Ble
        if (twr.getSetting().ble) {
            u8g2.drawGlyph(start_pos, 12, 0xe00b);
            start_pos -= 13;
        }
        // Wifi
        if (twr.getSetting().wifi) {
            u8g2.drawGlyph(start_pos, 12, 0xe21a);
            start_pos -= 10;
        }
        // SD
        if (SD.cardType() != CARD_NONE
                && SD.cardType() != CARD_UNKNOWN) {
            u8g2.drawGlyph(start_pos - 2, 11, 0xE120);
            start_pos -= 10;
        }

        if (isTransmit) {
            u8g2.drawGlyph(start_pos, 12, 0xe04C);
        }

        if (twr.isReceiving) {
            u8g2.drawGlyph(start_pos, 12, 0xe275);
            if (!showStrip) {
                strip.setPixelColor(0, strip.Color(0, 255, 0));
                strip.show();
                showStrip = true;
            }
        } else {
            if (showStrip) {
                showStrip = false;
                strip.setPixelColor(0, strip.Color(0, 0, 0));
                strip.show();
            }
        }

        int volume = radio.getVolume();
        for (int i = 0; i < volume; ++i) {
            u8g2.drawGlyph(5 + (4 * i), 12, 0x007c);
        }

        float transFreq = radio.getStetting().transFreq / 1000000.0;
        float recvFreq  = radio.getStetting().recvFreq / 1000000.0;

        // Draw Freq
        u8g2.setFont(u8g2_font_pxplusibmvga8_mr    );
        u8g2.setCursor(22, 35);
        u8g2.print("TX:");
        u8g2.setCursor(22, 50);
        u8g2.print("RX:");

        u8g2.setFont(u8g2_font_9x6LED_mn);
        u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(transFreq, 4).c_str()) - 21, 35 );
        u8g2.print(transFreq, 4);
        u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(recvFreq, 4).c_str()) - 21, 50 );
        u8g2.print(recvFreq, 4);

    } while ( u8g2.nextPage() );
}

void printMenu( uint8_t menuSelect )
{
    u8g2.firstPage();
    do {
        //ROW 1:
        if ( menuSelect > 0 ) {
            u8g2.drawXBMP(4, 2,  14, 14, xMenuUp);
            u8g2.setFont(u8g2_font_haxrcorp4089_tr);
            u8g2.setCursor(22, 13);
            u8g2.print(demo[menuSelect - 1].demo_item);

        }
        //ROW 2:
        u8g2.setFont(u8g2_font_nokiafc22_tu);
        u8g2.drawBox(0, 18, 128, 27);

        u8g2.setColorIndex(0);
        u8g2.drawXBMP(3, 24, 16, 16, demo[menuSelect].demo_icon);

        if ( strlen(demo[menuSelect].demo_description) == 0 ) {
            u8g2.drawStr(22, 35, demo[menuSelect].demo_item);
        } else {
            u8g2.drawStr(22, 30, demo[menuSelect].demo_item);
            u8g2.setFont(u8g2_font_micro_mr);
            u8g2.drawStr(22, 40, demo[menuSelect].demo_description);
        }

        //ROW 3:
        u8g2.setColorIndex(1);
        if ( menuSelect < itemsMENU - 1 ) {
            u8g2.drawXBMP(4, 48, 14, 14, xMenuDown);
            u8g2.setFont(u8g2_font_haxrcorp4089_tr);
            u8g2.setCursor(22, 58);
            u8g2.print(demo[menuSelect + 1].demo_item);
        }
        drawFrame();
    } while ( u8g2.nextPage() );
}

void demoLed( uint8_t menuSelect )
{
    int prevValue = 0;
    int value = strip.getBrightness();
    Button btnPressed;
    strip.setPixelColor(0, strip.Color(255, 0, 0));
    strip.show();

    do {
        value   = readRotary(value, 0, 250, 10);
        btnPressed = readButton();
        if (prevValue != value) {
            beep();
            strip.setBrightness(value);
            if (prevValue == 0) {
                strip.setPixelColor(0, strip.Color(255, 0, 0));
            }
            strip.show();
            prevValue  = value;
        }
        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);

            u8g2.drawFrame(13, 30, 102, 12);
            u8g2.drawFrame(14, 31, 100, 10);

            uint32_t bar = map(value, 0, 255, 0, 100);
            for ( int x = 0 ; x < bar ; x++ ) {
                u8g2.drawVLine(15 + x, 32, 8);
            }

            u8g2.setFont(u8g2_font_nokiafc22_tu);
            u8g2.setCursor(14, 51);
            u8g2.print(F("BRIGHTNESS:"));

            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(bar).c_str()) - 14, 51 );
            u8g2.print(value);
        } while ( u8g2.nextPage() );

    } while ( btnPressed != LongPress );


    SAVE_CONFIGURE(pixelBrightness, value);

    strip.clear();
    strip.show();
}

void demoSpeaker( uint8_t menuSelect )
{
    Button   btnPressed;
    uint32_t prevTime   = 0;
    int value = 600;

    if (!twr.isEnableBeep()) {
        twr.routingSpeakerChannel(TWRClass::TWR_ESP_TO_SPK);
    }

    do {
        value   = readRotary(value, 100, 3000, 100);
        btnPressed = readButton();

        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);

            u8g2.drawFrame(13, 30, 102, 12);
            u8g2.drawFrame(14, 31, 100, 10);

            uint32_t bar = map(value, 100, 3000, 0, 100);
            for ( int x = 0 ; x < bar ; x++ ) {
                u8g2.drawVLine(15 + x, 32, 8);
            }

            u8g2.setFont(u8g2_font_nokiafc22_tu);
            u8g2.setCursor(14, 51);
            u8g2.print(F("FREQ:"));

            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(bar).c_str()) - 35, 51 );
            u8g2.print(value); u8g2.print("HZ");

        } while ( u8g2.nextPage() );

        if (millis() > prevTime) {
            tone(ESP32_PWM_TONE, value, 100);
            prevTime = millis() + 300;
        }

    } while (btnPressed != LongPress);

    if (!twr.isEnableBeep()) {
        twr.routingSpeakerChannel(TWRClass::TWR_RADIO_TO_SPK);
    }
}

void demoOled( uint8_t menuSelect )
{
    int prevValue = 0;
    int value = twr.pdat.dispBrightness;
    Button btnPressed;
    do {

        value   = readRotary(value, 0, 250, 10);
        btnPressed = readButton();
        if (prevValue != value) {
            prevValue  = value;
            beep();
            u8g2.setContrast(value);
        }

        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);
            u8g2.drawFrame(13, 30, 102, 12);
            u8g2.drawFrame(14, 31, 100, 10);
            uint32_t bar = map(value, 0, 250, 0, 100);
            for ( int x = 0 ; x < bar ; x++ ) {
                u8g2.drawVLine(15 + x, 32, 8);
            }
            u8g2.setFont(u8g2_font_nokiafc22_tu);
            u8g2.setCursor(14, 51);
            u8g2.print(F("BRIGHTNESS:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(bar).c_str()) - 14, 51 );
            u8g2.print(value);
        } while (u8g2.nextPage());
    } while ( btnPressed != LongPress );

    SAVE_CONFIGURE(dispBrightness, value);
}

void demoButton( uint8_t menuSelect )
{
    static uint32_t  pressedCounter = 0;
    Button           btnPressed;
    do {
        btnPressed = readButton();
        if ( btnPressed == ShortPress) {
            pressedCounter++;
            beep();
        }
        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);

            u8g2.setFont(u8g2_font_nokiafc22_tu);
            u8g2.setCursor(14, 43);
            u8g2.print(F("CLICKS:"));

            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(pressedCounter).c_str()) - 14, 43 );
            u8g2.print(pressedCounter);
        } while ( u8g2.nextPage() );

    } while ( btnPressed != LongPress );
}

void demoAlarm(uint8_t menuSelect )
{
    Button   btnPressed;
    uint32_t prevTime   = 0;
    int value = 600;
    twr.routingMicrophoneChannel(TWRClass::TWR_MIC_TO_ESP);
    radio.transmit();

    strip.setPixelColor(0, strip.Color(0, 255, 0));
    strip.show();

    do {
        value   = readRotary(value, 100, 2000, 100);
        btnPressed = readButton();

        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);

            u8g2.drawFrame(13, 30, 102, 12);
            u8g2.drawFrame(14, 31, 100, 10);

            uint32_t bar = map(value, 100, 2000, 0, 100);
            for ( int x = 0 ; x < bar ; x++ ) {
                u8g2.drawVLine(15 + x, 32, 8);
            }

            u8g2.setFont(u8g2_font_nokiafc22_tu);
            u8g2.setCursor(14, 51);
            u8g2.print(F("FREQ:"));

            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(bar).c_str()) - 35, 51 );
            u8g2.print(value); u8g2.print("HZ");

        } while ( u8g2.nextPage() );

        if (millis() > prevTime) {
            tone(ESP2SA868_MIC, value, 100);
            prevTime = millis() + 1000;
        }

    } while (btnPressed != LongPress);

    radio.receive();

    twr.routingMicrophoneChannel(TWRClass::TWR_MIC_TO_RADIO);

    strip.clear();
    strip.show();
}

void demoMic( uint8_t menuSelect )
{
    Button     btnPressed;
    int8_t dataIn[128], dataOut[128];


    if (twr.getVersion() == TWRClass::TWR_REV2V0) {
        drawError(menuSelect); return;
    }

    twr.routingMicrophoneChannel(TWRClass::TWR_MIC_TO_ESP);


    do {
        btnPressed = readButton();
        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);


            for (int i = 0; i < 128; i++) {                     //take 128 samples
                int val = analogRead(ESP_MIC_ADC);              //get samples from GPIO15
                dataOut[i] = val / 4 - 128;                     //each element of array is val/4-128
                dataIn[i] = 0;
            };
            char x = 0, ylim = 60;
            fix_fft(dataOut, dataIn, 7, 0);                                    //perform the FFT on dataOut
            for (int i = 1; i < 64; i++) {                              // In the current design, 60Hz and noise
                int dat = sqrt(dataOut[i] * dataOut[i] + dataIn[i] * dataIn[i]);      //filter out noise and hum
                u8g2.drawLine(i * 2 + x, ylim, i * 2 + x, ylim - dat);  // draw bar graphics for freqs above 500Hz to buffer
            };

        } while ( u8g2.nextPage() );
    } while ( btnPressed != LongPress );

    twr.routingMicrophoneChannel(TWRClass::TWR_MIC_TO_RADIO);
}

void demoSensor( uint8_t menuSelect )
{
    float    humid = 0;
    float    pressure = 0;
    float    temperature = 0;
    uint32_t intervalue = 0;
    Button   btnPressed;
    do {
        if (isBMEOnline && millis() > intervalue) {
            humid          = bme.readHumidity() ;
            temperature    = bme.readTemperature() ;
            pressure       = (bme.readPressure() / 100.0F) ;
            intervalue = millis() + 1000;
        }
        btnPressed = readButton();

        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);

            u8g2.setFont(u8g2_font_nokiafc22_tu);

            u8g2.setCursor(14, 31);
            u8g2.print(F("HUMIDITY:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(humid, 2).c_str()) - 22, 31 );
            u8g2.print(humid, 2);
            u8g2.setCursor(109, 31);
            u8g2.print(F("%"));

            u8g2.setCursor(14, 44);
            u8g2.print(F("PRESSURE:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(pressure, 2).c_str()) - 22, 44 );
            u8g2.print(pressure, 2);
            u8g2.setCursor(109, 44);
            u8g2.print(F("HPA"));

            u8g2.setCursor(14, 57);
            u8g2.print(F("TEMP:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(temperature, 2).c_str()) - 22, 57 );
            u8g2.print(temperature, 2);
            u8g2.setCursor(109, 57);
            u8g2.print(F("C"));
        } while ( u8g2.nextPage() );

    } while ( btnPressed != LongPress );
}

void demoGPS(uint8_t menuSelect)
{
    Button btnPressed;
    uint8_t ledState = LOW;
    bool enableNMEAOutSerial = false;
    const uint32_t debounceDelay = 50;
    uint32_t lastDebounceTime = 0;

    do {

        btnPressed = readButton();

#if ARDUINO_USB_CDC_ON_BOOT
        if (btnPressed == ShortPress) {
            // DBG("enableNMEAOutSerial.....");
            enableNMEAOutSerial = !enableNMEAOutSerial;
        }
#endif

        // GPS PPS
        int pps = digitalRead(GNSS_PPS_PIN);
        if (pps == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
            ledState = !ledState;
            if (ledState) {
                strip.setPixelColor(0, strip.Color(0, 255, 0));
            } else {
                strip.clear();
            }
            strip.show();
            lastDebounceTime = millis();
            twr.ledToggle();
        }

        while (GPSSerial.available()) {
            int c = GPSSerial.read();
#if ARDUINO_USB_CDC_ON_BOOT
            if (enableNMEAOutSerial) {
                Serial.write(c);
            }
#endif
            gps.encode(c);
        }

        u8g2.firstPage();
        do {
            const uint8_t offset = 10;
            drawFrame();
            drawHeader(menuSelect);

            u8g2.setColorIndex(0);
            u8g2.setFont(u8g2_font_nokiafc22_tu);
            if (gps.location.isValid()) {
                u8g2.drawStr(100, 12, "3D");
            } else {
                u8g2.drawStr(100, 12, "N/A");
            }
            u8g2.setColorIndex(1);

            u8g2.setCursor(14, 31);
            u8g2.print(F("LONGITUDE:"));
            String lng = gps.location.isValid() ? String(gps.location.lng(), 5) : "N/A";
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(lng.c_str()) - offset, 31 );
            u8g2.print(lng);

            u8g2.setCursor(14, 44);
            u8g2.print(F("LATITUDE:"));
            String lat = gps.location.isValid() ? String(gps.location.lat(), 5).c_str() : "N/A";
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(lat.c_str()) - offset, 44 );
            u8g2.print(lat);

            u8g2.setCursor(14, 57);
            u8g2.print(F("SPEED:"));
            String kmph = gps.location.isValid() ? String(gps.speed.kmph(), 1).c_str() : "N/A";
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(kmph.c_str()) - 35, 57 );
            u8g2.print(kmph);
            u8g2.setCursor(99, 57);
            u8g2.print(F("KM/H"));

        } while ( u8g2.nextPage() );

    } while ( btnPressed != LongPress );

    strip.clear();
    strip.show();
}

void demoSDCard(uint8_t menuSelect)
{
    Button   btnPressed;
    uint32_t cardSize = 0;
    uint32_t cardTotal = 0;
    uint32_t cardUsed = 0;
    uint32_t intervalue = 0;

    do {
        btnPressed = readButton();
        u8g2.firstPage();
        do {

            if (SD.cardType() == CARD_NONE) {
                if (millis() > intervalue) {
                    if (!setupSDCard()) {
                        cardSize = 0;
                        cardTotal = 0;
                        cardUsed = 0;
                    }
                    intervalue = millis() + 8000UL;
                }
            }
            if (SD.cardType() != CARD_NONE && cardSize == 0) {
                cardSize  = SD.cardSize() / (1024 * 1024);
                cardTotal = SD.totalBytes() / (1024 * 1024);
                cardUsed  = SD.usedBytes() / (1024 * 1024);
            }

            drawFrame();
            drawHeader(menuSelect);

            u8g2.setFont(u8g2_font_nokiafc22_tu);

            u8g2.setCursor(14, 31);
            u8g2.print(F("SIZE:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(cardSize).c_str()) - 22, 31 );
            u8g2.print(cardSize);
            u8g2.setCursor(109, 31);
            u8g2.print(F("MB"));

            u8g2.setCursor(14, 44);
            u8g2.print(F("TOTAL:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(cardTotal).c_str()) - 22, 44 );
            u8g2.print(cardTotal);
            u8g2.setCursor(109, 44);
            u8g2.print(F("MB"));

            u8g2.setCursor(14, 57);
            u8g2.print(F("USED:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(cardUsed).c_str()) - 22, 57 );
            u8g2.print(cardUsed);
            u8g2.setCursor(109, 57);
            u8g2.print(F("MB"));
        } while ( u8g2.nextPage() );

    } while ( btnPressed != LongPress );
}

void demoPMU(uint8_t menuSelect)
{
    float    voltageUSB;
    float    voltageBattery;
    float    tempPMU;
    Button btnPressed;

    do {
        voltageUSB      = twr.getVbusVoltage() / 1000.0;
        voltageBattery  = twr.getBattVoltage() / 1000.0;
        tempPMU         = twr.getTemperature();
        btnPressed      = readButton();

        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);

            u8g2.setFont(u8g2_font_nokiafc22_tu);

            u8g2.setCursor(14, 31);
            u8g2.print(F("USB:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(voltageUSB).c_str()) - 22, 31 );
            u8g2.print(voltageUSB);
            u8g2.setCursor(109, 31);
            u8g2.print(F("V"));

            u8g2.setCursor(14, 44);
            u8g2.print(F("BATTERY:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(voltageBattery).c_str()) - 22, 44 );
            u8g2.print(voltageBattery);
            u8g2.setCursor(109, 44);
            u8g2.print(F("V"));

            u8g2.setCursor(14, 57);
            u8g2.print(F("TEMP. PMU:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(tempPMU).c_str()) - 22, 57 );
            u8g2.print(tempPMU);
            u8g2.setCursor(109, 57);
            u8g2.print(F("C"));
        } while ( u8g2.nextPage() );

    } while ( btnPressed != LongPress );
}

uint32_t demoFreqSetting(uint8_t menuSelect,
                         bool tx,
                         uint32_t curFreq,
                         uint8_t option,
                         uint32_t minOut,
                         uint32_t maxOut,
                         uint32_t steps
                        )
{
    Button btnPressed;
    uint32_t value = 0;
    uint32_t prevValue = 0;
    uint32_t bandwidth = radio.getStetting().bandwidth ;
    uint32_t  transFreq = tx ? radio.getStetting().transFreq : radio.getStetting().recvFreq;
    uint32_t CXCSS = tx ? radio.getStetting().txCXCSS : radio.getStetting().rxCXCSS;
    do {
        btnPressed = readButton();
        value = readRotary(curFreq, minOut, maxOut, steps);
        if ( btnPressed == ShortPress ) {
            beep();
        }
        if (prevValue != value) {
            beep();
            prevValue = value;
        }
        u8g2.firstPage();
        do {

            drawFrame();
            drawHeader(menuSelect);

            u8g2.setFont(u8g2_font_nokiafc22_tu);

            u8g2.setCursor(14, 31);
            u8g2.print(F("FREQ  :"));

            if (option == 0) {
                u8g2.drawRBox(55, 22, 58, 12, 2);
                u8g2.setColorIndex(0);
                u8g2.setCursor(U8G2_HOR_ALIGN_RIGHT(String(value / 1000000.0, 5).c_str()) - 22, 31 );
                u8g2.print(value / 1000000.0, 5);
                u8g2.setColorIndex(1);
            } else {
                u8g2.setCursor(U8G2_HOR_ALIGN_RIGHT(String(transFreq / 1000000.0, 5).c_str()) - 22, 31 );
                u8g2.print(transFreq / 1000000.0, 5);
            }

            u8g2.setCursor(14, 44);
            u8g2.print(F("STEPS:"));
            if (option == 1) {
                u8g2.drawRBox(55, 35, 58, 12, 2);
                u8g2.setColorIndex(0);
                u8g2.setCursor(U8G2_HOR_ALIGN_RIGHT(String(value / 1000.0, 1).c_str()) - 45, 44 );
                u8g2.print(value / 1000.0, 1);
                u8g2.print("KHZ");
                u8g2.setColorIndex(1);
            } else {
                u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(bandwidth / 1000.0, 1).c_str()) - 45, 44 );
                u8g2.print(bandwidth / 1000.0, 1);
                u8g2.print("KHZ");
            }

            u8g2.setCursor(14, 57);
            u8g2.print(F("CXCSS:"));
            if (option == 2) {
                u8g2.drawRBox(55, 48, 58, 12, 2);
                u8g2.setColorIndex(0);
                u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(cxcss[value]).c_str()) - 38, 57 );
                u8g2.print(cxcss[value]);
                u8g2.setColorIndex(1);
            } else {
                u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(cxcss[CXCSS]).c_str()) - 38, 57 );
                u8g2.print(cxcss[CXCSS]);
            }
        } while (u8g2.nextPage());

    } while ( btnPressed != ShortPress );

    return  value ;
}

void setTransFreq(uint8_t menuSelect, bool tx)
{
    Button btnPressed;
    uint32_t pos = 0;
    uint32_t prevPos = 0;
    uint32_t minOut = radio.getStetting().minFreq;
    uint32_t maxOut = radio.getStetting().maxFreq;
    uint32_t transFreq = tx ? radio.getStetting().transFreq : radio.getStetting().recvFreq;
    uint32_t bandwidth = radio.getStetting().bandwidth ;
    uint32_t CXCSS = tx ? radio.getStetting().txCXCSS : radio.getStetting().rxCXCSS;

    do {
        btnPressed = readButton();
        pos = readRotary(0, 0, 2);

        if (prevPos != pos) {
            prevPos  = pos;
            beep();
        }

        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);
            u8g2.setFont(u8g2_font_nokiafc22_tu);

            switch (pos) {
            case 0:
                u8g2.drawRFrame(55, 22, 58, 12, 2);
                break;
            case 1:
                u8g2.drawRFrame(55, 35, 58, 12, 2);
                break;
            case 2:
                u8g2.drawRFrame(55, 48, 58, 12, 2);
                break;
            default:
                break;
            }
            u8g2.setCursor(14, 31);
            u8g2.print(F("FREQ  :"));
            u8g2.setCursor(U8G2_HOR_ALIGN_RIGHT(String(transFreq / 1000000.0, 5).c_str()) - 22, 31 );
            u8g2.print(transFreq / 1000000.0, 5);

            u8g2.setCursor(14, 44);
            u8g2.print(F("STEPS:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(bandwidth / 1000.0, 1).c_str()) - 45, 44 );
            u8g2.print(bandwidth / 1000.0, 1);
            u8g2.print("KHZ");

            u8g2.setCursor(14, 57);
            u8g2.print(F("CXCSS:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(cxcss[CXCSS]).c_str()) - 38, 57 );
            u8g2.print(cxcss[CXCSS]);

        } while ( u8g2.nextPage() );

        if (btnPressed == ShortPress ) {

            beep();

            uint32_t steps = bandwidth;
            uint32_t cur = transFreq;

            switch (pos) {
            case 0:
                minOut = radio.getStetting().minFreq;
                maxOut = radio.getStetting().maxFreq;
                steps = radio.getStetting().bandwidth ;
                transFreq = tx ? radio.getStetting().transFreq : radio.getStetting().recvFreq;
                break;
            case 1:
                minOut = 12500;
                maxOut = 25000;
                steps =  12500;
                bandwidth = radio.getStetting().bandwidth ;
                cur = bandwidth;
                break;
            case 2:
                minOut = 0;
                maxOut = COUNT(cxcss);
                steps =  1;
                cur = CXCSS;
                break;
            default:
                break;
            }

            uint32_t setValue = demoFreqSetting(menuSelect, tx, cur, pos, minOut, maxOut, steps);
            DBG("Set opt:", pos, "value:", setValue);

            switch (pos) {
            case 0:
                tx ? radio.setTxFreq(setValue) : radio.setRxFreq(setValue);
                break;
            case 1:
                if (radio.getStetting().bandwidth != setValue) {
                    radio.setBandWidth(setValue);
                    // Bandwidth change, reset frequency to the minimum frequency value
                    tx ? radio.setTxFreq(radio.getStetting().minFreq) : radio.setRxFreq(radio.getStetting().minFreq);
                }
                break;
            case 2:
                tx ? radio.setTxCXCSS(setValue) : radio.setRxCXCSS(setValue);
                break;
            default:
                break;
            }
            transFreq = tx ? radio.getStetting().transFreq : radio.getStetting().recvFreq;
            bandwidth = radio.getStetting().bandwidth ;
            CXCSS = tx ? radio.getStetting().txCXCSS : radio.getStetting().rxCXCSS;
        }

    } while ( btnPressed != LongPress );

    radio.saveConfigure();
}

void demoTransFreq(uint8_t menuSelect)
{
    setTransFreq(menuSelect, true);
}

void demoRecvFreq(uint8_t menuSelect)
{
    setTransFreq(menuSelect, false);
}

void demoSquelchLevel(uint8_t menuSelect)
{
    int    value        = radio.getStetting().SQ;
    int    prevSQ       = value;
    Button btnPressed;
    do {
        value   = readRotary(value, 0, 8);
        btnPressed = readButton();

        if (prevSQ != value) {
            prevSQ  = value;
            beep();
        }

        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);

            u8g2.drawFrame(13, 30, 102, 12);
            u8g2.drawFrame(14, 31, 100, 10);

            uint32_t bar = map(value, 0, 8, 0, 100);
            for ( int x = 0 ; x < bar ; x++ ) {
                u8g2.drawVLine(15 + x, 32, 8);
            }

            u8g2.setFont(u8g2_font_nokiafc22_tu);
            u8g2.setCursor(14, 51);
            u8g2.print(F("SQUELCH LEVEL:"));
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(String(bar).c_str()) - 14, 51 );
            u8g2.print(value);
        } while ( u8g2.nextPage() );

    } while ( btnPressed != LongPress );

    radio.setSquelchLevel(value);

    radio.saveConfigure();
}

void demoPowerLevel(uint8_t menuSelect)
{
    Button btnPressed;
    int value = radio.getStetting().txPower;
    do {
        value           = readRotary(value, 0, 1);
        btnPressed      = readButton();
        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);

            u8g2.setFont(u8g2_font_nokiafc22_tu);
            u8g2.setCursor(45, 31);
            u8g2.print(F("LOW  1.6W"));
            u8g2.setCursor(45, 44);
            u8g2.print(F("HIGH 1.8W"));

            u8g2.setCursor(19, 31);
            // TX POWER: High and low power settings (0: high power; 1: low power)
            if (value) {
                u8g2.print(F("[ * ]"));
            } else {
                u8g2.print(F("[    ]"));
            }
            u8g2.setCursor(19, 44);
            if (!value) {
                u8g2.print(F("[ * ]"));
            } else {
                u8g2.print(F("[    ]"));
            }
        } while ( u8g2.nextPage() );

        if (btnPressed == ShortPress) {
            beep();
            value ? radio.lowPower() : radio.highPower();
            value = radio.getStetting().txPower;
        }

    } while ( btnPressed != LongPress );

    radio.saveConfigure();
}

void demoFilter(uint8_t menuSelect)
{
    Button btnPressed;
    int value = 0;
    int prevValue = 0;
    bool  emphasis = radio.getStetting().emphasis;
    bool  highPass = radio.getStetting().highPass;
    bool  lowPass = radio.getStetting().lowPass;

    do {
        value       = readRotary(0, 0, 2);
        btnPressed  = readButton();

        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);

            if (prevValue != value) {
                prevValue = value;
                beep();
            }
            u8g2.setFont(u8g2_font_nokiafc22_tu);

            u8g2.setCursor(14, 31);
            if (emphasis) {
                u8g2.print(F("   *    ")) ;
            } else {
                u8g2.print(F("   -    ")) ;
            }
            u8g2.setCursor(45, 31);
            u8g2.print(F("EMPHASIS")) ;

            u8g2.setCursor(14, 44);
            if (highPass) {
                u8g2.print(F("   *    ")) ;
            } else {
                u8g2.print(F("   -    ")) ;
            }
            u8g2.setCursor(45, 44);
            u8g2.print(F("HIGH PASS")) ;

            u8g2.setCursor(14, 57);
            if (lowPass) {
                u8g2.print(F("   *    ")) ;
            } else {
                u8g2.print(F("   -    ")) ;
            }
            u8g2.setCursor(45, 57);
            u8g2.print(F("LOW PASS")) ;

            switch (value) {
            case 0:
                u8g2.setCursor(14, 31);
                if (emphasis) {
                    u8g2.print(F("[  *  ]")) ;
                } else {
                    u8g2.print(F("[  -  ]")) ;
                }
                break;
            case 1:
                u8g2.setCursor(14, 44);
                if (highPass) {
                    u8g2.print(F("[  *  ]")) ;
                } else {
                    u8g2.print(F("[  -  ]")) ;
                }
                break;
            case 2:
                u8g2.setCursor(14, 57);
                if (lowPass) {
                    u8g2.print(F("[  *  ]")) ;
                } else {
                    u8g2.print(F("[  -  ]")) ;
                }
                break;
            default:
                break;
            }

            if (btnPressed == ShortPress) {
                beep();
                switch (value) {
                case 0:
                    radio.enableEmpHassis(!emphasis);
                    emphasis = radio.getStetting().emphasis;
                    break;
                case 1:
                    radio.enableHighPass(!highPass);
                    highPass = radio.getStetting().highPass;
                    break;
                case 2:
                    u8g2.setCursor(14, 57);
                    radio.enableLowPass(!lowPass);
                    lowPass = radio.getStetting().lowPass;
                    break;
                default:
                    break;
                }
            }

        } while ( u8g2.nextPage() );

    } while ( btnPressed != LongPress );

    radio.saveConfigure();
}

void demoBLE(uint8_t menuSelect)
{
    Button btnPressed;
    int prevValue = 0;
    int value = BLE::isRunning();
    do {
        value           = readRotary(value, 0, 1);
        btnPressed      = readButton();
        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);
            if (prevValue != value) {
                prevValue = value;
                beep();
            }
            u8g2.setFont(u8g2_font_nokiafc22_tu);

            u8g2.setCursor(14, 31);
            u8g2.print("NAME:");
            if (BLE::isRunning()) {
                u8g2.print(BLE::getDevName());
            } else {
                u8g2.print("N/A:");
            }

            u8g2.setCursor(14, 44);
            u8g2.print(F("[   ]  OFF"));
            u8g2.setCursor(14, 57);
            u8g2.print(F("[   ]  ON"));
            u8g2.setCursor(19, 44);
            u8g2.print(value == 0 ? F("*") : F(" "));
            u8g2.setCursor(19, 57);
            u8g2.print(value == 1 ? F("*") : F(" "));

        } while ( u8g2.nextPage() );

        if (btnPressed == ShortPress) {
            beep();
            if (value) {
                BLE::enableBLE();
            } else {
                BLE::disableBLE();
            }
            value = BLE::isRunning();
        }
    } while ( btnPressed != LongPress );
    SAVE_CONFIGURE(ble, value);
}

void demoWiFi(uint8_t menuSelect)
{
    Button btnPressed;
    int prevValue = 0;
    int value = WiFi.getMode() != WIFI_MODE_NULL;
    do {
        value           = readRotary(value, 0, 1);
        btnPressed      = readButton();
        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);
            if (prevValue != value) {
                prevValue = value;
                beep();
            }
            u8g2.setFont(u8g2_font_nokiafc22_tu);

            u8g2.setCursor(14, 31);
            u8g2.print("HOSTNAME:");
            if (WiFi.getMode() == WIFI_MODE_AP) {
                u8g2.print(WiFi.softAPgetHostname());
            } else {
                u8g2.print("N/A:");
            }

            u8g2.setCursor(14, 44);
            u8g2.print(F("[   ]  OFF"));
            u8g2.setCursor(14, 57);
            u8g2.print(F("[   ]  ON"));
            u8g2.setCursor(19, 44);
            u8g2.print(value == 0 ? F("*") : F(" "));
            u8g2.setCursor(19, 57);
            u8g2.print(value == 1 ? F("*") : F(" "));

        } while ( u8g2.nextPage() );

        if (btnPressed == ShortPress) {
            beep();

            if (value) {
                if (WiFi.getMode() != WIFI_MODE_AP) {
                    if (!WiFi.softAP(ssid, password)) {
                        log_e("Soft AP creation failed.");
                        return;
                    }
                    WiFi.softAPsetHostname("T-TWR");
                    IPAddress myIP = WiFi.softAPIP();
                    DBG("AP IP address: ");
                    DBG(myIP);
                    DBG("Server started");
                    setupWeb();
                }
            } else {
                if (WiFi.getMode() == WIFI_MODE_AP) {
                    endWeb();
                    WiFi.enableAP(false);
                }
            }
            value = WiFi.getMode() == WIFI_MODE_AP;
        }

    } while ( btnPressed != LongPress );

    SAVE_CONFIGURE(wifi, value);
}

void demoSetting(uint8_t menuSelect)
{
    Button btnPressed;
    int value = 0;
    int prevValue = 0;
    bool  beep = twr.getSetting().beep;

    if (twr.getVersion() == TWRClass::TWR_REV2V0) {
        drawError(menuSelect); return;
    }

    do {
        value       = readRotary(0, 0, 2);
        btnPressed  = readButton();

        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);

            if (prevValue != value) {
                prevValue = value;
            }
            u8g2.setFont(u8g2_font_nokiafc22_tu);

            u8g2.setCursor(14, 31);
            if (beep) {
                u8g2.print(F("   *    ")) ;
            } else {
                u8g2.print(F("   -    ")) ;
            }
            u8g2.setCursor(45, 31);
            u8g2.print(F("BEEP")) ;
            switch (value) {
            case 0:
                u8g2.setCursor(14, 31);
                if (beep) {
                    u8g2.print(F("[  *  ]")) ;
                } else {
                    u8g2.print(F("[  -  ]")) ;
                }
                break;
            default:
                break;
            }

            if (btnPressed == ShortPress) {
                switch (value) {
                case 0:
                    twr.enableBeep(!beep);
                    beep = twr.getSetting().beep;
                    if (twr.isEnableBeep()) {
                        twr.routingSpeakerChannel(TWRClass::TWR_ESP_TO_SPK);
                    } else {
                        twr.routingSpeakerChannel(TWRClass::TWR_RADIO_TO_SPK);
                    }
                    break;
                default:
                    break;
                }
            }

        } while ( u8g2.nextPage() );

    } while ( btnPressed != LongPress );
    SAVE_CONFIGURE(beep, beep);

}

void demoDevInfo(uint8_t menuSelect)
{
    Button btnPressed;
    RadioType  type = radio.getStetting().type;
    String  radioType = type == SA8X8_UHF ? "UHF" : "VHF";
    String fwType = radio.firmwareType();
    int value = type;

    TWRClass::TWR_Version _ver = twr.getVersion() ;
    do {
        btnPressed  = readButton();
        value       = readRotary(type, 0, 2);

        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);
            u8g2.setFont(u8g2_font_nokiafc22_tu);

            u8g2.setCursor(14, 31);
            u8g2.print(F("REV:"));
            String version = _ver == TWRClass::TWR_REV2V0 ? "2.0" : "2.1";
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT((version).c_str()) - 22, 31 );
            u8g2.print(version);

            u8g2.setCursor(14, 44);
            u8g2.print(F("FW:"));
            fwType.toUpperCase();
            u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT(fwType.c_str()) - 15, 44 );
            u8g2.print(fwType);


            u8g2.setCursor(14, 57);
            u8g2.print(F("BAND:"));

            if (_ver == TWRClass::TWR_REV2V1) {
                u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT((radioType).c_str()) - 22, 57);
                u8g2.print(radioType);
            } else {
                switch (value) {
                case SA8X8_VHF:
                    radioType = "VHF";
                    break;
                case SA8X8_UHF:
                    radioType = "UHF";
                    break;
                default:
                    break;
                }
                u8g2.drawRBox(83, 48, 30, 12, 2);
                u8g2.setColorIndex(0);
                u8g2.setCursor( U8G2_HOR_ALIGN_RIGHT((radioType).c_str()) - 22, 57);
                u8g2.print(radioType);
                u8g2.setColorIndex(1);
            }
        } while ( u8g2.nextPage() );

        if (btnPressed == ShortPress ) {
            beep();
            if (radioType == "UHF") {
                radio.setRadioType(SA8X8_UHF);
            } else if (radioType == "VHF") {
                radio.setRadioType(SA8X8_VHF);
            }
        }

    } while ( btnPressed != LongPress );
}


Button readButton()
{
    Button btnPressed   = Unknown;
    // Handle key events
    for (uint8_t i = 0; i < COUNT(buttonPins); i++) {
        buttons[i].check();
    }

    // Handling PMU events
    twr.tick();

    if (state != btnPressed) {
        btnPressed = state;
        state = Unknown;
    }
    return btnPressed;
}


uint32_t readRotary(uint32_t cur, uint32_t minOut, uint32_t maxOut, uint32_t steps)
{
    static uint32_t lastMin, lastMax, lastSteps;
    static uint32_t readPotValue;

    if (lastMin != minOut || lastMax != maxOut || lastSteps != steps) {
        setRotaryValue(cur, minOut, maxOut, steps);
        lastMin = minOut;
        lastMax = maxOut;
        lastSteps = steps;
        return cur;
    }

    xQueueReceive(rotaryMsg, &readPotValue, pdMS_TO_TICKS(50));
    return readPotValue;
}

void drawFrame()
{
    u8g2.drawRFrame(0, 0, 128, 64, 5);
}

void drawHeader( uint8_t menuSelect )
{
    u8g2.drawHLine(2, 1, 124);
    u8g2.drawHLine(1, 2, 126);
    u8g2.drawHLine(0, 3, 128);
    u8g2.drawBox(0, 4, 128, 14);

    u8g2.setColorIndex(0);
    u8g2.drawXBMP(4, 1,  16, 16, demo[menuSelect].demo_icon);
    u8g2.setFont(u8g2_font_nokiafc22_tu);
    u8g2.drawStr(22, 12, demo[menuSelect].demo_item);
    u8g2.setColorIndex(1);
}


void beep()
{
    if (twr.isEnableBeep()) {
        tone(ESP32_PWM_TONE, 800, 5);
    }
}


void drawError(uint8_t menuSelect)
{
    Button     btnPressed;
    do {
        btnPressed = readButton();
        u8g2.firstPage();
        do {
            drawFrame();
            drawHeader(menuSelect);
            u8g2.setFont(u8g2_font_nokiafc22_tu);
            u8g2.setCursor(14, 44);
            u8g2.print(F("REV2.0 NOT SUPPORT"));
        } while ( u8g2.nextPage() );
    } while ( btnPressed != LongPress );
}

