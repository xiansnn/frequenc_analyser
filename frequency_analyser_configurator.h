#pragma once

#define MSGEQ7_STROBE 19
#define MSGEQ7_RESET 18
#define MSGEQ7_OUT 27

#define CENTRAL_SWITCH_GPIO 6
#define ENCODER_CLK_GPIO 26
#define ENCODER_DT_GPIO 21
#define CENTRAL_SWITCH_TIME_OUT_us 3000000

#include "device/MSGEQ7/msgeq7.h"
#include "device/SSD1306/ssd1306.h"
#include "device/KY040/ky040.h"
#include "sw/widget_bargraph/widget_bargraph.h"


struct_ConfigMasterI2C cfg_i2c{
    .i2c = i2c0,
    .sda_pin = 8,
    .scl_pin = 9,
    .baud_rate = I2C_FAST_MODE};

struct_ConfigSSD1306 cfg_ssd1306{
    .i2c_address = 0x3C,
    .vertical_offset = 0,
    .scan_SEG_inverse_direction = true,
    .scan_COM_inverse_direction = true,
    .contrast = 128,
    .frequency_divider = 1,
    .frequency_factor = 0};

struct_ConfigSwitchButton cfg_central_switch{
    .debounce_delay_us = 5000,
    .long_release_delay_us = 1000000,
    .long_push_delay_us = 1000000,
    .time_out_delay_us = CENTRAL_SWITCH_TIME_OUT_us,
    .active_lo = true};

struct_ConfigSwitchButton cfg_encoder_clk{
    .debounce_delay_us = 5000};

struct_ConfigGraphicWidget vertical_bargraph_cfg = {
    .pixel_frame_width = 56,
    .pixel_frame_height = 48,
    .fg_color = PixelColor::WHITE,
    .bg_color = PixelColor::BLACK,
    .widget_anchor_x = 20,
    .widget_anchor_y = 0,
    .widget_with_border = false};
