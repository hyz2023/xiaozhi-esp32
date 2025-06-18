#pragma once

#include <memory>
#include <vector>
#include "lvgl.h"
#include "../components/widget_base.h"

// 布局度量结构体
struct LayoutMetrics {
    // 边距
    static constexpr int MARGIN_TOP = 10;
    static constexpr int MARGIN_BOTTOM = 10;
    static constexpr int MARGIN_LEFT = 10;
    static constexpr int MARGIN_RIGHT = 10;
    
    // 内边距
    static constexpr int PADDING = 5;
    
    // 组件尺寸
    static constexpr int CLOCK_HEIGHT = 80;
    static constexpr int CALENDAR_HEIGHT = 40;
    static constexpr int WEATHER_HEIGHT = 60;
    static constexpr int ANIMATION_HEIGHT = 100;
    
    // 边框
    static constexpr int BORDER_WIDTH = 1;
};

class LayoutBase {
public:
    explicit LayoutBase(lv_obj_t* parent);
    virtual ~LayoutBase() = default;

    // 显示控制
    virtual void Show();
    virtual void Hide();
    virtual void Update();

    // 组件管理
    void AddWidget(std::unique_ptr<WidgetBase> widget);
    void RemoveWidget(WidgetBase* widget);
    void ClearWidgets();

protected:
    lv_obj_t* container_;
    std::vector<std::unique_ptr<WidgetBase>> widgets_;

    // 布局设置
    virtual void SetupLayout() = 0;
    virtual void ArrangeWidgets() = 0;
    
    // 辅助函数
    void AddBorderStyle(lv_obj_t* obj);
}; 