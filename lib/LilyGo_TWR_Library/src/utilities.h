/**
 * @file      utilities.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-01-02
 *
 */
#pragma once



#undef I2C_SDA
#undef I2C_SCL


#define SA868_TX_PIN    (39)
#define SA868_RX_PIN    (48)
#define SA868_PTT_PIN   (41)
#define SA868_PD_PIN    (40)

#define MIC_CTRL_PIN    (17)

#define BUTTON_PTT_PIN  (3)
#define BUTTON_DOWN_PIN (0)

#define ENCODER_A_PIN   (47)
#define ENCODER_B_PIN   (46)
#define ENCODER_OK_PIN  (21)

#define I2C_SDA         (8)
#define I2C_SCL         (9)
#define PMU_IRQ         (4)

#define SPI_MOSI        (11)
#define SPI_MISO        (13)
#define SPI_SCK         (12)
#define SD_CS           (10)
#define USER_CS         (14)

#define GNSS_TX_PIN     (6)
#define GNSS_RX_PIN     (5)
#define GNSS_PPS_PIN    (7)
#define GNSS_BAUD_RATE  (9600)

#define AUDIO_SELECT_PIN (17)

#define PIXELS_PIN      (42)


#define ESP2SA868_MIC   (18)
#define SA8682ESP_AUDIO (1)

// Rev2.0
#define SA868_RF_PIN    (38)


// Rev2.1
#define ESP32_PWM_TONE  (45)
#define ESP_MIC_ADC     (15)
#define SA868_SQL       (2)

#define GPSSerial        Serial2
#define RadioSerial      Serial1