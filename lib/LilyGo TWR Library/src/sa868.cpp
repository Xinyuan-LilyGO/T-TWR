/**
 * @file      sa868.cpp
 * @author    lbuque , Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-11
 *
 */
#include "sa868.h"

#define SQL_KEY     "sa868"

SA868::SA868()
{
}

SA868::~SA868()
{
}

const char *SA868::firmwareType()
{
    // TODO: OpenRTX
    return "NiceRF";
}

void SA868::setPins(uint8_t pttPin, uint8_t pdPin, uint8_t rfPin)
{
    this->transmitPin = pttPin;
    this->powerDownPin = pdPin;
    this->powerControlPin = rfPin;
}

void SA868::setRadioType(RadioType type)
{
    if (this->pdat.type != type) {
        this->pdat.type = type;

        factoryNVS();

        setGroup(this->pdat.txPower, this->pdat.transFreq, this->pdat.recvFreq, (teCXCSS)this->pdat.txCXCSS, this->pdat.SQ, (teCXCSS)this->pdat.rxCXCSS);
        setFilter(this->pdat.emphasis, this->pdat.highPass, this->pdat.lowPass);
        this->pdat.txPower ?  lowPower() : highPower() ;
    }
}

bool SA868::factoryNVS()
{
    log_e("Data is not correct size!");
    switch (this->pdat.type) {
    case SA8X8_UHF:
        this->pdat.transFreq = DEFAULT_UHF_FREQ;
        this->pdat.recvFreq  = DEFAULT_UHF_FREQ;
        this->pdat.maxFreq  = __UHF_MAX_FREQ;
        this->pdat.minFreq  = __UHF_MIN_FREQ;
        break;
    case SA8X8_VHF:
        this->pdat.transFreq = DEFAULT_VHF_FREQ;
        this->pdat.recvFreq  = DEFAULT_VHF_FREQ;
        this->pdat.maxFreq  = __VHF_MAX_FREQ;
        this->pdat.minFreq  = __VHF_MIN_FREQ;
        break;
    default:
        return false;
    }

    this->pdat.txCXCSS   = 1;
    this->pdat.rxCXCSS   = 1;
    this->pdat.SQ        = 4;
    this->pdat.volume    = 4;
    this->pdat.emphasis  = 1;
    this->pdat.highPass  = 1;
    this->pdat.lowPass   = 1;
    this->pdat.txPower     = 1;
    this->pdat.bandwidth = 12500;

    size_t write = prefs.putBytes(SQL_KEY, &(this->pdat), sizeof(this->pdat));
    if (write != sizeof(this->pdat)) {
        log_e("Write database failed!"); return false;
    } else {
        log_i("Write database succeeded!");
    }
    return true;
}

bool SA868::begin(Stream &serial, enum RadioType type)
{
    _transmitStatus = false;
    xSemaphore = xSemaphoreCreateBinary();
    unlock();

    this->serial = &serial;

    pinMode(transmitPin, OUTPUT);
    digitalWrite(transmitPin, HIGH);

    if (powerControlPin != 0xFF) {
        pinMode(powerControlPin, OUTPUT);
        digitalWrite(powerDownPin, LOW);
    }

    pinMode(powerDownPin, OUTPUT);
    digitalWrite(powerDownPin, HIGH);

    delay(1000);

    int retry = 5;
    while (retry--) {
        if (isConnected()) {
            break;
        }
        delay(100);
    }

    if (!isConnected()) {
        return false;
    }

    if (fwType == SA8X8_OPENRTX) {
        log_e("OpenRTX community firmware is not currently supported");
        return false;
    }

    // For Rev2.0 the BAND is obtained from NVS
    if (type != SA8X8_UNKONW) {
        this->pdat.type = type;
    }

    if (!prefs.begin(SQL_KEY)) {
        log_e("NVS init failed");
        return false;
    }

    size_t schLen = prefs.getBytes(SQL_KEY, &this->pdat, sizeof(this->pdat));
    if (schLen == 0) { // simple check that data fits

        // For Rev2.0, it is impossible to detect which type it belongs to, the default is UHF
        if (type == SA8X8_UNKONW) {
            this->pdat.type = SA8X8_UHF;
        }

        factoryNVS();

    }

    if (type != SA8X8_UNKONW) {
        if (this->pdat.type != type) {
            factoryNVS();
        }
    }


    log_i("RadioType:%s", this->pdat.type == SA8X8_UHF ? "UHF" : this->pdat.type == SA8X8_VHF ? "VHF" : "UNKONW");
    log_i("TransFreq:%lu", this->pdat.transFreq);
    log_i("RecvFreq :%lu", this->pdat.recvFreq);
    log_i("TX CXCSS :%u", this->pdat.txCXCSS);
    log_i("RX CXCSS :%u", this->pdat.rxCXCSS);
    log_i("BandWidth:%u", this->pdat.bandwidth);
    log_i("SQ       :%u", this->pdat.SQ);
    log_i("Volume   :%u", this->pdat.volume);
    log_i("Emphasis :%d", this->pdat.emphasis);
    log_i("HighPass :%d", this->pdat.highPass);
    log_i("LowPass  :%d", this->pdat.lowPass);
    log_i("TxPower  :[%d] %s", this->pdat.txPower, this->pdat.txPower ?   "LOW (1.6W)" : "HIGH(1.8W)");
    setGroup(this->pdat.txPower, this->pdat.transFreq, this->pdat.recvFreq, (teCXCSS)this->pdat.txCXCSS, this->pdat.SQ, (teCXCSS)this->pdat.rxCXCSS);
    setFilter(this->pdat.emphasis, this->pdat.highPass, this->pdat.lowPass);
    this->pdat.txPower ?  lowPower() : highPower() ;
    return true;
}


bool SA868::saveConfigure()
{
    size_t write = prefs.putBytes(SQL_KEY, &(this->pdat), sizeof(this->pdat));
    if (write != sizeof(this->pdat)) {
        log_e("Write database failed!");
        return false;
    } else {
        log_i("Write database succeeded!");
        return true;
    }
}

bool SA868::isConnected()
{
    lock();
    serial->println("AT+DMOCONNECT");
    if (waitResponse("+DMOCONNECT:0\r\n") == 1) {
        unlock();
        fwType = SA8X8_NICERF;
        return true;
    }
    serial->println("AT+MODEL");
    if (waitResponse("SA868S-") == 1) {
        String model = serial->readString();
        if (model.startsWith("UHF") ||
                model.startsWith("VHF")) {
            fwType = SA8X8_OPENRTX;
            return true;
        }
        unlock();
        return true;
    }
    return false;
}

void SA868::sleep()
{
    lock();
    digitalWrite(powerDownPin, LOW);
    unlock();
}

void SA868::wakeUp()
{
    lock();
    digitalWrite(powerDownPin, HIGH);
    unlock();
}

void SA868::highPower()
{
    lock();
    if (powerControlPin != 0xFF) {
        pinMode(powerControlPin, OUTPUT);
        digitalWrite(powerControlPin, HIGH);
    }
    this->pdat.txPower = 0;
    unlock();
    setGroup(this->pdat.txPower, this->pdat.transFreq, this->pdat.recvFreq, (teCXCSS)this->pdat.txCXCSS, this->pdat.SQ, (teCXCSS)this->pdat.rxCXCSS);
}

void SA868::lowPower()
{
    lock();
    if (powerControlPin != 0xFF) {
        pinMode(powerControlPin, OUTPUT);
        digitalWrite(powerControlPin, LOW);
    }
    this->pdat.txPower = 1;
    unlock();
    setGroup(this->pdat.txPower, this->pdat.transFreq, this->pdat.recvFreq, (teCXCSS)this->pdat.txCXCSS, this->pdat.SQ, (teCXCSS)this->pdat.rxCXCSS);
}

void SA868::receive()
{
    lock();
    if (!_transmitStatus) {
        unlock();
        return ;
    }
    _transmitStatus = false;
    digitalWrite(transmitPin, HIGH);
    unlock();
}

bool SA868::isTransmit()
{
    bool result = false;
    lock();
    result = _transmitStatus;
    unlock();
    return result;
}


void SA868::transmit()
{
    lock();
    if (_transmitStatus) {
        unlock();
        return ;
    }
    _transmitStatus = true;
    digitalWrite(transmitPin, LOW);
    unlock();
}


bool SA868::scanRF(double freq)
{
    lock();
    String data;
    // todo check freq
    serial->printf("S+%.4lf\r\n", freq);
    if (waitResponse(data, "\r\n") ) {
        return data.indexOf('0') != -1 ? true : false;
    } else {
        unlock();
        // timeout or error
        return false;
    }
    unlock();
}

void SA868::setTxFreq(uint32_t freq)
{
    setGroup(pdat.txPower,
             freq,
             pdat.recvFreq,
             (teCXCSS)pdat.txCXCSS,
             pdat.SQ,
             (teCXCSS)pdat.rxCXCSS
            );

}

void SA868::setTxCXCSS(uint32_t val)
{
    setGroup(pdat.txPower,
             pdat.transFreq,
             pdat.recvFreq,
             (teCXCSS)val,
             pdat.SQ,
             (teCXCSS)pdat.rxCXCSS
            );

}

void SA868::setRxFreq(uint32_t freq)
{
    setGroup(pdat.txPower,
             pdat.transFreq,
             freq,
             (teCXCSS)pdat.txCXCSS,
             pdat.SQ,
             (teCXCSS)pdat.rxCXCSS
            );

}

void SA868::setRxCXCSS(uint32_t val)
{
    setGroup(pdat.txPower,
             pdat.transFreq,
             pdat.recvFreq,
             (teCXCSS)pdat.txCXCSS,
             pdat.SQ,
             (teCXCSS)val
            );

}

void SA868::setSquelchLevel(uint8_t val)
{
    setGroup(pdat.txPower,
             pdat.transFreq,
             pdat.recvFreq,
             (teCXCSS)pdat.txCXCSS,
             val,
             (teCXCSS)pdat.rxCXCSS
            );
}


bool SA868::setGroup(bool txPower, uint32_t transFreq, uint32_t recvFreq, teCXCSS   txCXCSS, uint8_t   sq, teCXCSS   rxCXCSS)
{
    String data;
    char cmd[128] = { 0 };
    size_t index = 0;

    lock();

    if (!checkFreq(transFreq)) {
        log_e("transFreq frequency error");
        unlock();
        return false;
    }
    if (!checkFreq(recvFreq)) {
        log_e("recvFreq frequency error");
        unlock();
        return false;
    }

    if (sq > 8) {
        log_e("Background noise is out of range");
        unlock();
        return false;
    }
    //AT+DMOSETGROUP=0,415.1250,415.1250,0000,4,0000\r\n
    index += sprintf(&cmd[index], "AT+DMOSETGROUP=%d,", txPower);
    index += sprintf(&cmd[index], "%03d.", (transFreq / (1000 * 1000)));
    index += sprintf(&cmd[index], "%04d,", ((transFreq % (1000 * 1000)) / 100));
    index += sprintf(&cmd[index], "%03d.", (recvFreq / (1000 * 1000)));
    index += sprintf(&cmd[index], "%04d,", ((recvFreq % (1000 * 1000)) / 100));
    index += sprintf(&cmd[index], "%s,%d,%s\r\n", cxcss[txCXCSS], sq, cxcss[rxCXCSS]);
    log_i("%s\n", cmd);

    serial->printf(cmd);
    if (waitResponse(data, "\r\n") ) {
        if (data.indexOf('0') != -1) {
            this->pdat.transFreq = transFreq;
            this->pdat.recvFreq = recvFreq;
            this->pdat.txCXCSS = txCXCSS;
            this->pdat.SQ = sq;
            this->pdat.rxCXCSS = rxCXCSS;
            unlock();
            return true;
        }
    }
    unlock();
    // timeout or error
    return false;
}

uint8_t SA868::getVolume()
{
    uint8_t v = this->pdat.volume;
    lock();
    v =  this->pdat.volume;
    unlock();
    return v;
}

bool SA868::setVolume(uint8_t volume)
{
    lock();
    String data;
    if (volume > 8) {
        log_e("volume out of range");
        return false;
    }
    serial->printf("AT+DMOSETVOLUME=%d\r\n", volume);
    if (waitResponse(data, "\r\n")) {
        if (data.indexOf('0') != -1) {
            log_v("This volume:%u\n", this->pdat.volume);
            this->pdat.volume = volume;
            unlock();
            return true;
        }
        unlock();
        return false;
    } else {
        unlock();
        // timeout or error
        return false;
    }
}


int SA868::getRSSI()
{
    lock();
    String data;
    serial->printf("AT+RSSI?\r\n");
    if (waitResponse(data, "\r\n") ) {
        String rssi = data.substring(data.indexOf("RSSI=") + strlen("RSSI="), data.indexOf("\r\n"));
        unlock();
        return rssi.toInt();
    } else {
        unlock();
        return 0;
    }
    unlock();
}


bool SA868::enableEmpHassis(bool en)
{
    return setFilter(en, this->pdat.highPass, this->pdat.lowPass);
}


bool SA868::enableHighPass(bool en)
{
    return setFilter(this->pdat.emphasis, en, this->pdat.lowPass);
}

bool SA868::enableLowPass(bool en)
{
    return setFilter(this->pdat.emphasis, this->pdat.highPass, en);
}

bool SA868::setFilter(bool emphasis, bool highPass, bool lowPass)
{
    lock();
    String data;
    log_i("AT+SETFILTER=%d,%d,%d\r\n", !emphasis, !highPass, !lowPass);
    serial->printf("AT+SETFILTER=%d,%d,%d\r\n", !emphasis, !highPass, !lowPass);
    if (waitResponse(data, "\r\n") ) {
        if (data.indexOf('0') != -1) {
            this->pdat.emphasis = emphasis;
            this->pdat.highPass = highPass;
            this->pdat.lowPass = lowPass;
            unlock();
            return true;
        }

    }
    unlock();
    return false;
}

bool SA868::waitResponse(String rsp, uint32_t timeout)
{
    String data;
    return waitResponse(data, rsp, timeout);
}

bool SA868::waitResponse(String &data, String rsp, uint32_t timeout)
{
    uint32_t startMillis = millis();
    do {
        while (serial->available() > 0) {
            int8_t ch = serial->read();
            data += static_cast<char>(ch);
            if (rsp.length() && data.endsWith(rsp)) {
                return true;
            }
        }
    } while (millis() - startMillis < 1000);
    return false;
}

bool SA868::checkFreq(uint32_t freq)
{
    switch (this->pdat.type) {
    case SA8X8_UHF:
        if (freq < __UHF_MIN_FREQ || freq > __UHF_MAX_FREQ) {
            log_e("%lu  frequency out of range", freq);
            return false;
        }
        break;
    case SA8X8_VHF:
        if (freq < __VHF_MIN_FREQ || freq > __VHF_MAX_FREQ) {
            log_e("%lu frequency out of range", freq);
            return false;
        }
        break;
    default:
        return false;
    }
    return true;
}

SA868  radio;
