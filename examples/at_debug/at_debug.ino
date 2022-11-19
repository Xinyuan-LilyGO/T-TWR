#include "pin.h"
#include <Arduino.h>

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT  Serial1

void setup() {
    pinMode(CONFIG_SA868_PD_PIN, OUTPUT);
    digitalWrite(CONFIG_SA868_PD_PIN, HIGH);
    delay(1000);

    pinMode(CONFIG_SA868_PTT_PIN, OUTPUT);
    digitalWrite(CONFIG_SA868_PTT_PIN, HIGH);

    pinMode(CONFIG_SA868_RF_PIN, OUTPUT);
    digitalWrite(CONFIG_SA868_RF_PIN, LOW);

    pinMode(CONFIG_BUTTON_PTT_PIN, INPUT);

    SerialMon.begin(115200);
    SerialAT.begin(9600, SERIAL_8N1, CONFIG_SA868_RX_PIN, CONFIG_SA868_TX_PIN);
    xTaskCreatePinnedToCore(buttonTask, "buttonTask", 4096, NULL, 4, NULL, ARDUINO_RUNNING_CORE);
}


void loop() {
    while (SerialAT.available()) {
        SerialMon.write(SerialAT.read());
    }
    while (SerialMon.available()) {
        SerialAT.write(SerialMon.read());
    }
    delay(1);
}


void buttonTask(void *pvParameters) {
    while (1) {
        if (digitalRead(CONFIG_BUTTON_PTT_PIN)) {
            digitalWrite(CONFIG_SA868_PTT_PIN, HIGH);
        } else {
            digitalWrite(CONFIG_SA868_PTT_PIN, LOW);
        }
        delay(10);
    }
}