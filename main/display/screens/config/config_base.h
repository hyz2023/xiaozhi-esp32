#pragma once

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

namespace xiaozhi {

// 配置基类
class ConfigBase {
public:
    virtual ~ConfigBase() = default;
    virtual void FromJson(const nlohmann::json& json) = 0;
    virtual nlohmann::json ToJson() const = 0;
};

// 布局配置
struct LayoutConfig : public ConfigBase {
    std::string type;  // 布局类型
    nlohmann::json params;  // 布局参数
    
    // 转换为JSON
    nlohmann::json ToJson() const override {
        nlohmann::json j;
        j["type"] = type;
        j["params"] = params;
        return j;
    }
    
    // 从JSON加载
    void FromJson(const nlohmann::json& j) override {
        type = j["type"];
        params = j["params"];
    }
};

// 组件配置
struct WidgetConfig : public ConfigBase {
    std::string type;  // 组件类型
    std::string id;    // 组件ID
    nlohmann::json params;  // 组件参数
    
    // 转换为JSON
    nlohmann::json ToJson() const override {
        nlohmann::json j;
        j["type"] = type;
        j["id"] = id;
        j["params"] = params;
        return j;
    }
    
    // 从JSON加载
    void FromJson(const nlohmann::json& j) override {
        type = j["type"];
        id = j["id"];
        params = j["params"];
    }
};

// 主题配置
struct ThemeConfig : public ConfigBase {
    std::string name;  // 主题名称
    nlohmann::json colors;  // 颜色配置
    nlohmann::json fonts;   // 字体配置
    nlohmann::json styles;  // 样式配置
    
    // 转换为JSON
    nlohmann::json ToJson() const override {
        nlohmann::json j;
        j["name"] = name;
        j["colors"] = colors;
        j["fonts"] = fonts;
        j["styles"] = styles;
        return j;
    }
    
    // 从JSON加载
    void FromJson(const nlohmann::json& j) override {
        name = j["name"];
        colors = j["colors"];
        fonts = j["fonts"];
        styles = j["styles"];
    }
};

// 待机页面配置
class IdleConfig : public ConfigBase {
public:
    LayoutConfig layout;  // 布局配置
    std::vector<WidgetConfig> widgets;  // 组件配置列表
    ThemeConfig theme;  // 主题配置
    int update_interval{1000};  // 更新间隔(ms)
    bool auto_enter{true};  // 是否自动进入待机
    int auto_enter_delay{5000};  // 自动进入延迟(ms)
    
    // 转换为JSON
    nlohmann::json ToJson() const override {
        nlohmann::json j;
        j["layout"] = layout.ToJson();
        j["widgets"] = nlohmann::json::array();
        for (const auto& widget : widgets) {
            j["widgets"].push_back(widget.ToJson());
        }
        j["theme"] = theme.ToJson();
        j["update_interval"] = update_interval;
        j["auto_enter"] = auto_enter;
        j["auto_enter_delay"] = auto_enter_delay;
        return j;
    }
    
    // 从JSON加载
    void FromJson(const nlohmann::json& j) override {
        layout.FromJson(j["layout"]);
        widgets.clear();
        for (const auto& widget : j["widgets"]) {
            WidgetConfig w;
            w.FromJson(widget);
            widgets.push_back(w);
        }
        theme.FromJson(j["theme"]);
        update_interval = j["update_interval"];
        auto_enter = j["auto_enter"];
        auto_enter_delay = j["auto_enter_delay"];
    }
};

} // namespace xiaozhi 