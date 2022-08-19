
#include "sa868.h"
#include <Arduino.h>

#define INFO "I [SA868]: "
#define WARN "W [SA868]: "

SA868::SA868(Stream& stream, uint8_t pttPin, uint8_t pdPin, uint8_t rfPin) {
    _stream = &stream;
    this->_pttPin = pttPin;
    this->_pdPin = pdPin;

    pinMode(this->_pttPin, OUTPUT);
    digitalWrite(this->_pttPin, HIGH);

    pinMode(this->_pdPin, OUTPUT);
    digitalWrite(this->_pdPin, LOW);
}


/**
 * destructor
 */
SA868::~SA868() {
    this->_stream->flush();
}


void SA868::begin() {
    digitalWrite(this->_pdPin, HIGH);
}


void SA868::receive() {
    if (!_transmitStatus) return ;
    _transmitStatus = false;
    digitalWrite(_pttPin, HIGH);
}


void SA868::transmit() {
    if (_transmitStatus) return ;
    _transmitStatus = true;
    digitalWrite(_pttPin, LOW);
}


bool SA868::scanRF(double freq) {
    String data;

    // todo check freq
    _stream->printf("S+%.4lf\r\n", freq);
    if (waitResponse(data) ) {
        Serial.println(INFO + data);
        return data.indexOf('0') != -1 ? true : false;
    } else {
        // timeout or error
        return false;
    }
}


bool SA868::setGroup(bool     bandwidth,
                     double   transFreq,
                     double   recvFreq,
                     teCXCSS txCXCSS,
                     uint8_t  sq,
                     teCXCSS rxCXCSS) {
    String data;

    if (!checkFreq(bandwidth, transFreq)) {
        Serial.println(WARN"transFreq frequency error");
        return false;
    }
    if (!checkFreq(bandwidth, recvFreq)) {
        Serial.println(WARN"recvFreq frequency error");
        return false;
    }

    if (sq > 8) {
        Serial.println(WARN"Background noise is out of range");
        return false;
    }

    //AT+DMOSETGROUP=0,415.1250,415.1250,0000,4,0000\r\n
    Serial.printf("AT+DMOSETGROUP=%d,%.4lf,%.4lf,%s,%d,%s\r\n",
                    bandwidth ? 1 : 0,
                    transFreq,
                    recvFreq,
                    cxcss[txCXCSS],
                    sq,
                    cxcss[rxCXCSS]);
    _stream->printf("AT+DMOSETGROUP=%d,%.4lf,%.4lf,%s,%d,%s\r\n",
                    bandwidth ? 1 : 0,
                    transFreq,
                    recvFreq,
                    cxcss[txCXCSS],
                    sq,
                    cxcss[rxCXCSS]);
    if (waitResponse(data) ) {
        Serial.println(INFO + data);
        if (data.indexOf('0') != -1) {
            this->bandwidth = bandwidth;
            this->transFreq = transFreq;
            this->recvFreq = recvFreq;
            this->txCXCSS = txCXCSS;
            this->sq = sq;
            this->rxCXCSS = rxCXCSS;
            return true;
        }
        return false;
    } else {
        // timeout or error
        return false;
    }
}


bool SA868::setVolume(uint8_t volume) {
    String data;

    if (volume < 0 && volume > 8) {
        Serial.println(WARN"volume out of range");
        return false;
    }
    _stream->printf("AT+DMOSETVOLUME=%d\r\n", volume);
    if (waitResponse(data) ) {
        Serial.println(INFO + data);
        if (data.indexOf('0') != -1) {
            this->volume = volume;
            return true;
        }
        return false;
    } else {
        // timeout or error
        return false;
    }
}


int SA868::getRSSI() {
    String data;

    _stream->printf("AT+RSSI?\r\n");
    if (waitResponse(data) ) {
        Serial.println(INFO + data);
        String rssi = data.substring(data.indexOf("RSSI=") + strlen("RSSI="), data.indexOf("\r\n"));
        this->rssi = rssi.toInt();
        return rssi.toInt();
    } else {
        // timeout or error
        return 0;
    }
}


bool SA868::setFilter(bool emphasis, bool highPass, bool lowPass) {
    String data;

    _stream->printf("AT+SETFILTER=%d,%d,%d\r\n", !emphasis, !highPass, !lowPass);
    if (waitResponse(data) ) {
        Serial.println(INFO + data);
        if (data.indexOf('0') != -1) {
            this->emphasis = emphasis;
            this->highPass = highPass;
            this->lowPass = lowPass;
            return true;
        }
        return false;
    } else {
        // timeout or error
        return false;
    }
}


bool SA868::waitResponse(String& data, String rsp, uint32_t timeout) {
    uint32_t startMillis = millis();
    do {
        while (_stream->available() > 0) {
            int8_t ch = _stream->read();
            data += static_cast<char>(ch);
            if (rsp.length() && data.endsWith(rsp)) {
                return true;
            }
        }
    } while (millis() - startMillis < 1000);
    return false;
}

bool SA868::checkFreq(bool bandwidth, double freq) {
    long long t = (long long)freq;
    int bw = bandwidth ? 250 : 125;
    t = (freq - t) * 1000;
    if ((freq < 134.0 && freq > 480.0) || (freq > 174.0 && freq < 400.0)) {
        Serial.println(WARN"frequency out of range");
        return false;
    }

    if (t % bw != 0) {
        Serial.println(WARN"frequency error");
        return false;
    }
    return true;
}
