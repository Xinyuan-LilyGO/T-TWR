#include "ui.h"
#include <U8g2lib.h>

#define CONFIG_DEFAULT_FONT u8g2_font_open_iconic_all_4x_t
#define CONFIG_DISP_W (128)
#define CONFIG_DISP_H (64)

// head
// https://github.com/olikraus/u8g2/wiki/fntgrpsiji
#define CONFIG_RSSI_FONT           u8g2_font_siji_t_6x10 // u8g2_font_waffle_t_all
#define CONFIG_RSSI_X              (96)
#define CONFIG_RSSI_Y              (16)
#define CONFIG_RSSI_W              (CONFIG_RSSI_MARGIN_LEFT + \
                                    CONFIG_RSSI_BORDER_LEFT + \
                                    CONFIG_RSSI_PADDING_LEFT + \
                                    CONFIG_RSSI_CONTENT_W + \
                                    CONFIG_RSSI_PADDING_RIGHT + \
                                    CONFIG_RSSI_BORDER_RIGHT + \
                                    CONFIG_RSSI_MARGIN_RIGHT)
#define CONFIG_RSSI_H              (CONFIG_RSSI_MARGIN_TOP + \
                                    CONFIG_RSSI_BORDER_TOP + \
                                    CONFIG_RSSI_PADDING_TOP + \
                                    CONFIG_RSSI_CONTENT_H + \
                                    CONFIG_RSSI_PADDING_BOTTOM + \
                                    CONFIG_RSSI_BORDER_BOTTOM + \
                                    CONFIG_RSSI_MARGIN_BOTTOM)
#define CONFIG_RSSI_MARGIN_TOP     (0) // margin top
#define CONFIG_RSSI_MARGIN_BOTTOM  (0) // margin bottom
#define CONFIG_RSSI_MARGIN_LEFT    (0) // margin left
#define CONFIG_RSSI_MARGIN_RIGHT   (0) // margin right
#define CONFIG_RSSI_BORDER_TOP     (0) // border top
#define CONFIG_RSSI_BORDER_BOTTOM  (0) // border bottom
#define CONFIG_RSSI_BORDER_LEFT    (0) // border left
#define CONFIG_RSSI_BORDER_RIGHT   (0) // border right
#define CONFIG_RSSI_PADDING_TOP    (0) // padding top
#define CONFIG_RSSI_PADDING_BOTTOM (0) // padding bottom
#define CONFIG_RSSI_PADDING_LEFT   (0) // padding left
#define CONFIG_RSSI_PADDING_RIGHT  (0) // padding right
#define CONFIG_RSSI_CONTENT_X      (CONFIG_RSSI_X + \
                                    CONFIG_RSSI_MARGIN_LEFT + \
                                    CONFIG_RSSI_BORDER_LEFT + \
                                    CONFIG_RSSI_PADDING_LEFT)
#define CONFIG_RSSI_CONTENT_Y      (CONFIG_RSSI_Y + \
                                    CONFIG_RSSI_MARGIN_TOP + \
                                    CONFIG_RSSI_BORDER_TOP + \
                                    CONFIG_RSSI_PADDING_TOP + \
                                    CONFIG_RSSI_CONTENT_H)
#define CONFIG_RSSI_CONTENT_W      (32)
#define CONFIG_RSSI_CONTENT_H      (16)

#define CONFIG_VOLUME_FONT           u8g2_font_open_iconic_all_1x_t
#define CONFIG_VOLUME_X              (CONFIG_RSSI_X + CONFIG_RSSI_W)
#define CONFIG_VOLUME_Y              (CONFIG_RSSI_Y)
#define CONFIG_VOLUME_W              (CONFIG_VOLUME_MARGIN_LEFT + \
                                      CONFIG_VOLUME_BORDER_LEFT + \
                                      CONFIG_VOLUME_PADDING_LEFT + \
                                      CONFIG_VOLUME_CONTENT_W + \
                                      CONFIG_VOLUME_PADDING_RIGHT + \
                                      CONFIG_VOLUME_BORDER_RIGHT + \
                                      CONFIG_VOLUME_MARGIN_RIGHT)
#define CONFIG_VOLUME_H              (CONFIG_VOLUME_MARGIN_TOP + \
                                      CONFIG_VOLUME_BORDER_TOP + \
                                      CONFIG_VOLUME_PADDING_TOP + \
                                      CONFIG_VOLUME_CONTENT_H + \
                                      CONFIG_VOLUME_PADDING_BOTTOM + \
                                      CONFIG_VOLUME_BORDER_BOTTOM + \
                                      CONFIG_VOLUME_MARGIN_BOTTOM)
#define CONFIG_VOLUME_MARGIN_TOP     (0) // margin top
#define CONFIG_VOLUME_MARGIN_BOTTOM  (0) // margin bottom
#define CONFIG_VOLUME_MARGIN_LEFT    (8) // margin left
#define CONFIG_VOLUME_MARGIN_RIGHT   (8) // margin right
#define CONFIG_VOLUME_BORDER_TOP     (0) // border top
#define CONFIG_VOLUME_BORDER_BOTTOM  (0) // border bottom
#define CONFIG_VOLUME_BORDER_LEFT    (0) // border left
#define CONFIG_VOLUME_BORDER_RIGHT   (0) // border right
#define CONFIG_VOLUME_PADDING_TOP    (0) // padding top
#define CONFIG_VOLUME_PADDING_BOTTOM (0) // padding bottom
#define CONFIG_VOLUME_PADDING_LEFT   (0) // padding left
#define CONFIG_VOLUME_PADDING_RIGHT  (0) // padding right
#define CONFIG_VOLUME_CONTENT_X      (CONFIG_VOLUME_X + \
                                      CONFIG_VOLUME_MARGIN_LEFT + \
                                      CONFIG_VOLUME_BORDER_LEFT + \
                                      CONFIG_VOLUME_PADDING_LEFT)
#define CONFIG_VOLUME_CONTENT_Y      (CONFIG_VOLUME_Y + \
                                      CONFIG_VOLUME_MARGIN_TOP + \
                                      CONFIG_VOLUME_BORDER_TOP + \
                                      CONFIG_VOLUME_PADDING_TOP + \
                                      CONFIG_VOLUME_CONTENT_H)
#define CONFIG_VOLUME_CONTENT_W      (16)
#define CONFIG_VOLUME_CONTENT_H      (16)

#define CONFIG_ELECTRICITY_FONT           u8g2_font_waffle_t_all
#define CONFIG_ELECTRICITY_X              (CONFIG_VOLUME_X + CONFIG_VOLUME_W)
#define CONFIG_ELECTRICITY_Y              (CONFIG_VOLUME_Y)
#define CONFIG_ELECTRICITY_W              (CONFIG_ELECTRICITY_MARGIN_LEFT + \
                                           CONFIG_ELECTRICITY_BORDER_LEFT + \
                                           CONFIG_ELECTRICITY_PADDING_LEFT + \
                                           CONFIG_ELECTRICITY_CONTENT_W + \
                                           CONFIG_ELECTRICITY_PADDING_RIGHT + \
                                           CONFIG_ELECTRICITY_BORDER_RIGHT + \
                                           CONFIG_ELECTRICITY_MARGIN_RIGHT)
#define CONFIG_ELECTRICITY_H              (CONFIG_ELECTRICITY_MARGIN_TOP + \
                                           CONFIG_ELECTRICITY_BORDER_TOP + \
                                           CONFIG_ELECTRICITY_PADDING_TOP + \
                                           CONFIG_ELECTRICITY_CONTENT_H + \
                                           CONFIG_ELECTRICITY_PADDING_BOTTOM + \
                                           CONFIG_ELECTRICITY_BORDER_BOTTOM + \
                                           CONFIG_ELECTRICITY_MARGIN_BOTTOM)
#define CONFIG_ELECTRICITY_MARGIN_TOP     (0) // margin top
#define CONFIG_ELECTRICITY_MARGIN_BOTTOM  (0) // margin bottom
#define CONFIG_ELECTRICITY_MARGIN_LEFT    (0) // margin left
#define CONFIG_ELECTRICITY_MARGIN_RIGHT   (0) // margin right
#define CONFIG_ELECTRICITY_BORDER_TOP     (0) // border top
#define CONFIG_ELECTRICITY_BORDER_BOTTOM  (0) // border bottom
#define CONFIG_ELECTRICITY_BORDER_LEFT    (0) // border left
#define CONFIG_ELECTRICITY_BORDER_RIGHT   (0) // border right
#define CONFIG_ELECTRICITY_PADDING_TOP    (0) // padding top
#define CONFIG_ELECTRICITY_PADDING_BOTTOM (0) // padding bottom
#define CONFIG_ELECTRICITY_PADDING_LEFT   (0) // padding left
#define CONFIG_ELECTRICITY_PADDING_RIGHT  (0) // padding right
#define CONFIG_ELECTRICITY_CONTENT_X      (CONFIG_ELECTRICITY_X + \
                                           CONFIG_ELECTRICITY_MARGIN_LEFT + \
                                           CONFIG_ELECTRICITY_BORDER_LEFT + \
                                           CONFIG_ELECTRICITY_PADDING_LEFT)
#define CONFIG_ELECTRICITY_CONTENT_Y      (CONFIG_ELECTRICITY_Y + \
                                           CONFIG_ELECTRICITY_MARGIN_TOP + \
                                           CONFIG_ELECTRICITY_BORDER_TOP + \
                                           CONFIG_ELECTRICITY_PADDING_TOP + \
                                           CONFIG_ELECTRICITY_CONTENT_H)
#define CONFIG_ELECTRICITY_CONTENT_W      (16)
#define CONFIG_ELECTRICITY_CONTENT_H      (16)

// main page
#define CONFIG_MAIN_PAGE_X (0)
#define CONFIG_MAIN_PAGE_Y (0)
#define CONFIG_MAIN_PAGE_W (64)
#define CONFIG_MAIN_PAGE_H (110)

#define CONFIG_MAIN_PAGE_MARGIN_TOP     (0) // margin top
#define CONFIG_MAIN_PAGE_MARGIN_BOTTOM  (0) // margin bottom
#define CONFIG_MAIN_PAGE_MARGIN_LEFT    (0) // margin left
#define CONFIG_MAIN_PAGE_MARGIN_RIGHT   (0) // margin right
#define CONFIG_MAIN_PAGE_BORDER_TOP     (0) // border top
#define CONFIG_MAIN_PAGE_BORDER_BOTTOM  (0) // border bottom
#define CONFIG_MAIN_PAGE_BORDER_LEFT    (0) // border left
#define CONFIG_MAIN_PAGE_BORDER_RIGHT   (0) // border right
#define CONFIG_MAIN_PAGE_PADDING_TOP    (0) // padding top
#define CONFIG_MAIN_PAGE_PADDING_BOTTOM (0) // padding bottom
#define CONFIG_MAIN_PAGE_PADDING_LEFT   (0) // padding left
#define CONFIG_MAIN_PAGE_PADDING_RIGHT  (0) // padding right
// content
#define CONFIG_MAIN_PAGE_CONTENT_X      (CONFIG_MAIN_PAGE_X + \
                                         CONFIG_MAIN_PAGE_MARGIN_LEFT + \
                                         CONFIG_MAIN_PAGE_BORDER_LEFT + \
                                         CONFIG_MAIN_PAGE_PADDING_LEFT)
#define CONFIG_MAIN_PAGE_CONTENT_Y      (CONFIG_MAIN_PAGE_Y + \
                                         CONFIG_MAIN_PAGE_MARGIN_TOP + \
                                         CONFIG_MAIN_PAGE_BORDER_TOP + \
                                         CONFIG_MAIN_PAGE_PADDING_TOP)
#define CONFIG_MAIN_PAGE_CONTENT_W      (CONFIG_MAIN_PAGE_W - \
                                         CONFIG_MAIN_PAGE_MARGIN_LEFT - \
                                         CONFIG_MAIN_PAGE_BORDER_LEFT - \
                                         CONFIG_MAIN_PAGE_PADDING_LEFT - \
                                         CONFIG_MAIN_PAGE_PADDING_RIGHT - \
                                         CONFIG_MAIN_PAGE_BORDER_RIGHT - \
                                         CONFIG_MAIN_PAGE_MARGIN_RIGHT)
#define CONFIG_MAIN_PAGE_CONTENT_H      (CONFIG_MAIN_PAGE_H - \
                                         CONFIG_MAIN_PAGE_MARGIN_TOP - \
                                         CONFIG_MAIN_PAGE_BORDER_TOP - \
                                         CONFIG_MAIN_PAGE_PADDING_TOP - \
                                         CONFIG_MAIN_PAGE_PADDING_BOTTOM - \
                                         CONFIG_MAIN_PAGE_BORDER_BOTTOM - \
                                         CONFIG_MAIN_PAGE_MARGIN_BOTTOM)

// transmit status
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_FONT           u8g2_font_open_iconic_all_4x_t
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_X              (32)
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_Y              (0)
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_W              (CONFIG_MAIN_PAGE_TRANSMIT_STATUS_MARGIN_LEFT + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_BORDER_LEFT + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_PADDING_LEFT + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_CONTENT_W + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_PADDING_RIGHT + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_BORDER_RIGHT + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_MARGIN_RIGHT)
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_H              (CONFIG_MAIN_PAGE_TRANSMIT_STATUS_MARGIN_TOP + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_BORDER_TOP + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_PADDING_TOP + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_CONTENT_H + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_PADDING_BOTTOM + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_BORDER_BOTTOM + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_MARGIN_BOTTOM)
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_MARGIN_TOP     (24) // margin top
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_MARGIN_BOTTOM  (0) // margin buttom
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_MARGIN_LEFT    (16) // margin left
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_MARGIN_RIGHT   (0) // margin right
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_BORDER_TOP     (0) // border top
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_BORDER_BOTTOM  (0) // border buttom
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_BORDER_LEFT    (0) // border left
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_BORDER_RIGHT   (0) // border right
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_PADDING_TOP    (0) // padding top
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_PADDING_BOTTOM (0) // padding buttom
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_PADDING_LEFT   (0) // padding left
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_PADDING_RIGHT  (0) // padding right
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_CONTENT_X      (CONFIG_MAIN_PAGE_TRANSMIT_STATUS_X + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_MARGIN_LEFT +\
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_BORDER_LEFT + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_PADDING_LEFT) // content
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_CONTENT_Y      (CONFIG_MAIN_PAGE_TRANSMIT_STATUS_Y + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_MARGIN_TOP + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_BORDER_TOP + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_PADDING_TOP + \
                                                         CONFIG_MAIN_PAGE_TRANSMIT_STATUS_CONTENT_H)
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_CONTENT_W      (64)
#define CONFIG_MAIN_PAGE_TRANSMIT_STATUS_CONTENT_H      (64)

// settings menu
#define CONFIG_SETTINGS_MENU_FONT           u8g2_font_open_iconic_embedded_2x_t
#define CONFIG_SETTINGS_MENU_X              (CONFIG_MAIN_PAGE_X)
#define CONFIG_SETTINGS_MENU_Y              (CONFIG_MAIN_PAGE_Y)
#define CONFIG_SETTINGS_MENU_W              (CONFIG_SETTINGS_MENU_MARGIN_LEFT + \
                                             CONFIG_SETTINGS_MENU_BORDER_LEFT + \
                                             CONFIG_SETTINGS_MENU_PADDING_LEFT + \
                                             CONFIG_SETTINGS_MENU_CONTENT_W + \
                                             CONFIG_SETTINGS_MENU_PADDING_RIGHT + \
                                             CONFIG_SETTINGS_MENU_BORDER_RIGHT + \
                                             CONFIG_SETTINGS_MENU_MARGIN_RIGHT)
#define CONFIG_SETTINGS_MENU_H              (CONFIG_SETTINGS_MENU_MARGIN_TOP + \
                                             CONFIG_SETTINGS_MENU_BORDER_TOP + \
                                             CONFIG_SETTINGS_MENU_PADDING_TOP + \
                                             CONFIG_SETTINGS_MENU_CONTENT_H + \
                                             CONFIG_SETTINGS_MENU_PADDING_BOTTOM + \
                                             CONFIG_SETTINGS_MENU_BORDER_BOTTOM + \
                                             CONFIG_SETTINGS_MENU_MARGIN_BOTTOM)
#define CONFIG_SETTINGS_MENU_MARGIN_TOP     (0) // margin top
#define CONFIG_SETTINGS_MENU_MARGIN_BOTTOM  (0) // margin buttom
#define CONFIG_SETTINGS_MENU_MARGIN_LEFT    (0) // margin left
#define CONFIG_SETTINGS_MENU_MARGIN_RIGHT   (0) // margin right
#define CONFIG_SETTINGS_MENU_BORDER_TOP     (1) // border top
#define CONFIG_SETTINGS_MENU_BORDER_BOTTOM  (1) // border buttom
#define CONFIG_SETTINGS_MENU_BORDER_LEFT    (1) // border left
#define CONFIG_SETTINGS_MENU_BORDER_RIGHT   (1) // border right
#define CONFIG_SETTINGS_MENU_PADDING_TOP    (7) // padding top
#define CONFIG_SETTINGS_MENU_PADDING_BOTTOM (7) // padding bottom
#define CONFIG_SETTINGS_MENU_PADDING_LEFT   (7) // padding left
#define CONFIG_SETTINGS_MENU_PADDING_RIGHT  (7) // padding right
#define CONFIG_SETTINGS_MENU_CONTENT        (0x0042)
#define CONFIG_SETTINGS_MENU_CONTENT_X      (CONFIG_SETTINGS_MENU_X + \
                                             CONFIG_SETTINGS_MENU_MARGIN_LEFT + \
                                             CONFIG_SETTINGS_MENU_BORDER_LEFT + \
                                             CONFIG_SETTINGS_MENU_PADDING_LEFT)
#define CONFIG_SETTINGS_MENU_CONTENT_Y      (CONFIG_SETTINGS_MENU_Y + \
                                             CONFIG_SETTINGS_MENU_MARGIN_TOP + \
                                             CONFIG_SETTINGS_MENU_BORDER_TOP + \
                                             CONFIG_SETTINGS_MENU_PADDING_TOP + \
                                             CONFIG_SETTINGS_MENU_CONTENT_H)
#define CONFIG_SETTINGS_MENU_CONTENT_W      (16)
#define CONFIG_SETTINGS_MENU_CONTENT_H      (16)

// filter menu
#define CONFIG_FILTER_MENU_FONT           u8g2_font_open_iconic_embedded_2x_t
#define CONFIG_FILTER_MENU_X              (CONFIG_SETTINGS_MENU_X)
#define CONFIG_FILTER_MENU_Y              (CONFIG_SETTINGS_MENU_Y + CONFIG_SETTINGS_MENU_H)
#define CONFIG_FILTER_MENU_W              (CONFIG_FILTER_MENU_MARGIN_LEFT + \
                                           CONFIG_FILTER_MENU_BORDER_LEFT + \
                                           CONFIG_FILTER_MENU_PADDING_LEFT + \
                                           CONFIG_FILTER_MENU_CONTENT_W + \
                                           CONFIG_FILTER_MENU_PADDING_RIGHT + \
                                           CONFIG_FILTER_MENU_BORDER_RIGHT + \
                                           CONFIG_FILTER_MENU_MARGIN_RIGHT)
#define CONFIG_FILTER_MENU_H              (CONFIG_FILTER_MENU_MARGIN_TOP + \
                                           CONFIG_FILTER_MENU_BORDER_TOP + \
                                           CONFIG_FILTER_MENU_PADDING_TOP + \
                                           CONFIG_FILTER_MENU_CONTENT_H + \
                                           CONFIG_FILTER_MENU_PADDING_BOTTOM + \
                                           CONFIG_FILTER_MENU_BORDER_BOTTOM + \
                                           CONFIG_FILTER_MENU_MARGIN_BOTTOM)
#define CONFIG_FILTER_MENU_MARGIN_TOP     (0) // margin top
#define CONFIG_FILTER_MENU_MARGIN_BOTTOM  (0) // margin buttom
#define CONFIG_FILTER_MENU_MARGIN_LEFT    (0) // margin left
#define CONFIG_FILTER_MENU_MARGIN_RIGHT   (0) // margin right
#define CONFIG_FILTER_MENU_BORDER_TOP     (1) // border top
#define CONFIG_FILTER_MENU_BORDER_BOTTOM  (1) // border buttom
#define CONFIG_FILTER_MENU_BORDER_LEFT    (1) // border left
#define CONFIG_FILTER_MENU_BORDER_RIGHT   (1) // border right
#define CONFIG_FILTER_MENU_PADDING_TOP    (7) // padding top
#define CONFIG_FILTER_MENU_PADDING_BOTTOM (7) // padding bottom
#define CONFIG_FILTER_MENU_PADDING_LEFT   (7) // padding left
#define CONFIG_FILTER_MENU_PADDING_RIGHT  (7) // padding right
#define CONFIG_FILTER_MENU_CONTENT        (0x0046)
#define CONFIG_FILTER_MENU_CONTENT_X      (CONFIG_FILTER_MENU_X + \
                                           CONFIG_FILTER_MENU_MARGIN_LEFT + \
                                           CONFIG_FILTER_MENU_BORDER_LEFT + \
                                           CONFIG_FILTER_MENU_PADDING_LEFT)
#define CONFIG_FILTER_MENU_CONTENT_Y      (CONFIG_FILTER_MENU_Y + \
                                           CONFIG_FILTER_MENU_MARGIN_TOP + \
                                           CONFIG_FILTER_MENU_BORDER_TOP + \
                                           CONFIG_FILTER_MENU_PADDING_TOP + \
                                           CONFIG_FILTER_MENU_CONTENT_H)
#define CONFIG_FILTER_MENU_CONTENT_W      (16)
#define CONFIG_FILTER_MENU_CONTENT_H      (16)

// settings page
#define CONFIG_SETTINGS_PAGE_FONT                u8g2_font_nokiafc22_tr
#define CONFIG_SETTINGS_PAGE_X                   (0)
#define CONFIG_SETTINGS_PAGE_Y                   (0)
#define CONFIG_SETTINGS_PAGE_W                   (128)
#define CONFIG_SETTINGS_PAGE_H                   (64)

// title
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_FONT       u8g2_font_nokiafc22_tr
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_X          (0)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y          (CONFIG_SETTINGS_PAGE_Y)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W          (CONFIG_DISP_W)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_H          (CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_TOP + \
                                                       CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_TOP + \
                                                       CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_TOP + \
                                                       CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_H + \
                                                       CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM + \
                                                       CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM + \
                                                       CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_BOTTOM)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_TOP     (1)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_BOTTOM  (1)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT    (1)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT   (1)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_TOP     (0)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM  (1)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT    (0)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT   (0)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_TOP    (2)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM (2)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT   (2)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT  (2)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X         (CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y         (CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y + \
                                                       CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_TOP + \
                                                       CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_TOP + \
                                                       CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_TOP + \
                                                       CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_H)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_W         (8)
#define CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_H         (8)


// band width
#define CONFIG_SETTINGS_PAGE_BAR_BD_BOX_FONT       u8g2_font_nokiafc22_tr
#define CONFIG_SETTINGS_PAGE_BAR_BD_BOX_X          (0)
#define CONFIG_SETTINGS_PAGE_BAR_BD_BOX_Y          (CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y + CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_H)
#define CONFIG_SETTINGS_PAGE_BAR_BD_BOX_W          (CONFIG_DISP_W)
#define CONFIG_SETTINGS_PAGE_BAR_BD_BOX_H          (CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_BD_BORDER_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_BD_PADDING_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_H + \
                                                    CONFIG_SETTINGS_PAGE_BAR_BD_PADDING_BOTTOM + \
                                                    CONFIG_SETTINGS_PAGE_BAR_BD_BORDER_BOTTOM + \
                                                    CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_BOTTOM)
#define CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP     (1)
#define CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_BOTTOM  (1)
#define CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_LEFT    (1)
#define CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_RIGHT   (1)
#define CONFIG_SETTINGS_PAGE_BAR_BD_BORDER_TOP     (0)
#define CONFIG_SETTINGS_PAGE_BAR_BD_BORDER_BOTTOM  (0)
#define CONFIG_SETTINGS_PAGE_BAR_BD_BORDER_LEFT    (0)
#define CONFIG_SETTINGS_PAGE_BAR_BD_BORDER_RIGHT   (0)
#define CONFIG_SETTINGS_PAGE_BAR_BD_PADDING_TOP    (2)
#define CONFIG_SETTINGS_PAGE_BAR_BD_PADDING_BOTTOM (2)
#define CONFIG_SETTINGS_PAGE_BAR_BD_PADDING_LEFT   (2)
#define CONFIG_SETTINGS_PAGE_BAR_BD_PADDING_RIGHT  (2)
#define CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_X         (CONFIG_SETTINGS_PAGE_BAR_BD_PADDING_LEFT)
#define CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_Y         (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_Y + \
                                                    CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_BD_BORDER_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_BD_PADDING_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_H)
#define CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_W         (8)
#define CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_H         (8)

// trans freq
#define CONFIG_SETTINGS_PAGE_BAR_TF_BOX_FONT       u8g2_font_nokiafc22_tr
#define CONFIG_SETTINGS_PAGE_BAR_TF_BOX_X          (0)
#define CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y          (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_Y + CONFIG_SETTINGS_PAGE_BAR_BD_BOX_H)
#define CONFIG_SETTINGS_PAGE_BAR_TF_BOX_W          (CONFIG_DISP_W)
#define CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H          (CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_TF_BORDER_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_TF_PADDING_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_H + \
                                                    CONFIG_SETTINGS_PAGE_BAR_TF_PADDING_BOTTOM + \
                                                    CONFIG_SETTINGS_PAGE_BAR_TF_BORDER_BOTTOM + \
                                                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_BOTTOM)
#define CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP     (1)
#define CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_BOTTOM  (1)
#define CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT    (1)
#define CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_RIGHT   (1)
#define CONFIG_SETTINGS_PAGE_BAR_TF_BORDER_TOP     (0)
#define CONFIG_SETTINGS_PAGE_BAR_TF_BORDER_BOTTOM  (0)
#define CONFIG_SETTINGS_PAGE_BAR_TF_BORDER_LEFT    (0)
#define CONFIG_SETTINGS_PAGE_BAR_TF_BORDER_RIGHT   (0)
#define CONFIG_SETTINGS_PAGE_BAR_TF_PADDING_TOP    (2)
#define CONFIG_SETTINGS_PAGE_BAR_TF_PADDING_BOTTOM (2)
#define CONFIG_SETTINGS_PAGE_BAR_TF_PADDING_LEFT   (2)
#define CONFIG_SETTINGS_PAGE_BAR_TF_PADDING_RIGHT  (2)
#define CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_X         (CONFIG_SETTINGS_PAGE_BAR_TF_PADDING_LEFT)
#define CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_Y         (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y + \
                                                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_TF_BORDER_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_TF_PADDING_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_H)
#define CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_W         (8)
#define CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_H         (8)

// // trans freq page
// #define CONFIG_TRANSFREQ_PAGE_FONT           (u8g2_font_nokiafc22_tr)
// #define CONFIG_TRANSFREQ_PAGE_X              (0)
// #define CONFIG_TRANSFREQ_PAGE_Y              (18)
// #define CONFIG_TRANSFREQ_PAGE_W              (CONFIG_DISP_W - CONFIG_TRANSFREQ_PAGE_X)
// #define CONFIG_TRANSFREQ_PAGE_H              (CONFIG_DISP_H - CONFIG_TRANSFREQ_PAGE_Y)

// // trans freq title
// #define CONFIG_TRANSFREQ_PAGE_TITLE_FONT           (u8g2_font_nokiafc22_tr)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_X              (CONFIG_TRANSFREQ_PAGE_X)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_Y              (CONFIG_TRANSFREQ_PAGE_Y)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_W              (CONFIG_TRANSFREQ_PAGE_TITLE_MARGIN_LEFT + \
//                                                     CONFIG_TRANSFREQ_PAGE_TITLE_BORDER_LEFT + \
//                                                     CONFIG_TRANSFREQ_PAGE_TITLE_PADDING_LEFT + \
//                                                     CONFIG_TRANSFREQ_PAGE_TITLE_TEXT_W)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_H              ()
// #define CONFIG_TRANSFREQ_PAGE_TITLE_MARGIN_TOP     (2)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_MARGIN_BOTTOM  (2)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_MARGIN_LEFT    (2)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_MARGIN_RIGHT   (2)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_BORDER_TOP     (1)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_BORDER_BOTTOM  (1)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_BORDER_LEFT    (1)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_BORDER_RIGHT   (1)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_PADDING_TOP    (2)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_PADDING_BOTTOM (2)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_PADDING_LEFT   (2)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_PADDING_RIGHT  (2)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_TEXT_X         ()
// #define CONFIG_TRANSFREQ_PAGE_TITLE_TEXT_Y         ()
// #define CONFIG_TRANSFREQ_PAGE_TITLE_TEXT_W         (8)
// #define CONFIG_TRANSFREQ_PAGE_TITLE_TEXT_H         (8)

// recv Freq
#define CONFIG_SETTINGS_PAGE_BAR_RF_BOX_FONT       u8g2_font_nokiafc22_tr
#define CONFIG_SETTINGS_PAGE_BAR_RF_BOX_X          (0)
#define CONFIG_SETTINGS_PAGE_BAR_RF_BOX_Y          (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y + CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H)
#define CONFIG_SETTINGS_PAGE_BAR_RF_BOX_W          (CONFIG_DISP_W)
#define CONFIG_SETTINGS_PAGE_BAR_RF_BOX_H          (CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_RF_BORDER_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_RF_PADDING_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_H + \
                                                    CONFIG_SETTINGS_PAGE_BAR_RF_PADDING_BOTTOM + \
                                                    CONFIG_SETTINGS_PAGE_BAR_RF_BORDER_BOTTOM + \
                                                    CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_BOTTOM)
#define CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_TOP     (1)
#define CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_BOTTOM  (1)
#define CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_LEFT    (1)
#define CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_RIGHT   (1)
#define CONFIG_SETTINGS_PAGE_BAR_RF_BORDER_TOP     (0)
#define CONFIG_SETTINGS_PAGE_BAR_RF_BORDER_BOTTOM  (0)
#define CONFIG_SETTINGS_PAGE_BAR_RF_BORDER_LEFT    (0)
#define CONFIG_SETTINGS_PAGE_BAR_RF_BORDER_RIGHT   (0)
#define CONFIG_SETTINGS_PAGE_BAR_RF_PADDING_TOP    (2)
#define CONFIG_SETTINGS_PAGE_BAR_RF_PADDING_BOTTOM (2)
#define CONFIG_SETTINGS_PAGE_BAR_RF_PADDING_LEFT   (2)
#define CONFIG_SETTINGS_PAGE_BAR_RF_PADDING_RIGHT  (2)
#define CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_X         (CONFIG_SETTINGS_PAGE_BAR_RF_PADDING_LEFT)
#define CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_Y         (CONFIG_SETTINGS_PAGE_BAR_RF_BOX_Y + \
                                                    CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_RF_BORDER_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_RF_PADDING_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_H)
#define CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_W         (8)
#define CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_H         (8)

// TX CXCSS
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_FONT       u8g2_font_nokiafc22_tr
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_X          (0)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_Y          (CONFIG_SETTINGS_PAGE_BAR_RF_BOX_Y + CONFIG_SETTINGS_PAGE_BAR_RF_BOX_H)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_W          (CONFIG_DISP_W)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_H          (CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BORDER_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_PADDING_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_TEXT_H + \
                                                         CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_PADDING_BOTTOM + \
                                                         CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BORDER_BOTTOM + \
                                                         CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_BOTTOM)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_TOP     (1)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_BOTTOM  (1)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_LEFT    (1)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_RIGHT   (1)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BORDER_TOP     (0)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BORDER_BOTTOM  (0)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BORDER_LEFT    (0)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BORDER_RIGHT   (0)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_PADDING_TOP    (2)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_PADDING_BOTTOM (2)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_PADDING_LEFT   (2)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_PADDING_RIGHT  (2)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_TEXT_X         (CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_PADDING_LEFT)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_TEXT_Y         (CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_Y + \
                                                         CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BORDER_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_PADDING_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_TEXT_H)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_TEXT_W         (8)
#define CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_TEXT_H         (8)

// SQ
#define CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_FONT       u8g2_font_nokiafc22_tr
#define CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_X          (0)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_Y          (CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_Y + CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_H)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_W          (CONFIG_DISP_W)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_H          (CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_SQ_BORDER_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_SQ_PADDING_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_SQ_TEXT_H + \
                                                    CONFIG_SETTINGS_PAGE_BAR_SQ_PADDING_BOTTOM + \
                                                    CONFIG_SETTINGS_PAGE_BAR_SQ_BORDER_BOTTOM + \
                                                    CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_BOTTOM)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_TOP     (1)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_BOTTOM  (1)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_LEFT    (1)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_RIGHT   (1)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_BORDER_TOP     (0)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_BORDER_BOTTOM  (0)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_BORDER_LEFT    (0)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_BORDER_RIGHT   (0)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_PADDING_TOP    (2)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_PADDING_BOTTOM (2)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_PADDING_LEFT   (2)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_PADDING_RIGHT  (2)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_TEXT_X         (CONFIG_SETTINGS_PAGE_BAR_SQ_PADDING_LEFT)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_TEXT_Y         (CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_Y + \
                                                    CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_SQ_BORDER_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_SQ_PADDING_TOP + \
                                                    CONFIG_SETTINGS_PAGE_BAR_SQ_TEXT_H)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_TEXT_W         (8)
#define CONFIG_SETTINGS_PAGE_BAR_SQ_TEXT_H         (8)

// RX CXCSS
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_FONT       u8g2_font_nokiafc22_tr
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_X          (0)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_Y          (CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_Y + CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_H)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_W          (CONFIG_DISP_W)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_H          (CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BORDER_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_PADDING_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_TEXT_H + \
                                                         CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_PADDING_BOTTOM + \
                                                         CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BORDER_BOTTOM + \
                                                         CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_BOTTOM)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_TOP     (1)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_BOTTOM  (1)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_LEFT    (1)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_RIGHT   (1)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BORDER_TOP     (0)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BORDER_BOTTOM  (0)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BORDER_LEFT    (0)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BORDER_RIGHT   (0)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_PADDING_TOP    (2)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_PADDING_BOTTOM (2)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_PADDING_LEFT   (2)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_PADDING_RIGHT  (2)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_TEXT_X         (CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_PADDING_LEFT)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_TEXT_Y         (CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_Y + \
                                                         CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BORDER_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_PADDING_TOP + \
                                                         CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_TEXT_H)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_TEXT_W         (8)
#define CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_TEXT_H         (8)

// filter page
#define CONFIG_FILTER_PAGE_FONT u8g2_font_nokiafc22_tr
#define CONFIG_FILTER_PAGE_X    (0)
#define CONFIG_FILTER_PAGE_Y    (0)
#define CONFIG_FILTER_PAGE_W    (128)
#define CONFIG_FILTER_PAGE_H    (64)

// title
#define CONFIG_FILTER_PAGE_BAR_TITLE_BOX_FONT       u8g2_font_nokiafc22_tr
#define CONFIG_FILTER_PAGE_BAR_TITLE_BOX_X          (CONFIG_FILTER_PAGE_X)
#define CONFIG_FILTER_PAGE_BAR_TITLE_BOX_Y          (CONFIG_FILTER_PAGE_Y)
#define CONFIG_FILTER_PAGE_BAR_TITLE_BOX_W          (CONFIG_FILTER_PAGE_W)
#define CONFIG_FILTER_PAGE_BAR_TITLE_BOX_H          (CONFIG_FILTER_PAGE_BAR_TITLE_MARGIN_TOP + \
                                                     CONFIG_FILTER_PAGE_BAR_TITLE_BORDER_TOP + \
                                                     CONFIG_FILTER_PAGE_BAR_TITLE_PADDING_TOP + \
                                                     CONFIG_FILTER_PAGE_BAR_TITLE_TEXT_H + \
                                                     CONFIG_FILTER_PAGE_BAR_TITLE_PADDING_BOTTOM + \
                                                     CONFIG_FILTER_PAGE_BAR_TITLE_BORDER_BOTTOM + \
                                                     CONFIG_FILTER_PAGE_BAR_TITLE_MARGIN_BOTTOM)
#define CONFIG_FILTER_PAGE_BAR_TITLE_MARGIN_TOP     (1)
#define CONFIG_FILTER_PAGE_BAR_TITLE_MARGIN_BOTTOM  (1)
#define CONFIG_FILTER_PAGE_BAR_TITLE_MARGIN_LEFT    (1)
#define CONFIG_FILTER_PAGE_BAR_TITLE_MARGIN_RIGHT   (1)
#define CONFIG_FILTER_PAGE_BAR_TITLE_BORDER_TOP     (0)
#define CONFIG_FILTER_PAGE_BAR_TITLE_BORDER_BOTTOM  (1)
#define CONFIG_FILTER_PAGE_BAR_TITLE_BORDER_LEFT    (0)
#define CONFIG_FILTER_PAGE_BAR_TITLE_BORDER_RIGHT   (0)
#define CONFIG_FILTER_PAGE_BAR_TITLE_PADDING_TOP    (2)
#define CONFIG_FILTER_PAGE_BAR_TITLE_PADDING_BOTTOM (2)
#define CONFIG_FILTER_PAGE_BAR_TITLE_PADDING_LEFT   (2)
#define CONFIG_FILTER_PAGE_BAR_TITLE_PADDING_RIGHT  (2)
#define CONFIG_FILTER_PAGE_BAR_TITLE_TEXT_X         (CONFIG_FILTER_PAGE_BAR_TITLE_PADDING_LEFT)
#define CONFIG_FILTER_PAGE_BAR_TITLE_TEXT_Y         (CONFIG_FILTER_PAGE_BAR_TITLE_BOX_Y + \
                                                     CONFIG_FILTER_PAGE_BAR_TITLE_MARGIN_TOP + \
                                                     CONFIG_FILTER_PAGE_BAR_TITLE_BORDER_TOP + \
                                                     CONFIG_FILTER_PAGE_BAR_TITLE_PADDING_TOP + \
                                                     CONFIG_FILTER_PAGE_BAR_TITLE_TEXT_H)
#define CONFIG_FILTER_PAGE_BAR_TITLE_TEXT_W         (8)
#define CONFIG_FILTER_PAGE_BAR_TITLE_TEXT_H         (8)

// bar1
#define CONFIG_FILTER_PAGE_FONT                u8g2_font_nokiafc22_tr
#define CONFIG_FILTER_PAGE_BAR1_BOX_X          (0)
#define CONFIG_FILTER_PAGE_BAR1_BOX_Y          (CONFIG_FILTER_PAGE_BAR_TITLE_BOX_Y + CONFIG_FILTER_PAGE_BAR_TITLE_BOX_H)
#define CONFIG_FILTER_PAGE_BAR1_BOX_W          (CONFIG_DISP_W)
#define CONFIG_FILTER_PAGE_BAR1_BOX_H          (CONFIG_FILTER_PAGE_BAR1_MARGIN_TOP + \
                                                CONFIG_FILTER_PAGE_BAR1_BORDER_TOP + \
                                                CONFIG_FILTER_PAGE_BAR1_PADDING_TOP + \
                                                CONFIG_FILTER_PAGE_BAR1_TEXT_H + \
                                                CONFIG_FILTER_PAGE_BAR1_PADDING_BOTTOM + \
                                                CONFIG_FILTER_PAGE_BAR1_BORDER_BOTTOM + \
                                                CONFIG_FILTER_PAGE_BAR1_MARGIN_BOTTOM)
#define CONFIG_FILTER_PAGE_BAR1_MARGIN_TOP     (1)
#define CONFIG_FILTER_PAGE_BAR1_MARGIN_BOTTOM  (1)
#define CONFIG_FILTER_PAGE_BAR1_MARGIN_LEFT    (1)
#define CONFIG_FILTER_PAGE_BAR1_MARGIN_RIGHT   (1)
#define CONFIG_FILTER_PAGE_BAR1_BORDER_TOP     (0)
#define CONFIG_FILTER_PAGE_BAR1_BORDER_BOTTOM  (0)
#define CONFIG_FILTER_PAGE_BAR1_BORDER_LEFT    (0)
#define CONFIG_FILTER_PAGE_BAR1_BORDER_RIGHT   (0)
#define CONFIG_FILTER_PAGE_BAR1_PADDING_TOP    (2)
#define CONFIG_FILTER_PAGE_BAR1_PADDING_BOTTOM (2)
#define CONFIG_FILTER_PAGE_BAR1_PADDING_LEFT   (2)
#define CONFIG_FILTER_PAGE_BAR1_PADDING_RIGHT  (2)
#define CONFIG_FILTER_PAGE_BAR1_TEXT_X         (CONFIG_FILTER_PAGE_BAR1_BOX_X + \
                                                CONFIG_FILTER_PAGE_BAR1_MARGIN_LEFT + \
                                                CONFIG_FILTER_PAGE_BAR1_BORDER_LEFT + \
                                                CONFIG_FILTER_PAGE_BAR1_PADDING_LEFT)
#define CONFIG_FILTER_PAGE_BAR1_TEXT_Y         (CONFIG_FILTER_PAGE_BAR1_BOX_Y + \
                                                CONFIG_FILTER_PAGE_BAR1_MARGIN_TOP + \
                                                CONFIG_FILTER_PAGE_BAR1_BORDER_TOP + \
                                                CONFIG_FILTER_PAGE_BAR1_PADDING_TOP + \
                                                CONFIG_FILTER_PAGE_BAR1_TEXT_H)
#define CONFIG_FILTER_PAGE_BAR1_TEXT_W         (8)
#define CONFIG_FILTER_PAGE_BAR1_TEXT_H         (8)

// bar2
#define CONFIG_FILTER_PAGE_FONT                u8g2_font_nokiafc22_tr
#define CONFIG_FILTER_PAGE_BAR2_BOX_X          (0)
#define CONFIG_FILTER_PAGE_BAR2_BOX_Y          (CONFIG_FILTER_PAGE_BAR1_BOX_Y + \
                                                CONFIG_FILTER_PAGE_BAR1_BOX_H)
#define CONFIG_FILTER_PAGE_BAR2_BOX_W          (CONFIG_DISP_W)
#define CONFIG_FILTER_PAGE_BAR2_BOX_H          (CONFIG_FILTER_PAGE_BAR2_MARGIN_TOP + \
                                                CONFIG_FILTER_PAGE_BAR2_BORDER_TOP + \
                                                CONFIG_FILTER_PAGE_BAR2_PADDING_TOP + \
                                                CONFIG_FILTER_PAGE_BAR2_TEXT_H + \
                                                CONFIG_FILTER_PAGE_BAR2_PADDING_BOTTOM + \
                                                CONFIG_FILTER_PAGE_BAR2_BORDER_BOTTOM + \
                                                CONFIG_FILTER_PAGE_BAR2_MARGIN_BOTTOM)
#define CONFIG_FILTER_PAGE_BAR2_MARGIN_TOP     (1)
#define CONFIG_FILTER_PAGE_BAR2_MARGIN_BOTTOM  (1)
#define CONFIG_FILTER_PAGE_BAR2_MARGIN_LEFT    (1)
#define CONFIG_FILTER_PAGE_BAR2_MARGIN_RIGHT   (1)
#define CONFIG_FILTER_PAGE_BAR2_BORDER_TOP     (0)
#define CONFIG_FILTER_PAGE_BAR2_BORDER_BOTTOM  (0)
#define CONFIG_FILTER_PAGE_BAR2_BORDER_LEFT    (0)
#define CONFIG_FILTER_PAGE_BAR2_BORDER_RIGHT   (0)
#define CONFIG_FILTER_PAGE_BAR2_PADDING_TOP    (2)
#define CONFIG_FILTER_PAGE_BAR2_PADDING_BOTTOM (2)
#define CONFIG_FILTER_PAGE_BAR2_PADDING_LEFT   (2)
#define CONFIG_FILTER_PAGE_BAR2_PADDING_RIGHT  (2)
#define CONFIG_FILTER_PAGE_BAR2_TEXT_X         (CONFIG_FILTER_PAGE_BAR2_BOX_X + \
                                                CONFIG_FILTER_PAGE_BAR2_MARGIN_LEFT + \
                                                CONFIG_FILTER_PAGE_BAR2_BORDER_LEFT + \
                                                CONFIG_FILTER_PAGE_BAR2_PADDING_LEFT)
#define CONFIG_FILTER_PAGE_BAR2_TEXT_Y         (CONFIG_FILTER_PAGE_BAR2_BOX_Y + \
                                                CONFIG_FILTER_PAGE_BAR2_MARGIN_TOP + \
                                                CONFIG_FILTER_PAGE_BAR2_BORDER_TOP + \
                                                CONFIG_FILTER_PAGE_BAR2_PADDING_TOP + \
                                                CONFIG_FILTER_PAGE_BAR2_TEXT_H)
#define CONFIG_FILTER_PAGE_BAR2_TEXT_W         (8)
#define CONFIG_FILTER_PAGE_BAR2_TEXT_H         (8)

// bar3
#define CONFIG_FILTER_PAGE_FONT                u8g2_font_nokiafc22_tr
#define CONFIG_FILTER_PAGE_BAR3_BOX_X          (0)
#define CONFIG_FILTER_PAGE_BAR3_BOX_Y          (CONFIG_FILTER_PAGE_BAR2_BOX_Y + CONFIG_FILTER_PAGE_BAR2_BOX_H)
#define CONFIG_FILTER_PAGE_BAR3_BOX_W          (CONFIG_DISP_W)
#define CONFIG_FILTER_PAGE_BAR3_BOX_H          (CONFIG_FILTER_PAGE_BAR3_MARGIN_TOP + \
                                                CONFIG_FILTER_PAGE_BAR3_BORDER_TOP + \
                                                CONFIG_FILTER_PAGE_BAR3_PADDING_TOP + \
                                                CONFIG_FILTER_PAGE_BAR3_TEXT_H + \
                                                CONFIG_FILTER_PAGE_BAR3_PADDING_BOTTOM + \
                                                CONFIG_FILTER_PAGE_BAR3_BORDER_BOTTOM + \
                                                CONFIG_FILTER_PAGE_BAR3_MARGIN_BOTTOM)
#define CONFIG_FILTER_PAGE_BAR3_MARGIN_TOP     (1)
#define CONFIG_FILTER_PAGE_BAR3_MARGIN_BOTTOM  (1)
#define CONFIG_FILTER_PAGE_BAR3_MARGIN_LEFT    (1)
#define CONFIG_FILTER_PAGE_BAR3_MARGIN_RIGHT   (1)
#define CONFIG_FILTER_PAGE_BAR3_BORDER_TOP     (0)
#define CONFIG_FILTER_PAGE_BAR3_BORDER_BOTTOM  (0)
#define CONFIG_FILTER_PAGE_BAR3_BORDER_LEFT    (0)
#define CONFIG_FILTER_PAGE_BAR3_BORDER_RIGHT   (0)
#define CONFIG_FILTER_PAGE_BAR3_PADDING_TOP    (2)
#define CONFIG_FILTER_PAGE_BAR3_PADDING_BOTTOM (2)
#define CONFIG_FILTER_PAGE_BAR3_PADDING_LEFT   (2)
#define CONFIG_FILTER_PAGE_BAR3_PADDING_RIGHT  (2)
#define CONFIG_FILTER_PAGE_BAR3_TEXT_X         (CONFIG_FILTER_PAGE_BAR3_BOX_X + \
                                                CONFIG_FILTER_PAGE_BAR3_MARGIN_LEFT + \
                                                CONFIG_FILTER_PAGE_BAR3_BORDER_LEFT + \
                                                CONFIG_FILTER_PAGE_BAR3_PADDING_LEFT)
#define CONFIG_FILTER_PAGE_BAR3_TEXT_Y         (CONFIG_FILTER_PAGE_BAR3_BOX_Y + \
                                                CONFIG_FILTER_PAGE_BAR3_MARGIN_TOP + \
                                                CONFIG_FILTER_PAGE_BAR3_BORDER_TOP + \
                                                CONFIG_FILTER_PAGE_BAR3_PADDING_TOP + \
                                                CONFIG_FILTER_PAGE_BAR3_TEXT_H)
#define CONFIG_FILTER_PAGE_BAR3_TEXT_W         (8)
#define CONFIG_FILTER_PAGE_BAR3_TEXT_H         (8)

#define CLEAR_AREA(x, y, w, h) { \
    u8g2.setDrawColor(0x00);  \
    u8g2.drawBox(x, y, w, h); \
    u8g2.setDrawColor(0xff);  \
}

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void feedVolume(uint8_t volume) {
    CLEAR_AREA(32,
               0,
               64,
               10);
    if (volume > 8) {
        return ;
    }
    u8g2.setFont(CONFIG_VOLUME_FONT);
    u8g2.drawGlyph(32 + 7, 9, 0x00e1);
    u8g2.drawFrame(32 + 7 + 8 + 2, 2, 40, 6);
    u8g2.drawBox(32 + 7 + 8 + 2, 2, 5 * volume, 6);
}


void feedVolumeSlider(void) {
    CLEAR_AREA(32, 0, 64, 10);
}


void feedElectricity(uint8_t percentage) {
    char buf[8] = { 0 };
    CLEAR_AREA(96,
               0,
               32,
               16);
    u8g2.setFont(CONFIG_RSSI_FONT);
    if (percentage >= 60) {
        u8g2.drawGlyph(96 + 16 + 4, 16 - 2, 0xe1ff);
    } else if (percentage >= 20) {
        u8g2.drawGlyph(96 + 16 + 4, 16 - 2, 0xe1fe);
    } else {
        u8g2.drawGlyph(96 + 16 + 4, 16 - 2, 0xe1fd);
    }
    u8g2.setFont(u8g2_font_nokiafc22_tr);
    sprintf(buf, "%d%%", percentage);
    u8g2.drawStr(96, 16 - 2, buf);
}


void feedRSSI(int rssi) {
    char buf[8] = { 0 };

    CLEAR_AREA(CONFIG_RSSI_X,
               CONFIG_RSSI_Y,
               CONFIG_RSSI_W,
               CONFIG_RSSI_H);

    u8g2.setFont(CONFIG_RSSI_FONT);
    if (rssi >= 80) {
        u8g2.drawGlyph(CONFIG_RSSI_CONTENT_X + 16, CONFIG_RSSI_CONTENT_Y, 0xe261);
    } else if (rssi >= 40) {
        u8g2.drawGlyph(CONFIG_RSSI_CONTENT_X + 16, CONFIG_RSSI_CONTENT_Y, 0xe260);
    } else if (rssi >= 20) {
        u8g2.drawGlyph(CONFIG_RSSI_CONTENT_X + 16, CONFIG_RSSI_CONTENT_Y, 0xe25f);
    } else if (rssi >= 10) {
        u8g2.drawGlyph(CONFIG_RSSI_CONTENT_X + 16, CONFIG_RSSI_CONTENT_Y, 0xe25e);
    } else {
        u8g2.drawGlyph(CONFIG_RSSI_CONTENT_X + 16, CONFIG_RSSI_CONTENT_Y, 0xe25d);
    }
    u8g2.setFont(u8g2_font_nokiafc22_tr);
    sprintf(buf, "%d", rssi);
    u8g2.drawStr(96, 32 -4, buf);
}


void feedTxCXCSS(const char *cxcss) {
    CLEAR_AREA(96,
               32,
               32,
               16);
    // u8g2.setFont(u8g2_font_tenstamps_mf);
    // u8g2.drawGlyph(96 + 16, 48 + 16, 0x0054);
    u8g2.setFont(u8g2_font_nokiafc22_tr);
    u8g2.drawStr(96, 48 - 4, cxcss);
}


void feedRxCXCSS(const char *cxcss) {
    CLEAR_AREA(96,
               48,
               32,
               16);
    // u8g2.setFont(u8g2_font_tenstamps_mf);
    // u8g2.drawGlyph(96 + 16, 48 + 16, 0x0054);
    u8g2.setFont(u8g2_font_nokiafc22_tr);
    u8g2.drawStr(96, 64 - 4, cxcss);
}


void feedMainPage() {
    CLEAR_AREA(CONFIG_MAIN_PAGE_X, CONFIG_MAIN_PAGE_Y, CONFIG_MAIN_PAGE_W, CONFIG_MAIN_PAGE_H);
    // feedTransmitStatus();
    feedSettingsMenu(false);
    feedFilterMenu(false);
}


void feedTransmitStatus() {
    CLEAR_AREA(CONFIG_MAIN_PAGE_TRANSMIT_STATUS_X,
               CONFIG_MAIN_PAGE_TRANSMIT_STATUS_Y,
               64,
               64);

    u8g2.setFont(CONFIG_MAIN_PAGE_TRANSMIT_STATUS_FONT);
    u8g2.drawGlyph(CONFIG_MAIN_PAGE_TRANSMIT_STATUS_X + 16, CONFIG_MAIN_PAGE_TRANSMIT_STATUS_Y + 16 + 32, 0xdc);
}


void feedRecvFreq(long long freq) {
    char buf[20] = { 0 };
    CLEAR_AREA(CONFIG_MAIN_PAGE_TRANSMIT_STATUS_X,
               CONFIG_MAIN_PAGE_TRANSMIT_STATUS_Y,
               64,
               64);

    sprintf(buf, "%03ld", freq / (1000 * 1000));
    u8g2.setFont(u8g2_font_courB18_tf);
    int8_t height = 16 + u8g2.getAscent();
    u8g2.drawStr(CONFIG_MAIN_PAGE_TRANSMIT_STATUS_X + 7, height, buf);
    height += abs(u8g2.getDescent());

    u8g2.setFont(u8g2_font_courB12_tf);
    height += u8g2.getAscent();
    sprintf(buf, "%04d", (freq % (1000 * 1000)) / 100);
    u8g2.drawStr(CONFIG_MAIN_PAGE_TRANSMIT_STATUS_X + 8, height, buf);
}


void feedSettingsMenu(bool select) {
    u8g2.setFont(CONFIG_SETTINGS_MENU_FONT);
    CLEAR_AREA(CONFIG_SETTINGS_MENU_X,
               CONFIG_SETTINGS_MENU_Y,
               CONFIG_SETTINGS_MENU_W,
               CONFIG_SETTINGS_MENU_H);
    u8g2.drawGlyph(CONFIG_SETTINGS_MENU_CONTENT_X,
                   CONFIG_SETTINGS_MENU_CONTENT_Y,
                   CONFIG_SETTINGS_MENU_CONTENT);
    if (select) {
        u8g2.drawFrame(CONFIG_SETTINGS_MENU_X + CONFIG_SETTINGS_MENU_MARGIN_LEFT,
                       CONFIG_SETTINGS_MENU_Y + CONFIG_SETTINGS_MENU_MARGIN_TOP,
                       CONFIG_SETTINGS_MENU_W - CONFIG_SETTINGS_MENU_MARGIN_LEFT - CONFIG_SETTINGS_MENU_MARGIN_RIGHT,
                       CONFIG_SETTINGS_MENU_H - CONFIG_SETTINGS_MENU_MARGIN_TOP - CONFIG_SETTINGS_MENU_MARGIN_BOTTOM);
    }
}


void feedFilterMenu(bool select) {
    u8g2.setFont(CONFIG_FILTER_MENU_FONT);
    CLEAR_AREA(CONFIG_FILTER_MENU_X,
               CONFIG_FILTER_MENU_Y,
               CONFIG_FILTER_MENU_W,
               CONFIG_FILTER_MENU_H);
    u8g2.drawGlyph(CONFIG_FILTER_MENU_CONTENT_X,
                   CONFIG_FILTER_MENU_CONTENT_Y,
                   CONFIG_FILTER_MENU_CONTENT);
    if (select) {
        u8g2.drawFrame(CONFIG_FILTER_MENU_X + CONFIG_FILTER_MENU_MARGIN_LEFT,
                       CONFIG_FILTER_MENU_Y + CONFIG_FILTER_MENU_MARGIN_TOP,
                       CONFIG_FILTER_MENU_W - CONFIG_FILTER_MENU_MARGIN_LEFT - CONFIG_FILTER_MENU_MARGIN_RIGHT,
                       CONFIG_FILTER_MENU_H - CONFIG_FILTER_MENU_MARGIN_TOP - CONFIG_FILTER_MENU_MARGIN_BOTTOM);
    }
}

void feedSettingsPageTitle();

void feedSettingsPage() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_X,
               CONFIG_SETTINGS_PAGE_Y,
               CONFIG_SETTINGS_PAGE_W,
               CONFIG_SETTINGS_PAGE_H);

    feedSettingsPageTitle();
    feedSettingsPagebarBandWidth(false);
    feedSettingsPagebarTransFreq(false);
    feedSettingsPagebarRecvFreq(false);
    feedSettingsPagebarTxCXCSS(false);
    feedSettingsPagebarSQ(false);
    feedSettingsPagebarRxCXCSS(false);
}


const char title[][12] = {
    "Band Width",
    "Trans Freq",
    "Recv Freq",
    "TX CXCSS",
    "SQ",
    "RX CXCSS"
};


void feedSettingsPage1(uint8_t select) {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_X,
               CONFIG_SETTINGS_PAGE_Y,
               CONFIG_SETTINGS_PAGE_W,
               CONFIG_SETTINGS_PAGE_H);

    feedSettingsPageTitle();

    u8g2.setDrawColor(0xff);
    u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_BD_BOX_X + \
                    CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_LEFT),
                    (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_Y + \
                    CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP),
                    (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_LEFT),
                    (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_H - \
                    CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_BOTTOM - \
                    CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP));
    u8g2.setDrawColor(0x00);
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_Y, title[select]);

    u8g2.setDrawColor(0x00);
    u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_TF_BOX_X + \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                    (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y + \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP),
                    (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                    (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H - \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_BOTTOM - \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP));
    u8g2.setDrawColor(0xff);
    if (select + 1 > 5) {
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_Y, title[select + 1 - 6]);
    } else {
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_Y, title[select + 1]);
    }

    u8g2.setDrawColor(0x00);
    u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_RF_BOX_X + \
                    CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_LEFT),
                    (CONFIG_SETTINGS_PAGE_BAR_RF_BOX_Y + \
                    CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_TOP),
                    (CONFIG_SETTINGS_PAGE_BAR_RF_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_LEFT),
                    (CONFIG_SETTINGS_PAGE_BAR_RF_BOX_H - \
                    CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_BOTTOM - \
                    CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_TOP));
    u8g2.setDrawColor(0xff);
    if (select + 2 > 5) {
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_Y, title[select + 2 - 6]);
    } else {
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_Y, title[select + 2]);
    }

    // feedSettingsPagebarRecvFreq(false);
    // feedSettingsPagebarTxCXCSS(false);
    // feedSettingsPagebarSQ(false);
    // feedSettingsPagebarRxCXCSS(false);
}


void feedSettingsPageTitle() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_H);
    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_FONT);
    int w = u8g2.getStrWidth("Settings");
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X + \
                 (((CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT) - w) / 2),
                 CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y,
                 "Settings");
    if (CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM) {
        u8g2.drawHLine((CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT));
    }
}


void feedSettingsPagebarBandWidth(bool select) {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_BD_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_BD_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_BD_BOX_H);

    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_FONT);

    if (select) {
        u8g2.setDrawColor(0xff);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_BD_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP));
        u8g2.setDrawColor(0x00);
    } else {
        u8g2.setDrawColor(0x00);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_BD_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP));
        u8g2.setDrawColor(0xff);
    }

    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_Y, "Band Width");
}


void feedSettingsPagebarBandWidthPageTitle();

void feedSettingsPagebarBandWidthPage() {
    // feedSettingsPagebarBandWidthPageBar1();
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_X,
               CONFIG_SETTINGS_PAGE_Y,
               CONFIG_SETTINGS_PAGE_W,
               CONFIG_SETTINGS_PAGE_H);
    feedSettingsPagebarBandWidthPageTitle();
}


void feedSettingsPagebarBandWidthPageTitle() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_H);
    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_FONT);
    int w = u8g2.getStrWidth("Band Width");
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X + \
                 (((CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT) - w) / 2),
                 CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y,
                 "Band Width");
    if (CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM) {
        u8g2.drawHLine((CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT));
    }
}


void feedSettingsPagebarBandWidthPageBar1(bool select/*, bool enable*/) {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_BD_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_BD_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_BD_BOX_H);

    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_FONT);

    if (select) {
        u8g2.setDrawColor(0xff);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_BD_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP));
        u8g2.setDrawColor(0x00);
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_Y, "* 12.5K");
    } else {
        u8g2.setDrawColor(0x00);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_BD_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_BD_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_BD_MARGIN_TOP));
        u8g2.setDrawColor(0xff);
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_Y, "  12.5K");
    }

    // if (enable) {
    //     u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_Y, "* 12.5K");
    // } else {
    //     u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_Y, "  12.5K");
    // }
}


void feedSettingsPagebarBandWidthPageBar2(bool select/*, bool enable*/) {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TF_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_TF_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H);

    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_TF_BOX_FONT);

    if (select) {
        u8g2.setDrawColor(0xff);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_TF_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP));
        u8g2.setDrawColor(0x00);
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_Y, "* 25K");
    } else {
        u8g2.setDrawColor(0x00);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_TF_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP));
        u8g2.setDrawColor(0xff);
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_Y, "  25K");
    }

    // if (enable) {
    //     u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_Y, "* 25K");
    // } else {
    //     u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_Y, "  25K");
    // }
}


void feedSettingsPagebarTransFreq(bool select) {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TF_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_TF_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H);

    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_TF_BOX_FONT);

    if (select) {
        u8g2.setDrawColor(0xff);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_TF_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP));
        u8g2.setDrawColor(0x00);
    } else {
        u8g2.setDrawColor(0x00);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_TF_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP));
        u8g2.setDrawColor(0xff);
    }

    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_Y, "Trans Freq");
}


void feedSettingsPagebarTransFreqPageTitle() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_H);
    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_FONT);
    int w = u8g2.getStrWidth("Trans Freq");
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X + \
                 (((CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT) - w) / 2),
                 CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y,
                 "Trans Freq");
    if (CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM) {
        u8g2.drawHLine((CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT));
    }
}


void feedSettingsPagebarTransFreqPage() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_X,
               CONFIG_SETTINGS_PAGE_Y,
               CONFIG_SETTINGS_PAGE_W,
               CONFIG_SETTINGS_PAGE_H);
    feedSettingsPagebarTransFreqPageTitle();
}


void feedSettingsPagebarTransFreqPageCXCSSList1(long long freq, int bandwidth, long long minFreq, long long maxFreq) {
    char optionbuf[20] = { 0 };
    long long freq1 = freq / (1000 * 1000);
    long long minFreq1 = minFreq / (1000 * 1000);
    long long maxFreq1 = maxFreq / (1000 * 1000);

    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y + \
               CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM + \
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM + \
               CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_BOTTOM,
               (CONFIG_SETTINGS_PAGE_W - CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X) / 2,
               CONFIG_SETTINGS_PAGE_H - CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y);

    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_FONT);
    u8g2.setDrawColor(0xff);

    u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_TF_BOX_X + \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                 (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y + \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP),
                 (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_W - \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_RIGHT - \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT) / 2,
                 (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H - \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_BOTTOM - \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP));
    u8g2.setDrawColor(0x00);
    sprintf(optionbuf, "%03lld", freq1);
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_Y, optionbuf);

    u8g2.setDrawColor(0xff);
    if (freq1 - 1 < minFreq1) {
        sprintf(optionbuf, "%03ld", freq1 - 1 + (maxFreq1 - minFreq1));
    } else {
        sprintf(optionbuf, "%03ld", freq1 - 1);
    }
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_Y, optionbuf);

    if (freq1 + 1 < maxFreq1) {
        sprintf(optionbuf, "%03ld", freq1 + 1);
    } else {
        sprintf(optionbuf, "%03ld", freq1 + 1 - (maxFreq1 - minFreq1));
    }
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_Y, optionbuf);

}


void feedSettingsPagebarTransFreqPageCXCSSList2(long long freq, int bandwidth) {
    char optionbuf[20] = { 0 };
    // int freq1 = (int)(freq * 1000 * 10);
    int freq1 = freq % (1000 * 1000);
    // int bandwidth1 = (int)(bandwidth * 1000 * 10);

    CLEAR_AREA(64 + CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y + \
               CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM + \
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM + \
               CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_BOTTOM,
               (CONFIG_SETTINGS_PAGE_W - CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X) / 2,
               CONFIG_SETTINGS_PAGE_H - CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y);

    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_FONT);
    u8g2.setDrawColor(0xff);
    u8g2.drawBox(64 + (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_X + \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                 (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y + \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP),
                 (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_W - \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_RIGHT - \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT) / 2,
                 (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H - \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_BOTTOM - \
                  CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP));
    u8g2.setDrawColor(0x00);
    sprintf(optionbuf, "%04d", freq1 / 100);
    u8g2.drawStr(64 + CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_Y, optionbuf);

    u8g2.setDrawColor(0xff);
    if (freq1 - bandwidth < 0) {
        sprintf(optionbuf, "%04d", (freq1 - bandwidth + 1000000) / 100);
    } else {
        sprintf(optionbuf, "%04d", (freq1 - bandwidth) / 100);
    }
    u8g2.drawStr(64 + CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_Y, optionbuf);

    if (freq1 + bandwidth < 1000000) {
        sprintf(optionbuf, "%04d", (freq1 + bandwidth) / 100);
    } else {
        sprintf(optionbuf, "%04d", (freq1 + bandwidth - 1000000) / 100);
    }
    u8g2.drawStr(64 + CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_Y, optionbuf);
}


void feedSettingsPagebarRecvFreq(bool select) {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_RF_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_RF_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_RF_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_RF_BOX_H);

    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_RF_BOX_FONT);

    if (select) {
        u8g2.setDrawColor(0xff);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_RF_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_RF_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_RF_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_RF_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_TOP));
        u8g2.setDrawColor(0x00);
    } else {
        u8g2.setDrawColor(0x00);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_RF_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_RF_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_RF_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_RF_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_RF_MARGIN_TOP));
        u8g2.setDrawColor(0xff);
    }

    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_Y, "Recv Freq");
}


void feedSettingsPagebarRecvFreqPageTitle() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_H);
    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_FONT);
    int w = u8g2.getStrWidth("Recv Freq");
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X + \
                 (((CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT) - w) / 2),
                 CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y,
                 "Recv Freq");
    if (CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM) {
        u8g2.drawHLine((CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT));
    }
}

void feedSettingsPagebarRecvFreqPage() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_X,
               CONFIG_SETTINGS_PAGE_Y,
               CONFIG_SETTINGS_PAGE_W,
               CONFIG_SETTINGS_PAGE_H);
    feedSettingsPagebarRecvFreqPageTitle();
}


void feedSettingsPagebarTxCXCSS(bool select) {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_H);

    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_FONT);

    if (select) {
        u8g2.setDrawColor(0xff);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_TOP));
        u8g2.setDrawColor(0x00);
    } else {
        u8g2.setDrawColor(0x00);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_MARGIN_TOP));
        u8g2.setDrawColor(0xff);
    }

    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_TXCXCSS_TEXT_Y, "TX CXCSS");
}

void feedSettingsPagebarTxCXCSSPageTitle();

void feedSettingsPagebarTxCXCSSPage() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_X,
               CONFIG_SETTINGS_PAGE_Y,
               CONFIG_SETTINGS_PAGE_W,
               CONFIG_SETTINGS_PAGE_H);
    feedSettingsPagebarTxCXCSSPageTitle();
}


void feedSettingsPagebarTxCXCSSPageTitle() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_H);
    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_FONT);
    int w = u8g2.getStrWidth("TX CXCSS");
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X + \
                 (((CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT) - w) / 2),
                 CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y,
                 "TX CXCSS");
    if (CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM) {
        u8g2.drawHLine((CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT));
    }
}

void feedSettingsPagebarSQPageTitle();

void feedSettingsPagebarSQPage() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_X,
               CONFIG_SETTINGS_PAGE_Y,
               CONFIG_SETTINGS_PAGE_W,
               CONFIG_SETTINGS_PAGE_H);
    feedSettingsPagebarSQPageTitle();
}


void feedSettingsPagebarSQPageTitle() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_H);
    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_FONT);
    int w = u8g2.getStrWidth("SQ");
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X + \
                 (((CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT) - w) / 2),
                 CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y,
                 "SQ");
    if (CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM) {
        u8g2.drawHLine((CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT));
    }
}


void feedSettingsPagebarSQPageSlider(int value) {
    char optionbuf[20] = { 0 };
    u8g2.drawFrame(32, 32, 64, 8);
    u8g2.drawBox(32, 32, 8 * value, 8);
    sprintf(optionbuf, "%d", value);
    int w = u8g2.getStrWidth(optionbuf);
    u8g2.drawStr((128 - w) / 2, 50, optionbuf);
}


void feedSettingsPagebarRxCXCSSPageTitle();

void feedSettingsPagebarRxCXCSSPage() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_X,
               CONFIG_SETTINGS_PAGE_Y,
               CONFIG_SETTINGS_PAGE_W,
               CONFIG_SETTINGS_PAGE_H);
    feedSettingsPagebarRxCXCSSPageTitle();
}


void feedSettingsPagebarRxCXCSSPageTitle() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_H);
    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_FONT);
    int w = u8g2.getStrWidth("RX CXCSS");
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X + \
                 (((CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT) - w) / 2),
                 CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y,
                 "RX CXCSS");
    if (CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM) {
        u8g2.drawHLine((CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT));
    }
}

void feedSettingsPagebarCXCSSPageCXCSSList(int index, const char list[][5], int len) {
    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_FONT);
    u8g2.setDrawColor(0xff);
    u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_TF_BOX_X + \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                    (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_Y + \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP),
                    (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_LEFT),
                    (CONFIG_SETTINGS_PAGE_BAR_TF_BOX_H - \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_BOTTOM - \
                    CONFIG_SETTINGS_PAGE_BAR_TF_MARGIN_TOP));
    u8g2.setDrawColor(0x00);
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_TF_TEXT_Y, list[index]);
    u8g2.setDrawColor(0xff);

    if (index - 1 < 0) {
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_Y, list[len - abs(index - 1)]);
    } else {
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_BD_TEXT_Y, list[index - 1]);
    }
    if (index + 1 > 121) {
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_Y, list[index + 1 - len]);
    } else {
        u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_RF_TEXT_Y, list[index + 1]);
    }
}


void feedSettingsPagebarSQ(bool select) {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_H);

    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_FONT);

    if (select) {
        u8g2.setDrawColor(0xff);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_TOP));
        u8g2.setDrawColor(0x00);
    } else {
        u8g2.setDrawColor(0x00);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_SQ_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_SQ_MARGIN_TOP));
        u8g2.setDrawColor(0xff);
    }

    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_SQ_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_SQ_TEXT_Y, "SQ");
}


void feedSettingsPagebarRxCXCSS(bool select) {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_H);

    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_FONT);

    if (select) {
        u8g2.setDrawColor(0xff);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_TOP));
        u8g2.setDrawColor(0x00);
    } else {
        u8g2.setDrawColor(0x00);
        u8g2.drawBox((CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_X + \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_Y + \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_TOP),
                     (CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_W - \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_RIGHT - \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_LEFT),
                     (CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_BOX_H - \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_BOTTOM - \
                      CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_MARGIN_TOP));
        u8g2.setDrawColor(0xff);
    }

    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_TEXT_X, CONFIG_SETTINGS_PAGE_BAR_RXCXCSS_TEXT_Y, "RX CXCSS");
}

void feedFilterPageTitle();

void feedFilterPage() {
    u8g2.setFont(CONFIG_FILTER_PAGE_FONT);
    CLEAR_AREA(CONFIG_FILTER_PAGE_X,
               CONFIG_FILTER_PAGE_Y,
               CONFIG_FILTER_PAGE_W,
               CONFIG_FILTER_PAGE_H);
    feedFilterPageTitle();
}


void feedFilterPageTitle() {
    CLEAR_AREA(CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_X,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_Y,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W,
               CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_H);
    u8g2.setFont(CONFIG_SETTINGS_PAGE_BAR_BD_BOX_FONT);
    int w = u8g2.getStrWidth("Filter");
    u8g2.drawStr(CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X + \
                 (((CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                    CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT) - w) / 2),
                 CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y,
                 "Filter");
    if (CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_BOTTOM) {
        u8g2.drawHLine((CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_X +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_BOTTOM),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_TEXT_Y +
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT),
                       (CONFIG_SETTINGS_PAGE_BAR_TITLE_BOX_W - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_LEFT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_PADDING_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_BORDER_RIGHT - \
                        CONFIG_SETTINGS_PAGE_BAR_TITLE_MARGIN_RIGHT));
    }
}


void feedFilterPageBar1(bool select, bool enable) {
    char optionbuf[20] = { 0 };

    if (select) {
        u8g2.setDrawColor(0xff);
        u8g2.drawBox((CONFIG_FILTER_PAGE_BAR1_BOX_X + \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR1_BOX_Y + \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_TOP),
                     (CONFIG_FILTER_PAGE_BAR1_BOX_W - \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_RIGHT - \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR1_BOX_H - \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_BOTTOM - \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_TOP));
        u8g2.setDrawColor(0x00);
    } else {
        u8g2.setDrawColor(0x00);
        u8g2.drawBox((CONFIG_FILTER_PAGE_BAR1_BOX_X + \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR1_BOX_Y + \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_TOP),
                     (CONFIG_FILTER_PAGE_BAR1_BOX_W - \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_RIGHT - \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR1_BOX_H - \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_BOTTOM - \
                      CONFIG_FILTER_PAGE_BAR1_MARGIN_TOP));
        u8g2.setDrawColor(0xff);
    }

    u8g2.setFont(CONFIG_FILTER_PAGE_FONT);
    sprintf(optionbuf, "[%s] Emphasis", enable ? "*" : " ");
    u8g2.drawStr(CONFIG_FILTER_PAGE_BAR1_TEXT_X, CONFIG_FILTER_PAGE_BAR1_TEXT_Y, optionbuf);
}


void feedFilterPageBar2(bool select, bool enable) {
    char optionbuf[20] = { 0 };

    if (select) {
        u8g2.setDrawColor(0xff);
        u8g2.drawBox((CONFIG_FILTER_PAGE_BAR2_BOX_X + \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR2_BOX_Y + \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_TOP),
                     (CONFIG_FILTER_PAGE_BAR2_BOX_W - \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_RIGHT - \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR2_BOX_H - \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_BOTTOM - \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_TOP));
        u8g2.setDrawColor(0x00);
    } else {
        u8g2.setDrawColor(0x00);
        u8g2.drawBox((CONFIG_FILTER_PAGE_BAR2_BOX_X + \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR2_BOX_Y + \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_TOP),
                     (CONFIG_FILTER_PAGE_BAR2_BOX_W - \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_RIGHT - \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR2_BOX_H - \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_BOTTOM - \
                      CONFIG_FILTER_PAGE_BAR2_MARGIN_TOP));
        u8g2.setDrawColor(0xff);
    }

    u8g2.setFont(CONFIG_FILTER_PAGE_FONT);
    sprintf(optionbuf, "[%s] High Pass", enable ? "*" : " ");
    u8g2.drawStr(CONFIG_FILTER_PAGE_BAR2_TEXT_X, CONFIG_FILTER_PAGE_BAR2_TEXT_Y, optionbuf);
}


void feedFilterPageBar3(bool select, bool enable) {
    char optionbuf[20] = { 0 };

    if (select) {
        u8g2.setDrawColor(0xff);
        u8g2.drawBox((CONFIG_FILTER_PAGE_BAR3_BOX_X + \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR3_BOX_Y + \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_TOP),
                     (CONFIG_FILTER_PAGE_BAR3_BOX_W - \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_RIGHT - \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR3_BOX_H - \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_BOTTOM - \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_TOP));
        u8g2.setDrawColor(0x00);
    } else {
        u8g2.setDrawColor(0x00);
        u8g2.drawBox((CONFIG_FILTER_PAGE_BAR3_BOX_X + \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR3_BOX_Y + \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_TOP),
                     (CONFIG_FILTER_PAGE_BAR3_BOX_W - \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_RIGHT - \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_LEFT),
                     (CONFIG_FILTER_PAGE_BAR3_BOX_H - \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_BOTTOM - \
                      CONFIG_FILTER_PAGE_BAR3_MARGIN_TOP));
        u8g2.setDrawColor(0xff);
    }

    u8g2.setFont(CONFIG_FILTER_PAGE_FONT);
    sprintf(optionbuf, "[%s] Low Pass", enable ? "*" : " ");
    u8g2.drawStr(CONFIG_FILTER_PAGE_BAR3_TEXT_X, CONFIG_FILTER_PAGE_BAR3_TEXT_Y, optionbuf);
}
