#ifndef IDLE_DISPLAY_H
#define IDLE_DISPLAY_H

#include <lvgl.h>
#include <esp_timer.h>
#include <memory>
#include <vector>
#include <string>
#include <functional>

// 前向声明
class BaseWidget;
class ClockWidget;
class WeatherWidget;
class AnimationWidget;

// 时钟数据结构
struct ClockData {
    std::string time_str;
    std::string date_str;
    std::string lunar_date;
    bool is_24h_format = true;
    std::string timezone = "Asia/Shanghai";
};

// 天气数据结构
struct WeatherData {
    struct Current {
        float temperature = 0.0f;
        std::string condition;
        std::string icon;
        int humidity = 0;
        float wind_speed = 0.0f;
        std::string city;
        
        struct AirQuality {
            int aqi = 0;
            std::string level;
            int pm2_5 = 0;
            int pm10 = 0;
        } air_quality;
        
        struct UVIndex {
            int value = 0;
            std::string level;
            std::string protection;
        } uv_index;
    } current;
    
    struct Forecast {
        std::string date;
        std::string lunar_date;
        float max_temp = 0.0f;
        float min_temp = 0.0f;
        std::string condition;
        std::string icon;
        int aqi = 0;
        std::string aqi_level;
    };
    
    std::vector<Forecast> forecast;
};

// 动画数据结构
struct AnimationData {
    std::vector<std::string> frame_paths;
    int current_frame = 0;
    int frame_interval_ms = 1000;
    bool is_playing = false;
};

// 主题配置
struct IdleTheme {
    uint32_t background_color = 0x000000;
    uint32_t text_color = 0xFFFFFF;
    uint32_t border_color = 0xCCCCCC;
    uint32_t widget_bg_color = 0x1A1A1A;
    int border_width = 1;
    int margin = 20;
    int padding = 15;
    int widget_spacing = 20;
};

// 布局配置
struct LayoutConfig {
    int widget_width = 240;
    int widget_height = 200;
    int forecast_height = 120;
    int clock_height = 150;
    bool show_lunar = true;
    bool show_air_quality = true;
    bool show_uv_index = true;
};

// 待机显示核心类
class IdleDisplay {
public:
    static IdleDisplay& GetInstance();
    
    // 删除拷贝构造函数和赋值运算符
    IdleDisplay(const IdleDisplay&) = delete;
    IdleDisplay& operator=(const IdleDisplay&) = delete;
    
    // 生命周期管理
    void Init();
    void Start();
    void Stop();
    void Update();
    
    // 显示控制
    void ShowClock(const ClockData& data);
    void ShowWeather(const WeatherData& data);
    void ShowAnimation(const AnimationData& data);
    
    // 配置管理
    void SetTheme(const IdleTheme& theme);
    void SetLayout(const LayoutConfig& layout);
    void SetLanguage(const std::string& language);
    
    // 状态查询
    bool IsActive() const { return is_active_; }
    bool IsVisible() const { return is_visible_; }
    
    // 工具方法
    lv_obj_t* GetMainScreen() const { return main_screen_; }
    const IdleTheme& GetTheme() const { return theme_; }
    const LayoutConfig& GetLayout() const { return layout_; }
    
    // 动画定时器回调 - 改为public
    void OnAnimationTimer();

private:
    IdleDisplay();
    ~IdleDisplay();
    
    // 私有方法
    void CreateMainScreen();
    void CreateWidgets();
    void UpdateLayout();
    void ApplyTheme();
    void StartAnimationTimer();
    void StopAnimationTimer();
    
    // 成员变量
    bool is_active_ = false;
    bool is_visible_ = false;
    std::string language_ = "zh_CN";
    
    lv_obj_t* main_screen_ = nullptr;
    lv_obj_t* container_ = nullptr;
    
    // 组件
    std::unique_ptr<ClockWidget> clock_widget_;
    std::unique_ptr<WeatherWidget> weather_widget_;
    std::unique_ptr<AnimationWidget> animation_widget_;
    
    // 配置
    IdleTheme theme_;
    LayoutConfig layout_;
    
    // 定时器
    esp_timer_handle_t animation_timer_ = nullptr;
    
    // 数据缓存
    ClockData current_clock_data_;
    WeatherData current_weather_data_;
    AnimationData current_animation_data_;
};

#endif // IDLE_DISPLAY_H 