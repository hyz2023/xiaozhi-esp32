#pragma once

#include <memory>
#include <vector>
#include "display.h"
#include "lvgl.h"

// 前向声明
class Display;
class LayoutBase;
class ClockWidget;
class CalendarWidget;
class WeatherWidget;
class AnimationWidget;

class IdleScreen {
public:
    explicit IdleScreen(Display* parent);
    virtual ~IdleScreen() = default;

    // 显示控制
    virtual void Show();
    virtual void Hide();
    virtual void Update();

    // 布局控制
    void SwitchLayout(size_t index);
    void AnimateTransition(Direction direction);

    // 配置管理
    void LoadConfig();
    void SaveConfig();

protected:
    Display* parent_;
    lv_obj_t* container_;
    std::unique_ptr<LayoutBase> current_layout_;
    std::vector<std::unique_ptr<LayoutBase>> layouts_;
    size_t current_layout_index_;

    // 组件
    std::unique_ptr<ClockWidget> clock_widget_;
    std::unique_ptr<CalendarWidget> calendar_widget_;
    std::unique_ptr<WeatherWidget> weather_widget_;
    std::unique_ptr<AnimationWidget> animation_widget_;

    // 初始化
    virtual void SetupUI();
    void SetupComponents();
    void SetupLayouts();
}; 