/**
 * @file      ble.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-12-05
 * @note      Bluetooth has no function, just broadcasting
 */
#pragma once

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define BLE_SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BLE_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define BLE_DEVICE_NAME     "T-TWR"


class BLE
{
public:
    static bool isRunning()
    {
        return _started;
    }

    static const char *getDevName()
    {
        return BLE_DEVICE_NAME;
    }

    static void enableBLE()
    {
        if (_started) {
            return;
        }
        _started = true;
        BLEDevice::init(BLE_DEVICE_NAME);
        BLEServer *pServer = BLEDevice::createServer();
        BLEService *pService = pServer->createService(BLE_SERVICE_UUID);
        /*BLECharacteristic *pCharacteristic = */
        pService->createCharacteristic(
            BLE_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

        pService->start();
        // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
        pAdvertising->setMinPreferred(0x12);
        BLEDevice::startAdvertising();
    }

    static void disableBLE()
    {
        if (_started) {
            BLEDevice::deinit(false);
        }
        _started = false;
    }


private:
    static  bool _started ;
};
bool BLE::_started = false;