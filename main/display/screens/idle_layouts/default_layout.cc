#include "default_layout.h"
#include <esp_log.h>

#define TAG "DefaultLayout"

DefaultLayout::DefaultLayout(lv_obj_t* parent) : LayoutBase(parent) {
    // 创建组件
    clock_widget_ = std::make_unique<ClockWidget>(container_);
    weather_widget_ = std::make_unique<WeatherWidget>(container_);
    animation_widget_ = std::make_unique<AnimationWidget>(container_);

    // 添加组件到布局
    AddWidget(std::move(clock_widget_));
    AddWidget(std::move(weather_widget_));
    AddWidget(std::move(animation_widget_));
}

void DefaultLayout::SetupLayout() {
    // 设置容器样式
    lv_obj_set_style_bg_color(container_, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(container_, LV_OPA_COVER, 0);
}

void DefaultLayout::ArrangeWidgets() {
    // 获取容器尺寸
    lv_coord_t width = lv_obj_get_width(container_);
    lv_coord_t height = lv_obj_get_height(container_);

    // 设置时钟组件位置和大小
    if (clock_widget_) {
        lv_obj_t* clock_container = clock_widget_->GetContainer();
        lv_obj_set_size(clock_container, width * 0.4, height * 0.3);
        lv_obj_align(clock_container, LV_ALIGN_TOP_LEFT, LayoutMetrics::MARGIN, LayoutMetrics::MARGIN);
    }

    // 设置天气组件位置和大小
    if (weather_widget_) {
        lv_obj_t* weather_container = weather_widget_->GetContainer();
        lv_obj_set_size(weather_container, width * 0.4, height * 0.3);
        lv_obj_align(weather_container, LV_ALIGN_TOP_RIGHT, -LayoutMetrics::MARGIN, LayoutMetrics::MARGIN);
    }

    // 设置动画组件位置和大小
    if (animation_widget_) {
        lv_obj_t* animation_container = animation_widget_->GetContainer();
        lv_obj_set_size(animation_container, width * 0.8, height * 0.3);
        lv_obj_align(animation_container, LV_ALIGN_BOTTOM_MID, 0, -LayoutMetrics::MARGIN);
    }
} 