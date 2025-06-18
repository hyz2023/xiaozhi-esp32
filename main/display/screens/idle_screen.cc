#include "idle_screen.h"
#include "display.h"
#include "idle_layouts/layout_base.h"
#include "components/clock_widget.h"
#include "components/calendar_widget.h"
#include "components/weather_widget.h"
#include "components/animation_widget.h"
#include <esp_log.h>

#define TAG "IdleScreen"

IdleScreen::IdleScreen(Display* parent) : parent_(parent) {
    SetupUI();
    SetupComponents();
    SetupLayouts();
}

void IdleScreen::Show() {
    if (container_) {
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }
    if (current_layout_) {
        current_layout_->Show();
    }
}

void IdleScreen::Hide() {
    if (container_) {
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }
    if (current_layout_) {
        current_layout_->Hide();
    }
}

void IdleScreen::Update() {
    if (current_layout_) {
        current_layout_->Update();
    }
}

void IdleScreen::SwitchLayout(size_t index) {
    if (index >= layouts_.size()) {
        ESP_LOGE(TAG, "Invalid layout index: %d", index);
        return;
    }

    if (current_layout_) {
        current_layout_->Hide();
    }

    current_layout_ = std::move(layouts_[index]);
    current_layout_index_ = index;

    if (current_layout_) {
        current_layout_->Show();
    }
}

void IdleScreen::AnimateTransition(Direction direction) {
    if (!current_layout_) return;

    // 创建动画
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, container_);
    lv_anim_set_time(&anim, 300);
    lv_anim_set_delay(&anim, 0);
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);

    // 根据方向设置动画参数
    switch (direction) {
        case Direction::LEFT:
            lv_anim_set_values(&anim, 0, -LV_HOR_RES);
            break;
        case Direction::RIGHT:
            lv_anim_set_values(&anim, 0, LV_HOR_RES);
            break;
        default:
            return;
    }

    // 开始动画
    lv_anim_start(&anim);
}

void IdleScreen::LoadConfig() {
    // TODO: 从配置文件加载设置
}

void IdleScreen::SaveConfig() {
    // TODO: 保存设置到配置文件
}

void IdleScreen::SetupUI() {
    // 创建容器
    container_ = lv_obj_create(lv_screen_active());
    lv_obj_set_size(container_, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_pad_all(container_, 0, 0);
    lv_obj_set_style_border_width(container_, 0, 0);
    lv_obj_set_style_radius(container_, 0, 0);
    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, 0);
}

void IdleScreen::SetupComponents() {
    // 创建组件
    clock_widget_ = std::make_unique<ClockWidget>(container_);
    calendar_widget_ = std::make_unique<CalendarWidget>(container_);
    weather_widget_ = std::make_unique<WeatherWidget>(container_);
    animation_widget_ = std::make_unique<AnimationWidget>(container_);
}

void IdleScreen::SetupLayouts() {
    // TODO: 创建布局
    // layouts_.push_back(std::make_unique<Layout1>(container_));
    // layouts_.push_back(std::make_unique<Layout2>(container_));
    
    if (!layouts_.empty()) {
        current_layout_ = std::move(layouts_[0]);
        current_layout_index_ = 0;
    }
} 