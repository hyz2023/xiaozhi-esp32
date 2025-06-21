#ifndef CLOCK_WIDGET_H
#define CLOCK_WIDGET_H

#include "base_widget.h"
#include <string>

// 前向声明
struct ClockData;

// 时钟组件
class ClockWidget : public BaseWidget {
public:
    ClockWidget(lv_obj_t* parent);
    ~ClockWidget() override;
    
    // 核心接口实现
    void Update() override;
    void SetData(const ClockData& data);
    
    // 布局相关
    int GetPreferredWidth() const override { return 400; }
    int GetPreferredHeight() const override { return 150; }
    void UpdateLayout(const LayoutConfig& layout) override;
    
    // 样式控制
    void SetTimeFont(const lv_font_t* font);
    void SetDateFont(const lv_font_t* font);
    void SetLunarFont(const lv_font_t* font);
    
    // 显示控制
    void ShowLunar(bool show);
    void SetTimeFormat(bool is_24h);
    
private:
    // 私有方法
    void CreateWidgets();
    void DestroyWidgets();
    void UpdateTimeDisplay();
    void UpdateDateDisplay();
    void UpdateLunarDisplay();
    void ApplyTextStyles();
    
    // UI元素
    lv_obj_t* time_label_ = nullptr;
    lv_obj_t* date_label_ = nullptr;
    lv_obj_t* lunar_label_ = nullptr;
    
    // 字体
    const lv_font_t* time_font_ = nullptr;
    const lv_font_t* date_font_ = nullptr;
    const lv_font_t* lunar_font_ = nullptr;
    
    // 数据
    ClockData current_data_;
    bool show_lunar_ = true;
    bool is_24h_format_ = true;
    
    // 样式
    lv_style_t time_style_;
    lv_style_t date_style_;
    lv_style_t lunar_style_;
    bool styles_initialized_ = false;
};

#endif // CLOCK_WIDGET_H 