/*

  UpdateArea.ino

  Demonstration for the UpdateDisplayArea command

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "LilyGo_TWR.h"

/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.

*/
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);



int16_t offset;             // current offset for the scrolling text
u8g2_uint_t width;          // pixel width of the scrolling text (must be lesser than 128 unless U8G2_16BIT is defined
const char *text = "U8g2";  // scroll this text from right to left

const uint8_t tile_area_x_pos = 2;  // Update area left position (in tiles)
const uint8_t tile_area_y_pos = 3;  // Update area upper position (distance from top in tiles)
const uint8_t tile_area_width = 12;
const uint8_t tile_area_height = 3; // this will allow cour18 chars to fit into the area

const u8g2_uint_t pixel_area_x_pos = tile_area_x_pos * 8;
const u8g2_uint_t pixel_area_y_pos = tile_area_y_pos * 8;
const u8g2_uint_t pixel_area_width = tile_area_width * 8;
const u8g2_uint_t pixel_area_height = tile_area_height * 8;

void setup(void)
{
    Serial.begin(115200);

    // Initializing PMU is related to other peripherals
    twr.begin();

    if (twr.getVersion() == TWRClass::TWR_REV2V1) {
        Serial.println("Detection using TWR Rev2.1");
    } else {
        Serial.println("Detection using TWR Rev2.0");
    }

    uint8_t addr = twr.getOLEDAddress();
    while (addr == 0xFF) {
        Serial.println("OLED is not detected, please confirm whether OLED is installed normally.");
        delay(1000);
    }
    u8g2.setI2CAddress(addr << 1);
    u8g2.begin();

    u8g2.clearBuffer();                   // clear the internal memory
    u8g2.setFont(u8g2_font_helvR10_tr);   // choose a suitable font
    u8g2.drawStr(0, 12, "UpdateDisplayArea"); // write something to the internal memory

    // draw a frame, only the content within the frame will be updated
    // the frame is never drawn again, but will stay on the display
    u8g2.drawBox(pixel_area_x_pos - 1, pixel_area_y_pos - 1, pixel_area_width + 2, pixel_area_height + 2);

    u8g2.sendBuffer();                    // transfer internal memory to the display

    u8g2.setFont(u8g2_font_courB18_tr);   // set the target font for the text width calculation
    width = u8g2.getUTF8Width(text);      // calculate the pixel width of the text
    offset = width + pixel_area_width;


}

void loop(void)
{
    u8g2.clearBuffer();                       // clear the complete internal memory

    // draw the scrolling text at current offset
    u8g2.setFont(u8g2_font_courB18_tr);       // set the target font
    u8g2.drawUTF8(
        pixel_area_x_pos - width + offset,
        pixel_area_y_pos + pixel_area_height + u8g2.getDescent() - 1,
        text);                              // draw the scolling text

    // now only update the selected area, the rest of the display content is not changed
    u8g2.updateDisplayArea(tile_area_x_pos, tile_area_y_pos, tile_area_width, tile_area_height);

    offset--;                             // scroll by one pixel
    if ( offset == 0 )
        offset = width + pixel_area_width;          // start over again

    delay(10);                            // do some small delay
}

