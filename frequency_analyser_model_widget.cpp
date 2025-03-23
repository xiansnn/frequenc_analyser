
#include "frequency_analyser_configurator.h"
#include "sw/widget_square_led/widget_square_led.h"

class FrequencyFilterModel : public UIControlledModel
{
private:
    std::string center_frequency;

public:
    FrequencyFilterModel(std::string center_frequency);
    ~FrequencyFilterModel();
    void process_control_event(UIControlEvent _event);
};

FrequencyFilterModel::FrequencyFilterModel(std::string center_frequency)
{
    this->center_frequency = center_frequency;
}

FrequencyFilterModel::~FrequencyFilterModel()
{
}

void FrequencyFilterModel::process_control_event(UIControlEvent _event)
{
}

class FrequencyAnalyserModel : public ModelBargraph
{
private:
    std::array<std::string, 7> frequency_filters = {
        "   63Hz",
        "  160Hz",
        "  400Hz",
        "   1kHz",
        " 2.5kHz",
        "6.25kHz",
        "  16kHz"};

protected:
public:
    std::array<FrequencyFilterModel *, 7> filters;

    FrequencyAnalyserModel(size_t number_of_bar, int min_value, int max_value);
    ~FrequencyAnalyserModel();
    void update_values(std::array<uint16_t, 7> array);
};

FrequencyAnalyserModel::FrequencyAnalyserModel(size_t number_of_bar,
                                               int min_value, int max_value)
    : ModelBargraph(number_of_bar, min_value, max_value)
{
    for (size_t i = 0; i < 7; i++)
    {
        filters[i] = new FrequencyFilterModel(frequency_filters[i]);
        filters[i]->update_status(ControlledObjectStatus::IS_ACTIVE);
    }
}

FrequencyAnalyserModel::~FrequencyAnalyserModel()
{
}

void FrequencyAnalyserModel::update_values(std::array<uint16_t, 7> array)
{
    for (size_t i = 0; i < array.size(); i++)
        values[i] = (int)array[i];
}

class FrequencyAnalyserWidget : public Widget
{
private:
    WidgetVerticalBargraph *bargraph;
    std::array<WidgetFocusIndicator *, 7> bar_focus_indicators;

public:
    FrequencyAnalyserWidget(FrequencyAnalyserModel *frequency_analyser_model,
                            GraphicDisplayDevice *graphic_display_screen,
                            struct_ConfigGraphicWidget graph_cfg);
    ~FrequencyAnalyserWidget();
    void draw();
};

FrequencyAnalyserWidget::FrequencyAnalyserWidget(FrequencyAnalyserModel *frequency_analyser_model,
                                                 GraphicDisplayDevice *graphic_display_screen,
                                                 struct_ConfigGraphicWidget graph_cfg)
    : Widget(frequency_analyser_model, graphic_display_screen)
{
#define WIDGET_SPACE 1
#define LED_HEIGHT 8
    bargraph = new WidgetVerticalBargraph(frequency_analyser_model, graphic_display_screen, graph_cfg);

    struct_ConfigGraphicWidget current_cfg = {
        .pixel_frame_width = bargraph->bar_width,
        .pixel_frame_height = 8,
        .fg_color = PixelColor::WHITE,
        .bg_color = PixelColor::BLACK,
        .widget_anchor_y = 48,
        .widget_with_border = false};
    ;
    for (size_t i = 0; i < 7; i++)
    {
        current_cfg.widget_anchor_x = graph_cfg.widget_anchor_x + i * (current_cfg.pixel_frame_width + 1) + 1;
        bar_focus_indicators[i] = new WidgetFocusIndicator(((FrequencyAnalyserModel *)actual_displayed_model)->filters[i],
                                                           graphic_display_screen, current_cfg);
        bar_focus_indicators[i]->set_blink_us(300000);
    }
}

FrequencyAnalyserWidget::~FrequencyAnalyserWidget()
{
    delete bargraph;
    for (auto &&w : bar_focus_indicators)
        delete w;
}

void FrequencyAnalyserWidget::draw()
{
    bargraph->draw();
    for (auto &&w : bar_focus_indicators)
        w->draw();
    actual_displayed_model->draw_widget_done();
}

class FrequencyAnalyserFocusManager : public UIModelManager
{
private:
    /* data */
public:
    FrequencyAnalyserFocusManager(/* args */);
    ~FrequencyAnalyserFocusManager();
    void set_managed_focus(FrequencyAnalyserModel *frequency_analyser);
    void process_control_event(UIControlEvent event);
};

FrequencyAnalyserFocusManager::FrequencyAnalyserFocusManager(/* args */)
{
}

FrequencyAnalyserFocusManager::~FrequencyAnalyserFocusManager()
{
}

void FrequencyAnalyserFocusManager::set_managed_focus(FrequencyAnalyserModel *frequency_analyser)
{
    for (auto &&m : frequency_analyser->filters)
        add_managed_model(m);
}

void FrequencyAnalyserFocusManager::process_control_event(UIControlEvent event)
{
}
