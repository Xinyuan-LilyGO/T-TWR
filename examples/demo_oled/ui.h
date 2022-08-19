#include <stdint.h>

void feedRSSI(int rssi);
void feedVolume(uint8_t volume);
void feedElectricity(double electricity);

void feedMainPage();
void feedTransmitStatus(bool status);
void feedSettingsMenu(bool select);
void feedFilterMenu(bool select);

void feedSettingsPage();
void feedSettingsPagebarBandWidth(bool select);
void feedSettingsPagebarBandWidthPage();
void feedSettingsPagebarBandWidthPageBar1(bool select/*, bool enable*/);
void feedSettingsPagebarBandWidthPageBar2(bool select/*, bool enable*/);
void feedSettingsPagebarTransFreq(bool select);
void feedSettingsPagebarTransFreqPage();
void feedSettingsPagebarTransFreqPageCXCSSList1(long long freq, double bandwidth, double minFreq, double maxFreq);
void feedSettingsPagebarTransFreqPageCXCSSList2(double freq, double bandwidth);
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
