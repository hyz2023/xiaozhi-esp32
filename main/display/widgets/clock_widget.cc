#include "clock_widget.h"

#include <esp_log.h>

static const char* TAG = "ClockWidget";

ClockWidget::ClockWidget(lv_obj_t* parent)
    : BaseWidget(parent, "ClockWidget") {
    ESP_LOGI(TAG, "Creating ClockWidget");
    CreateWidgets();
}

ClockWidget::~ClockWidget() {
    ESP_LOGI(TAG, "Destroying ClockWidget");
    DestroyWidgets();
}

void ClockWidget::CreateWidgets() {
    if (!container_) {
        ESP_LOGE(TAG, "Container is null, cannot create widgets");
        return;
    }
    
    // 创建时间标签
    time_label_ = lv_label_create(container_);
    if (time_label_) {
        lv_label_set_text(time_label_, "12:00");
        lv_obj_set_style_text_align(time_label_, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(time_label_, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_width(time_label_, lv_pct(100));
        lv_obj_set_pos(time_label_, 0, 10);
    }
    
    // 创建日期标签
    date_label_ = lv_label_create(container_);
    if (date_label_) {
        lv_label_set_text(date_label_, "2024-03-20");
        lv_obj_set_style_text_align(date_label_, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(date_label_, lv_color_hex(0xCCCCCC), 0);
        lv_obj_set_width(date_label_, lv_pct(100));
        lv_obj_set_pos(date_label_, 0, 60);
    }
    
    // 创建农历标签
    lunar_label_ = lv_label_create(container_);
    if (lunar_label_) {
        lv_label_set_text(lunar_label_, "农历二月十一");
        lv_obj_set_style_text_align(lunar_label_, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(lunar_label_, lv_color_hex(0x999999), 0);
        lv_obj_set_width(lunar_label_, lv_pct(100));
        lv_obj_set_pos(lunar_label_, 0, 100);
    }
    
    ApplyTextStyles();
    ESP_LOGI(TAG, "Clock widgets created successfully");
}

void ClockWidget::DestroyWidgets() {
    if (time_label_) {
        lv_obj_del(time_label_);
        time_label_ = nullptr;
    }
    if (date_label_) {
        lv_obj_del(date_label_);
        date_label_ = nullptr;
    }
    if (lunar_label_) {
        lv_obj_del(lunar_label_);
        lunar_label_ = nullptr;
    }
    ESP_LOGI(TAG, "Clock widgets destroyed");
}

void ClockWidget::Update() {
    UpdateTimeDisplay();
    UpdateDateDisplay();
    UpdateLunarDisplay();
}

void ClockWidget::SetData(const ClockData& data) {
    current_data_ = data;
    is_24h_format_ = data.is_24h_format;
    Update();
}

void ClockWidget::UpdateLayout(const LayoutConfig& layout) {
    BaseWidget::UpdateLayout(layout);
    
    // 调整组件大小和位置
    SetSize(layout.widget_width, layout.clock_height);
    
    // 重新定位标签
    if (time_label_) {
        lv_obj_set_pos(time_label_, 0, 10);
    }
    if (date_label_) {
        lv_obj_set_pos(date_label_, 0, 60);
    }
    if (lunar_label_) {
        lv_obj_set_pos(lunar_label_, 0, 100);
    }
    
    ESP_LOGI(TAG, "Clock layout updated");
}

void ClockWidget::SetTimeFont(const lv_font_t* font) {
    time_font_ = font;
    if (time_label_ && font) {
        lv_obj_set_style_text_font(time_label_, font, 0);
    }
}

void ClockWidget::SetDateFont(const lv_font_t* font) {
    date_font_ = font;
    if (date_label_ && font) {
        lv_obj_set_style_text_font(date_label_, font, 0);
    }
}

void ClockWidget::SetLunarFont(const lv_font_t* font) {
    lunar_font_ = font;
    if (lunar_label_ && font) {
        lv_obj_set_style_text_font(lunar_label_, font, 0);
    }
}

void ClockWidget::ShowLunar(bool show) {
    show_lunar_ = show;
    if (lunar_label_) {
        lv_obj_set_style_opa(lunar_label_, show ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
    }
}

void ClockWidget::SetTimeFormat(bool is_24h) {
    is_24h_format_ = is_24h;
    UpdateTimeDisplay();
}

void ClockWidget::UpdateTimeDisplay() {
    if (!time_label_) return;
    
    std::string time_text = current_data_.time_str;
    if (time_text.empty()) {
        time_text = "12:00";
    }
    
    lv_label_set_text(time_label_, time_text.c_str());
    ESP_LOGD(TAG, "Time updated: %s", time_text.c_str());
}

void ClockWidget::UpdateDateDisplay() {
    if (!date_label_) return;
    
    std::string date_text = current_data_.date_str;
    if (date_text.empty()) {
        date_text = "2024-03-20";
    }
    
    lv_label_set_text(date_label_, date_text.c_str());
    ESP_LOGD(TAG, "Date updated: %s", date_text.c_str());
}

void ClockWidget::UpdateLunarDisplay() {
    if (!lunar_label_) return;
    
    if (!show_lunar_) {
        lv_label_set_text(lunar_label_, "");
        return;
    }
    
    std::string lunar_text = current_data_.lunar_date;
    if (lunar_text.empty()) {
        lunar_text = "农历二月十一";
    }
    
    lv_label_set_text(lunar_label_, lunar_text.c_str());
    ESP_LOGD(TAG, "Lunar date updated: %s", lunar_text.c_str());
}

void ClockWidget::ApplyTextStyles() {
    if (styles_initialized_) return;
    
    // 初始化样式
    lv_style_init(&time_style_);
    lv_style_init(&date_style_);
    lv_style_init(&lunar_style_);
    
    // 时间样式
    lv_style_set_text_color(&time_style_, lv_color_hex(0xFFFFFF));
    lv_style_set_text_align(&time_style_, LV_TEXT_ALIGN_CENTER);
    
    // 日期样式
    lv_style_set_text_color(&date_style_, lv_color_hex(0xCCCCCC));
    lv_style_set_text_align(&date_style_, LV_TEXT_ALIGN_CENTER);
    
    // 农历样式
    lv_style_set_text_color(&lunar_style_, lv_color_hex(0x999999));
    lv_style_set_text_align(&lunar_style_, LV_TEXT_ALIGN_CENTER);
    
    // 应用样式
    if (time_label_) {
        lv_obj_add_style(time_label_, &time_style_, 0);
    }
    if (date_label_) {
        lv_obj_add_style(date_label_, &date_style_, 0);
    }
    if (lunar_label_) {
        lv_obj_add_style(lunar_label_, &lunar_style_, 0);
    }
    
    styles_initialized_ = true;
    ESP_LOGI(TAG, "Text styles applied");
} 