#pragma once

#include <string>
#include <vector>
#include <memory>
#include "config_base.h"
#include "config_observer.h"

namespace xiaozhi {

class IdleConfigManager {
public:
    static IdleConfigManager& GetInstance();

    // 观察者管理
    void AddObserver(ConfigObserver* observer);
    void RemoveObserver(ConfigObserver* observer);
    
    // 配置变更通知
    void NotifyConfigChanged(ConfigChangeType type, const void* data);

    // 加载配置
    bool LoadConfig(const std::string& path);
    
    // 保存配置
    bool SaveConfig(const std::string& path);
    
    // 获取配置
    const IdleConfig& GetConfig() const { return config_; }
    
    // 更新配置
    void UpdateConfig(const IdleConfig& config);
    
    // 更新布局配置
    void UpdateLayoutConfig(const LayoutConfig& layout);
    
    // 更新组件配置
    void UpdateWidgetConfig(const std::string& widget_id, const WidgetConfig& widget);
    
    // 更新主题配置
    void UpdateThemeConfig(const ThemeConfig& theme);
    
    // 添加组件
    void AddWidget(const WidgetConfig& widget);
    
    // 移除组件
    void RemoveWidget(const std::string& widget_id);
    
    // 重置为默认配置
    void ResetToDefault();

private:
    IdleConfigManager() = default;
    ~IdleConfigManager() = default;
    
    // 禁止拷贝和赋值
    IdleConfigManager(const IdleConfigManager&) = delete;
    IdleConfigManager& operator=(const IdleConfigManager&) = delete;
    
    // 创建默认配置
    void CreateDefaultConfig();
    
    // 验证配置
    bool ValidateConfig(const IdleConfig& config);
    
    // 配置数据
    IdleConfig config_;
    
    // 观察者列表
    std::vector<ConfigObserver*> observers_;
};

} // namespace xiaozhi 