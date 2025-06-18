#include "idle_config.h"
#include <fstream>
#include <esp_log.h>

#define TAG "IdleConfig"

namespace xiaozhi {

IdleConfigManager& IdleConfigManager::GetInstance() {
    static IdleConfigManager instance;
    return instance;
}

bool IdleConfigManager::LoadConfig(const std::string& path) {
    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            ESP_LOGE(TAG, "Failed to open config file: %s", path.c_str());
            return false;
        }

        nlohmann::json j;
        file >> j;
        
        // 解析配置
        config_.layout.type = j["layout"]["type"];
        config_.layout.params = j["layout"]["params"];
        
        config_.widgets.clear();
        for (const auto& w : j["widgets"]) {
            WidgetConfig widget;
            widget.type = w["type"];
            widget.id = w["id"];
            widget.params = w["params"];
            config_.widgets.push_back(widget);
        }
        
        config_.theme.name = j["theme"]["name"];
        config_.theme.colors = j["theme"]["colors"];
        config_.theme.fonts = j["theme"]["fonts"];
        config_.theme.styles = j["theme"]["styles"];
        
        config_.update_interval = j["update_interval"];
        config_.auto_enter = j["auto_enter"];
        config_.auto_enter_delay = j["auto_enter_delay"];
        
        if (!ValidateConfig(config_)) {
            ESP_LOGE(TAG, "Invalid config");
            return false;
        }
        
        ESP_LOGI(TAG, "Config loaded successfully");
        return true;
    } catch (const std::exception& e) {
        ESP_LOGE(TAG, "Failed to load config: %s", e.what());
        return false;
    }
}

bool IdleConfigManager::SaveConfig(const std::string& path) {
    try {
        nlohmann::json j;
        
        // 构建配置JSON
        j["layout"]["type"] = config_.layout.type;
        j["layout"]["params"] = config_.layout.params;
        
        j["widgets"] = nlohmann::json::array();
        for (const auto& widget : config_.widgets) {
            nlohmann::json w;
            w["type"] = widget.type;
            w["id"] = widget.id;
            w["params"] = widget.params;
            j["widgets"].push_back(w);
        }
        
        j["theme"]["name"] = config_.theme.name;
        j["theme"]["colors"] = config_.theme.colors;
        j["theme"]["fonts"] = config_.theme.fonts;
        j["theme"]["styles"] = config_.theme.styles;
        
        j["update_interval"] = config_.update_interval;
        j["auto_enter"] = config_.auto_enter;
        j["auto_enter_delay"] = config_.auto_enter_delay;
        
        // 保存到文件
        std::ofstream file(path);
        if (!file.is_open()) {
            ESP_LOGE(TAG, "Failed to open config file for writing: %s", path.c_str());
            return false;
        }
        
        file << j.dump(4);  // 使用4空格缩进
        ESP_LOGI(TAG, "Config saved successfully");
        return true;
    } catch (const std::exception& e) {
        ESP_LOGE(TAG, "Failed to save config: %s", e.what());
        return false;
    }
}

void IdleConfigManager::AddObserver(ConfigObserver* observer) {
    if (observer) {
        observers_.push_back(observer);
        ESP_LOGI(TAG, "Observer added");
    }
}

void IdleConfigManager::RemoveObserver(ConfigObserver* observer) {
    auto it = std::find(observers_.begin(), observers_.end(), observer);
    if (it != observers_.end()) {
        observers_.erase(it);
        ESP_LOGI(TAG, "Observer removed");
    }
}

void IdleConfigManager::NotifyConfigChanged(ConfigChangeType type, const void* data) {
    for (auto observer : observers_) {
        if (observer) {
            observer->OnConfigChanged(type, data);
        }
    }
}

void IdleConfigManager::UpdateConfig(const IdleConfig& config) {
    if (ValidateConfig(config)) {
        config_ = config;
        NotifyConfigChanged(ConfigChangeType::SETTINGS, &config_);
        ESP_LOGI(TAG, "Config updated");
    } else {
        ESP_LOGE(TAG, "Invalid config, update failed");
    }
}

void IdleConfigManager::UpdateLayoutConfig(const LayoutConfig& layout) {
    config_.layout = layout;
    NotifyConfigChanged(ConfigChangeType::LAYOUT, &config_.layout);
    ESP_LOGI(TAG, "Layout config updated");
}

void IdleConfigManager::UpdateWidgetConfig(const std::string& widget_id, const WidgetConfig& widget) {
    for (auto& w : config_.widgets) {
        if (w.id == widget_id) {
            w = widget;
            NotifyConfigChanged(ConfigChangeType::WIDGET, &w);
            ESP_LOGI(TAG, "Widget config updated: %s", widget_id.c_str());
            return;
        }
    }
    ESP_LOGW(TAG, "Widget not found: %s", widget_id.c_str());
}

void IdleConfigManager::UpdateThemeConfig(const ThemeConfig& theme) {
    config_.theme = theme;
    NotifyConfigChanged(ConfigChangeType::THEME, &config_.theme);
    ESP_LOGI(TAG, "Theme config updated");
}

void IdleConfigManager::AddWidget(const WidgetConfig& widget) {
    // 检查ID是否已存在
    for (const auto& w : config_.widgets) {
        if (w.id == widget.id) {
            ESP_LOGW(TAG, "Widget ID already exists: %s", widget.id.c_str());
            return;
        }
    }
    
    config_.widgets.push_back(widget);
    NotifyConfigChanged(ConfigChangeType::WIDGET, &config_.widgets.back());
    ESP_LOGI(TAG, "Widget added: %s", widget.id.c_str());
}

void IdleConfigManager::RemoveWidget(const std::string& widget_id) {
    auto it = std::find_if(config_.widgets.begin(), config_.widgets.end(),
                          [&](const WidgetConfig& w) { return w.id == widget_id; });
    
    if (it != config_.widgets.end()) {
        WidgetConfig removed_widget = *it;  // 保存被移除的组件配置
        config_.widgets.erase(it);
        NotifyConfigChanged(ConfigChangeType::WIDGET, &removed_widget);
        ESP_LOGI(TAG, "Widget removed: %s", widget_id.c_str());
    } else {
        ESP_LOGW(TAG, "Widget not found: %s", widget_id.c_str());
    }
}

void IdleConfigManager::ResetToDefault() {
    CreateDefaultConfig();
    ESP_LOGI(TAG, "Config reset to default");
}

void IdleConfigManager::CreateDefaultConfig() {
    // 默认布局配置
    config_.layout.type = "default";
    config_.layout.params = {
        {"margin", 10},
        {"padding", 5},
        {"spacing", 8}
    };
    
    // 默认组件配置
    config_.widgets.clear();
    
    // 默认时钟组件
    WidgetConfig clock_widget;
    clock_widget.type = "clock";
    clock_widget.id = "clock_1";
    clock_widget.params = {
        {"format", "24h"},
        {"show_date", true},
        {"show_lunar", true}
    };
    config_.widgets.push_back(clock_widget);
    
    // 默认天气组件
    WidgetConfig weather_widget;
    weather_widget.type = "weather";
    weather_widget.id = "weather_1";
    weather_widget.params = {
        {"city", "auto"},
        {"unit", "celsius"},
        {"update_interval", 1800}
    };
    config_.widgets.push_back(weather_widget);
    
    // 默认主题配置
    config_.theme.name = "default";
    config_.theme.colors = {
        {"background", "#000000"},
        {"text", "#FFFFFF"},
        {"accent", "#007AFF"}
    };
    config_.theme.fonts = {
        {"time", "puhui_30_4"},
        {"date", "puhui_20_4"},
        {"info", "puhui_16_4"}
    };
    config_.theme.styles = {
        {"border_radius", 8},
        {"shadow", true}
    };
    
    // 其他默认设置
    config_.update_interval = 1000;
    config_.auto_enter = true;
    config_.auto_enter_delay = 30000;  // 30秒
}

bool IdleConfigManager::ValidateConfig(const IdleConfig& config) {
    // 验证布局配置
    if (config.layout.type.empty()) {
        ESP_LOGE(TAG, "Invalid layout type");
        return false;
    }
    
    // 验证组件配置
    for (const auto& widget : config.widgets) {
        if (widget.type.empty() || widget.id.empty()) {
            ESP_LOGE(TAG, "Invalid widget config");
            return false;
        }
    }
    
    // 验证主题配置
    if (config.theme.name.empty()) {
        ESP_LOGE(TAG, "Invalid theme name");
        return false;
    }
    
    // 验证其他参数
    if (config.update_interval < 100) {
        ESP_LOGE(TAG, "Invalid update interval");
        return false;
    }
    
    if (config.auto_enter && config.auto_enter_delay < 1000) {
        ESP_LOGE(TAG, "Invalid auto enter delay");
        return false;
    }
    
    return true;
}

} // namespace xiaozhi 