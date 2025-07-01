#include "idle_display.h"
#include "widgets/clock_widget.h"
#include "widgets/weather_widget.h"
#include "widgets/animation_widget.h"
#include <esp_log.h>
#include <esp_timer.h>

static const char* TAG = "IdleDisplay";

// 动画定时器回调
static void animation_timer_callback(void* arg) {
    IdleDisplay* display = static_cast<IdleDisplay*>(arg);
    if (display) {
        display->OnAnimationTimer();
    }
}

IdleDisplay::IdleDisplay() {
    ESP_LOGI(TAG, "IdleDisplay constructor called");
}

IdleDisplay::~IdleDisplay() {
    ESP_LOGI(TAG, "IdleDisplay destructor called");
    Stop();
}

IdleDisplay& IdleDisplay::GetInstance() {
    static IdleDisplay instance;
    return instance;
}

void IdleDisplay::Init() {
    ESP_LOGI(TAG, "Initializing IdleDisplay");
    
    // 初始化主题
    theme_.background_color = 0x000000;
    theme_.text_color = 0xFFFFFF;
    theme_.border_color = 0xCCCCCC;
    theme_.widget_bg_color = 0x1A1A1A;
    theme_.border_width = 1;
    theme_.margin = 20;
    theme_.padding = 15;
    theme_.widget_spacing = 20;
    
    // 初始化布局
    layout_.widget_width = 240;
    layout_.widget_height = 200;
    layout_.forecast_height = 120;
    layout_.clock_height = 150;
    layout_.show_lunar = true;
    layout_.show_air_quality = true;
    layout_.show_uv_index = true;

    ESP_LOGI(TAG, "lv_display_get_default() = %p", lv_display_get_default());
    assert(lv_display_get_default() != nullptr);
    
    ESP_LOGI(TAG, "IdleDisplay initialized");
}

void IdleDisplay::Start() {
    if (is_active_) {
        ESP_LOGW(TAG, "IdleDisplay is already active");
        return;
    }
    if (lv_display_get_default() == nullptr) {
        ESP_LOGW("IdleDisplay", "LVGL未初始化，待机显示延迟启动");
        // 可选：设置标志，主循环或定时器里重试
        return;
    }
    
    ESP_LOGI(TAG, "Starting IdleDisplay");
    
    CreateMainScreen();
    CreateWidgets();
    UpdateLayout();
    ApplyTheme();
    
    is_active_ = true;
    is_visible_ = true;
    
    // 显示主屏幕
    if (main_screen_) {
        lv_scr_load(main_screen_);
    }
    
    ESP_LOGI(TAG, "lv_display_get_default() = %p", lv_display_get_default());
    assert(lv_display_get_default() != nullptr);

    ESP_LOGI(TAG, "IdleDisplay started successfully");
}

void IdleDisplay::Stop() {
    if (!is_active_) {
        return;
    }
    
    ESP_LOGI(TAG, "Stopping IdleDisplay");
    
    StopAnimationTimer();
    
    // 销毁组件
    clock_widget_.reset();
    weather_widget_.reset();
    animation_widget_.reset();
    
    // 销毁主屏幕
    if (main_screen_) {
        lv_obj_del(main_screen_);
        main_screen_ = nullptr;
    }
    
    container_ = nullptr;
    is_active_ = false;
    is_visible_ = false;
    
    ESP_LOGI(TAG, "IdleDisplay stopped");
}

void IdleDisplay::Update() {
    if (!is_active_) return;
    
    // 更新所有组件
    if (clock_widget_) {
        clock_widget_->Update();
    }
    if (weather_widget_) {
        weather_widget_->Update();
    }
    if (animation_widget_) {
        animation_widget_->Update();
    }
}

void IdleDisplay::ShowClock(const ClockData& data) {
    current_clock_data_ = data;
    if (clock_widget_) {
        clock_widget_->SetData(data);
    }
}

void IdleDisplay::ShowWeather(const WeatherData& data) {
    current_weather_data_ = data;
    if (weather_widget_) {
        weather_widget_->SetData(data);
    }
}

void IdleDisplay::ShowAnimation(const AnimationData& data) {
    current_animation_data_ = data;
    if (animation_widget_) {
        animation_widget_->SetData(data);
    }
}

void IdleDisplay::SetTheme(const IdleTheme& theme) {
    theme_ = theme;
    ApplyTheme();
}

void IdleDisplay::SetLayout(const LayoutConfig& layout) {
    layout_ = layout;
    UpdateLayout();
}

void IdleDisplay::SetLanguage(const std::string& language) {
    language_ = language;
    // 这里可以添加语言切换逻辑
}

void IdleDisplay::CreateMainScreen() {
    // 创建主屏幕
    main_screen_ = lv_obj_create(nullptr);
    if (!main_screen_) {
        ESP_LOGE(TAG, "Failed to create main screen");
        return;
    }
    
    // 设置屏幕大小
    lv_obj_set_size(main_screen_, 800, 480);
    lv_obj_set_style_bg_color(main_screen_, lv_color_hex(theme_.background_color), 0);
    lv_obj_set_style_bg_opa(main_screen_, LV_OPA_COVER, 0);
    
    // 创建主容器
    container_ = lv_obj_create(main_screen_);
    if (!container_) {
        ESP_LOGE(TAG, "Failed to create main container");
        return;
    }
    
    // 设置容器属性
    lv_obj_set_size(container_, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(container_, 0, 0);
    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container_, 0, 0);
    lv_obj_set_style_pad_all(container_, theme_.margin, 0);
    
    ESP_LOGI(TAG, "Main screen created successfully");
}

void IdleDisplay::CreateWidgets() {
    if (!container_) {
        ESP_LOGE(TAG, "Container is null, cannot create widgets");
        return;
    }
    
    // 创建时钟组件
    clock_widget_ = std::make_unique<ClockWidget>(container_);
    if (clock_widget_) {
        clock_widget_->SetData(current_clock_data_);
        clock_widget_->ApplyTheme(theme_);
    }
    
    // 创建天气组件
    weather_widget_ = std::make_unique<WeatherWidget>(container_);
    if (weather_widget_) {
        weather_widget_->SetData(current_weather_data_);
        weather_widget_->ApplyTheme(theme_);
    }
    
    // 创建动画组件
    animation_widget_ = std::make_unique<AnimationWidget>(container_);
    if (animation_widget_) {
        animation_widget_->SetData(current_animation_data_);
        animation_widget_->ApplyTheme(theme_);
    }
    
    ESP_LOGI(TAG, "All widgets created successfully");
}

void IdleDisplay::UpdateLayout() {
    if (!container_) return;
    
    // 计算布局位置
    int x = theme_.margin;
    int y = theme_.margin;
    
    // 时钟组件 - 顶部居中
    if (clock_widget_) {
        int clock_width = layout_.widget_width * 2;  // 时钟组件宽度加倍
        int clock_x = (800 - clock_width) / 2;  // 居中显示
        clock_widget_->SetPosition(clock_x, y);
        clock_widget_->SetSize(clock_width, layout_.clock_height);
        clock_widget_->UpdateLayout(layout_);
        y += layout_.clock_height + theme_.widget_spacing;
    }
    
    // 底部三个组件横向排列
    int bottom_y = 480 - layout_.widget_height - theme_.margin;
    int widget_x = x;
    
    // 天气组件 - 左侧
    if (weather_widget_) {
        weather_widget_->SetPosition(widget_x, bottom_y);
        weather_widget_->SetSize(layout_.widget_width, layout_.widget_height);
        weather_widget_->UpdateLayout(layout_);
        widget_x += layout_.widget_width + theme_.widget_spacing;
    }
    
    // 动画组件 - 中间
    if (animation_widget_) {
        animation_widget_->SetPosition(widget_x, bottom_y);
        animation_widget_->SetSize(layout_.widget_width, layout_.widget_height);
        animation_widget_->UpdateLayout(layout_);
        widget_x += layout_.widget_width + theme_.widget_spacing;
    }
    
    // 预留右侧空间给其他组件
    
    ESP_LOGI(TAG, "Layout updated");
}

void IdleDisplay::ApplyTheme() {
    if (!main_screen_) return;
    
    // 应用主题到主屏幕
    lv_obj_set_style_bg_color(main_screen_, lv_color_hex(theme_.background_color), 0);
    
    // 应用主题到所有组件
    if (clock_widget_) {
        clock_widget_->ApplyTheme(theme_);
    }
    if (weather_widget_) {
        weather_widget_->ApplyTheme(theme_);
    }
    if (animation_widget_) {
        animation_widget_->ApplyTheme(theme_);
    }
    
    ESP_LOGI(TAG, "Theme applied");
}

void IdleDisplay::StartAnimationTimer() {
    if (animation_timer_) {
        esp_timer_stop(animation_timer_);
        esp_timer_delete(animation_timer_);
        animation_timer_ = nullptr;
    }
    
    esp_timer_create_args_t timer_args = {
        .callback = animation_timer_callback,
        .arg = this,
        .name = "idle_animation_timer"
    };
    
    esp_err_t ret = esp_timer_create(&timer_args, &animation_timer_);
    if (ret == ESP_OK && animation_timer_) {
        esp_timer_start_periodic(animation_timer_, 1000000);  // 1秒间隔
        ESP_LOGI(TAG, "Animation timer started");
    } else {
        ESP_LOGE(TAG, "Failed to create animation timer");
    }
}

void IdleDisplay::StopAnimationTimer() {
    if (animation_timer_) {
        esp_timer_stop(animation_timer_);
        esp_timer_delete(animation_timer_);
        animation_timer_ = nullptr;
        ESP_LOGI(TAG, "Animation timer stopped");
    }
}

void IdleDisplay::OnAnimationTimer() {
    // 定时更新动画
    if (animation_widget_) {
        animation_widget_->Update();
    }
} 