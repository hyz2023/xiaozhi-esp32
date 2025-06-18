#pragma once

#include <string>
#include <memory>
#include "config_base.h"

namespace xiaozhi {

// 配置变更类型
enum class ConfigChangeType {
    LAYOUT,     // 布局变更
    WIDGET,     // 组件变更
    THEME,      // 主题变更
    SETTINGS    // 其他设置变更
};

// 配置观察者接口
class ConfigObserver {
public:
    virtual ~ConfigObserver() = default;
    
    // 配置变更通知
    virtual void OnConfigChanged(ConfigChangeType type, const void* data) = 0;
};

// 配置变更事件
struct ConfigChangeEvent {
    ConfigChangeType type;
    union {
        const LayoutConfig* layout;
        const WidgetConfig* widget;
        const ThemeConfig* theme;
        const IdleConfig* settings;
    } data;
};

} // namespace xiaozhi 