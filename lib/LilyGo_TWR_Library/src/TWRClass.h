/**
 * @file      TWRClass.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-01-02
 * @note
 * @note    Power Domain
 *          ! Rev 2.1
 *          | DC1   | ESP + OLED + PIXEL         | 3.3V |
 *          | DC3   | Unuse                      | 3.3V |
 *          | DC5   | Unuse                      | -    |
 *          | ALDO1 | Unuse                      | -    |
 *          | ALDO2 | TF Card                    | 3.3V |
 *          | ALDO3 | Audio amplification switch | 3.3V |
 *          | ALOD4 | GPS                        | 3.3V |
 *          | BLDO1 | Microphone                 | 3.3V |
 *          | BLDO2 | Radio                      | 3.3V |
 *          | DLDO1 | Dowerloader                | 3.3V |
 *
 *          ! Rev 2.0
 *          | DC1   | ESP + OLED                 | 3.3V |
 *          | DC3   | Radio  + PIXEL             | 3.4V |
 *          | DC5   | Unuse                      | -    |
 *          | ALDO1 | Unuse                      | -    |
 *          | ALDO2 | TF Card                    | 3.3V |
 *          | ALDO3 | Unuse                      | 3.3V |
 *          | ALOD4 | GPS                        | 3.3V |
 *          | BLDO1 | Microphone                 | 3.3V |
 *          | BLDO2 | Unuse                      | -    |
 *          | DLDO1 | Unuse                      | -    |
 *
 */

#include <XPowersLib.h>
#include <Preferences.h>
#include <SPI.h>
#include <Wire.h>
#include "utilities.h"
#include "sa868.h"

#ifdef DEBUG_PORT
namespace
{
template <typename T>
static void DBG_PLAIN(T last)
{
    DEBUG_PORT.println(last);
}

template <typename T, typename... Args>
static void DBG_PLAIN(T head, Args... tail)
{
    DEBUG_PORT.print(head);
    DEBUG_PORT.print(' ');
    DBG_PLAIN(tail...);
}

template <typename... Args>
static void DBG(Args... args)
{
    DEBUG_PORT.print("[");
    DEBUG_PORT.print(millis());
    DEBUG_PORT.print("] ");
    DBG_PLAIN(args...);
}
}  // namespace
#else
#define DBG_PLAIN(...)
#define DBG(...)
#endif

struct SysSetting {
    bool beep;
    bool wifi;
    bool ble;
    uint8_t dispBrightness;
    uint8_t pixelBrightness;
};
#define SYS_KEY "sys"
#define SAVE_CONFIGURE(VAL,NEW_VAL)         do{ \
        twr.pdat.VAL = NEW_VAL;                 \
        twr.saveConfigure();                    \
    }while(0)


enum TWR_Model {
    LILYGO_TWR_REV2_0,
    LILYGO_TWR_REV2_1,
    LILYGO_TWR_AUTO,
};


class TWRClass : public XPowersAXP2101
{
public:
    typedef void (*CallbackFunction)(void);

    struct SysSetting pdat;

    enum TWR_Version {
        TWR_REV2V0,
        TWR_REV2V1,
    };

    enum IOMUX_Channel {
        TWR_IO_MUX_RADIO,
        TWR_IO_MUX_PIN,
    };

    enum MIC_Channel {
        TWR_MIC_TO_ESP,// Microphone will be routed to ESP ADC IO15 and the SA868 audio input will be routed to ESP IO18
        TWR_MIC_TO_RADIO, //Microphone will be routed to the SA868 audio input
    };

    enum SPK_Channel {
        TWR_ESP_TO_SPK,
        TWR_RADIO_TO_SPK,
    };

    TWR_Version getVersion();

    uint8_t getOLEDAddress();

    RadioType getBandDefinition();

    void routingIO2Downloader(IOMUX_Channel ch);

    void routingMicrophoneChannel(MIC_Channel ch);

    void routingSpeakerChannel(SPK_Channel ch);

    bool begin(TWR_Model model = LILYGO_TWR_AUTO);

    bool saveConfigure();

    void enableBeep(bool enable);

    bool isEnableBeep();

    struct SysSetting getSetting() const ;

    void enablePowerOff(bool enable);

    void attachLongPressed(CallbackFunction cb);

    void attachClick(CallbackFunction fn);

    void tick();

    void selectEsp2Amp(bool enable);

    void selectIO2Radio(bool enable);


    inline void  enableRadio();

    inline void  disableRadio();

    inline void enableGPS();

    inline void disableGPS();

    inline void enableSD();

    inline void disableSD();

    inline void enableDownloadSwitch();

    inline void disableDownloadSwitch();

    inline void enableMicrophone();

    inline void disableMicrophone();

    void ledToggle();

    bool isChargeDone();

    bool isCharging();

    void deviceScan(Stream *stream);


    void sleep();

private:

    static void setFlag();

    bool beginPower();

    bool initPreferences();


    static volatile bool interruptFlag;
    static volatile bool _isChargeStart;
    static volatile bool _isChargeDone ;
    CallbackFunction clickCallback = NULL;
    CallbackFunction longPressedCallback = NULL;
    TWR_Version _version;
    Preferences prefs;
    uint8_t _oledAddress = 0xFF;
public:
    static volatile bool isReceiving;
};


extern TWRClass twr;
