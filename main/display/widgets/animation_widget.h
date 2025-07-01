#ifndef ANIMATION_WIDGET_H
#define ANIMATION_WIDGET_H

#include "base_widget.h"
#include "idle_display.h"
#include <esp_timer.h>
#include <string>
#include <vector>

// 前向声明
struct AnimationData;

// 动画组件
class AnimationWidget : public BaseWidget {
public:
    AnimationWidget(lv_obj_t* parent);
    ~AnimationWidget() override;
    
    // 核心接口实现
    void Update() override;
    void SetData(const AnimationData& data);
    
    // 布局相关
    int GetPreferredWidth() const override { return 240; }
    int GetPreferredHeight() const override { return 200; }
    void UpdateLayout(const LayoutConfig& layout) override;
    
    // 动画控制
    void StartAnimation();
    void StopAnimation();
    void PauseAnimation();
    void ResumeAnimation();
    void SetFrameInterval(int interval_ms);
    void SetLoop(bool loop);
    
    // 显示控制
    void SetFramePaths(const std::vector<std::string>& paths);
    void SetCurrentFrame(int frame_index);
    void ShowFrame(int frame_index);
    void OnFrameTimer();
    
private:
    // 私有方法
    void CreateWidgets();
    void DestroyWidgets();
    void LoadFrameImage(const std::string& path);
    void NextFrame();
    void PreviousFrame();
    void UpdateFrameDisplay();
    void StartFrameTimer();
    void StopFrameTimer();
    void UpdateAnimation();
    void UpdateInfo();
    void ApplyTextStyles();
    
    
    // UI元素
    lv_obj_t* image_widget_ = nullptr;
    lv_obj_t* frame_label_ = nullptr;
    
    // 动画数据
    AnimationData current_data_;
    bool is_playing_ = false;
    bool is_looping_ = true;
    
    // 定时器
    esp_timer_handle_t frame_timer_ = nullptr;
    
    // 图片资源
    std::vector<const lv_img_dsc_t*> frame_images_;
    int current_frame_index_ = 0;
    
    // 样式
    lv_style_t image_style_;
    lv_style_t label_style_;
    bool styles_initialized_ = false;
};

#endif // ANIMATION_WIDGET_H 