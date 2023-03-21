
/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include "config.h"
#include "ui.h"
#include "data.h"
#include "sa868.h"
#include "logo.h"

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

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

#define SA868_TX_PIN  (47)
#define SA868_RX_PIN  (48)
#define SA868_PTT_PIN (41)
#define SA868_PD_PIN  (40)
#define SA868_RF_PIN  (39)

#define BUTTON_PTT_PIN  (38)
#define BUTTON_UP_PIN   (3)
#define BUTTON_DOWN_PIN (0)

#define ENCODER_A_PIN  (9)
#define ENCODER_B_PIN  (5)
#define ENCODER_OK_PIN (7)

#define BATTERY_ADC_PIN (6)
#define OLED_POWER_PIN  (21)
#define LED_PIN         (1)

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

OneButton buttonUp(BUTTON_UP_PIN, true);
OneButton buttonDown(BUTTON_DOWN_PIN, true);
OneButton buttonOK(ENCODER_OK_PIN, true);
OneButton buttonPTT(BUTTON_PTT_PIN, true);
RotaryEncoder encoder(ENCODER_B_PIN, ENCODER_A_PIN, RotaryEncoder::LatchMode::FOUR3);

SA868 sa868(Serial1, SA868_PTT_PIN, SA868_PD_PIN, SA868_RF_PIN);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 14, 13);

EncoderCallback encoderFn;
TaskHandle_t dataTaskHandler;
TaskHandle_t clearvolumeSliderTaskHandler;
int vref = 1100;

/******************************************************************************/
/***        exported functions                                              ***/
/******************************************************************************/

void setup() {
    Serial.begin(115200);
    delay(1000);

    print_wakeup_reason();

    app_nvs_init();
    data_init(sa868);
    adc_init();

    Serial1.begin(9600, SERIAL_8N1, SA868_RX_PIN, SA868_TX_PIN);
    sa868.begin();

    pinMode(OLED_POWER_PIN, OUTPUT);
    digitalWrite(OLED_POWER_PIN, LOW);

    u8g2.begin();
    u8g2.setContrast(0);
    u8g2.clearBuffer();
    u8g2.drawXBM(0, 0, 128, 64, gImage_logo);
    u8g2.sendBuffer();
    pinMode(OLED_POWER_PIN, OUTPUT);
    digitalWrite(OLED_POWER_PIN, HIGH);
    u8g2.setContrast(255);
    delay(1500);
    ui();

    buttonUp.attachClick(volumeUp);
    buttonDown.attachClick(volumeDown);
    buttonOK.attachClick(buttonOKClickCallbackMainPage);
    buttonOK.setPressTicks(2000);
    buttonOK.attachLongPressStart(buttonOKLongPressCallbackScreenOff);
    buttonOK.attachLongPressStop(buttonOKLongPressCallbackDeepSleep);
    buttonPTT.attachLongPressStart(transmit);
    buttonPTT.attachLongPressStop(receive);

    xTaskCreatePinnedToCore(encoderTask, "encoderTask", 4096, NULL, 8, NULL, 1);
    xTaskCreatePinnedToCore(updateTask, "updateTask", 4096, NULL, 6, NULL, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(dataTask, "dataTask", 4096, NULL, 5, &dataTaskHandler, 1);
    xTaskCreatePinnedToCore(clearvolumeSliderTask, "clearvolumeSliderTask", 4096, NULL, 2, &clearvolumeSliderTaskHandler, 1);
}


/******************************************************************************/
/***        local functions                                                 ***/
/******************************************************************************/

uint8_t curPage = 0;
int8_t curCursor = 0;
int8_t lastCursor = 0;

void app_nvs_init(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}


void adc_init() {
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        Serial.printf("eFuse Vref: %u mV", adc_chars.vref);
        vref = adc_chars.vref;
    }
}


uint8_t getBatteryPercentage() {
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
}


void updateTask(void *pvParameters) {
    while (1) {
        if (curPage == 0) {
            feedElectricity(getBatteryPercentage());
            feedRSSI(sa868.getRSSI());
            u8g2.sendBuffer();
        }
        delay(5000);
    }
    vTaskDelete(NULL);
}


void dataTask(void *pvParameter) {
    vTaskSuspend(dataTaskHandler);
    while (1) {
        data_save(sa868);
        vTaskSuspend(dataTaskHandler);
        delay(1000);
    }
    vTaskDelete(NULL);
}


void clearvolumeSliderTask(void *pvParameter) {
    vTaskSuspend(clearvolumeSliderTaskHandler);
    while (1) {
        delay(1500);
        feedVolumeSlider();
        u8g2.sendBuffer();
        vTaskSuspend(clearvolumeSliderTaskHandler);
    }
    vTaskDelete(NULL);
}


void loop() {
    buttonOK.tick();
    buttonUp.tick();
    buttonDown.tick();
    buttonPTT.tick();
    delay(10);
}


void transmit() {
    if (curPage == 0) {
        sa868.transmit();
        feedTransmitStatus();
        u8g2.sendBuffer();
    }
}


void receive() {
    if (curPage == 0) {
        sa868.receive();
        feedRecvFreq(sa868.recvFreq);
        u8g2.sendBuffer();
    }
}


void encoderTask(void *pvParameters) {
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
                    encoderFn((void *)-1);
                }
            }
            pos = newPos;
        }
        delay(10);
    }

    vTaskDelete(NULL);
}


void ui() {
    feedSettingsMenu(false);
    feedFilterMenu(false);
    feedRecvFreq(sa868.recvFreq);
    feedRSSI(sa868.getRSSI());
    feedElectricity(getBatteryPercentage());
    feedTxCXCSS(cxcss[sa868.txCXCSS]);
    feedRxCXCSS(cxcss[sa868.rxCXCSS]);
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


void volumeUp() {
    vTaskSuspend(clearvolumeSliderTaskHandler);
    sa868.setVolume(sa868.volume + 1);
    vTaskResume(dataTaskHandler);
    feedVolume(sa868.volume);
    u8g2.sendBuffer();
    vTaskResume(clearvolumeSliderTaskHandler);
}


void volumeDown() {
    vTaskSuspend(clearvolumeSliderTaskHandler);
    sa868.setVolume(sa868.volume - 1);
    vTaskResume(dataTaskHandler);
    feedVolume(sa868.volume);
    u8g2.sendBuffer();
    vTaskResume(clearvolumeSliderTaskHandler);
}


// settings page
void buttonOKClickCallbackSettingsPage() {
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
    }
}


void encoderCallbackSettingsPage(void *pvParameters) {
    curCursor = curCursor + (int)pvParameters;

    if (curCursor > 5) {
        curCursor = 0;
    } else if (curCursor < 0) {
        curCursor = 5;
    }
    // curCursor = pos;
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
}


void buttonOKDoubleClickCallbackSettingsPage() {
    curPage = 1;
    curCursor = lastCursor;
    encoderFn = encoderCallbackSettingsPage;
    // feedSettingsPage();
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


// bandwidth page
void buttonOKClickCallbackBandwidthPage() {
    sa868.bandwidth = curCursor == 0 ? false : true;
    curPage = 1;
    curCursor = lastCursor;
    encoderFn = encoderCallbackSettingsPage;
    // feedSettingsPage();
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void encoderCallbackBandWidthPage(void *pvParameters) {
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

long long tempFreq = 0.0;

// TransFreq Page
void buttonOKClickCallbackTransFreqPage1() {
    tempFreq = MIN_FREQ + curCursor * 1000 * 1000;
    if (sa868.bandwidth) {
        curCursor = (sa868.transFreq % (1000 * 1000)) / 25000;
    } else {
        curCursor = (sa868.transFreq % (1000 * 1000)) / 12500;
    }
    encoderFn = encoderCallbackTransFreqPage2;
    buttonOK.attachClick(buttonOKClickCallbackTransFreqPage2);
}


void encoderCallbackTransFreqPage1(void *pvParameters) {
    int pos = curCursor;

    pos = pos + (int)pvParameters;
    if (pos > FREQ_RANGE -1) {
        pos = 0;
    } else if (pos < 0) {
        pos = FREQ_RANGE - 1;
    }
    curCursor = pos;
    feedSettingsPagebarTransFreqPageCXCSSList1((MIN_FREQ + curCursor * (1 * 1000 * 1000)), sa868.bandwidth, MIN_FREQ, MAX_FREQ);
    u8g2.sendBuffer();
}


void buttonOKClickCallbackTransFreqPage2() {
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
    // feedSettingsPage();
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void encoderCallbackTransFreqPage2(void *pvParameters) {
    int pos = curCursor;
    int range = 0;
    pos = pos + (int)pvParameters;
    if (sa868.bandwidth) {
        range = (1 * 1000 * 1000 / 25000) - 1;
    } else {
        range= (1 * 1000 * 1000 / 12500) - 1;
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
void buttonOKClickCallbackRecvFreqPage1() {
    tempFreq = MIN_FREQ + curCursor * 1000 * 1000;

    if (sa868.bandwidth) {
        curCursor = (sa868.transFreq % (1000 * 1000)) / 25000;
    } else {
        curCursor = (sa868.transFreq % (1000 * 1000)) / 12500;
    }
    // curCursor = lastCursor;
    encoderFn = encoderCallbackTransFreqPage2;
    buttonOK.attachClick(buttonOKClickCallbackRecvFreqPage2);
}


void buttonOKClickCallbackRecvFreqPage2() {
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
    // feedSettingsPage();
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


//TXCXCSS page
void buttonOKClickCallbackTXCXCSSPage() {
    // sa868.txCXCSS = curCursor;
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
    // feedSettingsPage();
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void encoderCallbackTXCXCSSPage(void *pvParameters) {
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
void buttonOKClickCallbackSQPage() {
    // sa868.sq = curCursor;
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
    // feedSettingsPage();
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void encoderCallbackSQPage(void *pvParameters) {
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
void buttonOKClickCallbackRXCXCSSPage() {
    // sa868.rxCXCSS = curCursor;
    sa868.setGroup(sa868.bandwidth, sa868.transFreq, sa868.recvFreq, (teCXCSS)sa868.txCXCSS, sa868.sq, (teCXCSS)curCursor);
    vTaskResume(dataTaskHandler);
    curPage = 1;
    curCursor = lastCursor;
    encoderFn = encoderCallbackSettingsPage;
    // feedSettingsPage();
    feedSettingsPage1(curCursor);
    u8g2.sendBuffer();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void encoderCallbackRXCXCSSPage(void *pvParameters) {
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
void buttonOKClickCallbackFilterPage() {
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


void encoderCallbackFilterPage(void *pvParameters) {
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


void buttonOKDoubleClickCallbackMainPage() {
    Serial.println("I [Main]: Ok button double click");
    curPage = 0;
    ui();
    buttonOK.attachClick(buttonOKClickCallbackMainPage);
}


void encoderCallbackMainPage(void *pvParameters) {
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


void buttonOKLongPressCallbackScreenOff() {
    sa868.sleep();
    digitalWrite(OLED_POWER_PIN, LOW);
}


void buttonOKLongPressCallbackDeepSleep() {
    esp_sleep_enable_ext1_wakeup(((uint64_t)(((uint64_t)1)<<ENCODER_OK_PIN)), ESP_EXT1_WAKEUP_ALL_LOW);
    esp_deep_sleep_start();
}


void buttonOKLongPressCallbackScreenOn() {
    buttonOK.attachLongPressStart(buttonOKLongPressCallbackScreenOff);
    sa868.wake();
    digitalWrite(OLED_POWER_PIN, HIGH);
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