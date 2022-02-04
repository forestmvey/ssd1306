/**********************************************
 * Module: ssd1306
 * File:   ssd1306.c
 * Creator: Forest Vey
 *
 * Description: This driver program supports the
 * ssd1306 64x128 display for use with the raspberry
 * pi pico. The oled display will support display text within
 * quadrants with variable select sizes.
 *---------------------------------------------
 *  02/03/2022 | Forest Vey | Initial Commit  
 *---------------------------------------------
 */ 

#pragma once

#ifndef SSD1306_COMMANDS
#define SSD1306_COMMANDS

#include <hardware/i2c.h>

#define PAGE_MAX 8
#define COLUMN_MAX 128

/**
 * Types
 **/

enum BASE_CMD_ADDRS
{
    CMD_CONTRAST              = 0x81,
    CMD_DISPLAYALL_ON_RESUME  = 0xA4,
    CMD_DISPLAYALL_ON         = 0xA5,
    CMD_INVERTED_OFF          = 0xA6,
    CMD_INVERTED_ON           = 0xA7,
    CMD_DISPLAY_OFF           = 0xAE,
    CMD_DISPLAY_ON            = 0xAF,
    CMD_DISPLAYOFFSET         = 0xD3,
    CMD_COMPINS               = 0xDA,
    CMD_VCOMDETECT            = 0xDB,
    CMD_DISPLAYCLOCKDIV       = 0xD5,
    CMD_PRECHARGE             = 0xD9,
    CMD_MULTIPLEX             = 0xA8,
    CMD_LOWCOLUMN             = 0x00,
    CMD_HIGHCOLUMN            = 0x10,
    CMD_STARTLINE             = 0x40,
    CMD_MEMORYMODE            = 0x20,
    CMD_MEMORYMODE_HORZONTAL  = 0x00,
    CMD_MEMORYMODE_VERTICAL   = 0x01,
    CMD_MEMORYMODE_PAGE       = 0x10,
    CMD_COLUMNADDR            = 0x21,
    CMD_PAGEADDR              = 0x22,
    CMD_COM_REMAP_OFF         = 0xC0,
    CMD_COM_REMAP_ON          = 0xC8,
    CMD_CLUMN_REMAP_OFF       = 0xA0,
    CMD_CLUMN_REMAP_ON        = 0xA1,
    CMD_CHARGEPUMP            = 0x8D,
    CMD_EXTERNALVCC           = 0x1,
    CMD_SWITCHCAPVCC          = 0x2,
    CMD_SET_CHARGE_PUMP       = 0x8D,
};

enum SCROLL_CMD_ADDRS
{
    CMD_SET_LOW_COL           = 0x0,  // Page addressing mode only
    CMD_SET_HI_COL            = 0x10, // Page addressing mode only
    CMD_SET_MEM_MODE          = 0x20, // 2 bytes; 0b - horizontal;  1b - vertical; 10b page addressing mode
    CMD_SET_COL_ADDR          = 0x21, // 3 bytes; 0 - 127d start; 0 - 127d end
    CMD_SET_PAGE_ADDR         = 0x22, // 3 bytes; 0 - 7d start; 0 - 7d end
    CMD_SET_PAGE_START        = 0xB0, // | 0-7d page addressing mode
};

enum HARDWARE_CMD_ADDRS
{
    CMD_SET_CMD_BYTE          = 0x00, // Denotes configuration cmd in first byte
    CMD_SET_DISPLAY_START     = 0x40, // | 0-63d
    CMD_SET_SEG_REMAP         = 0xA0, // | 0 - col 0 map to seg0; 1 - col 127 mapped to seg0
    CMD_SET_MULTIPLEX_RATIO   = 0xA8, // 2 bytes; 15-63
};

/**
 * Functions
 **/

int ssd1306_send_cmd(const uint8_t commands[], size_t len);
int ssd1306_send_data(const uint8_t buffer[PAGE_MAX][COLUMN_MAX]);
void ssd1306_set_id(const uint8_t id);
int ssd1306_init();
int ssd1306_setup_horizontal();

/**
 * Variables
 **/

    uint8_t m_oled_id;          /** Address for communication with oled device. **/
    bool m_is_init;             /** State of ssd1306 initialization. **/

#endif