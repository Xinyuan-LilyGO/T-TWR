#pragma once

/**
 * UHF band: 400 ~ 480 MHz
 */
 #define UHF_BAND 1

/**
 * 350 band: 320 ~ 400MHz
 */
// #define _350_BAND 1

/**
 * VHF band: 134 ~ 174MHz
 */
// #define VHF_BAND 1

#if UHF_BAND
#define MIN_FREQ (400 * 1000 * 1000)
#define MAX_FREQ (400 * 1000 * 1000)
#define FREQ_RANGE (80)
#define DEFAULT_TRANSFREQ MIN_FREQ
#elif _350_BAND
#define MIN_FREQ (320 * 1000 * 1000)
#define MAX_FREQ (400 * 1000 * 1000)
#define FREQ_RANGE (80)
#define DEFAULT_TRANSFREQ MIN_FREQ
#elif VHF_BAND
#define MIN_FREQ (134 * 1000 * 1000)
#define MAX_FREQ (174 * 1000 * 1000)
#define FREQ_RANGE (40)
#define DEFAULT_TRANSFREQ MIN_FREQ
#else
#error "Please select a frequency band(UHF_BAND \ _350_BAND \ VHF_BAND)"
#endif


#define CONFIG_DATA_DEFAULT_BANDWIDTH (false)
#define CONFIG_DATA_DEFAULT_TRANSFREQ DEFAULT_TRANSFREQ
#define CONFIG_DATA_DEFAULT_RECVFREQ  DEFAULT_TRANSFREQ
#define CONFIG_DATA_DEFAULT_TXCXCSS   (0)
#define CONFIG_DATA_DEFAULT_SQ        (4)
#define CONFIG_DATA_DEFAULT_RXCXCSS   (0)
#define CONFIG_DATA_DEFAULT_VOLUME    (4)
#define CONFIG_DATA_DEFAULT_EMPHASIS  (true)
#define CONFIG_DATA_DEFAULT_HIGHPASS  (true)
#define CONFIG_DATA_DEFAULT_LOWPASS   (true)
