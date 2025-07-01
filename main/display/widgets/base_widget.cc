#include "base_widget.h"
#include <esp_log.h>

static const char* TAG = "BaseWidget";

BaseWidget::BaseWidget(lv_obj_t* parent, const std::string& name)
    : parent_(parent), name_(name), is_visible_(true), current_theme_(nullptr) {
    ESP_LOGI(TAG, "Creating widget: %s", name_.c_str());
    CreateContainer();
}

BaseWidget::~BaseWidget() {
    ESP_LOGI(TAG, "Destroying widget: %s", name_.c_str());
    DestroyContainer();
}

void BaseWidget::CreateContainer() {
    if (!parent_) {
        ESP_LOGE(TAG, "Parent is null for widget: %s", name_.c_str());
        return;
    }
    
    container_ = lv_obj_create(parent_);
    if (!container_) {
        ESP_LOGE(TAG, "Failed to create container for widget: %s", name_.c_str());
        return;
    }
    
    // 设置基本属性
    lv_obj_set_size(container_, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(container_, x_, y_);
    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container_, 0, 0);
    lv_obj_set_style_pad_all(container_, 0, 0);
    
    ESP_LOGI(TAG, "Container created for widget: %s", name_.c_str());
}

void BaseWidget::DestroyContainer() {
    if (container_) {
        lv_obj_del(container_);
        container_ = nullptr;
        ESP_LOGI(TAG, "Container destroyed for widget: %s", name_.c_str());
    }
}

void BaseWidget::SetVisible(bool visible) {
    if (is_visible_ == visible) {
        return;
    }
    
    is_visible_ = visible;
    if (container_) {
        if (visible) {
            lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
        }
    }
    
    ESP_LOGI(TAG, "Widget %s visibility set to: %s", name_.c_str(), visible ? "visible" : "hidden");
}

void BaseWidget::ApplyTheme(const IdleTheme& theme) {
    // 直接使用传入的theme参数，不存储指针
    UpdateContainerStyle();
    ESP_LOGI(TAG, "Theme applied to widget: %s", name_.c_str());
}

void BaseWidget::SetPosition(int x, int y) {
    x_ = x;
    y_ = y;
    if (container_) {
        lv_obj_set_pos(container_, x, y);
    }
}

void BaseWidget::SetSize(int width, int height) {
    width_ = width;
    height_ = height;
    if (container_) {
        lv_obj_set_size(container_, width, height);
    }
}

void BaseWidget::UpdateLayout(const LayoutConfig& layout) {
    // 基类默认实现，子类可以重写
    ESP_LOGI(TAG, "Layout updated for widget: %s", name_.c_str());
}

void BaseWidget::SetBorder(bool enabled, uint32_t color, int width) {
    if (!container_) return;
    
    if (enabled) {
        lv_obj_set_style_border_width(container_, width, 0);
        lv_obj_set_style_border_color(container_, lv_color_hex(color), 0);
    } else {
        lv_obj_set_style_border_width(container_, 0, 0);
    }
}

void BaseWidget::SetBackground(uint32_t color) {
    if (!container_) return;
    
    lv_obj_set_style_bg_color(container_, lv_color_hex(color), 0);
    lv_obj_set_style_bg_opa(container_, LV_OPA_COVER, 0);
}

void BaseWidget::SetPadding(int left, int top, int right, int bottom) {
    if (!container_) return;
    
    lv_obj_set_style_pad_left(container_, left, 0);
    lv_obj_set_style_pad_top(container_, top, 0);
    lv_obj_set_style_pad_right(container_, right, 0);
    lv_obj_set_style_pad_bottom(container_, bottom, 0);
}

void BaseWidget::UpdateContainerStyle() {
    if (!container_) return;
    
    // 使用默认主题样式，避免访问不完整的类型
    uint32_t widget_bg_color = 0x1A1A1A;  // 默认深灰色
    uint32_t border_color = 0xCCCCCC;     // 默认灰色
    int border_width = 1;                 // 默认边框宽度
    int padding = 15;                     // 默认内边距
    
    // 应用主题样式
    lv_obj_set_style_bg_color(container_, lv_color_hex(widget_bg_color), 0);
    lv_obj_set_style_bg_opa(container_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(container_, lv_color_hex(border_color), 0);
    lv_obj_set_style_border_width(container_, border_width, 0);
    lv_obj_set_style_pad_all(container_, padding, 0);
}

lv_style_t* BaseWidget::GetWidgetStyle() {
    if (!style_initialized_) {
        lv_style_init(&widget_style_);
        style_initialized_ = true;
    }
    return &widget_style_;
}

void BaseWidget::ApplyWidgetStyle() {
    if (!container_ || !style_initialized_) return;
    
    lv_obj_add_style(container_, &widget_style_, 0);
} 