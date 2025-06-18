#include "oled_idle_screen.h"
#include <esp_log.h>

#define TAG "OledIdleScreen"

void OledIdleScreen::SetupUI() {
    // 调用基类的SetupUI
    IdleScreen::SetupUI();

    // OLED特定的UI设置
    lv_obj_set_style_bg_color(container_, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(container_, LV_OPA_COVER, 0);

    // 设置OLED特定的样式
    if (clock_widget_) {
        lv_obj_set_style_text_font(clock_widget_->GetContainer(), &lv_font_montserrat_24, 0);
    }
    if (calendar_widget_) {
        lv_obj_set_style_text_font(calendar_widget_->GetContainer(), &lv_font_montserrat_14, 0);
    }
    if (weather_widget_) {
        lv_obj_set_style_text_font(weather_widget_->GetContainer(), &lv_font_montserrat_14, 0);
    }
} 