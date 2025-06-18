#include "layout_base.h"
#include <esp_log.h>

#define TAG "LayoutBase"

LayoutBase::LayoutBase(lv_obj_t* parent) {
    // 创建容器
    container_ = lv_obj_create(parent);
    lv_obj_set_size(container_, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_pad_all(container_, LayoutMetrics::PADDING, 0);
    lv_obj_set_style_border_width(container_, 0, 0);
    lv_obj_set_style_radius(container_, 0, 0);
    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, 0);

    // 设置布局
    SetupLayout();
}

void LayoutBase::Show() {
    if (container_) {
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }
    for (const auto& widget : widgets_) {
        if (widget) {
            widget->Show();
        }
    }
}

void LayoutBase::Hide() {
    if (container_) {
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }
    for (const auto& widget : widgets_) {
        if (widget) {
            widget->Hide();
        }
    }
}

void LayoutBase::Update() {
    for (const auto& widget : widgets_) {
        if (widget) {
            widget->Update();
        }
    }
}

void LayoutBase::AddWidget(std::unique_ptr<WidgetBase> widget) {
    if (widget) {
        widgets_.push_back(std::move(widget));
        ArrangeWidgets();
    }
}

void LayoutBase::RemoveWidget(WidgetBase* widget) {
    auto it = std::find_if(widgets_.begin(), widgets_.end(),
        [widget](const std::unique_ptr<WidgetBase>& w) {
            return w.get() == widget;
        });
    
    if (it != widgets_.end()) {
        widgets_.erase(it);
        ArrangeWidgets();
    }
}

void LayoutBase::ClearWidgets() {
    widgets_.clear();
}

void LayoutBase::AddBorderStyle(lv_obj_t* obj) {
    lv_obj_set_style_border_width(obj, LayoutMetrics::BORDER_WIDTH, 0);
    lv_obj_set_style_border_color(obj, lv_color_hex(0xCCCCCC), 0);
    lv_obj_set_style_radius(obj, 5, 0);
} 