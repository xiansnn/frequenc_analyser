#include <stdio.h>
#include "pico/stdlib.h"
#include <array>

#include "device/MSGEQ7/msgeq7.h"
#include "device/SSD1306/ssd1306.h"
#include "sw/widget_bargraph/widget_bargraph.h"
#include "utilities/probe/probe.h"

Probe pr_D1 = Probe(1);
Probe pr_D4 = Probe(4);
Probe pr_D5 = Probe(5);

#define MSGEQ7_STROBE 19
#define MSGEQ7_RESET 18
#define MSGEQ7_OUT 27

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

std::vector<int> convert_array_to_vector(std::array<uint16_t, 7> array)
{
    std::vector<int> vector(7);
    for (size_t i = 0; i < 7; i++)
    {
        vector[i] = (int)array[i];
    }
    return vector;
};

struct_ConfigGraphicWidget vertical_bargraph_cfg = {
    .pixel_frame_width = 56,
    .pixel_frame_height = 56,
    .fg_color = PixelColor::WHITE,
    .bg_color = PixelColor::BLACK,
    .widget_anchor_x = 20,
    .widget_anchor_y = 0,
    .widget_with_border = true};

int main()
{
    MSGEQ7 spectrum_analyser = MSGEQ7(MSGEQ7_STROBE, MSGEQ7_RESET, MSGEQ7_OUT);

    HW_I2C_Master master = HW_I2C_Master(cfg_i2c);
    SSD1306 display = SSD1306(&master, cfg_ssd1306);
    ModelBargraph my_model = ModelBargraph(7, 0, 4095);
    WidgetVerticalBargraph my_widget = WidgetVerticalBargraph(&my_model,
                                                              &display,
                                                              vertical_bargraph_cfg);

    display.clear_device_screen_buffer();

    while (true)
    {
        pr_D1.pulse_us(10);

        my_model.update_values(convert_array_to_vector(spectrum_analyser.get_spectrum()));

        my_model.draw_refresh_all_attached_widgets();

        sleep_ms(20);
    }
}
