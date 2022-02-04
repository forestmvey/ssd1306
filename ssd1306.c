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

#include "ssd1306.h"
#include <pico/binary_info.h>
#include <pico/stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const uint8_t DISPLAY_RESET[] = {
        CMD_SET_CMD_BYTE,
        CMD_SET_COL_ADDR,
        0x0,
        0x7f,
        CMD_SET_PAGE_ADDR,
        0x0,
        0x7
};

const uint8_t setup_horizontal[] = {
        CMD_DISPLAY_OFF,
        CMD_MEMORYMODE,
        CMD_MEMORYMODE_HORZONTAL,
        CMD_CONTRAST,
        0xFF,
        CMD_INVERTED_OFF,
        CMD_MULTIPLEX,
        63,
        CMD_DISPLAYOFFSET,
        0x00,
        CMD_DISPLAYCLOCKDIV,
        0x80,
        CMD_PRECHARGE,
        0x22,
        CMD_COMPINS,
        0x12,
        CMD_VCOMDETECT,
        0x40,
        CMD_CHARGEPUMP,
        0x14,
        CMD_DISPLAYALL_ON_RESUME,
        CMD_DISPLAY_ON
};

/**
 * brief: Sends command for configuration of oled device.
 *
 * param - data: dataset of commands.
 * param - len: size of data.
 * returns: 0 on success.
 **/
int ssd1306_send_cmd(const uint8_t data[], size_t len)
{
    uint8_t *out_data = malloc(len + 1);
    // out_data[0] denotes cmd when addressed 0x00
    out_data[0] = CMD_SET_CMD_BYTE;
    memcpy(out_data + 1, data, len);

    if(!m_is_init)
    {
        printf("ssd1306 is not initialized\n");
        return -1;
    }
    i2c_write_blocking(i2c_default, m_oled_id, out_data, len + 1, false);
    free(out_data);
    return 0;
}

/**
 * brief: Send bitmap of pixels to be displayed on oled.
 *
 * param - data: Bitmap of 64 x 128 pixel structure of oled. 
 * returns: 0 on success.
 **/
int ssd1306_send_data(const uint8_t data[PAGE_MAX][COLUMN_MAX])
{
    int out_size = PAGE_MAX * COLUMN_MAX + 1;
    uint8_t *out_data = malloc(out_size);

    if(!m_is_init)
    {
        printf("ssd1306 is not initialized\n");
        return -1;
    }

    i2c_write_blocking(i2c_default, m_oled_id, DISPLAY_RESET, (sizeof(DISPLAY_RESET) / sizeof(DISPLAY_RESET[0])), false);
    // out_data[0] denotes data when addressed 0x40
    out_data[0] = CMD_SET_DISPLAY_START;
    for (int i = 0; i < PAGE_MAX; i++) {
        for (int k = 0; k < COLUMN_MAX; k++) {
            out_data[1 + (i * COLUMN_MAX) + k] = data[i][k];
        }
    }
    i2c_write_blocking(i2c_default, m_oled_id, out_data, out_size, false);
    free(out_data);
    return 0;
}

/**
 * brief: Set oled id address.
 *
 * param - id: Probe address of oled.
 **/
void ssd1306_set_id(const uint8_t id)
{
    m_oled_id = id;
}

/**
 * brief: Checks if address is reserved. Addresses of the form 000 0xxx or 111 1xxx are reserved.
 *
 * param - addr: Address to check if is reserved.
 **/
bool ssd1306_addr_reserved(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

/**
 * brief: Initialize communication with valid probe address.
 **/
int ssd1306_init()
{
    stdio_init_all();
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
    m_is_init = false;

    for(size_t addr = 0; addr < (1 << 7); ++addr) {
        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.
 
        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if(ssd1306_addr_reserved(addr))
        {
            ret = PICO_ERROR_GENERIC;
        }
        else
        {
            ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);
        }
 
        if(ret > 0)
        {
            ssd1306_set_id(addr);
            m_is_init = true;
            return 0;
        }
    }

    printf("Failed to initialize oled on any probe addresses\n");
    return -1;
}

/**
 * brief: Setup the oled device in horizontal mode.
 *
 * returns: 0 on success.
 **/
int ssd1306_setup_horizontal()
{
    if(!m_is_init)
    {
        printf("ssd1306 is not initialized\n");
        return -1;
    }
    ssd1306_send_cmd(setup_horizontal, (sizeof(setup_horizontal) / sizeof(setup_horizontal[0])));      
    return 0;
}