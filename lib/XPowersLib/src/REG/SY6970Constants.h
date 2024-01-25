/**
 *
 * @license MIT License
 *
 * Copyright (c) 2022 lewis he
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      SY6970Constants.h
 * @author    Lewis He (lewishe@outlook.com)
 * @date      2023-07-20
 *
 */
#pragma once

#define SY6970_SLAVE_ADDRESS                                    (0x6A)

#define POWERS_SY6970_REG_00H                                   (0x00)
#define POWERS_SY6970_REG_01H                                   (0x01)
#define POWERS_SY6970_REG_02H                                   (0x02)
#define POWERS_SY6970_REG_03H                                   (0x03)
#define POWERS_SY6970_REG_04H                                   (0x04)
#define POWERS_SY6970_REG_05H                                   (0x05)
#define POWERS_SY6970_REG_06H                                   (0x06)
#define POWERS_SY6970_REG_07H                                   (0x07)
#define POWERS_SY6970_REG_08H                                   (0x08)
#define POWERS_SY6970_REG_09H                                   (0x09)
#define POWERS_SY6970_REG_0AH                                   (0x0A)
#define POWERS_SY6970_REG_0BH                                   (0x0B)
#define POWERS_SY6970_REG_0CH                                   (0x0C)
#define POWERS_SY6970_REG_0DH                                   (0x0D)
#define POWERS_SY6970_REG_0EH                                   (0x0E)
#define POWERS_SY6970_REG_0FH                                   (0x0F)
#define POWERS_SY6970_REG_10H                                   (0x10)
#define POWERS_SY6970_REG_11H                                   (0x11)
#define POWERS_SY6970_REG_12H                                   (0x12)
#define POWERS_SY6970_REG_13H                                   (0x13)
#define POWERS_SY6970_REG_14H                                   (0x14)

#define POWERS_SY6970_VBUS_MASK_VAL(val)                        (val & 0x7F)
#define POWERS_SY6970_VBAT_MASK_VAL(val)                        (val & 0x7F)
#define POWERS_SY6970_VSYS_MASK_VAL(val)                        (val & 0x7F)
#define POWERS_SY6970_VBUS_BASE_VAL                             (2600)
#define POWERS_SY6970_VBAT_BASE_VAL                             (2304)
#define POWERS_SY6970_VSYS_BASE_VAL                             (2304)
#define POWERS_SY6970_VBUS_VOL_STEP                             (100)
#define POWERS_SY6970_VBAT_VOL_STEP                             (20)
#define POWERS_SY6970_VSYS_VOL_STEP                             (20)

#define POWERS_SY6970_PRE_CHG_CUR_BASE                          (64)
#define POWERS_SY6970_FAST_CHG_CUR_STEP                         (64)
#define POWERS_SY6970_PRE_CHG_CUR_STEP                          (64)

#define POWERS_FAST_CHG_CURRENT_MIN                             (0)
#define POWERS_FAST_CHG_CURRENT_MAX                             (5056)

#define POWERS_PRE_CHG_CURRENT_MIN                              (64)
#define POWERS_PRE_CHG_CURRENT_MAX                              (1024)

#define POWERS_SY6970_CHG_VOL_BASE                              (3840)
#define POWERS_SY6970_CHG_VOL_STEP                              (16)
#define POWERS_FAST_CHG_VOL_MIN                                 (3840)
#define POWERS_FAST_CHG_VOL_MAX                                 (4608)

#define POWERS_SY6970_SYS_VOL_STEPS         (100)
#define POWERS_SY6970_SYS_VOFF_VOL_MIN      (3000)
#define POWERS_SY6970_SYS_VOFF_VOL_MAX      (3700)

#define POWERS_SY6970_IN_CURRENT_STEP                          (50)
#define POWERS_SY6970_IN_CURRENT_MIN                            (100)
#define POWERS_SY6970_IN_CURRENT_MAX                            (3250)
