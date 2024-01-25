#ifndef FIXFFT_H
#define FIXFFT_H

#if (defined(__AVR__))
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h" /* This is where the standard Arduino code lies */
#endif

#define N_WAVE  256    /* full length of Sinewave[] */
#define LOG2_N_WAVE 8   /* log2(N_WAVE) */

/*
  Since we only use 3/4 of N_WAVE, we define only
  this many samples, in order to conserve data space.
*/

const int8_t Sinewave[N_WAVE - N_WAVE / 4] PROGMEM = {
    0, 3, 6, 9, 12, 15, 18, 21,
    24, 28, 31, 34, 37, 40, 43, 46,
    48, 51, 54, 57, 60, 63, 65, 68,
    71, 73, 76, 78, 81, 83, 85, 88,
    90, 92, 94, 96, 98, 100, 102, 104,
    106, 108, 109, 111, 112, 114, 115, 117,
    118, 119, 120, 121, 122, 123, 124, 124,
    125, 126, 126, 127, 127, 127, 127, 127,

    127, 127, 127, 127, 127, 127, 126, 126,
    125, 124, 124, 123, 122, 121, 120, 119,
    118, 117, 115, 114, 112, 111, 109, 108,
    106, 104, 102, 100, 98, 96, 94, 92,
    90, 88, 85, 83, 81, 78, 76, 73,
    71, 68, 65, 63, 60, 57, 54, 51,
    48, 46, 43, 40, 37, 34, 31, 28,
    24, 21, 18, 15, 12, 9, 6, 3,

    0, -3, -6, -9, -12, -15, -18, -21,
    -24, -28, -31, -34, -37, -40, -43, -46,
    -48, -51, -54, -57, -60, -63, -65, -68,
    -71, -73, -76, -78, -81, -83, -85, -88,
    -90, -92, -94, -96, -98, -100, -102, -104,
    -106, -108, -109, -111, -112, -114, -115, -117,
    -118, -119, -120, -121, -122, -123, -124, -124,
    -125, -126, -126, -127, -127, -127, -127, -127,

    /*-127, -127, -127, -127, -127, -127, -126, -126,
    -125, -124, -124, -123, -122, -121, -120, -119,
    -118, -117, -115, -114, -112, -111, -109, -108,
    -106, -104, -102, -100, -98, -96, -94, -92,
    -90, -88, -85, -83, -81, -78, -76, -73,
    -71, -68, -65, -63, -60, -57, -54, -51,
    -48, -46, -43, -40, -37, -34, -31, -28,
    -24, -21, -18, -15, -12, -9, -6, -3, */
};

/*
  fix_fft() - perform forward/inverse fast Fourier transform.
  fr[n],fi[n] are real and imaginary arrays, both INPUT AND
  RESULT (in-place FFT), with 0 <= n < 2**m; set inverse to
  0 for forward transform (FFT), or 1 for iFFT.
*/
int16_t fix_fft(int8_t fr[], int8_t fi[], int16_t m, int16_t inverse);

/*
  fix_fftr() - forward/inverse FFT on array of real numbers.
  Real FFT/iFFT using half-size complex FFT by distributing
  even/odd samples into real/imaginary arrays respectively.
  In order to save data space (i.e. to avoid two arrays, one
  for real, one for imaginary samples), we proceed in the
  following two steps: a) samples are rearranged in the real
  array so that all even samples are in places 0-(N/2-1) and
  all imaginary samples in places (N/2)-(N-1), and b) fix_fft
  is called with fr and fi pointing to index 0 and index N/2
  respectively in the original array. The above guarantees
  that fix_fft "sees" consecutive real samples as alternating
  real and imaginary samples in the complex array.
*/
int16_t fix_fftr(int8_t f[], int16_t m, int16_t inverse);

#endif
