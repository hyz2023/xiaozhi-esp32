#include "idle_config.h"
#include <esp_log.h>
#include <esp_spiffs.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

static const char* TAG = "IdleConfig";

// IdleConfig 实现
nlohmann::json IdleConfig::ToJson() const {
    nlohmann::json j;
    
    // 时钟配置
    j["clock"]["format"] = clock.format;
    j["clock"]["timezone"] = clock.timezone;
    j["clock"]["date_format"] = clock.date_format;
    j["clock"]["show_lunar"] = clock.show_lunar;
    j["clock"]["language"] = clock.language;
    
    // 天气配置
    j["weather"]["city"] = weather.city;
    j["weather"]["update_interval"] = weather.update_interval;
    j["weather"]["units"] = weather.units;
    j["weather"]["show_air_quality"] = weather.show_air_quality;
    j["weather"]["show_uv_index"] = weather.show_uv_index;
    j["weather"]["api_key"] = weather.api_key;
    j["weather"]["api_url"] = weather.api_url;
    
    // 动画配置
    j["animation"]["frame_interval"] = animation.frame_interval;
    j["animation"]["frames"] = animation.frames;
    j["animation"]["auto_play"] = animation.auto_play;
    j["animation"]["loop"] = animation.loop;
    
    // 显示配置
    j["display"]["width"] = display.width;
    j["display"]["height"] = display.height;
    j["display"]["refresh_rate"] = display.refresh_rate;
    j["display"]["power_save"]["enabled"] = display.power_save.enabled;
    j["display"]["power_save"]["timeout"] = display.power_save.timeout;
    j["display"]["power_save"]["brightness"] = display.power_save.brightness;
    
    // 资源配置
    j["resources"]["fonts"]["clock"] = resources.fonts.clock;
    j["resources"]["fonts"]["text"] = resources.fonts.text;
    j["resources"]["fonts"]["icon"] = resources.fonts.icon;
    j["resources"]["images"]["weather_icons"] = resources.images.weather_icons;
    j["resources"]["images"]["animation_frames"] = resources.images.animation_frames;
    
    // 布局配置
    j["layout"]["margin"] = layout.margin;
    j["layout"]["padding"] = layout.padding;
    j["layout"]["widget_spacing"] = layout.widget_spacing;
    j["layout"]["widget_width"] = layout.widget_width;
    j["layout"]["widget_height"] = layout.widget_height;
    j["layout"]["forecast_height"] = layout.forecast_height;
    j["layout"]["border_width"] = layout.border_width;
    j["layout"]["border_color"] = layout.border_color;
    j["layout"]["current_layout"] = layout.current_layout;
    
    // 语言配置
    j["language"] = language;
    
    return j;
}

void IdleConfig::FromJson(const nlohmann::json& j) {
    try {
        // 时钟配置
        if (j.contains("clock")) {
            const auto& c = j["clock"];
            if (c.contains("format")) clock.format = c["format"];
            if (c.contains("timezone")) clock.timezone = c["timezone"];
            if (c.contains("date_format")) clock.date_format = c["date_format"];
            if (c.contains("show_lunar")) clock.show_lunar = c["show_lunar"];
            if (c.contains("language")) clock.language = c["language"];
        }
        
        // 天气配置
        if (j.contains("weather")) {
            const auto& w = j["weather"];
            if (w.contains("city")) weather.city = w["city"];
            if (w.contains("update_interval")) weather.update_interval = w["update_interval"];
            if (w.contains("units")) weather.units = w["units"];
            if (w.contains("show_air_quality")) weather.show_air_quality = w["show_air_quality"];
            if (w.contains("show_uv_index")) weather.show_uv_index = w["show_uv_index"];
            if (w.contains("api_key")) weather.api_key = w["api_key"];
            if (w.contains("api_url")) weather.api_url = w["api_url"];
        }
        
        // 动画配置
        if (j.contains("animation")) {
            const auto& a = j["animation"];
            if (a.contains("frame_interval")) animation.frame_interval = a["frame_interval"];
            if (a.contains("frames")) animation.frames = a["frames"];
            if (a.contains("auto_play")) animation.auto_play = a["auto_play"];
            if (a.contains("loop")) animation.loop = a["loop"];
        }
        
        // 显示配置
        if (j.contains("display")) {
            const auto& d = j["display"];
            if (d.contains("width")) display.width = d["width"];
            if (d.contains("height")) display.height = d["height"];
            if (d.contains("refresh_rate")) display.refresh_rate = d["refresh_rate"];
            if (d.contains("power_save")) {
                const auto& ps = d["power_save"];
                if (ps.contains("enabled")) display.power_save.enabled = ps["enabled"];
                if (ps.contains("timeout")) display.power_save.timeout = ps["timeout"];
                if (ps.contains("brightness")) display.power_save.brightness = ps["brightness"];
            }
        }
        
        // 资源配置
        if (j.contains("resources")) {
            const auto& r = j["resources"];
            if (r.contains("fonts")) {
                const auto& f = r["fonts"];
                if (f.contains("clock")) resources.fonts.clock = f["clock"];
                if (f.contains("text")) resources.fonts.text = f["text"];
                if (f.contains("icon")) resources.fonts.icon = f["icon"];
            }
            if (r.contains("images")) {
                const auto& i = r["images"];
                if (i.contains("weather_icons")) resources.images.weather_icons = i["weather_icons"];
                if (i.contains("animation_frames")) resources.images.animation_frames = i["animation_frames"];
            }
        }
        
        // 布局配置
        if (j.contains("layout")) {
            const auto& l = j["layout"];
            if (l.contains("margin")) layout.margin = l["margin"];
            if (l.contains("padding")) layout.padding = l["padding"];
            if (l.contains("widget_spacing")) layout.widget_spacing = l["widget_spacing"];
            if (l.contains("widget_width")) layout.widget_width = l["widget_width"];
            if (l.contains("widget_height")) layout.widget_height = l["widget_height"];
            if (l.contains("forecast_height")) layout.forecast_height = l["forecast_height"];
            if (l.contains("border_width")) layout.border_width = l["border_width"];
            if (l.contains("border_color")) layout.border_color = l["border_color"];
            if (l.contains("current_layout")) layout.current_layout = l["current_layout"];
        }
        
        // 语言配置
        if (j.contains("language")) {
            language = j["language"];
        }
        
    } catch (const std::exception& e) {
        ESP_LOGE(TAG, "Error parsing config JSON: %s", e.what());
    }
}

bool IdleConfig::Validate() const {
    // 基本验证
    if (clock.format != "12h" && clock.format != "24h") {
        ESP_LOGE(TAG, "Invalid clock format: %s", clock.format.c_str());
        return false;
    }
    
    if (weather.update_interval < 60 || weather.update_interval > 86400) {
        ESP_LOGE(TAG, "Invalid weather update interval: %d", weather.update_interval);
        return false;
    }
    
    if (weather.units != "metric" && weather.units != "imperial") {
        ESP_LOGE(TAG, "Invalid weather units: %s", weather.units.c_str());
        return false;
    }
    
    if (animation.frame_interval < 100 || animation.frame_interval > 10000) {
        ESP_LOGE(TAG, "Invalid animation frame interval: %d", animation.frame_interval);
        return false;
    }
    
    if (display.width <= 0 || display.height <= 0) {
        ESP_LOGE(TAG, "Invalid display dimensions: %dx%d", display.width, display.height);
        return false;
    }
    
    if (layout.widget_width <= 0 || layout.widget_height <= 0) {
        ESP_LOGE(TAG, "Invalid widget dimensions: %dx%d", layout.widget_width, layout.widget_height);
        return false;
    }
    
    return true;
}

IdleConfig IdleConfig::GetDefault() {
    IdleConfig config;
    
    // 使用默认值（构造函数已设置）
    return config;
}

// ConfigManager 实现
ConfigManager::ConfigManager() {
    ESP_LOGI(TAG, "ConfigManager constructor called");
}

ConfigManager::~ConfigManager() {
    ESP_LOGI(TAG, "ConfigManager destructor called");
}

ConfigManager& ConfigManager::GetInstance() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::LoadConfig(const std::string& file_path) {
    config_path_ = file_path;
    
    ESP_LOGI(TAG, "Loading config from: %s", file_path.c_str());
    
    // 检查文件是否存在
    FILE* file = fopen(file_path.c_str(), "r");
    if (!file) {
        ESP_LOGW(TAG, "Config file not found, creating default config");
        return CreateDefaultConfig();
    }
    fclose(file);
    
    try {
        // 读取文件内容
        std::ifstream ifs(file_path);
        if (!ifs.is_open()) {
            ESP_LOGE(TAG, "Failed to open config file");
            return false;
        }
        
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string content = buffer.str();
        
        // 解析JSON
        nlohmann::json j = nlohmann::json::parse(content);
        config_.FromJson(j);
        
        // 验证配置
        if (!config_.Validate()) {
            ESP_LOGW(TAG, "Config validation failed, using default config");
            config_ = IdleConfig::GetDefault();
        }
        
        config_loaded_ = true;
        ESP_LOGI(TAG, "Config loaded successfully");
        return true;
        
    } catch (const std::exception& e) {
        ESP_LOGE(TAG, "Error loading config: %s", e.what());
        return CreateDefaultConfig();
    }
}

bool ConfigManager::SaveConfig(const std::string& file_path) {
    if (!config_loaded_) {
        ESP_LOGW(TAG, "No config loaded, cannot save");
        return false;
    }
    
    ESP_LOGI(TAG, "Saving config to: %s", file_path.c_str());
    
    try {
        // 确保目录存在
        if (!EnsureConfigDirectory()) {
            ESP_LOGE(TAG, "Failed to ensure config directory");
            return false;
        }
        
        // 序列化为JSON
        nlohmann::json j = config_.ToJson();
        std::string content = j.dump(2);  // 美化输出
        
        // 写入文件
        std::ofstream ofs(file_path);
        if (!ofs.is_open()) {
            ESP_LOGE(TAG, "Failed to open config file for writing");
            return false;
        }
        
        ofs << content;
        ofs.close();
        
        ESP_LOGI(TAG, "Config saved successfully");
        return true;
        
    } catch (const std::exception& e) {
        ESP_LOGE(TAG, "Error saving config: %s", e.what());
        return false;
    }
}

void ConfigManager::SetConfig(const IdleConfig& config) {
    config_ = config;
    config_loaded_ = true;
    ESP_LOGI(TAG, "Config set");
}

void ConfigManager::UpdateClockConfig(const ClockConfig& config) {
    config_.clock = config;
    ESP_LOGI(TAG, "Clock config updated");
}

void ConfigManager::UpdateWeatherConfig(const WeatherConfig& config) {
    config_.weather = config;
    ESP_LOGI(TAG, "Weather config updated");
}

void ConfigManager::UpdateAnimationConfig(const AnimationConfig& config) {
    config_.animation = config;
    ESP_LOGI(TAG, "Animation config updated");
}

void ConfigManager::UpdateLayoutConfig(const LayoutConfig& config) {
    config_.layout = config;
    ESP_LOGI(TAG, "Layout config updated");
}

bool ConfigManager::ValidateAndFix() {
    if (!config_.Validate()) {
        ESP_LOGW(TAG, "Config validation failed, fixing...");
        
        // 尝试修复配置
        if (config_.clock.format != "12h" && config_.clock.format != "24h") {
            config_.clock.format = "24h";
        }
        
        if (config_.weather.update_interval < 60) {
            config_.weather.update_interval = 3600;
        }
        
        if (config_.weather.units != "metric" && config_.weather.units != "imperial") {
            config_.weather.units = "metric";
        }
        
        if (config_.animation.frame_interval < 100) {
            config_.animation.frame_interval = 1000;
        }
        
        if (config_.display.width <= 0) {
            config_.display.width = 800;
        }
        
        if (config_.display.height <= 0) {
            config_.display.height = 480;
        }
        
        ESP_LOGI(TAG, "Config fixed");
        return true;
    }
    
    return true;
}

bool ConfigManager::CreateDefaultConfig() {
    config_ = IdleConfig::GetDefault();
    config_loaded_ = true;
    
    // 保存默认配置
    return SaveConfig(config_path_);
}

bool ConfigManager::EnsureConfigDirectory() {
    // 获取目录路径
    size_t last_slash = config_path_.find_last_of('/');
    if (last_slash == std::string::npos) {
        return true;  // 没有目录部分
    }
    
    std::string dir_path = config_path_.substr(0, last_slash);
    
    // 检查目录是否存在
    struct stat st;
    if (stat(dir_path.c_str(), &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    
    // 创建目录
    ESP_LOGI(TAG, "Creating config directory: %s", dir_path.c_str());
    return mkdir(dir_path.c_str(), 0755) == 0;
}

void ConfigManager::LogConfigError(const std::string& error) {
    ESP_LOGE(TAG, "Config error: %s", error.c_str());
} 