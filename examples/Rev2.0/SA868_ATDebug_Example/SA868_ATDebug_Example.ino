#include <Arduino.h>
#define XPOWERS_CHIP_AXP2101
#include <XPowersLib.h>
#include "utilities.h"

XPowersPMU PMU;

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT  Serial1

void setupPower()
{
    bool result = PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, I2C_SDA, I2C_SCL);
    if (result == false) {
        while (1) {
            Serial.println("PMU is not online...");
            delay(500);
        }
    }
    //! DC3 Radio Pixels VDD , Don't change
    PMU.setDC3Voltage(3400);
    PMU.enableDC3();
}


void setup()
{
    pinMode(SA868_PD_PIN, OUTPUT);
    digitalWrite(SA868_PD_PIN, HIGH);

    //! IO39 Can only be set to LOW or floating,
    //! can not be set to HIGH, otherwise it will be damaged
    pinMode(SA868_RF_PIN, OUTPUT);
    digitalWrite(SA868_RF_PIN, LOW);

    pinMode(SA868_PTT_PIN, OUTPUT);
    digitalWrite(SA868_PTT_PIN, HIGH);

    setupPower();

    SerialMon.begin(115200);

    SerialAT.begin(9600, SERIAL_8N1, SA868_RX_PIN, SA868_TX_PIN);
}


void loop()
{
    while (SerialAT.available()) {
        SerialMon.write(SerialAT.read());
    }
    while (SerialMon.available()) {
        SerialAT.write(SerialMon.read());
    }
    delay(1);
}


