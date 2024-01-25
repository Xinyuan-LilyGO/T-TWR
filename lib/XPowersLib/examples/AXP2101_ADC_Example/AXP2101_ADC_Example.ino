/*
MIT License

Copyright (c) 2022 lewis he

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
! WARN:
Please do not run the example without knowing the external load voltage of the PMU,
it may burn your external load, please check the voltage setting before running the example,
if there is any loss, please bear it by yourself
*/
#ifndef XPOWERS_NO_ERROR
#error "Running this example is known to not damage the device! Please go and uncomment this!"
#endif
// Defined using AXP2102
#define XPOWERS_CHIP_AXP2101

#include <Wire.h>
#include <Arduino.h>
#include "XPowersLib.h"

#ifndef CONFIG_PMU_SDA
#define CONFIG_PMU_SDA 15
#endif

#ifndef CONFIG_PMU_SCL
#define CONFIG_PMU_SCL 7
#endif

#ifndef CONFIG_PMU_IRQ
#define CONFIG_PMU_IRQ 6
#endif

XPowersPMU PMU;

const uint8_t i2c_sda = CONFIG_PMU_SDA;
const uint8_t i2c_scl = CONFIG_PMU_SCL;
const uint8_t pmu_irq_pin = CONFIG_PMU_IRQ;
bool  pmu_flag = false;
bool  adc_switch = false;

void setFlag(void)
{
    pmu_flag = true;
}


void adcOn()
{
    PMU.enableTemperatureMeasure();
    // Enable internal ADC detection
    PMU.enableBattDetection();
    PMU.enableVbusVoltageMeasure();
    PMU.enableBattVoltageMeasure();
    PMU.enableSystemVoltageMeasure();
}

void adcOff()
{
    PMU.disableTemperatureMeasure();
    // Enable internal ADC detection
    PMU.disableBattDetection();
    PMU.disableVbusVoltageMeasure();
    PMU.disableBattVoltageMeasure();
    PMU.disableSystemVoltageMeasure();
}

void setup()
{
    Serial.begin(115200);

    bool result = PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, i2c_sda, i2c_scl);

    if (result == false) {
        Serial.println("PMU is not online..."); while (1)delay(50);
    }

    // Force add pull-up
    pinMode(pmu_irq_pin, INPUT_PULLUP);
    attachInterrupt(pmu_irq_pin, setFlag, FALLING);

    PMU.disableIRQ(XPOWERS_AXP2101_ALL_IRQ);
    // Clear all interrupt flags
    PMU.clearIrqStatus();
    // Enable the required interrupt function
    PMU.enableIRQ(
        XPOWERS_AXP2101_PKEY_SHORT_IRQ    //POWER KEY
    );

    /*
      The default setting is CHGLED is automatically controlled by the PMU.
    - XPOWERS_CHG_LED_OFF,
    - XPOWERS_CHG_LED_BLINK_1HZ,
    - XPOWERS_CHG_LED_BLINK_4HZ,
    - XPOWERS_CHG_LED_ON,
    - XPOWERS_CHG_LED_CTRL_CHG,
    * */
    PMU.setChargingLedMode(XPOWERS_CHG_LED_ON);

    //Default turn on adc
    adcOn();

}




void loop()
{

    if (pmu_flag) {
        pmu_flag = false;
        // Get PMU Interrupt Status Register
        uint32_t status = PMU.getIrqStatus();
        if (PMU.isPekeyShortPressIrq()) {
            if (adc_switch) {
                adcOn(); Serial.println("Enable ADC\n\n\n");
            } else {
                adcOff(); Serial.println("Disable ADC\n\n\n");
            }
            adc_switch = !adc_switch;
        }
        // Clear PMU Interrupt Status Register
        PMU.clearIrqStatus();
    }

    Serial.print("PMU Temperature:"); Serial.print(PMU.getTemperature()); Serial.println("*C");
    Serial.print("isCharging:"); Serial.println(PMU.isCharging() ? "YES" : "NO");
    Serial.print("isDischarge:"); Serial.println(PMU.isDischarge() ? "YES" : "NO");
    Serial.print("isStandby:"); Serial.println(PMU.isStandby() ? "YES" : "NO");
    Serial.print("isVbusIn:"); Serial.println(PMU.isVbusIn() ? "YES" : "NO");
    Serial.print("isVbusGood:"); Serial.println(PMU.isVbusGood() ? "YES" : "NO");
    Serial.print("getChargerStatus:");
    uint8_t charge_status = PMU.getChargerStatus();
    if (charge_status == XPOWERS_AXP2101_CHG_TRI_STATE) {
        Serial.println("tri_charge");
    } else if (charge_status == XPOWERS_AXP2101_CHG_PRE_STATE) {
        Serial.println("pre_charge");
    } else if (charge_status == XPOWERS_AXP2101_CHG_CC_STATE) {
        Serial.println("constant charge");
    } else if (charge_status == XPOWERS_AXP2101_CHG_CV_STATE) {
        Serial.println("constant voltage");
    } else if (charge_status == XPOWERS_AXP2101_CHG_DONE_STATE) {
        Serial.println("charge done");
    } else if (charge_status == XPOWERS_AXP2101_CHG_STOP_STATE) {
        Serial.println("not chargin");
    }

    // After the ADC detection is turned off, the register will return to the last reading. The PMU will not refresh the data register
    Serial.print("getBattVoltage:"); Serial.print(PMU.getBattVoltage()); Serial.println("mV");
    Serial.print("getVbusVoltage:"); Serial.print(PMU.getVbusVoltage()); Serial.println("mV");
    Serial.print("getSystemVoltage:"); Serial.print(PMU.getSystemVoltage()); Serial.println("mV");

    // The battery percentage may be inaccurate at first use, the PMU will automatically
    // learn the battery curve and will automatically calibrate the battery percentage
    // after a charge and discharge cycle
    if (PMU.isBatteryConnect()) {
        Serial.print("getBatteryPercent:"); Serial.print(PMU.getBatteryPercent()); Serial.println("%");
    }

    // Serial.println();
    delay(1000);
}

