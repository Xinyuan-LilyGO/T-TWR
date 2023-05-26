/**
 * @file      Walkie_Talkie_Example.ino
 * @author    lbuque , Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-11
 *
 */


/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include "config.h"
#include "ui.h"
#include "data.h"
#include "sa868.h"

#include <U8g2lib.h>
#include <ArduinoJson.h>
#include <OneButton.h>
// #include <Encoder.h>
#include <RotaryEncoder.h>

#include <nvs.h>
#include <nvs_flash.h>
#include <esp_adc_cal.h>

#include <Wire.h>
#include <Arduino.h>

#define XPOWERS_CHIP_AXP2101
#include <XPowersLib.h>
#include <TinyGPS++.h>
#include <SD.h>
#include "WiFi.h"
#include "utilities.h"

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

/******************************************************************************/
/***        type definitions                                                ***/
/******************************************************************************/

typedef void (*EncoderCallback)(void *);

/******************************************************************************/
/***        local function prototypes                                       ***/
/******************************************************************************/

/******************************************************************************/
/***        exported variables                                              ***/
/******************************************************************************/

/******************************************************************************/
/***        local variables                                                 ***/
/******************************************************************************/

OneButton buttonDown(BUTTON_DOWN_PIN, true);
OneButton buttonOK(ENCODER_OK_PIN, true);
OneButton buttonPTT(BUTTON_PTT_PIN, true);
RotaryEncoder encoder(ENCODER_B_PIN, ENCODER_A_PIN, RotaryEncoder::LatchMode::FOUR3);

SA868 sa868(Serial1, SA868_PTT_PIN, SA868_PD_PIN, SA868_RF_PIN);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

EncoderCallback encoderFn;
TaskHandle_t dataTaskHandler;
TaskHandle_t clearvolumeSliderTaskHandler;
TaskHandle_t gnssTaskHandler;
TaskHandle_t encoderTaskHandler;
TaskHandle_t updateTaskHandler;
TaskHandle_t pielxTaskHandler;

XPowersPMU PMU;
TinyGPSPlus gps;

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

int     vref = 1100;
bool    pmu_flag = false;
bool    isOutputNmea = true;
bool    vbusIn = false;
uint8_t curPage = 0;
int8_t curCursor = 0;
int8_t lastCursor = 0;
bool    downloadMode = false;
void loopPMU();
/******************************************************************************/
/***        exported functions                                              ***/
/******************************************************************************/
void setFlag(void)
{
    pmu_flag = true;
}

void scanDevices(TwoWire *w)
{
    uint8_t err, addr;
    int nDevices = 0;
    uint32_t start = 0;
    for (addr = 1; addr < 127; addr++) {
        start = millis();
        w->beginTransmission(addr); delay(2);
        err = w->endTransmission();
        if (err == 0) {
            nDevices++;
            Serial.print("I2C device found at address 0x");
            if (addr < 16) {
                Serial.print("0");
            }
            Serial.print(addr, HEX);
            Serial.println(" !");

        } else if (err == 4) {
            Serial.print("Unknow error at address 0x");
            if (addr < 16) {
                Serial.print("0");
            }
            Serial.println(addr, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
}

void setupPower()
{
    bool result = PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, I2C_SDA, I2C_SCL);
    if (result == false) {
        while (1) {
            Serial.println("PMU is not online...");
            delay(500);
        }
    }

    // Set the minimum common working voltage of the PMU VBUS input,
    // below this value will turn off the PMU
    PMU.setVbusVoltageLimit(XPOWERS_AXP2101_VBUS_VOL_LIM_3V88);

    // Set the maximum current of the PMU VBUS input,
    // higher than this value will turn off the PMU
    PMU.setVbusCurrentLimit(XPOWERS_AXP2101_VBUS_CUR_LIM_2000MA);


    // Get the VSYS shutdown voltage
    uint16_t vol = PMU.getSysPowerDownVoltage();
    Serial.printf("->  getSysPowerDownVoltage:%u\n", vol);

    // Set VSY off voltage as 2600mV , Adjustment range 2600mV ~ 3300mV
    PMU.setSysPowerDownVoltage(2600);


    //! DC1 ESP32S3 Core VDD , Don't change
    // PMU.setDC1Voltage(3300);

    //! DC3 Radio & Pixels VDD , Don't change
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
    //! DC1 ESP32S3 Core VDD , Don't change
    // PMU.enableDC3();

    //! External pin power supply
    PMU.enableDC5();
    PMU.enableALDO1();
    PMU.enableALDO3();
    PMU.enableBLDO2();

    //! ALDO2 MICRO TF Card VDD
    PMU.enableALDO2();

    //! ALDO4 GNSS VDD
    PMU.enableALDO4();

    //! BLDO1 MIC VDD
    PMU.enableBLDO1();

    //! DC3 Radio & Pixels VDD
    PMU.enableDC3();

    // power off when not in use
    PMU.disableDC2();
    PMU.disableDC4();
    PMU.disableCPUSLDO();
    PMU.disableDLDO1();
    PMU.disableDLDO2();



    Serial.println("DCDC=======================================================================");
    Serial.printf("DC1  : %s   Voltage:%u mV \n",  PMU.isEnableDC1()  ? "+" : "-", PMU.getDC1Voltage());
    Serial.printf("DC2  : %s   Voltage:%u mV \n",  PMU.isEnableDC2()  ? "+" : "-", PMU.getDC2Voltage());
    Serial.printf("DC3  : %s   Voltage:%u mV \n",  PMU.isEnableDC3()  ? "+" : "-", PMU.getDC3Voltage());
    Serial.printf("DC4  : %s   Voltage:%u mV \n",  PMU.isEnableDC4()  ? "+" : "-", PMU.getDC4Voltage());
    Serial.printf("DC5  : %s   Voltage:%u mV \n",  PMU.isEnableDC5()  ? "+" : "-", PMU.getDC5Voltage());
    Serial.println("ALDO=======================================================================");
    Serial.printf("ALDO1: %s   Voltage:%u mV\n",  PMU.isEnableALDO1()  ? "+" : "-", PMU.getALDO1Voltage());
    Serial.printf("ALDO2: %s   Voltage:%u mV\n",  PMU.isEnableALDO2()  ? "+" : "-", PMU.getALDO2Voltage());
    Serial.printf("ALDO3: %s   Voltage:%u mV\n",  PMU.isEnableALDO3()  ? "+" : "-", PMU.getALDO3Voltage());
    Serial.printf("ALDO4: %s   Voltage:%u mV\n",  PMU.isEnableALDO4()  ? "+" : "-", PMU.getALDO4Voltage());
    Serial.println("BLDO=======================================================================");
    Serial.printf("BLDO1: %s   Voltage:%u mV\n",  PMU.isEnableBLDO1()  ? "+" : "-", PMU.getBLDO1Voltage());
    Serial.printf("BLDO2: %s   Voltage:%u mV\n",  PMU.isEnableBLDO2()  ? "+" : "-", PMU.getBLDO2Voltage());
    Serial.println("===========================================================================");

    // Set the time of pressing the button to turn off
    PMU.setPowerKeyPressOffTime(XPOWERS_POWEROFF_4S);
    uint8_t opt = PMU.getPowerKeyPressOffTime();
    Serial.print("PowerKeyPressOffTime:");
    switch (opt) {
    case XPOWERS_POWEROFF_4S: Serial.println("4 Second");
        break;
    case XPOWERS_POWEROFF_6S: Serial.println("6 Second");
        break;
    case XPOWERS_POWEROFF_8S: Serial.println("8 Second");
        break;
    case XPOWERS_POWEROFF_10S: Serial.println("10 Second");
        break;
    default:
        break;
    }
    // Set the button power-on press time
    PMU.setPowerKeyPressOnTime(XPOWERS_POWERON_128MS);
    opt = PMU.getPowerKeyPressOnTime();
    Serial.print("PowerKeyPressOnTime:");
    switch (opt) {
    case XPOWERS_POWERON_128MS: Serial.println("128 Ms");
        break;
    case XPOWERS_POWERON_512MS: Serial.println("512 Ms");
        break;
    case XPOWERS_POWERON_1S: Serial.println("1 Second");
        break;
    case XPOWERS_POWERON_2S: Serial.println("2 Second");
        break;
    default:
        break;
    }

    Serial.println("===========================================================================");
    // It is necessary to disable the detection function of the TS pin on the board
    // without the battery temperature detection function, otherwise it will cause abnormal charging
    PMU.disableTSPinMeasure();


    // Enable internal ADC detection
    PMU.enableBattDetection();
    PMU.enableVbusVoltageMeasure();
    PMU.enableBattVoltageMeasure();
    PMU.enableSystemVoltageMeasure();


    /*
      The default setting is CHGLED is automatically controlled by the PMU.
    - XPOWERS_CHG_LED_OFF,
    - XPOWERS_CHG_LED_BLINK_1HZ,
    - XPOWERS_CHG_LED_BLINK_4HZ,
    - XPOWERS_CHG_LED_ON,
    - XPOWERS_CHG_LED_CTRL_CHG,
    * */
    PMU.setChargingLedMode(XPOWERS_CHG_LED_BLINK_1HZ);

    // Force add pull-up
    pinMode(PMU_IRQ, INPUT_PULLUP);
    attachInterrupt(PMU_IRQ, setFlag, FALLING);

    // Disable all interrupts
    PMU.disableIRQ(XPOWERS_AXP2101_ALL_IRQ);
    // Clear all interrupt flags
    PMU.clearIrqStatus();
    // Enable the required interrupt function
    PMU.enableIRQ(
        XPOWERS_AXP2101_BAT_INSERT_IRQ    | XPOWERS_AXP2101_BAT_REMOVE_IRQ      |   //BATTERY
        XPOWERS_AXP2101_VBUS_INSERT_IRQ   | XPOWERS_AXP2101_VBUS_REMOVE_IRQ     |   //VBUS
        XPOWERS_AXP2101_PKEY_SHORT_IRQ    | XPOWERS_AXP2101_PKEY_LONG_IRQ       |   //POWER KEY
        XPOWERS_AXP2101_BAT_CHG_DONE_IRQ  | XPOWERS_AXP2101_BAT_CHG_START_IRQ       //CHARGE
    );

    // Set the precharge charging current
    PMU.setPrechargeCurr(XPOWERS_AXP2101_PRECHARGE_150MA);

    // Set constant current charge current limit
    //! Using inferior USB cables and adapters will not reach the maximum charging current.
    //! Please pay attention to add a suitable heat sink above the PMU when setting the charging current to 1A
    PMU.setChargerConstantCurr(XPOWERS_AXP2101_CHG_CUR_1000MA);

    // Set stop charging termination current
    PMU.setChargerTerminationCurr(XPOWERS_AXP2101_CHG_ITERM_150MA);

    // Set charge cut-off voltage
    PMU.setChargeTargetVoltage(XPOWERS_AXP2101_CHG_VOL_4V2);

    // Disable the PMU long press shutdown function
    PMU.disableLongPressShutdown();


    // Get charging target current
    const uint16_t currTable[] = {
        0, 0, 0, 0, 100, 125, 150, 175, 200, 300, 400, 500, 600, 700, 800, 900, 1000
    };
    uint8_t val = PMU.getChargerConstantCurr();
    Serial.print("Val = "); Serial.println(val);
    Serial.print("Setting Charge Target Current : ");
    Serial.println(currTable[val]);

    // Get charging target voltage
    const uint16_t tableVoltage[] = {
        0, 4000, 4100, 4200, 4350, 4400, 255
    };
    val = PMU.getChargeTargetVoltage();
    Serial.print("Setting Charge Target Voltage : ");
    Serial.println(tableVoltage[val]);

}

void setupSDCard()
{
    //SPI Bus
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    if (SD.begin(SD_CS, SPI)) {
        uint8_t cardType = SD.cardType();
        if (cardType == CARD_NONE) {
            Serial.println("No SD_MMC card attached");
            return ;
        } else {
            Serial.print("SD_MMC Card Type: ");
            if (cardType == CARD_MMC) {
                Serial.println("MMC");
            } else if (cardType == CARD_SD) {
                Serial.println("SDSC");
            } else if (cardType == CARD_SDHC) {
                Serial.println("SDHC");
            } else {
                Serial.println("UNKNOWN");
            }
            uint32_t cardSize = SD.cardSize() / (1024 * 1024);
            uint32_t cardTotal = SD.totalBytes() / (1024 * 1024);
            uint32_t cardUsed = SD.usedBytes() / (1024 * 1024);
            Serial.printf("SD Card Size: %lu MB\n", cardSize);
            Serial.printf("Total space: %lu MB\n",  cardTotal);
            Serial.printf("Used space: %lu MB\n",   cardUsed);
        }
    }
}

void setup()
{
    Serial.begin(115200);

    print_wakeup_reason();

    // Set the audio input channel to Mic -> Radio
    pinMode(MIC_CTRL_PIN, OUTPUT);
    digitalWrite(MIC_CTRL_PIN, LOW);

    setupPower();

    setupSDCard();

    strip.setBrightness(50);
    strip.begin();

    data_init(sa868);

    adc_init();

    // GNSS Serial
    Serial2.begin(9600, SERIAL_8N1, GNSS_RX_PIN, GNSS_TX_PIN);
    // Radio Serial
    Serial1.begin(9600, SERIAL_8N1, SA868_RX_PIN, SA868_TX_PIN);

    sa868.begin();

    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFlipMode(0);
    u8g2.setFontMode(1); // Transparent
    u8g2.setDrawColor(1);
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb19_mr);
    u8g2.drawStr(0, 30, "LilyGo");
    u8g2.drawHLine(2, 35, 47);
    u8g2.drawHLine(3, 36, 47);
    u8g2.drawVLine(45, 32, 12);
    u8g2.drawVLine(46, 33, 12);
    u8g2.setFont(u8g2_font_inb19_mf);
    u8g2.drawStr(63, 60, "TWR");
    u8g2.sendBuffer();
    u8g2.setContrast(255);
    delay(3000);


    ui();

    buttonDown.attachClick(volumeDown);
    buttonOK.attachClick(buttonOKClickCallbackMainPage);
    buttonOK.setPressTicks(2000);

    //! Press and hold the middle button of the rotary encoder for 2 seconds to enter download mode,
    //! turn off all peripheral power, and wait for the program to download
    //! The blue indicator light will flash quickly
    buttonOK.attachDuringLongPress(buttonOKLongPressCallbackDownloadMode);

    buttonPTT.attachLongPressStart(transmit);
    buttonPTT.attachLongPressStop(receive);

    if (PMU.isVbusIn()) {
        vbusIn = true;
    }

    xTaskCreatePinnedToCore(encoderTask, "encoderTask", 4096, NULL, 8, &encoderTaskHandler, 1);
    xTaskCreatePinnedToCore(updateTask, "updateTask", 4096, NULL, 6,  &updateTaskHandler, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(dataTask, "dataTask", 4096, NULL, 5, &dataTaskHandler, 1);
    xTaskCreatePinnedToCore(clearvolumeSliderTask, "clearvolumeSliderTask", 4096, NULL, 2, &clearvolumeSliderTaskHandler, 1);
    xTaskCreatePinnedToCore(gnssTask, "gnssTask", 4096, NULL, 2, &gnssTaskHandler, 1);
    xTaskCreatePinnedToCore(pielxTask, "pielxTask", 4096, NULL, 2, &pielxTaskHandler, 1);


}


/******************************************************************************/
/***        local functions                                                 ***/
/******************************************************************************/
void adc_init()
{
#if BATTERY_ADC_PIN != -1
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        Serial.printf("eFuse Vref: %u mV", adc_chars.vref);
        vref = adc_chars.vref;
    }
#endif
}


uint8_t getBatteryPercentage()
{
#if BATTERY_ADC_PIN != -1
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
    uint32_t v = esp_adc_cal_raw_to_voltage(analogRead(BATTERY_ADC_PIN), &adc_chars);
    float battery_voltage = ((float)v / 1000) * 2;
    Serial.print("ADC RAW: ");
    Serial.println(v);
    Serial.print("Battery voltage: ");
    Serial.println(battery_voltage);
    if (battery_voltage > 4.2) {
        return 100;
    } else {
        return (uint8_t)(((battery_voltage - 3.0) / (4.2 - 3.0)) * 100);
    }
#else
    return PMU.isBatteryConnect() ? PMU.getBatteryPercent() : 0;
#endif
}


void updateTask(void *pvParameters)
{
    while (1) {
        if (curPage == 0) {
            feedElectricity(getBatteryPercentage());
            feedRSSI(sa868.getRSSI());
            feedGNSS(gps.location.isValid());
            u8g2.sendBuffer();
        }
        delay(5000);
    }
    vTaskDelete(NULL);
}


void dataTask(void *pvParameter)
{
    vTaskSuspend(dataTaskHandler);
    while (1) {
        data_save(sa868);
        vTaskSuspend(dataTaskHandler);
        delay(1000);
    }
    vTaskDelete(NULL);
}


void clearvolumeSliderTask(void *pvParameter)
{
    vTaskSuspend(clearvolumeSliderTaskHandler);
    while (1) {
        delay(1500);
        feedVolumeSlider();
        u8g2.sendBuffer();
        vTaskSuspend(clearvolumeSliderTaskHandler);
    }
    vTaskDelete(NULL);
}

void gnssTask(void *ptr)
{
    while (1) {
        while (Serial2.available()) {
            int c = Serial2.read();
            if (isOutputNmea) {
                Serial.write(c);
            }
            gps.encode(c);
        }
        delay(8);
    }
}

void pielxTask(void *ptr)
{
    while (1) {
        strip.setPixelColor(0, strip.Color(255, 0, 0));
        strip.show(); delay(500);
        strip.setPixelColor(0, strip.Color(0, 255, 0));
        strip.show(); delay(500);
        strip.setPixelColor(0, strip.Color(0, 0, 255));
        strip.show(); delay(500);
    }
}

void loop()
{
    buttonOK.tick();
    if (!downloadMode) {
        loopPMU();
        buttonDown.tick();
        buttonPTT.tick();
    }

    delay(10);
}

void loopPMU()
{
    if (!pmu_flag) {
        return;
    }

    pmu_flag = false;
    // Get PMU Interrupt Status Register
    uint32_t status = PMU.getIrqStatus();
    Serial.print("STATUS => HEX:");
    Serial.print(status, HEX);
    Serial.print(" BIN:");
    Serial.println(status, BIN);

    if (PMU.isPekeyShortPressIrq()) {
        volumeUp();
    }

    if (PMU.isBatChagerStartIrq() || PMU.isBatteryConnect()) {
        PMU.setChargingLedMode(XPOWERS_CHG_LED_CTRL_CHG);
    }

    if (PMU.isBatRemoveIrq()) {
        PMU.setChargingLedMode(XPOWERS_CHG_LED_BLINK_1HZ);
    }

    if (PMU.isVbusInsertIrq()) {
        vbusIn = true;
    }

    if (PMU.isVbusRemoveIrq()) {
        vbusIn = false;
    }

    if (PMU.isPekeyLongPressIrq()) {
        vTaskDelete(dataTaskHandler);
        vTaskDelete(clearvolumeSliderTaskHandler);
        vTaskDelete(gnssTaskHandler);
        vTaskDelete(encoderTaskHandler);
        vTaskDelete(updateTaskHandler);
        vTaskDelete(pielxTaskHandler);

        u8g2.setFont(u8g2_font_timR14_tf);
        u8g2.clearBuffer();
        int8_t height = 30 + u8g2.getAscent();
        u8g2.drawStr(20, height, "Power OFF");
        u8g2.sendBuffer();
        delay(3000);
        PMU.shutdown();
    }

    // Clear PMU Interrupt Status Register
    PMU.clearIrqStatus();
}

void transmit()
{
    if (curPage == 0) {
        sa868.transmit();
        feedTransmitStatus();
        u8g2.sendBuffer();
    }
}


void receive()
{
    if (curPage == 0) {
        sa868.receive();
        feedRecvFreq(sa868.recvFreq);
        u8g2.sendBuffer();
    }
}


void encoderTask(void *pvParameters)
{
    int pos = 0;

    while (1) {
        encoder.tick();
        int newPos = encoder.getPosition();
        if (pos != newPos) {
            Serial.printf("I [Main]: pos = %d\r\n", newPos);
            if (encoderFn) {
                if (newPos > pos) {
                    encoderFn((void *)1);
                } else {
                    encoderFn((void *) -1);
                }
            }
            pos = newPos;
        }
        delay(10);
    }

    vTaskDelete(NULL);
}


void ui()
{
    feedSettingsMenu(false);
    feedFilterMenu(false);
    feedRecvFreq(sa868.recvFreq);
    feedRSSI(sa868.getRSSI());
    feedElectricity(getBatteryPercentage());
    feedGNSS(false);
    // feedTxCXCSS(cxcss[sa868.txCXCSS]);//0000
    // feedRxCXCSS(cxcss[sa868.rxCXCSS]);//0000
    feedSD(SD.cardType() != CARD_NONE, (double)(SD.totalBytes() / (1024.0 * 1024.0 * 1024.0)));
    u8g2.setFont(u8g2_font_tenstamps_mf);
    if (sa868.emphasis) {
        u8g2.drawStr(32, 60, "E");
    }
    if (sa868.highPass) {
        u8g2.drawStr(48, 60, "H");
    }
    if (sa868.lowPass) {
        u8g2.drawStr(64, 60, "L");
    }
    char sq_str[2] = { 0 };
    sprintf(sq_str, "%d", sa868.sq);
    u8g2.drawStr(80, 60, sq_str);
    u8g2.sendBuffer();
    encoderFn = encoderCallbackMainPage;
}


void volumeUp()
{
    vTaskSuspend(clearvolumeSliderTaskHandler);
    sa868.setVolume(sa868.volume + 1);
    vTaskResume(dataTaskHandler);
    feedVolume(sa868.volume);
    u8g2.sendBuffer();
    vTaskResume(clearvolumeSliderTaskHandler);
}


void volumeDown()
{
    vTaskSuspend(clearvolumeSliderTaskHandler);
    sa868.setVolume(sa868.volume - 1);
    vTaskResume(dataTaskHandler);
    feedVolume(sa868.volume);
    u8g2.sendBuffer();
    vTaskResume(clearvolumeSliderTaskHandler);
}


// settings page
void buttonOKClickCallbackSettingsPage()
{
    Serial.printf("%s  : %d\n", __func__, curCursor);
    lastCursor = curCursor;
    if (curCursor == 0) {
        // band width setting
        curPage = 3;
        feedSettingsPagebarBandWidthPage();
        feedSettingsPagebarBandWidthPageBar1(!sa868.bandwidth);
        feedSettingsPagebarBandWidthPageBar2(sa868.bandwidth);
        u8g2.sendBuffer();
        encoderFn = encoderCallbackBandWidthPage;
        if (sa868.bandwidth) {
            curCursor = 1;
        } else {
            curCursor = 0;
        }
        buttonOK.attachClick(buttonOKClickCallbackBandwidthPage);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    } else if (curCursor == 1) {
        // trans freq setting
        curPage = 4;
        feedSettingsPagebarTransFreqPage();
        feedSettingsPagebarTransFreqPageCXCSSList1(sa868.transFreq, sa868.bandwidth, MIN_FREQ, MAX_FREQ);
        if (sa868.bandwidth) {
            feedSettingsPagebarTransFreqPageCXCSSList2(sa868.transFreq, 25000);
        } else {
            feedSettingsPagebarTransFreqPageCXCSSList2(sa868.transFreq, 12500);
        }
        u8g2.sendBuffer();
        encoderFn = encoderCallbackTransFreqPage1;
        curCursor = (sa868.transFreq - MIN_FREQ) / (1 * 1000 * 1000);
        buttonOK.attachClick(buttonOKClickCallbackTransFreqPage1);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    } else if (curCursor == 2) {
        // recv freq setting
        curPage = 5;
        feedSettingsPagebarRecvFreqPage();
        feedSettingsPagebarTransFreqPageCXCSSList1(sa868.recvFreq, sa868.bandwidth, MIN_FREQ, MAX_FREQ);
        if (sa868.bandwidth) {
            feedSettingsPagebarTransFreqPageCXCSSList2(sa868.recvFreq, 25000);
        } else {
            feedSettingsPagebarTransFreqPageCXCSSList2(sa868.recvFreq, 12500);
        }
        u8g2.sendBuffer();
        encoderFn = encoderCallbackTransFreqPage1;
        curCursor = (sa868.recvFreq - MIN_FREQ) / (1 * 1000 * 1000);
        buttonOK.attachClick(buttonOKClickCallbackRecvFreqPage1);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    } else if (curCursor == 3) {
        // tx CSCSS setting
        curPage = 6;
        encoderFn = encoderCallbackTXCXCSSPage;
        curCursor = sa868.txCXCSS;
        feedSettingsPagebarTxCXCSSPage();
        feedSettingsPagebarCXCSSPageCXCSSList(sa868.txCXCSS, cxcss, 122);
        u8g2.sendBuffer();
        buttonOK.attachClick(buttonOKClickCallbackTXCXCSSPage);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    } else if (curCursor == 4) {
        // sq setting
        curPage = 7;
        encoderFn = encoderCallbackSQPage;
        curCursor = sa868.sq;
        feedSettingsPagebarSQPage();
        feedSettingsPagebarSQPageSlider(sa868.sq);
        u8g2.sendBuffer();
        buttonOK.attachClick(buttonOKClickCallbackSQPage);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    } else if (curCursor == 5) {
        // rx CSCSS setting
        curPage = 8;
        encoderFn = encoderCallbackRXCXCSSPage;
        curCursor = sa868.rxCXCSS;
        feedSettingsPagebarRxCXCSSPage();
        feedSettingsPagebarCXCSSPageCXCSSList(sa868.rxCXCSS, cxcss, 122);
        u8g2.sendBuffer();
        buttonOK.attachClick(buttonOKClickCallbackRXCXCSSPage);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    } else if (curCursor == 6) {
        // power setting
        curPage = 9;
        feedSettingsPagebarPowerPage();
        feedSettingsPagebarPowerPageBar1(!sa868.power);
        feedSettingsPagebarPowerPageBar2(sa868.power);
        u8g2.sendBuffer();
        encoderFn = encoderCallbackPowerPage;
        Serial.printf("power is %d\n", sa868.power);
        if (sa868.power) {
            curCursor = 1;
        } else {
            curCursor = 0;
        }
        buttonOK.attachClick(buttonOKClickCallbackPowerPage);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    }
}


void encoderCallbackSettingsPage(void *pvParameters)
{
    curCursor = curCursor + (int)pvParameters;

    if (curCursor > 6) {
        curCursor = 0;
    } else if (curCursor < 0) {
        curCursor = 6;
    }
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
}


void buttonOKDoubleClickCallbackSettingsPage()
{
    curPage = 1;
    curCursor = lastCursor;
    encoderFn = encoderCallbackSettingsPage;
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


// bandwidth page
void buttonOKClickCallbackBandwidthPage()
{
    sa868.bandwidth = curCursor == 0 ? false : true;
    curPage = 1;
    curCursor = lastCursor;
    encoderFn = encoderCallbackSettingsPage;
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void encoderCallbackBandWidthPage(void *pvParameters)
{
    int pos = curCursor;

    pos = pos + (int)pvParameters;
    if (pos > 1) {
        pos = 0;
    } else if (pos < 0) {
        pos = 1;
    }
    curCursor = pos;
    switch (curCursor) {
    case 0:
        feedSettingsPagebarBandWidthPageBar1(true);
        feedSettingsPagebarBandWidthPageBar2(false);
        break;

    case 1:
        feedSettingsPagebarBandWidthPageBar1(false);
        feedSettingsPagebarBandWidthPageBar2(true);
        break;

    default:
        break;
    }
    u8g2.sendBuffer();
}

// Power page
void buttonOKClickCallbackPowerPage()
{
    sa868.power = curCursor == 0 ? false : true;

    if (sa868.power) {
        sa868.highPower();
    } else {
        sa868.lowPower();
    }
    curPage = 1;
    curCursor = lastCursor;
    encoderFn = encoderCallbackSettingsPage;
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    vTaskResume(dataTaskHandler);
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void encoderCallbackPowerPage(void *pvParameters)
{
    int pos = curCursor;

    pos = pos + (int)pvParameters;
    if (pos > 1) {
        pos = 0;
    } else if (pos < 0) {
        pos = 1;
    }
    curCursor = pos;
    switch (curCursor) {
    case 0:
        feedSettingsPagebarPowerPageBar1(true);
        feedSettingsPagebarPowerPageBar2(false);
        break;

    case 1:
        feedSettingsPagebarPowerPageBar1(false);
        feedSettingsPagebarPowerPageBar2(true);
        break;

    default:
        break;
    }
    u8g2.sendBuffer();
}

long long tempFreq = 0.0;

// TransFreq Page
void buttonOKClickCallbackTransFreqPage1()
{
    tempFreq = MIN_FREQ + curCursor * 1000 * 1000;
    if (sa868.bandwidth) {
        curCursor = (sa868.transFreq % (1000 * 1000)) / 25000;
    } else {
        curCursor = (sa868.transFreq % (1000 * 1000)) / 12500;
    }
    encoderFn = encoderCallbackTransFreqPage2;
    buttonOK.attachClick(buttonOKClickCallbackTransFreqPage2);
}


void encoderCallbackTransFreqPage1(void *pvParameters)
{
    int pos = curCursor;

    pos = pos + (int)pvParameters;
    if (pos > FREQ_RANGE - 1) {
        pos = 0;
    } else if (pos < 0) {
        pos = FREQ_RANGE - 1;
    }
    curCursor = pos;
    feedSettingsPagebarTransFreqPageCXCSSList1((MIN_FREQ + curCursor * (1 * 1000 * 1000)), sa868.bandwidth, MIN_FREQ, MAX_FREQ);
    u8g2.sendBuffer();
}


void buttonOKClickCallbackTransFreqPage2()
{
    if (sa868.bandwidth) {
        tempFreq = tempFreq + curCursor * 25000 ;
    } else {
        tempFreq = tempFreq + curCursor * 12500 ;
    }
    if (sa868.checkFreq(sa868.bandwidth, sa868.recvFreq)) {
        sa868.setGroup(sa868.bandwidth, tempFreq, sa868.recvFreq, (teCXCSS)sa868.txCXCSS, sa868.sq, (teCXCSS)sa868.rxCXCSS);
    } else {
        sa868.setGroup(sa868.bandwidth, tempFreq, tempFreq, (teCXCSS)sa868.txCXCSS, sa868.sq, (teCXCSS)sa868.rxCXCSS);
    }
    vTaskResume(dataTaskHandler);
    Serial.printf("I [Main]: set tempFreq: %lld\r\n", tempFreq);
    Serial.printf("I [Main]: set transFreq: %lld\r\n", sa868.transFreq);
    curPage = 1;
    curCursor = lastCursor;
    encoderFn = encoderCallbackSettingsPage;
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void encoderCallbackTransFreqPage2(void *pvParameters)
{
    int pos = curCursor;
    int range = 0;
    pos = pos + (int)pvParameters;
    if (sa868.bandwidth) {
        range = (1 * 1000 * 1000 / 25000) - 1;
    } else {
        range = (1 * 1000 * 1000 / 12500) - 1;
    }
    if (pos > range) {
        pos = 0;
    } else if (pos < 0) {
        pos = range;
    }
    curCursor = pos;

    if (sa868.bandwidth) {
        feedSettingsPagebarTransFreqPageCXCSSList2(curCursor * 25000, 25000);
    } else {
        feedSettingsPagebarTransFreqPageCXCSSList2(curCursor * 12500, 12500);
    }
    u8g2.sendBuffer();
}


// recv page
void buttonOKClickCallbackRecvFreqPage1()
{
    tempFreq = MIN_FREQ + curCursor * 1000 * 1000;

    if (sa868.bandwidth) {
        curCursor = (sa868.transFreq % (1000 * 1000)) / 25000;
    } else {
        curCursor = (sa868.transFreq % (1000 * 1000)) / 12500;
    }
    encoderFn = encoderCallbackTransFreqPage2;
    buttonOK.attachClick(buttonOKClickCallbackRecvFreqPage2);
}


void buttonOKClickCallbackRecvFreqPage2()
{
    if (sa868.bandwidth) {
        tempFreq = tempFreq + curCursor * 25000 ;
    } else {
        tempFreq = tempFreq + curCursor * 12500 ;
    }
    if (sa868.checkFreq(sa868.bandwidth, sa868.transFreq)) {
        sa868.setGroup(sa868.bandwidth,
                       sa868.transFreq,
                       tempFreq,
                       (teCXCSS)sa868.txCXCSS,
                       sa868.sq,
                       (teCXCSS)sa868.rxCXCSS);
    } else {
        sa868.setGroup(sa868.bandwidth,
                       tempFreq,
                       tempFreq,
                       (teCXCSS)sa868.txCXCSS,
                       sa868.sq,
                       (teCXCSS)sa868.rxCXCSS);
    }
    vTaskResume(dataTaskHandler);
    Serial.printf("I [Main]: set tempFreq: %lld\r\n", tempFreq);
    Serial.printf("I [Main]: set recvFreq: %lld\r\n", sa868.recvFreq);
    curPage = 1;
    curCursor = lastCursor;
    encoderFn = encoderCallbackSettingsPage;
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


//TXCXCSS page
void buttonOKClickCallbackTXCXCSSPage()
{
    sa868.setGroup(sa868.bandwidth,
                   sa868.transFreq,
                   sa868.recvFreq,
                   (teCXCSS)curCursor,
                   sa868.sq,
                   (teCXCSS)sa868.rxCXCSS);
    vTaskResume(dataTaskHandler);
    curPage = 1;
    curCursor = lastCursor;
    encoderFn = encoderCallbackSettingsPage;
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void encoderCallbackTXCXCSSPage(void *pvParameters)
{
    int pos = curCursor;

    pos = pos + (int)pvParameters;
    if (pos > 121) {
        pos = 0;
    } else if (pos < 0) {
        pos = 121;
    }
    curCursor = pos;

    feedSettingsPagebarTxCXCSSPage();
    feedSettingsPagebarCXCSSPageCXCSSList(curCursor, cxcss, 122);
    u8g2.sendBuffer();
}


// SQ page
void buttonOKClickCallbackSQPage()
{
    sa868.setGroup(sa868.bandwidth,
                   sa868.transFreq,
                   sa868.recvFreq,
                   (teCXCSS)sa868.txCXCSS,
                   curCursor,
                   (teCXCSS)sa868.rxCXCSS);
    vTaskResume(dataTaskHandler);
    curPage = 1;
    curCursor = lastCursor;
    encoderFn = encoderCallbackSettingsPage;
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void encoderCallbackSQPage(void *pvParameters)
{
    int pos = curCursor;

    pos = pos + (int)pvParameters;
    if (pos > 8) {
        pos = 0;
    } else if (pos < 0) {
        pos = 8;
    }
    curCursor = pos;

    feedSettingsPagebarSQPage();
    feedSettingsPagebarSQPageSlider(pos);
    u8g2.sendBuffer();
}


//RXCXCSS page
void buttonOKClickCallbackRXCXCSSPage()
{
    sa868.setGroup(sa868.bandwidth, sa868.transFreq, sa868.recvFreq, (teCXCSS)sa868.txCXCSS, sa868.sq, (teCXCSS)curCursor);
    vTaskResume(dataTaskHandler);
    curPage = 1;
    curCursor = lastCursor;
    encoderFn = encoderCallbackSettingsPage;
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void encoderCallbackRXCXCSSPage(void *pvParameters)
{
    int pos = curCursor;

    pos = pos + (int)pvParameters;
    if (pos > 121) {
        pos = 0;
    } else if (pos < 0) {
        pos = 121;
    }
    curCursor = pos;

    feedSettingsPagebarRxCXCSSPage();
    feedSettingsPagebarCXCSSPageCXCSSList(curCursor, cxcss, 122);
    u8g2.sendBuffer();
}


// filter page
void buttonOKClickCallbackFilterPage()
{
    if (curCursor == 0) {
        feedFilterPageBar1(true, !sa868.emphasis);
        sa868.setFilter(!sa868.emphasis, sa868.highPass, sa868.lowPass);
    } else if (curCursor == 1) {
        feedFilterPageBar2(true, !sa868.highPass);
        sa868.setFilter(sa868.emphasis, !sa868.highPass, sa868.lowPass);
    } else if (curCursor == 2) {
        feedFilterPageBar3(true, !sa868.lowPass);
        sa868.setFilter(sa868.emphasis, sa868.highPass, !sa868.lowPass);
    }
    vTaskResume(dataTaskHandler);
    u8g2.sendBuffer();
}


void encoderCallbackFilterPage(void *pvParameters)
{
    // static int pos = 0;

    curCursor = curCursor + (int)pvParameters;
    if (curCursor > 2) {
        curCursor = 0;
    } else if (curCursor < 0) {
        curCursor = 2;
    }
    // curCursor = pos;
    switch (curCursor) {
    case 0:
        feedFilterPageBar1(true, sa868.emphasis);
        feedFilterPageBar2(false, sa868.highPass);
        feedFilterPageBar3(false, sa868.lowPass);
        break;

    case 1:
        feedFilterPageBar1(false, sa868.emphasis);
        feedFilterPageBar2(true, sa868.highPass);
        feedFilterPageBar3(false, sa868.lowPass);
        break;

    case 2:
        feedFilterPageBar1(false, sa868.emphasis);
        feedFilterPageBar2(false, sa868.highPass);
        feedFilterPageBar3(true, sa868.lowPass);
        break;

    default:
        break;
    }
    u8g2.sendBuffer();
}


// main page
void buttonOKClickCallbackMainPage()
{
    Serial.println("I [Main]: Ok button click");
    if (curPage == 0) {
        if (curCursor == 0) {
            curPage = 1;
            encoderFn = encoderCallbackSettingsPage;
            // feedSettingsPage();
            feedSettingsPage1(curCursor);
            u8g2.sendBuffer();
            buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
            buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
        } else if (curCursor == 1) {
            curPage = 2;
            feedFilterPage();
            feedFilterPageBar1(true, sa868.emphasis);
            feedFilterPageBar2(false, sa868.highPass);
            feedFilterPageBar3(false, sa868.lowPass);
            u8g2.sendBuffer();
            curCursor = 0;
            encoderFn = encoderCallbackFilterPage;
            buttonOK.attachClick(buttonOKClickCallbackFilterPage);
            buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
        }
    }
}


void buttonOKDoubleClickCallbackMainPage()
{
    Serial.println("I [Main]: Ok button double click");
    curPage = 0;
    ui();
    buttonOK.attachClick(buttonOKClickCallbackMainPage);
}


void encoderCallbackMainPage(void *pvParameters)
{
    static int pos = 0;

    pos = pos + (int)pvParameters;
    if (pos > 1) {
        pos = 0;
    } else if (pos < 0) {
        pos = 1;
    }

    switch (pos) {
    case 0:
        feedSettingsMenu(true);
        feedFilterMenu(false);
        curCursor = 0;
        break;

    case 1:
        feedSettingsMenu(false);
        feedFilterMenu(true);
        curCursor = 1;
        break;

    default:
        break;
    }
    u8g2.sendBuffer();
}




void buttonOKLongPressCallbackDownloadMode()
{
    sa868.sleep();

    downloadMode = true;
    PMU.setChargingLedMode(XPOWERS_CHG_LED_BLINK_4HZ);

    vTaskDelete(dataTaskHandler);
    vTaskDelete(clearvolumeSliderTaskHandler);
    vTaskDelete(gnssTaskHandler);
    vTaskDelete(encoderTaskHandler);
    vTaskDelete(updateTaskHandler);
    vTaskDelete(pielxTaskHandler);

    //! External pin power supply
    PMU.disableDC5();
    PMU.disableALDO1();
    PMU.disableALDO3();
    PMU.disableBLDO2();

    //! ALDO2 MICRO TF Card VDD
    PMU.disableALDO2();

    //! ALDO4 GNSS VDD
    PMU.disableALDO4();

    //! BLDO1 MIC VDD
    PMU.disableBLDO1();

    //! DC3 Radio & Pixels VDD
    PMU.disableDC3();

    // esp_sleep_enable_ext1_wakeup(((uint64_t)(((uint64_t)1) << ENCODER_OK_PIN)), ESP_EXT1_WAKEUP_ALL_LOW);
    // esp_deep_sleep_start();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(18, 30, "DOWLOAD");
    u8g2.drawStr(25, 58, " MODE");
    u8g2.sendBuffer();

}



void print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;

    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;

    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Wakeup caused by timer");
        break;

    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        Serial.println("Wakeup caused by touchpad");
        break;

    case ESP_SLEEP_WAKEUP_ULP:
        Serial.println("Wakeup caused by ULP program");
        break;

    default:
        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        break;
    }
}

/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/