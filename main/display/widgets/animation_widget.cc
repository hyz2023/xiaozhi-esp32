#include "animation_widget.h"
#include <esp_log.h>
#include <esp_timer.h>

static const char* TAG = "AnimationWidget";

// 定时器回调函数
static void frame_timer_callback(void* arg) {
    AnimationWidget* widget = static_cast<AnimationWidget*>(arg);
    if (widget) {
        widget->OnFrameTimer();
    }
}

AnimationWidget::AnimationWidget(lv_obj_t* parent)
    : BaseWidget(parent, "AnimationWidget") {
    ESP_LOGI(TAG, "Creating AnimationWidget");
    CreateWidgets();
}

AnimationWidget::~AnimationWidget() {
    ESP_LOGI(TAG, "Destroying AnimationWidget");
    StopAnimation();
    DestroyWidgets();
}

void AnimationWidget::CreateWidgets() {
    if (!container_) {
        ESP_LOGE(TAG, "Container is null, cannot create widgets");
        return;
    }
    
    // 创建图片显示组件
    image_widget_ = lv_img_create(container_);
    if (image_widget_) {
        lv_obj_set_size(image_widget_, 200, 150);
        lv_obj_set_pos(image_widget_, 20, 10);
        lv_obj_set_style_bg_opa(image_widget_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(image_widget_, 0, 0);
        
        // 设置默认图片（可以是占位符）
        lv_img_set_src(image_widget_, LV_SYMBOL_IMAGE);
    }
    
    // 创建帧信息标签
    frame_label_ = lv_label_create(container_);
    if (frame_label_) {
        lv_label_set_text(frame_label_, "动画区域");
        lv_obj_set_style_text_align(frame_label_, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_pos(frame_label_, 0, 170);
        lv_obj_set_width(frame_label_, lv_pct(100));
    }
    
    ApplyTextStyles();
    ESP_LOGI(TAG, "Animation widgets created successfully");
}

void AnimationWidget::DestroyWidgets() {
    if (image_widget_) {
        lv_obj_del(image_widget_);
        image_widget_ = nullptr;
    }
    if (frame_label_) {
        lv_obj_del(frame_label_);
        frame_label_ = nullptr;
    }
    ESP_LOGI(TAG, "Animation widgets destroyed");
}

void AnimationWidget::Update() {
    UpdateFrameDisplay();
}

void AnimationWidget::SetData(const AnimationData& data) {
    current_data_ = data;
    current_frame_index_ = data.current_frame;
    is_looping_ = data.is_playing;
    
    // 加载帧图片
    frame_images_.clear();
    for (const auto& path : data.frame_paths) {
        LoadFrameImage(path);
    }
    
    // 显示当前帧
    ShowFrame(current_frame_index_);
    
    // 如果设置了自动播放，开始动画
    if (data.is_playing) {
        StartAnimation();
    }
}

void AnimationWidget::UpdateLayout(const LayoutConfig& layout) {
    BaseWidget::UpdateLayout(layout);
    
    // 调整组件大小和位置
    SetSize(layout.widget_width, layout.widget_height);
    
    // 重新定位组件
    if (image_widget_) {
        int image_width = layout.widget_width - 40;
        int image_height = layout.widget_height - 60;
        lv_obj_set_size(image_widget_, image_width, image_height);
        lv_obj_set_pos(image_widget_, 20, 10);
    }
    
    if (frame_label_) {
        lv_obj_set_pos(frame_label_, 0, layout.widget_height - 30);
    }
    
    ESP_LOGI(TAG, "Animation layout updated");
}

void AnimationWidget::StartAnimation() {
    if (is_playing_ || frame_images_.empty()) {
        return;
    }
    
    is_playing_ = true;
    StartFrameTimer();
    ESP_LOGI(TAG, "Animation started");
}

void AnimationWidget::StopAnimation() {
    if (!is_playing_) {
        return;
    }
    
    is_playing_ = false;
    StopFrameTimer();
    ESP_LOGI(TAG, "Animation stopped");
}

void AnimationWidget::PauseAnimation() {
    if (!is_playing_) {
        return;
    }
    
    is_playing_ = false;
    StopFrameTimer();
    ESP_LOGI(TAG, "Animation paused");
}

void AnimationWidget::ResumeAnimation() {
    if (is_playing_ || frame_images_.empty()) {
        return;
    }
    
    is_playing_ = true;
    StartFrameTimer();
    ESP_LOGI(TAG, "Animation resumed");
}

void AnimationWidget::SetFrameInterval(int interval_ms) {
    current_data_.frame_interval_ms = interval_ms;
    if (is_playing_) {
        StopFrameTimer();
        StartFrameTimer();
    }
}

void AnimationWidget::SetLoop(bool loop) {
    is_looping_ = loop;
}

void AnimationWidget::SetFramePaths(const std::vector<std::string>& paths) {
    current_data_.frame_paths = paths;
    frame_images_.clear();
    
    for (const auto& path : paths) {
        LoadFrameImage(path);
    }
    
    if (!frame_images_.empty()) {
        ShowFrame(0);
    }
}

void AnimationWidget::SetCurrentFrame(int frame_index) {
    if (frame_index >= 0 && frame_index < static_cast<int>(frame_images_.size())) {
        current_frame_index_ = frame_index;
        ShowFrame(frame_index);
    }
}

void AnimationWidget::ShowFrame(int frame_index) {
    if (frame_index < 0 || frame_index >= static_cast<int>(frame_images_.size())) {
        return;
    }
    
    current_frame_index_ = frame_index;
    
    if (image_widget_ && frame_images_[frame_index]) {
        lv_img_set_src(image_widget_, frame_images_[frame_index]);
    }
    
    // 更新帧信息标签
    if (frame_label_) {
        std::string frame_info = "帧 " + std::to_string(frame_index + 1) + "/" + 
                                std::to_string(frame_images_.size());
        lv_label_set_text(frame_label_, frame_info.c_str());
    }
    
    ESP_LOGD(TAG, "Showing frame %d", frame_index);
}

void AnimationWidget::LoadFrameImage(const std::string& path) {
    // 这里应该根据路径加载图片资源
    // 暂时使用占位符
    ESP_LOGI(TAG, "Loading frame image: %s", path.c_str());
    
    // 在实际实现中，这里应该：
    // 1. 检查文件是否存在
    // 2. 加载图片数据
    // 3. 创建lv_img_dsc_t结构
    // 4. 添加到frame_images_向量中
    
    // 暂时添加nullptr作为占位符
    frame_images_.push_back(nullptr);
}

void AnimationWidget::NextFrame() {
    if (frame_images_.empty()) return;
    
    int next_frame = current_frame_index_ + 1;
    if (next_frame >= static_cast<int>(frame_images_.size())) {
        if (is_looping_) {
            next_frame = 0;
        } else {
            StopAnimation();
            return;
        }
    }
    
    ShowFrame(next_frame);
}

void AnimationWidget::PreviousFrame() {
    if (frame_images_.empty()) return;
    
    int prev_frame = current_frame_index_ - 1;
    if (prev_frame < 0) {
        if (is_looping_) {
            prev_frame = static_cast<int>(frame_images_.size()) - 1;
        } else {
            return;
        }
    }
    
    ShowFrame(prev_frame);
}

void AnimationWidget::UpdateFrameDisplay() {
    if (!frame_images_.empty()) {
        ShowFrame(current_frame_index_);
    }
}

void AnimationWidget::StartFrameTimer() {
    if (frame_timer_) {
        esp_timer_stop(frame_timer_);
        esp_timer_delete(frame_timer_);
        frame_timer_ = nullptr;
    }
    
    esp_timer_create_args_t timer_args = {
        .callback = frame_timer_callback,
        .arg = this,
        .name = "animation_frame_timer"
    };
    
    esp_err_t ret = esp_timer_create(&timer_args, &frame_timer_);
    if (ret == ESP_OK && frame_timer_) {
        esp_timer_start_periodic(frame_timer_, current_data_.frame_interval_ms * 1000);
        ESP_LOGI(TAG, "Frame timer started with interval %d ms", current_data_.frame_interval_ms);
    } else {
        ESP_LOGE(TAG, "Failed to create frame timer");
    }
}

void AnimationWidget::StopFrameTimer() {
    if (frame_timer_) {
        esp_timer_stop(frame_timer_);
        esp_timer_delete(frame_timer_);
        frame_timer_ = nullptr;
        ESP_LOGI(TAG, "Frame timer stopped");
    }
}

void AnimationWidget::OnFrameTimer() {
    NextFrame();
}

void AnimationWidget::ApplyTextStyles() {
    if (styles_initialized_) return;
    
    // 初始化样式
    lv_style_init(&image_style_);
    lv_style_init(&label_style_);
    
    // 图片样式
    lv_style_set_bg_opa(&image_style_, LV_OPA_TRANSP);
    lv_style_set_border_width(&image_style_, 0);
    
    // 标签样式
    lv_style_set_text_color(&label_style_, lv_color_hex(0xCCCCCC));
    lv_style_set_text_align(&label_style_, LV_TEXT_ALIGN_CENTER);
    
    // 应用样式
    if (image_widget_) {
        lv_obj_add_style(image_widget_, &image_style_, 0);
    }
    if (frame_label_) {
        lv_obj_add_style(frame_label_, &label_style_, 0);
    }
    
    styles_initialized_ = true;
    ESP_LOGI(TAG, "Animation styles applied");
} 