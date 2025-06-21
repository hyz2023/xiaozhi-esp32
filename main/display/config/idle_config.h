#ifndef IDLE_CONFIG_H
#define IDLE_CONFIG_H

#include <string>
#include <memory>
#include <nlohmann/json.hpp>

// 时钟配置
struct ClockConfig {
    std::string format = "24h";  // "12h" or "24h"
    std::string timezone = "Asia/Shanghai";
    std::string date_format = "YYYY-MM-DD";
    bool show_lunar = true;
    std::string language = "zh_CN";
};

// 天气配置
struct WeatherConfig {
    std::string city = "Shanghai";
    int update_interval = 3600;  // 秒
    std::string units = "metric";  // "metric" or "imperial"
    bool show_air_quality = true;
    bool show_uv_index = true;
    std::string api_key = "";
    std::string api_url = "";
};

// 动画配置
struct AnimationConfig {
    int frame_interval = 1000;  // 毫秒
    std::vector<std::string> frames;
    bool auto_play = true;
    bool loop = true;
};

// 显示配置
struct DisplayConfig {
    int width = 800;
    int height = 480;
    int refresh_rate = 30;
    
    struct PowerSave {
        bool enabled = true;
        int timeout = 300;  // 秒
        int brightness = 50;  // 百分比
    } power_save;
};

// 资源配置
struct ResourceConfig {
    struct Fonts {
        std::string clock = "fonts/clock.ttf";
        std::string text = "fonts/text.ttf";
        std::string icon = "fonts/icon.ttf";
    } fonts;
    
    struct Images {
        std::string weather_icons = "images/weather/";
        std::string animation_frames = "images/animation/";
    } images;
};

// 布局配置
struct LayoutConfig {
    int margin = 20;
    int padding = 15;
    int widget_spacing = 20;
    int widget_width = 240;
    int widget_height = 200;
    int forecast_height = 120;
    int border_width = 1;
    std::string border_color = "#CCCCCC";
    int current_layout = 0;  // 布局索引
};

// 完整配置结构
struct IdleConfig {
    ClockConfig clock;
    WeatherConfig weather;
    AnimationConfig animation;
    DisplayConfig display;
    ResourceConfig resources;
    LayoutConfig layout;
    std::string language = "zh_CN";
    
    // 序列化/反序列化
    nlohmann::json ToJson() const;
    void FromJson(const nlohmann::json& json);
    
    // 验证配置
    bool Validate() const;
    
    // 获取默认配置
    static IdleConfig GetDefault();
};

// 配置管理器
class ConfigManager {
public:
    static ConfigManager& GetInstance();
    
    // 删除拷贝构造函数和赋值运算符
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    // 配置管理
    bool LoadConfig(const std::string& file_path = "/spiffs/idle_config.json");
    bool SaveConfig(const std::string& file_path = "/spiffs/idle_config.json");
    void SetConfig(const IdleConfig& config);
    const IdleConfig& GetConfig() const { return config_; }
    
    // 配置更新
    void UpdateClockConfig(const ClockConfig& config);
    void UpdateWeatherConfig(const WeatherConfig& config);
    void UpdateAnimationConfig(const AnimationConfig& config);
    void UpdateLayoutConfig(const LayoutConfig& config);
    
    // 工具方法
    bool IsConfigLoaded() const { return config_loaded_; }
    std::string GetConfigPath() const { return config_path_; }
    
    // 配置验证
    bool ValidateAndFix();
    
private:
    ConfigManager();
    ~ConfigManager();
    
    IdleConfig config_;
    bool config_loaded_ = false;
    std::string config_path_ = "/spiffs/idle_config.json";
    
    // 私有方法
    bool CreateDefaultConfig();
    bool EnsureConfigDirectory();
    void LogConfigError(const std::string& error);
};

#endif // IDLE_CONFIG_H 