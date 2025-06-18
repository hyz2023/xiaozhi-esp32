#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <filesystem>
#include "config_observer.h"
#include "idle_config.h"

namespace xiaozhi {

class ConfigManager {
public:
    static ConfigManager& GetInstance() {
        static ConfigManager instance;
        return instance;
    }

    // 初始化配置管理器
    void Init(const std::string& config_dir = "/config");
    
    // 注册观察者
    void RegisterObserver(const std::string& config_type, ConfigObserver* observer);
    
    // 注销观察者
    void UnregisterObserver(const std::string& config_type, ConfigObserver* observer);
    
    // 获取配置
    template<typename T>
    std::shared_ptr<T> GetConfig(const std::string& config_type) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = configs_.find(config_type);
        if (it != configs_.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }
    
    // 更新配置
    template<typename T>
    void UpdateConfig(const std::string& config_type, const T& config) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = configs_.find(config_type);
        if (it != configs_.end()) {
            auto new_config = std::make_shared<T>(config);
            configs_[config_type] = new_config;
            NotifyObservers(config_type, new_config);
            
            // 自动保存配置
            if (auto_save_) {
                SaveConfig(config_type);
            }
        }
    }
    
    // 保存配置到文件
    bool SaveConfig(const std::string& config_type);
    
    // 从文件加载配置
    bool LoadConfig(const std::string& config_type);
    
    // 启动热重载
    void StartHotReload();
    
    // 停止热重载
    void StopHotReload();
    
    // 设置自动保存
    void SetAutoSave(bool enable) { auto_save_ = enable; }
    
    // 获取自动保存状态
    bool GetAutoSave() const { return auto_save_; }
    
    // 保存所有配置
    bool SaveAllConfigs();
    
    // 加载所有配置
    bool LoadAllConfigs();

private:
    ConfigManager() = default;
    ~ConfigManager();
    
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    // 通知观察者
    void NotifyObservers(const std::string& config_type, std::shared_ptr<ConfigBase> config);
    
    // 检查文件变化
    void CheckFileChanges();
    
    // 获取文件最后修改时间
    std::chrono::system_clock::time_point GetFileLastModified(const std::string& file_path);
    
    // 配置文件目录
    std::string config_dir_;
    
    // 配置对象
    std::unordered_map<std::string, std::shared_ptr<ConfigBase>> configs_;
    
    // 观察者列表
    std::unordered_map<std::string, std::vector<ConfigObserver*>> observers_;
    
    // 互斥锁
    std::mutex mutex_;
    
    // 热重载线程
    std::thread hot_reload_thread_;
    
    // 热重载标志
    std::atomic<bool> hot_reload_running_{false};
    
    // 文件修改时间记录
    std::unordered_map<std::string, std::chrono::system_clock::time_point> file_mod_times_;
    
    // 检查间隔(毫秒)
    const int check_interval_ms_ = 1000;
    
    // 自动保存标志
    bool auto_save_{true};
};

} // namespace xiaozhi 