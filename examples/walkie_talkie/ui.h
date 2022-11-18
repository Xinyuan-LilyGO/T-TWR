#pragma once

#include <stdint.h>

void feedRSSI(int rssi);
void feedVolume(uint8_t volume);
void feedVolumeSlider(void);
void feedElectricity(uint8_t percentage);
void feedTxCXCSS(const char *cxcss);
void feedRxCXCSS(const char *cxcss);

void feedMainPage();
void feedTransmitStatus();
void feedRecvFreq(long long freq);
void feedSettingsMenu(bool select);
void feedFilterMenu(bool select);

void feedSettingsPage();
void feedSettingsPage1(uint8_t select);
void feedSettingsPagebarBandWidth(bool select);
void feedSettingsPagebarBandWidthPage();
void feedSettingsPagebarBandWidthPageBar1(bool select/*, bool enable*/);
void feedSettingsPagebarBandWidthPageBar2(bool select/*, bool enable*/);
void feedSettingsPagebarTransFreq(bool select);
void feedSettingsPagebarTransFreqPage();
void feedSettingsPagebarTransFreqPageCXCSSList1(long long freq, int bandwidth, long long minFreq, long long maxFreq);
void feedSettingsPagebarTransFreqPageCXCSSList2(long long freq, int bandwidth);
void feedSettingsPagebarRecvFreq(bool select);
void feedSettingsPagebarRecvFreqPage();
void feedSettingsPagebarTxCXCSS(bool select);
void feedSettingsPagebarTxCXCSSPage();
void feedSettingsPagebarSQPage();
void feedSettingsPagebarSQPageSlider(int value);
void feedSettingsPagebarRxCXCSSPage();
void feedSettingsPagebarCXCSSPageCXCSSList(int index, const char list[][5], int len);
void feedSettingsPagebarSQ(bool select);
void feedSettingsPagebarRxCXCSS(bool select);


void feedFilterPage();
void feedFilterPageBar1(bool select, bool enable);
void feedFilterPageBar2(bool select, bool enable);
void feedFilterPageBar3(bool select, bool enable);
