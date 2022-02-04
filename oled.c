/**********************************************
 * Module: oled
 * File:   oled.c
 * Creator: Forest Vey
 *
 * Description: Simple application for ssd1306 64x128
 * oled device. Purpose is to provide a developer with
 * example code on how the ssd1306 pico driver can be
 * used within an application.
 *---------------------------------------------
 *  02/03/2022 | Forest Vey | Initial Commit  
 *---------------------------------------------
 */ 

#include "ssd1306.h"
#include "oled.h"
#include <hardware/i2c.h>
#include <pico/stdlib.h>
#include <pico/binary_info.h>
#include <string.h>
#include <stdio.h>

/**
 * brief: Initialize oled display and populate dataset.
 *
 **/
int main() {
    int ret = ssd1306_init();
    // Failed to initialize ssd1306 communication
    if(ret != 0)
    {
        return -1;
    }

    ssd1306_setup_horizontal();
    uint8_t initial_test_data[8][128] = {
        {0x7E,0x12,0x12,0x7E}, // A
        {0x7E, 0x42, 0x5A, 0x52, 0x5E}, //@
        {0x5E}, // !
        {0x02,0x00,0x02}, // ”
        {0x24,0x7E,0x24,0x7E,0x24}, // #
        {0x44,0x4A,0x7E,0x52,0x22}, // $
        {0x22,0x10,0x08,0x04,0x42}, // %
        {0x34,0x4A,0x54,0x20,0x40}, // &
    };
    ssd1306_send_data(initial_test_data);
}