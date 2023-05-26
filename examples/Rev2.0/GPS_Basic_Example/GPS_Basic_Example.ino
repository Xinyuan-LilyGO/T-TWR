
#define XPOWERS_CHIP_AXP2101
#include <XPowersLib.h>
#include <TinyGPS++.h>          //https://github.com/mikalhart/TinyGPSPlus
#include "utilities.h"

TinyGPSPlus gps;
XPowersPMU PMU;

void setupPower()
{
    bool result = PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, I2C_SDA, I2C_SCL);
    if (result == false) {
        while (1) {
            Serial.println("PMU is not online...");
            delay(500);
        }
    }
    //! ALDO4 GNSS VDD, Don't change
    PMU.setALDO4Voltage(3300);
    PMU.enableALDO4();
}

void setup()
{
    Serial.begin(115200);

    //GNSS Serial port
    Serial2.begin(9600, SERIAL_8N1, GNSS_RX_PIN, GNSS_TX_PIN);


    setupPower();


    Serial.println(F("BasicExample.ino"));
    Serial.println(F("Basic demonstration of TinyGPS++ (no device needed)"));
    Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
    Serial.println(F("by Mikal Hart"));
    Serial.println();

    while (Serial2.available()) {
        if (gps.encode(Serial2.read())) {
            displayInfo();
        }
    }

    Serial.println();
    Serial.println(F("Done."));
}

void loop()
{
}

void displayInfo()
{
    Serial.print(F("Location: "));
    if (gps.location.isValid()) {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.print(F("  Date/Time: "));
    if (gps.date.isValid()) {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (gps.time.isValid()) {
        if (gps.time.hour() < 10) Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10) Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10) Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10) Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.println();
}
