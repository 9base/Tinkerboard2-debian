/*
 * Author: Brian <brian@vamrs.com>
 * Copyright (c) 2019 Vamrs Corporation.
 *
 * SPDX-License-Identifier: MIT
 */

#include <mraa/common.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "arm/tinker_board.h"
#include "common.h"

#define DT_BASE "/proc/device-tree"
/*
* "Radxa ROCK Pi 4" is the model name on stock 5.x kernels
* "ASUS Tinker Board 2" is used on Radxa 4.4 kernel
* so we search for the string below by ignoring case
*/
#define PLATFORM_NAME_TINKER_BOARD_2 "ASUS Tinker Board 2 (Linux Opensource)"
#define PLATFORM_NAME_TINKER_BOARD_2_ANDROID "ASUS Tinker Board 2 (Android)"
//#define PLATFORM_NAME_ROCK_PI4_2 "ROCK PI 4"
#define MAX_SIZE 64

const char* tinkerboard2_serialdev[MRAA_TINKER2_UART_COUNT] = { "/dev/ttyS0", "/dev/ttyS4" };

void
mraa_tinkerboard_pininfo(mraa_board_t* board, int index, int sysfs_pin, mraa_pincapabilities_t pincapabilities_t, char* fmt, ...)
{
    va_list arg_ptr;
    if (index > board->phy_pin_count)
        return;

    mraa_pininfo_t* pininfo = &board->pins[index];
    va_start(arg_ptr, fmt);
    vsnprintf(pininfo->name, MRAA_PIN_NAME_SIZE, fmt, arg_ptr);

    if( pincapabilities_t.gpio == 1 ) {
        va_arg(arg_ptr, int);
        pininfo->gpio.gpio_chip = va_arg(arg_ptr, int);
        pininfo->gpio.gpio_line = va_arg(arg_ptr, int);
    }

    pininfo->capabilities = pincapabilities_t;

    va_end(arg_ptr);
    pininfo->gpio.pinmap = sysfs_pin;
    pininfo->gpio.mux_total = 0;
}

mraa_board_t*
mraa_tinkerboard()
{
    syslog(LOG_ERR, "mraa_tinkerboard 1=================");
    mraa_board_t* b = (mraa_board_t*) calloc(1, sizeof(mraa_board_t));
    if (b == NULL) {
        return NULL;
    }
syslog(LOG_ERR, "mraa_tinkerboard 2=================");
    b->adv_func = (mraa_adv_func_t*) calloc(1, sizeof(mraa_adv_func_t));
    if (b->adv_func == NULL) {
        free(b);
        return NULL;
    }

    // pin mux for buses are setup by default by kernel so tell mraa to ignore them
    b->no_bus_mux = 1;
    b->phy_pin_count = MRAA_TINKER2_PIN_COUNT + 1;
syslog(LOG_ERR, "mraa_tinkerboard 3=================");
    if (mraa_file_exist(DT_BASE "/model")) {
        // We are on a modern kernel, great!!!!
        syslog(LOG_ERR, "mraa_tinkerboard 33=================");
        if (mraa_file_contains(DT_BASE "/model", PLATFORM_NAME_TINKER_BOARD_2) ||
            mraa_file_contains(DT_BASE "/model", PLATFORM_NAME_TINKER_BOARD_2_ANDROID) 
           ) {
            syslog(LOG_ERR, "mraa_tinkerboard 333=================");
            b->platform_name = PLATFORM_NAME_TINKER_BOARD_2;
            b->uart_dev[0].device_path = (char*) tinkerboard2_serialdev[0];
            b->uart_dev[1].device_path = (char*) tinkerboard2_serialdev[1];
        }
    }
syslog(LOG_ERR, "mraa_tinkerboard=================");
    // UART
    b->uart_dev_count = MRAA_TINKER2_UART_COUNT;
    b->def_uart_dev = 0;
    b->uart_dev[0].index = 2;
    b->uart_dev[1].index = 4;
syslog(LOG_ERR, "mraa_tinkerboard2222=================");
    // I2C
    if (b->platform_name == NULL) {
    
    if (strncmp(b->platform_name, PLATFORM_NAME_TINKER_BOARD_2, MAX_SIZE) == 0 ||
        strncmp(b->platform_name, PLATFORM_NAME_TINKER_BOARD_2_ANDROID, MAX_SIZE) == 0
       ) {
        b->i2c_bus_count = MRAA_TINKER2_I2C_COUNT;
        b->def_i2c_bus = 0;
        b->i2c_bus[0].bus_id = 6;
        b->i2c_bus[1].bus_id = 7;
    }
    }
syslog(LOG_ERR, "mraa_tinkerboard 5=================");
    // SPI
    b->spi_bus_count = MRAA_TINKER2_SPI_COUNT;
    b->def_spi_bus = 0;
    b->spi_bus[0].bus_id = 1;
    b->spi_bus[1].bus_id = 5;

    b->pwm_dev_count = MRAA_TINKER2_PWM_COUNT;
    b->pwm_default_period = 500;
    b->pwm_max_period = 2147483;
    b->pwm_min_period = 1;
syslog(LOG_ERR, "mraa_tinkerboard 6=================");
    b->pins = (mraa_pininfo_t*) malloc(sizeof(mraa_pininfo_t) * b->phy_pin_count);
    if (b->pins == NULL) {
        free(b->adv_func);
        free(b);
        return NULL;
    }
syslog(LOG_ERR, "mraa_tinkerboard 7=================");
    b->pins[11].pwm.parent_id = 0;
    b->pins[11].pwm.mux_total = 0;
    b->pins[11].pwm.pinmap = 0;
    b->pins[13].pwm.parent_id = 1;
    b->pins[13].pwm.mux_total = 0;
    b->pins[13].pwm.pinmap = 0;

    //b->aio_count = MRAA_TINKER2_AIO_COUNT;
    //b->adc_raw = 10;
    //b->adc_supported = 10;
    //b->aio_dev[0].pin = 26;
    //b->aio_non_seq = 1;
syslog(LOG_ERR, "mraa_tinkerboard 8=================");
    mraa_tinkerboard_pininfo(b, 0,   -1, (mraa_pincapabilities_t){0,0,0,0,0,0,0,0}, "INVALID");
    mraa_tinkerboard_pininfo(b, 1,   -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "3V3");
    mraa_tinkerboard_pininfo(b, 2,   -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "5V");
    mraa_tinkerboard_pininfo(b, 3,   73, (mraa_pincapabilities_t){1,1,0,0,0,1,0,0}, "SDA6");
    mraa_tinkerboard_pininfo(b, 4,   -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "5V");
    mraa_tinkerboard_pininfo(b, 5,   74, (mraa_pincapabilities_t){1,1,0,0,0,1,0,0}, "SCL6");
    mraa_tinkerboard_pininfo(b, 6,   -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "GND");
    mraa_tinkerboard_pininfo(b, 7,    8, (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, "TEST_CLK2");
    mraa_tinkerboard_pininfo(b, 8,   81, (mraa_pincapabilities_t){1,1,0,0,0,0,0,1}, "TXD0");
    mraa_tinkerboard_pininfo(b, 9,   -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "GND");
    mraa_tinkerboard_pininfo(b, 10,  80, (mraa_pincapabilities_t){1,1,0,0,0,0,0,1}, "RXD0");
    mraa_tinkerboard_pininfo(b, 11,  83, (mraa_pincapabilities_t){1,1,0,0,0,0,0,1}, "RTSN0");
    mraa_tinkerboard_pininfo(b, 12, 120, (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, "I2S0_SCLK");
    mraa_tinkerboard_pininfo(b, 13,  85, (mraa_pincapabilities_t){1,1,0,0,1,0,0,0}, "SPI5TX");
    mraa_tinkerboard_pininfo(b, 14,  -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "GND");
    mraa_tinkerboard_pininfo(b, 15,  84, (mraa_pincapabilities_t){1,1,0,0,1,0,0,0}, "SPI5RX");
    mraa_tinkerboard_pininfo(b, 16,  86, (mraa_pincapabilities_t){1,1,0,0,1,0,0,0}, "SPI5CLK");
    mraa_tinkerboard_pininfo(b, 17,  -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "3V3");
    mraa_tinkerboard_pininfo(b, 18,  87, (mraa_pincapabilities_t){1,1,0,0,1,0,0,0}, "SPI5CS0");
    mraa_tinkerboard_pininfo(b, 19,  40, (mraa_pincapabilities_t){1,1,0,0,1,0,0,1}, "SPI1TX,TXD4");
    mraa_tinkerboard_pininfo(b, 20,  -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "GND");
    mraa_tinkerboard_pininfo(b, 21,  39, (mraa_pincapabilities_t){1,1,0,0,1,0,0,1}, "SPI1RX,RXD4");
    mraa_tinkerboard_pininfo(b, 22, 124, (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, "I2S0_SDO3");
    mraa_tinkerboard_pininfo(b, 23,  41, (mraa_pincapabilities_t){1,1,0,0,1,0,0,0}, "SPI1CLK");
    mraa_tinkerboard_pininfo(b, 24,  42, (mraa_pincapabilities_t){1,1,0,0,1,0,0,0}, "SPI1CS0");
    mraa_tinkerboard_pininfo(b, 25,  -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "GND");
    mraa_tinkerboard_pininfo(b, 26,   6, (mraa_pincapabilities_t){1,0,1,0,0,0,0,0}, "PWM3A");
    mraa_tinkerboard_pininfo(b, 27,  71, (mraa_pincapabilities_t){1,1,0,0,0,1,0,0}, "SDA7");
    mraa_tinkerboard_pininfo(b, 28,  72, (mraa_pincapabilities_t){1,1,0,0,0,1,0,0}, "SCL7");
    mraa_tinkerboard_pininfo(b, 29, 126, (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, "I2S0_SDO1");
    mraa_tinkerboard_pininfo(b, 30,  -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "GND");
    mraa_tinkerboard_pininfo(b, 31, 125, (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, "I2S0_SDO2");
    mraa_tinkerboard_pininfo(b, 32, 146, (mraa_pincapabilities_t){1,1,1,0,0,0,0,0}, "PWM0");
    mraa_tinkerboard_pininfo(b, 33, 150, (mraa_pincapabilities_t){1,1,1,0,0,0,0,0}, "PWM1");
    mraa_tinkerboard_pininfo(b, 34,  -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "GND");
    mraa_tinkerboard_pininfo(b, 35, 121, (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, "I2S0_FS");
    mraa_tinkerboard_pininfo(b, 36,  82, (mraa_pincapabilities_t){1,1,0,0,0,0,0,1}, "CTSN0");
    mraa_tinkerboard_pininfo(b, 37, 149, (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, "SPDIFTX");
    mraa_tinkerboard_pininfo(b, 38, 123, (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, "I2S0_SDI0");
    mraa_tinkerboard_pininfo(b, 39,  -1, (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, "GND");
    mraa_tinkerboard_pininfo(b, 40, 127, (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, "I2S0_SDO0");

    return b;
}
