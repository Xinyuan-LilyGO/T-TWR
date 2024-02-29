/**
 * @file      TWRClass.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-01-02
 * @note
 * @note     Power Domain
 *           |       |         ! Rev 2.1          |      |
 *           | ----- | -------------------------- | ---- |
 *           | DC1   | ESP + OLED + PIXEL         | 3.3V |
 *           | DC3   | Unused                     | 3.3V |
 *           | DC5   | Unused                     | -    |
 *           | ALDO1 | Unused                     | -    |
 *           | ALDO2 | TF Card                    | 3.3V |
 *           | ALDO3 | Audio amplification switch | 3.3V |
 *           | ALOD4 | GPS                        | 3.3V |
 *           | BLDO1 | Microphone                 | 3.3V |
 *           | BLDO2 | Unused                     | -    |
 *           | DLDO1 | Downloader                 | 3.3V |
 *
 *           |       |        ! Rev 2.0           |      |
 *           | ----- | --------------             | ---- |
 *           | DC1   | ESP + OLED                 | 3.3V |
 *           | DC3   | Radio  + PIXEL             | 3.4V |
 *           | DC5   | Unused                     | -    |
 *           | ALDO1 | Unused                     | -    |
 *           | ALDO2 | TF Card                    | 3.3V |
 *           | ALDO3 | Unused                     | 3.3V |
 *           | ALOD4 | GPS                        | 3.3V |
 *           | BLDO1 | Microphone                 | 3.3V |
 *           | BLDO2 | Unused                     | -    |
 *           | DLDO1 | Unused                     | -    |
 */
#include <SPI.h>
#include "TWRClass.h"

RadioType TWRClass::getBandDefinition()
{
    return _oledAddress == 0x3C ? SA8X8_VHF : SA8X8_UHF;
}

TWRClass::TWR_Version TWRClass::getVersion()
{
    return _version;
}

uint8_t TWRClass::getOLEDAddress()
{
    return _oledAddress;
}

void TWRClass::routingIO2Downloader(IOMUX_Channel ch)
{
    if (_version == TWR_REV2V0)return ;
    switch (ch) {
    case TWR_IO_MUX_RADIO:
        //! Set the IO(GPIO43,GPIO44，GPIO14) -> Radio Flash Pin
        this->selectIO2Radio(true);
        break;
    case TWR_IO_MUX_PIN:
        //! Set the IO(GPIO43,GPIO44，GPIO14) -> External expansion pin
        this->selectIO2Radio(false);
        break;
    default:
        break;
    }
}

void TWRClass::routingMicrophoneChannel(MIC_Channel ch)
{
    if (_version == TWR_REV2V0)return ;
    switch (ch) {
    case TWR_MIC_TO_ESP:
        //! Set the audio input channel to Mic -> ESP ADC , ESP PWM -> Radio audio in
        digitalWrite(MIC_CTRL_PIN, HIGH);
        break;
    case TWR_MIC_TO_RADIO:
        //! Set the audio input channel to Mic -> Radio
        digitalWrite(MIC_CTRL_PIN, LOW);
        break;
    default:
        break;
    }
}

void TWRClass::routingSpeakerChannel(SPK_Channel ch)
{
    if (_version == TWR_REV2V0)return ;
    switch (ch) {
    case TWR_ESP_TO_SPK:
        //! Set the audio power amplifier channel to ESP -> Amplifier
        this->selectEsp2Amp(true);
        break;
    case TWR_RADIO_TO_SPK:
        //! Set the audio power amplifier channel to Radio -> Amplifier
        this->selectEsp2Amp(false);
        break;
    default:
        break;
    }
}

void TWRClass::deviceScan(Stream *stream)
{
    uint8_t err, addr;
    int nDevices = 0;
    for (addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        err = Wire.endTransmission();
        if (err == 0) {
            if (stream) {
                stream->print("I2C device found at address 0x");
                if (addr < 16)
                    stream->print("0");
                stream->print(addr, HEX);
                stream->println(" !");
            }
            switch (addr) {
            case 0X3C:
                DBG("Find OLED ADDRESS : 0X3C");
                _oledAddress = 0x3C;
                break;
            case 0X3D:
                DBG("Find OLED ADDRESS : 0X3D");
                _oledAddress = 0X3D;
                break;
            default:
                break;
            }
            nDevices++;
        } else if (err == 4) {
            if (stream) {
                stream->print("Unknow error at address 0x");
                if (addr < 16)
                    stream->print("0");
                stream->println(addr, HEX);
            }
        }
    }
    if (nDevices == 0) {
        if (stream) {
            stream->println("No I2C devices found\n");
        }
    } else {
        if (stream) {
            stream->println("Done\n");
        }
    }
}

void TWRClass::sleep()
{

    detachInterrupt(PMU_IRQ);

    disableGPS();

    disableSD();

    disableDownloadSwitch();

    disableMicrophone();

    setChargingLedMode(XPOWERS_CHG_LED_OFF);

    RadioSerial.end();
    Wire.end();
    SPI.end();

    pinMode(GNSS_RX_PIN, OPEN_DRAIN);
    pinMode(GNSS_TX_PIN, OPEN_DRAIN);
    pinMode(SA868_RX_PIN, OPEN_DRAIN);
    pinMode(SA868_TX_PIN, OPEN_DRAIN);
    pinMode(GNSS_PPS_PIN, OPEN_DRAIN);
    pinMode(MIC_CTRL_PIN, OPEN_DRAIN);
    pinMode(SA868_PD_PIN, OPEN_DRAIN);
    pinMode(I2C_SDA, OPEN_DRAIN);
    pinMode(I2C_SCL, OPEN_DRAIN);
    pinMode(PMU_IRQ, OPEN_DRAIN);

    pinMode(SPI_MOSI, OPEN_DRAIN);
    pinMode(SPI_MISO, OPEN_DRAIN);
    pinMode(SPI_SCK, OPEN_DRAIN);
    pinMode(SD_CS, OPEN_DRAIN);
    pinMode(ESP2SA868_MIC, OPEN_DRAIN);
    pinMode(SA8682ESP_AUDIO, OPEN_DRAIN);

    pinMode(ENCODER_A_PIN, OPEN_DRAIN);
    pinMode(ENCODER_B_PIN, OPEN_DRAIN);


    if (_version == TWR_REV2V1 ) {
        pinMode(ESP32_PWM_TONE, OPEN_DRAIN);
        pinMode(ESP_MIC_ADC, OPEN_DRAIN);
        pinMode(SA868_SQL, OPEN_DRAIN);
    } else {
        pinMode(SA868_RF_PIN, OPEN_DRAIN);
    }

}


bool TWRClass::begin(TWR_Model model)
{
    Wire.begin(I2C_SDA, I2C_SCL);

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    // GNSS Serial
    GPSSerial.begin(GNSS_BAUD_RATE, SERIAL_8N1, GNSS_RX_PIN, GNSS_TX_PIN);

    // SA8x8 Serial
    RadioSerial.begin(9600, SERIAL_8N1, SA868_RX_PIN, SA868_TX_PIN);

    pinMode(GNSS_PPS_PIN, INPUT);

    // Microphone
    pinMode(MIC_CTRL_PIN, OUTPUT);
    // Set the audio input channel to Mic -> Radio
    // digitalWrite(MIC_CTRL_PIN, LOW);
    // Set the audio input channel to Mic -> ESP
    // digitalWrite(MIC_CTRL_PIN, HIGH);

    if (!beginPower()) {
        log_e("PMU is not online.");
        return false;
    }

    // Set SA8x8 PowerDown to HIGH
    pinMode(SA868_PD_PIN, OUTPUT);
    digitalWrite(SA868_PD_PIN, HIGH);

    pinMode(SA868_SQL, INPUT);

    if (model == LILYGO_TWR_AUTO) {
        int retry = 3;
        int state = LOW;
        while (retry--) {
            // Cycle three times for testing , This method is not reliable enough if Rev2.0 uses IO2 , Assuming IO2 is not used
            state = digitalRead(SA868_SQL); delay(30);
        }
        if (state == HIGH) {
            _version = TWR_REV2V1;
            log_i("It is detected that IO2 is HIGH level and the version is TWR Rev 2.1");
        } else {
            _version = TWR_REV2V0;
            log_i("It is detected that IO2 is LOW level and the version is TWR Rev 2.0");
        }
    } else {
        switch (model) {
        case LILYGO_TWR_REV2_0: _version = TWR_REV2V0;
            break;
        case LILYGO_TWR_REV2_1: _version = TWR_REV2V1;
            break;
            break;
        default:
            break;
        }
    }

    enableRadio();

    enableMicrophone();

    enableSD();

    enableGPS();

    if (_version == TWR_REV2V1) {
        // Rev2.1 has an additional pin to monitor whether it is in the receiving state.
        pinMode(SA868_SQL, INPUT_PULLUP);
        attachInterrupt(SA868_SQL, []() {
            if (digitalRead(SA868_SQL)) {
                isReceiving = false;
            } else {
                isReceiving = true;
            }
        }, CHANGE);

        // Rev2.1 now allows you to switch the audio amplifier input source selection
        routingSpeakerChannel(TWR_RADIO_TO_SPK);

        // Rev2.1 can route the serial port to the SA868 download interface
        routingIO2Downloader(TWR_IO_MUX_PIN);
    } else {
        // // When using Rev2.0, do not use IO38 to control power and keep IO open drain
        // pinMode(SA868_RF_PIN, HIGH);
    }

    initPreferences();

    routingMicrophoneChannel(TWR_MIC_TO_RADIO);


    if (XPowersAXP2101::isCharging()) {
        _isChargeStart = true;
    }

    deviceScan(NULL);

    DBG("----------------------------------------------------------------------------");
    DBG("DCDC1  : ", isEnableDC1()  ? "+" : "-",  "Voltage:", getDC1Voltage(), "mV");
    DBG("DCDC2  : ", isEnableDC2()  ? "+" : "-",  "Voltage:", getDC2Voltage(), "mV");
    DBG("DCDC3  : ", isEnableDC3()  ? "+" : "-",  "Voltage:", getDC3Voltage(), "mV");
    DBG("DCDC4  : ", isEnableDC4()  ? "+" : "-",  "Voltage:", getDC4Voltage(), "mV");
    DBG("DCDC5  : ", isEnableDC5()  ? "+" : "-",  "Voltage:", getDC5Voltage(), "mV");
    DBG("----------------------------------------------------------------------------");
    DBG("ALDO1  : ", isEnableALDO1()  ? "+" : "-", "Voltage:", getALDO1Voltage(), "mV");
    DBG("ALDO2  : ", isEnableALDO2()  ? "+" : "-", "Voltage:", getALDO2Voltage(), "mV");
    DBG("ALDO3  : ", isEnableALDO3()  ? "+" : "-", "Voltage:", getALDO3Voltage(), "mV");
    DBG("ALDO4  : ", isEnableALDO4()  ? "+" : "-", "Voltage:", getALDO4Voltage(), "mV");
    DBG("----------------------------------------------------------------------------");
    DBG("BLDO1  : ", isEnableBLDO1()  ? "+" : "-", "Voltage:", getBLDO1Voltage(), "mV");
    DBG("BLDO2  : ", isEnableBLDO2()  ? "+" : "-", "Voltage:", getBLDO2Voltage(), "mV");
    DBG("----------------------------------------------------------------------------");

    return true;
}

bool TWRClass::saveConfigure()
{
    size_t write = prefs.putBytes(SYS_KEY, &(this->pdat), sizeof(this->pdat));
    if (write != sizeof(this->pdat)) {
        log_e("Write database failed!");
        return false;
    } else {
        log_i("Write database succeeded!");
        return true;
    }
}

void TWRClass::enableBeep(bool enable)
{
    if (_version == TWR_REV2V0)return ;
    this->pdat.beep = enable;
}

bool TWRClass::isEnableBeep()
{
    if (_version == TWR_REV2V0)return false;
    return this->pdat.beep;
}

struct SysSetting TWRClass::getSetting() const {
    return pdat;
}

void TWRClass::enablePowerOff(bool enable)
{
    enable ? enableLongPressShutdown() : disableLongPressShutdown();
}

void TWRClass::attachLongPressed(CallbackFunction cb)
{
    longPressedCallback = cb;
}

void TWRClass::attachClick(CallbackFunction fn)
{
    clickCallback = fn;
}

void TWRClass::tick()
{
    if (!interruptFlag) {
        return;
    }

    interruptFlag = false;
    uint32_t status = getIrqStatus();
    DBG("STATUS => HEX:");
    DBG(status, HEX);
    DBG(" BIN:");
    DBG(status, BIN);
    (void)status;

    if (isPekeyShortPressIrq()) {
        if (clickCallback) {
            clickCallback();
        }
    }
    if (isPekeyLongPressIrq()) {
        if (longPressedCallback) {
            longPressedCallback();
        }
    }

    if (isBatChagerDoneIrq()) {
        _isChargeDone = true;
        _isChargeStart = false;
    }

    if (isBatChagerStartIrq()) {
        _isChargeStart = true;
    }

    if (isBatRemoveIrq() || isVbusRemoveIrq()) {
        _isChargeStart = false;
        _isChargeDone = false;
    }

    clearIrqStatus();
}

void TWRClass::selectEsp2Amp(bool enable)
{
    if (_version == TWR_REV2V1 ) {
        if (enable) {
            enableALDO3();
        } else {
            disableALDO3();
        }
    }
}

void TWRClass::selectIO2Radio(bool enable)
{
    if (_version == TWR_REV2V1 ) {
        if (enable) {
            enableDLDO1();
        } else {
            disableDLDO1();
        }
    }
}


void  TWRClass::enableRadio()
{
    switch (_version) {
    case TWR_REV2V0:
        enableDC3();
        break;
    case TWR_REV2V1:
        // enableBLDO2();
        break;
    default:
        break;
    }
}

void  TWRClass::disableRadio()
{
    switch (_version) {
    case TWR_REV2V0:
        disableDC3();
        break;
    case TWR_REV2V1:
        // disableBLDO2();
        break;
    default:
        break;
    }
}

void TWRClass::enableGPS()
{
    switch (_version) {
    case TWR_REV2V0:
    case TWR_REV2V1:
        enableALDO4();
        break;
    default:
        break;
    }
}

void TWRClass::disableGPS()
{
    switch (_version) {
    case TWR_REV2V0:
    case TWR_REV2V1:
        disableALDO4();
        break;
    default:
        break;
    }
}

void TWRClass::enableSD()
{
    switch (_version) {
    case TWR_REV2V0:
    case TWR_REV2V1:
        enableALDO2();
        break;
    default:
        break;
    }
}

void TWRClass::disableSD()
{
    switch (_version) {
    case TWR_REV2V0:
    case TWR_REV2V1:
        disableALDO2();
        break;
    default:
        break;
    }
}

void TWRClass::enableDownloadSwitch()
{
    switch (_version) {
    case TWR_REV2V0:
        break;
    case TWR_REV2V1:
        enableDLDO1();
        break;
    default:
        break;
    }
}

void TWRClass::disableDownloadSwitch()
{
    switch (_version) {
    case TWR_REV2V0:
        break;
    case TWR_REV2V1:
        disableDLDO1();
        break;
    default:
        break;
    }
}

void TWRClass::enableMicrophone()
{
    switch (_version) {
    case TWR_REV2V0:
    case TWR_REV2V1:
        enableBLDO1();
        break;
    default:
        break;
    }
}

void TWRClass::disableMicrophone()
{
    switch (_version) {
    case TWR_REV2V0:
    case TWR_REV2V1:
        disableBLDO1();
        break;
    default:
        break;
    }
}

void TWRClass::ledToggle()
{
    setChargingLedMode(getChargingLedMode() != XPOWERS_CHG_LED_OFF ? XPOWERS_CHG_LED_OFF  : XPOWERS_CHG_LED_ON);
}

bool TWRClass::isChargeDone()
{
    return _isChargeDone;
}

bool TWRClass::isCharging()
{
    return _isChargeStart;
}

void TWRClass::setFlag()
{
    interruptFlag = true;
}

bool TWRClass::beginPower()
{

    if (!XPowersAXP2101::init(Wire, I2C_SDA, I2C_SCL)) {
        return false;
    }

    // Set the minimum common working voltage of the PMU VBUS input,
    // below this value will turn off the PMU
    setVbusVoltageLimit(XPOWERS_AXP2101_VBUS_VOL_LIM_3V88);

    // Set the maximum current of the PMU VBUS input,
    // higher than this value will turn off the PMU
    setVbusCurrentLimit(XPOWERS_AXP2101_VBUS_CUR_LIM_2000MA);

    // Set VSY off voltage as 2600mV , Adjustment range 2600mV ~ 3300mV
    setSysPowerDownVoltage(2600);


    disableBLDO1(); //Rev2.x Microphone LDO
    disableDC3();   //Rev2.0 SA8x8  DC boost , Rev2.1 user LDO
    disableBLDO2(); //Rev2.1 User LDO
    disableALDO2(); //Rev2.x SD Card LDO
    disableALDO3(); //Rev2.1 Audio amplification switch ， Rev2.0 user LDO
    disableALDO4(); //Rev2.x GPS LDO
    disableDLDO1(); //Rev2.1 Download switch enable

    //! The following supply voltages can be controlled by the user
    disableALDO1();
    disableDC5();

    //! The following power supplies are unavailable
    disableDC2();
    disableDC4();
    disableCPUSLDO();
    disableDLDO2();

    setDC3Voltage(3400);
    setALDO2Voltage(3300);
    setALDO4Voltage(3300);
    setBLDO1Voltage(2000);
    setALDO3Voltage(3300);
    setBLDO2Voltage(3300);
    // There is no need to set the voltage because DLDO1 is just a switch and cannot adjust the voltage.
    // setDLDO1Voltage(3300); Invalid

    // External use
    setALDO1Voltage(3300);
    setDC5Voltage(3300);



    // Enable internal ADC detection
    enableBattDetection();
    enableVbusVoltageMeasure();
    enableBattVoltageMeasure();
    enableSystemVoltageMeasure();
    enableTemperatureMeasure();

    // Disable all interrupts
    disableIRQ(XPOWERS_AXP2101_ALL_IRQ);
    // Clear all interrupt flags
    clearIrqStatus();
    // Enable the required interrupt function
    enableIRQ(
        XPOWERS_AXP2101_BAT_INSERT_IRQ    | XPOWERS_AXP2101_BAT_REMOVE_IRQ      |   //BATTERY
        XPOWERS_AXP2101_VBUS_INSERT_IRQ   | XPOWERS_AXP2101_VBUS_REMOVE_IRQ     |   //VBUS
        XPOWERS_AXP2101_PKEY_SHORT_IRQ    | XPOWERS_AXP2101_PKEY_LONG_IRQ       |   //POWER KEY
        XPOWERS_AXP2101_BAT_CHG_DONE_IRQ  | XPOWERS_AXP2101_BAT_CHG_START_IRQ       //CHARGE
    );

    pinMode(PMU_IRQ, INPUT_PULLUP);
    attachInterrupt(PMU_IRQ, setFlag, FALLING);

    return true;
}

bool TWRClass::initPreferences()
{
    if (!prefs.begin(SYS_KEY)) {
        log_e("NVS init failed");
        return false;
    }
    size_t schLen = prefs.getBytes(SYS_KEY, &this->pdat, sizeof(this->pdat));
    if (schLen == 0) { // simple check that data fits
        log_e("Data is not correct size!");
        this->pdat.beep   = 0;
        this->pdat.ble   = 0;
        this->pdat.wifi  = 0;
        this->pdat.dispBrightness   = 120;
        this->pdat.pixelBrightness  = 50;
        size_t write = prefs.putBytes(SYS_KEY, &(this->pdat), sizeof(this->pdat));
        if (write != sizeof(this->pdat)) {
            log_e("Write database failed!");
        } else {
            log_i("Write database succeeded!");
        }
    }
    log_i("BEEP      :%u", this->pdat.beep);
    log_i("BLE       :%u", this->pdat.ble);
    log_i("WIFI      :%u", this->pdat.wifi);
    log_i("DISPLAY B :%u", this->pdat.dispBrightness);
    log_i("PIXELS  B :%u", this->pdat.pixelBrightness);

    // There is no startup detection of wifi and Bluetooth, so turn it off here.
    // If you add it later, you can comment it out here.
    this->pdat.ble   = 0;
    this->pdat.wifi  = 0;
    return true;
}


volatile bool TWRClass::_isChargeStart = false;
volatile bool TWRClass::_isChargeDone = false;
volatile bool TWRClass::interruptFlag = false;
volatile bool TWRClass::isReceiving = false;

TWRClass twr;
