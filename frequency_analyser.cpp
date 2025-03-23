#include <stdio.h>
#include "pico/stdlib.h"
#include <array>

#include "frequency_analyser_configurator.h"
#include "frequency_analyser_model_widget.cpp"

#include "utilities/probe/probe.h"
Probe pr_D1 = Probe(1);
Probe pr_D4 = Probe(4);
Probe pr_D5 = Probe(5);

void shared_irq_call_back(uint gpio, uint32_t event_mask);

KY040 ky040 = KY040(CENTRAL_SWITCH_GPIO,
                    ENCODER_CLK_GPIO,
                    ENCODER_DT_GPIO,
                    &shared_irq_call_back,
                    cfg_central_switch,
                    cfg_encoder_clk);

void shared_irq_call_back(uint gpio, uint32_t event_mask)
{
    pr_D1.hi();
    switch (gpio)
    {
    case ENCODER_CLK_GPIO:
        ky040.process_encoder_IRQ(event_mask);
        break;
    default:
        printf("unknown IRQ\n");
        break;
    };
    pr_D1.lo();
}
FrequencyAnalyserFocusManager *frequency_focus_manager = new FrequencyAnalyserFocusManager();

void manager_process_control_event(UIControlEvent event){
    frequency_focus_manager->process_control_event(event);
}

int main()
{
    MSGEQ7 spectrum_analyser = MSGEQ7(MSGEQ7_STROBE, MSGEQ7_RESET, MSGEQ7_OUT);

    ky040.update_UI_control_event_processor(manager_process_control_event);

    HW_I2C_Master master = HW_I2C_Master(cfg_i2c);
    SSD1306 display = SSD1306(&master, cfg_ssd1306);
    FrequencyAnalyserModel my_model = FrequencyAnalyserModel(7, 0, 4095);
    FrequencyAnalyserWidget my_widget = FrequencyAnalyserWidget(&my_model,
                                                                &display,
                                                                vertical_bargraph_cfg);

    frequency_focus_manager->update_current_controller(&ky040);
    frequency_focus_manager->set_managed_focus(&my_model);
    display.clear_device_screen_buffer();

    while (true)
    {
        my_model.update_values(spectrum_analyser.get_spectrum());

        my_model.draw_refresh_all_attached_widgets();

        sleep_ms(50);
    }
}
