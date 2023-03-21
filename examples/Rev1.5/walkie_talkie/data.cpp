/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include "data.h"
#include "config.h"

#include <ArduinoJson.h>

#include <nvs_flash.h>
#include <nvs.h>

#include <Arduino.h>
#include <stdlib.h>

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

#define WARN_TAG "W [Data]: "
#define INFO_TAG "I [Data]: "

/******************************************************************************/
/***        type definitions                                                ***/
/******************************************************************************/

/******************************************************************************/
/***        local function prototypes                                       ***/
/******************************************************************************/

/******************************************************************************/
/***        exported variables                                              ***/
/******************************************************************************/

/******************************************************************************/
/***        local variables                                                 ***/
/******************************************************************************/

/******************************************************************************/
/***        exported functions                                              ***/
/******************************************************************************/

void data_init(SA868 &sa868) {
    nvs_handle_t data_handle;
    size_t required_size = 0;
    esp_err_t err;

    err = nvs_open("data", NVS_READWRITE, &data_handle);
    if (err != ESP_OK) {
        Serial.println(F(INFO_TAG"nvs open error"));
        goto OUT;
    }

    err = nvs_get_blob(data_handle, "data", NULL, &required_size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        Serial.println(F(WARN_TAG"nvs open err"));
        goto OUT;
    }

    if (required_size == 0) {
        Serial.println(F(INFO_TAG"Nothing saved yet"));
        goto OUT;
    } else {
        uint8_t *str = (uint8_t *)malloc(required_size);
        if (!str) {
            Serial.println(F(WARN_TAG"malloc failed"));
            goto OUT;
        }
        err = nvs_get_blob(data_handle, "data", str, &required_size);
        if (err != ESP_OK) {
            Serial.println(F(WARN_TAG"Unable to get blob value"));
            free(str);
            goto OUT;
        }
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, str);
        if (error) {
            Serial.println(F(WARN_TAG"json parsing error"));
            goto OUT;
        }
        sa868.bandwidth = doc["band"]["bandwidth"].as<bool>();
        sa868.transFreq = doc["band"]["transFreq"].as<long long>();
        sa868.recvFreq  = doc["band"]["recvFreq"].as<long long>();
        sa868.txCXCSS   = doc["band"]["txCXCSS"].as<int>();
        sa868.sq        = doc["band"]["sq"].as<uint8_t>();
        sa868.rxCXCSS   = doc["band"]["rxCXCSS"].as<int>();
        sa868.volume    = doc["volume"].as<uint8_t>();
        sa868.emphasis  = doc["filter"]["emphasis"].as<bool>();
        sa868.highPass  = doc["filter"]["highPass"].as<bool>();
        sa868.lowPass   = doc["filter"]["lowPass"].as<bool>();
        free(str);
    }

    nvs_close(data_handle);
    return ;
OUT:
    Serial.println(F("Failed to restore data, use default data"));
    sa868.bandwidth = CONFIG_DATA_DEFAULT_BANDWIDTH;
    sa868.transFreq = CONFIG_DATA_DEFAULT_TRANSFREQ;
    sa868.recvFreq  = CONFIG_DATA_DEFAULT_RECVFREQ;
    sa868.txCXCSS   = CONFIG_DATA_DEFAULT_TXCXCSS;
    sa868.sq        = CONFIG_DATA_DEFAULT_SQ;
    sa868.rxCXCSS   = CONFIG_DATA_DEFAULT_RXCXCSS;
    sa868.volume    = CONFIG_DATA_DEFAULT_VOLUME;
    sa868.emphasis  = CONFIG_DATA_DEFAULT_EMPHASIS;
    sa868.highPass  = CONFIG_DATA_DEFAULT_HIGHPASS;
    sa868.lowPass   = CONFIG_DATA_DEFAULT_LOWPASS;
    nvs_close(data_handle);
    return ;
}


void data_save(SA868 &sa868) {
    nvs_handle_t data_handle;
    size_t required_size = 0;
    esp_err_t err;
    StaticJsonDocument<1024> doc;
    char str[1024] = { 0 };

    err = nvs_open("data", NVS_READWRITE, &data_handle);
    if (err != ESP_OK) {
        Serial.println(F(INFO_TAG"nvs open error"));
    }

    doc["band"]["bandwidth"]  = sa868.bandwidth;
    doc["band"]["transFreq"]  = sa868.transFreq;
    doc["band"]["recvFreq"]   = sa868.recvFreq;
    doc["band"]["txCXCSS"]    = sa868.txCXCSS;
    doc["band"]["sq"]         = sa868.sq;
    doc["band"]["rxCXCSS"]    = sa868.rxCXCSS;
    doc["volume"]             = sa868.volume;
    doc["filter"]["emphasis"] = sa868.emphasis;
    doc["filter"]["highPass"] = sa868.highPass;
    doc["filter"]["lowPass"]  = sa868.lowPass;

    serializeJson(doc, str, sizeof(str) - 1);

    nvs_set_blob(data_handle, "data", str, strlen(str));
    nvs_close(data_handle);
}

/******************************************************************************/
/***        local functions                                                 ***/
/******************************************************************************/

/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/