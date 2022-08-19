
/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include "ui.h"
#include "data.h"
#include "sa868.h"

#include <U8g2lib.h>
#include <ArduinoJson.h>
#include <OneButton.h>
#include <Encoder.h>

#include <nvs.h>
#include <nvs_flash.h>

#include <Wire.h>
#include <LittleFS.h>
#include <Arduino.h>

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

#define CONFIG_SA868_TX_PIN  (47)
#define CONFIG_SA868_RX_PIN  (48)
#define CONFIG_SA868_PTT_PIN (41)
#define CONFIG_SA868_PD_PIN  (40)
#define CONFIG_SA868_RF_PIN  (39)

#define CONFIG_BUTTON_PTT_PIN (38)
#define CONFIG_BUTTON_SW5_PIN (3)
#define CONFIG_BUTTON_SW6_PIN (4)

#define CONFIG_ENCODER_A_PIN   (9)
#define CONFIG_ENCODER_B_PIN   (5)
#define CONFIG_ENCODER_SW1_PIN (6)
#define CONFIG_ENCODER_SW2_PIN (7)
#define CONFIG_ENCODER_COM_PIN (8)

#define CONFIG_BATTERY_ADC_PIN (15)

#define CONFIG_LED_GREEN       (1)

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

OneButton buttonUp(CONFIG_BUTTON_SW5_PIN, true);
OneButton buttonDown(CONFIG_BUTTON_SW6_PIN, true);
OneButton buttonOK(CONFIG_ENCODER_SW1_PIN, true);
Encoder encoder(CONFIG_ENCODER_B_PIN, CONFIG_ENCODER_A_PIN);

SA868 sa868(Serial1, CONFIG_SA868_PTT_PIN, CONFIG_SA868_PD_PIN, CONFIG_SA868_RF_PIN);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R1, U8X8_PIN_NONE, 14, 13);

EncoderCallback encoderFn;

/******************************************************************************/
/***        exported functions                                              ***/
/******************************************************************************/

void setup() {
    Serial.begin(115200);

    app_nvs_init();
    data_init(sa868);

    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);

    Serial1.begin(9600, SERIAL_8N1, CONFIG_SA868_RX_PIN, CONFIG_SA868_TX_PIN);
    sa868.begin();
    pinMode(CONFIG_BUTTON_PTT_PIN, INPUT);
    u8g2.begin();
    u8g2.clearBuffer();
    delay(1000);
    ui();
    buttonOKInit();
    buttonUp.attachClick(volumeUp);
    buttonDown.attachClick(volumeDown);
    buttonOK.attachClick(buttonOKClickCallbackMainPage);

    xTaskCreatePinnedToCore(buttonTask, "buttonTask", 4096, NULL, 4, NULL, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(encoderTask, "encoderTask", 2048, NULL, 4, NULL, 1);
    // xTaskCreatePinnedToCore(scanRFTask, "scanRFTask", 4096, NULL, 6, NULL, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(uiTask, "uiTask", 4096, NULL, 6, NULL, 1);
}


/******************************************************************************/
/***        local functions                                                 ***/
/******************************************************************************/

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

#if 0
void scanRFTask(void *pvParameters) {
    double freq = 134.0;
    bool ret = false;
    while (1) {
        if (freq == 174.0)
            freq = 400.0;
        if (freq > 480.0)
            break;
        ret = sa868.scanRF(freq);
        if (ret)
            printf("scan freq: %lf, res: %s\n", freq, "Yes");
        freq += 0.125;
        delay(100);
    }
    vTaskDelete(NULL);
}
#endif

void uiTask(void *pvParameter) {
    while (1) {
        // u8g2.sendBuffer();
        data_save(sa868);
        delay(1000);
    }
    vTaskDelete(NULL);
}


uint8_t curPage = 0;
int8_t curCursor = 0;

void loop() {
    if (curPage == 0) {
        if (digitalRead(CONFIG_BUTTON_PTT_PIN)) {
            sa868.transmit();
            feedTransmitStatus(true);
        } else {
            sa868.receive();
            feedTransmitStatus(false);
        }
    }
    delay(10);
}


void buttonTask(void *pvParameters) {
    while (1) {
        buttonOK.tick();
        buttonUp.tick();
        buttonDown.tick();
        delay(10);
    }
    vTaskDelete(NULL);
}


void encoderTask(void *pvParameters) {
    int pos = 0;
    while (1) {
        int newPos = encoder.read();
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
    feedRSSI(sa868.getRSSI());
    feedVolume(sa868.volume);
    feedElectricity(0.0);
    feedSettingsMenu(false);
    feedFilterMenu(false);
    u8g2.sendBuffer();
    encoderFn = EncoderCallbackMainPage;
}


void volumeUp() {
    sa868.setVolume(sa868.volume + 1);
    feedVolume(sa868.volume);
}


void volumeDown() {
    sa868.setVolume(sa868.volume - 1);
    feedVolume(sa868.volume);
}


void buttonOKInit() {
    pinMode(CONFIG_ENCODER_COM_PIN, OUTPUT);
    digitalWrite(CONFIG_ENCODER_COM_PIN, LOW);

    pinMode(CONFIG_ENCODER_SW1_PIN, INPUT_PULLUP);

    pinMode(CONFIG_ENCODER_SW2_PIN, OUTPUT);
    digitalWrite(CONFIG_ENCODER_COM_PIN, LOW);
}


// settings page
void buttonOKClickCallbackSettingsPage()
{
    if (curCursor == 0) {
        curPage = 3;
        feedSettingsPagebarBandWidthPage();
        feedSettingsPagebarBandWidthPageBar1(!sa868.bandwidth);
        feedSettingsPagebarBandWidthPageBar2(sa868.bandwidth);
        u8g2.sendBuffer();
        encoderFn = EncoderCallbackBandWidthPage;
        if (sa868.bandwidth)
        {
            curCursor = 1;
        } else {
            curCursor = 0;
        }
        buttonOK.attachClick(buttonOKClickCallbackBandwidthPage);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    } else if (curCursor == 1) {
        curPage = 4;
        feedSettingsPagebarTransFreqPage();
        feedSettingsPagebarTransFreqPageCXCSSList1((long long)sa868.transFreq, sa868.bandwidth, 400.0, 480.0);
        long long t = (long long)sa868.transFreq;
        if (sa868.bandwidth) {
            feedSettingsPagebarTransFreqPageCXCSSList2(sa868.transFreq - t, 0.25);
        } else {
            feedSettingsPagebarTransFreqPageCXCSSList2(sa868.transFreq - t, 0.125);
        }
        u8g2.sendBuffer();
        encoderFn = EncoderCallbackTransFreqPage1;
        curCursor = (int)(sa868.transFreq - 400.0);
        buttonOK.attachClick(buttonOKClickCallbackTransFreqPage1);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    } else if (curCursor == 2) {
        curPage = 5;
        feedSettingsPagebarRecvFreqPage();
        feedSettingsPagebarTransFreqPageCXCSSList1((long long)sa868.recvFreq, sa868.bandwidth, 400.0, 480.0);
        long long t = (long long)sa868.recvFreq;
        if (sa868.bandwidth) {
            feedSettingsPagebarTransFreqPageCXCSSList2(sa868.recvFreq - t, 0.25);
        } else {
            feedSettingsPagebarTransFreqPageCXCSSList2(sa868.recvFreq - t, 0.125);
        }
        u8g2.sendBuffer();
        encoderFn = EncoderCallbackTransFreqPage1;
        curCursor = (int)(sa868.recvFreq - 400.0);
        buttonOK.attachClick(buttonOKClickCallbackRecvFreqPage1);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    } else if (curCursor == 3) {
        curPage = 6;
        Serial.printf("sa868.txCXCSS: %d\r\n", sa868.txCXCSS);
        encoderFn = EncoderCallbackTXCXCSSPage;
        curCursor = sa868.txCXCSS;
        feedSettingsPagebarTxCXCSSPage();
        feedSettingsPagebarCXCSSPageCXCSSList(sa868.txCXCSS, cxcss, 122);
        u8g2.sendBuffer();
        buttonOK.attachClick(buttonOKClickCallbackTXCXCSSPage);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    } else if (curCursor == 4) {
        curPage = 7;
        Serial.printf("sa868.sq: %d\r\n", sa868.sq);
        encoderFn = EncoderCallbackSQPage;
        curCursor = sa868.sq;
        feedSettingsPagebarSQPage();
        feedSettingsPagebarSQPageSlider(sa868.sq);
        u8g2.sendBuffer();
        buttonOK.attachClick(buttonOKClickCallbackSQPage);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    } else if (curCursor == 5) {
        curPage = 8;
        Serial.printf("sa868.rxCXCSS: %d\r\n", sa868.rxCXCSS);
        encoderFn = EncoderCallbackRXCXCSSPage;
        curCursor = sa868.rxCXCSS;
        feedSettingsPagebarRxCXCSSPage();
        feedSettingsPagebarCXCSSPageCXCSSList(sa868.rxCXCSS, cxcss, 122);
        u8g2.sendBuffer();
        buttonOK.attachClick(buttonOKClickCallbackRXCXCSSPage);
        buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackSettingsPage);
    }
}


void EncoderCallbackSettingsPage(void *pvParameters) {
    static int pos = 0;

    pos = pos + (int)pvParameters;
    if (pos > 5) {
        pos = 0;
    } else if (pos < 0) {
        pos = 5;
    }
    curCursor = pos;

    switch (pos) {
        case 0:
            feedSettingsPagebarBandWidth(true);
            feedSettingsPagebarTransFreq(false);
            feedSettingsPagebarRecvFreq(false);
            feedSettingsPagebarTxCXCSS(false);
            feedSettingsPagebarSQ(false);
            feedSettingsPagebarRxCXCSS(false);
            break;

        case 1:
            feedSettingsPagebarBandWidth(false);
            feedSettingsPagebarTransFreq(true);
            feedSettingsPagebarRecvFreq(false);
            feedSettingsPagebarTxCXCSS(false);
            feedSettingsPagebarSQ(false);
            feedSettingsPagebarRxCXCSS(false);
            break;

        case 2:
            feedSettingsPagebarBandWidth(false);
            feedSettingsPagebarTransFreq(false);
            feedSettingsPagebarRecvFreq(true);
            feedSettingsPagebarTxCXCSS(false);
            feedSettingsPagebarSQ(false);
            feedSettingsPagebarRxCXCSS(false);
            break;

        case 3:
            feedSettingsPagebarBandWidth(false);
            feedSettingsPagebarTransFreq(false);
            feedSettingsPagebarRecvFreq(false);
            feedSettingsPagebarTxCXCSS(true);
            feedSettingsPagebarSQ(false);
            feedSettingsPagebarRxCXCSS(false);
            break;

        case 4:
            feedSettingsPagebarBandWidth(false);
            feedSettingsPagebarTransFreq(false);
            feedSettingsPagebarRecvFreq(false);
            feedSettingsPagebarTxCXCSS(false);
            feedSettingsPagebarSQ(true);
            feedSettingsPagebarRxCXCSS(false);
            break;

        case 5:
            feedSettingsPagebarBandWidth(false);
            feedSettingsPagebarTransFreq(false);
            feedSettingsPagebarRecvFreq(false);
            feedSettingsPagebarTxCXCSS(false);
            feedSettingsPagebarSQ(false);
            feedSettingsPagebarRxCXCSS(true);
            break;

        default:
            break;
    }

    u8g2.sendBuffer();
}


void buttonOKDoubleClickCallbackSettingsPage()
{
    curPage = 1;
    encoderFn = EncoderCallbackSettingsPage;
    feedSettingsPage();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


// bandwidth page
void buttonOKClickCallbackBandwidthPage()
{
    // todo: set bandwidth
    if (curCursor == 0) {
        sa868.bandwidth = false;
        // sa868.setGroup(false, sa868.transFreq, sa868.recvFreq, (teCXCSS)sa868.txCXCSS, sa868.sq, (teCXCSS)sa868.rxCXCSS);
    } else {
        sa868.bandwidth = true;
        // sa868.setGroup(true, sa868.transFreq, sa868.recvFreq, (teCXCSS)sa868.txCXCSS, sa868.sq, (teCXCSS)sa868.rxCXCSS);
    }
    // sa868.setGroup(sSA868Data.bandwidth, sSA868Data.transFreq, sSA868Data.recvFreq, (teCXCSS)sSA868Data.txCXCSS, sSA868Data.sq, (teCXCSS)sSA868Data.rxCXCSS);
    curPage = 1;
    encoderFn = EncoderCallbackSettingsPage;
    feedSettingsPage();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void EncoderCallbackBandWidthPage(void *pvParameters) {
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

double tempFreq = 0.0;

// TransFreq Page
void buttonOKClickCallbackTransFreqPage1()
{
    encoderFn = EncoderCallbackTransFreqPage2;
    tempFreq = 400.0 + curCursor;
    long long t = (long long)sa868.transFreq;
    if (sa868.bandwidth) {
        curCursor = (sa868.transFreq - t) / 0.25;
    } else {
        curCursor = (sa868.transFreq - t) / 0.125;
    }
    buttonOK.attachClick(buttonOKClickCallbackTransFreqPage2);
}


void EncoderCallbackTransFreqPage1(void *pvParameters)
{
    int pos = curCursor;

    pos = pos + (int)pvParameters;
    if (pos > 79) {
        pos = 0;
    } else if (pos < 0) {
        pos = 79;
    }
    curCursor = pos;

    Serial.printf("curCursor: %d\r\n", curCursor);
    feedSettingsPagebarTransFreqPageCXCSSList1((long long)(400.0 + curCursor), sa868.bandwidth, 400.0, 480.0);
    u8g2.sendBuffer();
}


void buttonOKClickCallbackTransFreqPage2()
{
    // todo: set trans freq
    if (sa868.bandwidth) {
        tempFreq = tempFreq + curCursor * 0.25 ;
    } else {
        tempFreq = tempFreq + curCursor * 0.125 ;
    }
    sa868.setGroup(sa868.bandwidth, tempFreq, sa868.recvFreq, (teCXCSS)sa868.txCXCSS, sa868.sq, (teCXCSS)sa868.rxCXCSS);
    Serial.printf("I [Main]: set transFreq: %lf\r\n", sa868.transFreq);
    curPage = 1;
    encoderFn = EncoderCallbackSettingsPage;
    feedSettingsPage();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void EncoderCallbackTransFreqPage2(void *pvParameters)
{
    int pos = curCursor;
    int range = 0;
    pos = pos + (int)pvParameters;
    if (sa868.bandwidth) {
        range = (1 / 0.25) - 1;
    } else {
        range= (1 / 0.125) - 1;
    }
    if (pos > range) {
        pos = 0;
    } else if (pos < 0) {
        pos = range;
    }
    curCursor = pos;

    Serial.printf("curCursor: %d\r\n", curCursor);
    long long t = (long long)sa868.transFreq;
    if (sa868.bandwidth) {
        feedSettingsPagebarTransFreqPageCXCSSList2((curCursor * 0.25), 0.25);
    } else {
        feedSettingsPagebarTransFreqPageCXCSSList2((curCursor * 0.125), 0.125);
    }
    u8g2.sendBuffer();
}


// recv page
void buttonOKClickCallbackRecvFreqPage1()
{
    encoderFn = EncoderCallbackTransFreqPage2;
    tempFreq = 400.0 + curCursor;
    long long t = (long long)sa868.recvFreq;
    if (sa868.bandwidth) {
        curCursor = (sa868.recvFreq - t) / 0.25;
    } else {
        curCursor = (sa868.recvFreq - t) / 0.125;
    }
    buttonOK.attachClick(buttonOKClickCallbackRecvFreqPage2);
}


void buttonOKClickCallbackRecvFreqPage2()
{
    // todo: set recv freq
    if (sa868.bandwidth) {
        tempFreq = tempFreq + curCursor * 0.25 ;
    } else {
        tempFreq = tempFreq + curCursor * 0.125 ;
    }
    sa868.setGroup(sa868.bandwidth, sa868.transFreq, tempFreq, (teCXCSS)sa868.txCXCSS, sa868.sq, (teCXCSS)sa868.rxCXCSS);
    Serial.printf("I [Main]: set recvFreq: %lf\r\n", sa868.recvFreq);
    curPage = 1;
    encoderFn = EncoderCallbackSettingsPage;
    feedSettingsPage();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


//TXCXCSS page
void buttonOKClickCallbackTXCXCSSPage()
{
    // todo: set TXCXCSS
    // sa868.txCXCSS = curCursor;
    sa868.setGroup(sa868.bandwidth, sa868.transFreq, sa868.recvFreq, (teCXCSS)curCursor, sa868.sq, (teCXCSS)sa868.rxCXCSS);
    curPage = 1;
    encoderFn = EncoderCallbackSettingsPage;
    feedSettingsPage();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void EncoderCallbackTXCXCSSPage(void *pvParameters)
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
    // todo: set SQ
    // sa868.sq = curCursor;
    sa868.setGroup(sa868.bandwidth, sa868.transFreq, sa868.recvFreq, (teCXCSS)sa868.txCXCSS, curCursor, (teCXCSS)sa868.rxCXCSS);
    curPage = 1;
    encoderFn = EncoderCallbackSettingsPage;
    feedSettingsPage();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void EncoderCallbackSQPage(void *pvParameters)
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
    // todo: set RXCXCSS
    // sa868.rxCXCSS = curCursor;
    sa868.setGroup(sa868.bandwidth, sa868.transFreq, sa868.recvFreq, (teCXCSS)sa868.txCXCSS, sa868.sq, (teCXCSS)curCursor);
    curPage = 1;
    encoderFn = EncoderCallbackSettingsPage;
    feedSettingsPage();
    buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
    buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
}


void EncoderCallbackRXCXCSSPage(void *pvParameters)
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
        // sa868.emphasis = !sa868.emphasis;
        sa868.setFilter(!sa868.emphasis, sa868.highPass, sa868.lowPass);
    } else if (curCursor == 1) {
        feedFilterPageBar2(true, !sa868.highPass);
        // sa868.highPass = !sa868.highPass;
        sa868.setFilter(sa868.emphasis, !sa868.highPass, sa868.lowPass);
    } else if (curCursor == 2) {
        feedFilterPageBar3(true, !sa868.lowPass);
        // sa868.lowPass = !sa868.lowPass;
        sa868.setFilter(sa868.emphasis, sa868.highPass, !sa868.lowPass);
    }
    u8g2.sendBuffer();
}


void EncoderCallbackFilterPage(void *pvParameters)
{
    static int pos = 0;

    pos = pos + (int)pvParameters;
    if (pos > 2) {
        pos = 0;
    } else if (pos < 0) {
        pos = 2;
    }
    curCursor = pos;
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
            encoderFn = EncoderCallbackSettingsPage;
            feedSettingsPage();
            buttonOK.attachClick(buttonOKClickCallbackSettingsPage);
            buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
        } else if (curCursor == 1) {
            curPage = 2;
            feedFilterPage();
            feedFilterPageBar1(false, sa868.emphasis);
            feedFilterPageBar2(false, sa868.highPass);
            feedFilterPageBar3(false, sa868.lowPass);
            u8g2.sendBuffer();
            encoderFn = EncoderCallbackFilterPage;
            buttonOK.attachClick(buttonOKClickCallbackFilterPage);
            buttonOK.attachDoubleClick(buttonOKDoubleClickCallbackMainPage);
        }
    }
}


void buttonOKDoubleClickCallbackMainPage()
{
    Serial.println("I [Main]: Ok button double click");
    curPage = 0;
    feedMainPage();
    encoderFn = EncoderCallbackMainPage;
    buttonOK.attachClick(buttonOKClickCallbackMainPage);
}


void EncoderCallbackMainPage(void *pvParameters) {
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

/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/