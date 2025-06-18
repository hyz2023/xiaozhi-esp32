#include "config_manager.h"
#include <esp_log.h>
#include <fstream>
#include <nlohmann/json.hpp>

#define TAG "ConfigManager"

namespace xiaozhi {

ConfigManager::~ConfigManager() {
    StopHotReload();
    SaveAllConfigs();
}

void ConfigManager::Init(const std::string& config_dir) {
    config_dir_ = config_dir;
    
    // 创建配置目录
    std::filesystem::create_directories(config_dir_);
    
    // 初始化配置
    configs_["idle"] = std::make_shared<IdleConfig>();
    
    // 加载配置
    LoadAllConfigs();
}

void ConfigManager::RegisterObserver(const std::string& config_type, ConfigObserver* observer) {
    std::lock_guard<std::mutex> lock(mutex_);
    observers_[config_type].push_back(observer);
}

void ConfigManager::UnregisterObserver(const std::string& config_type, ConfigObserver* observer) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& obs = observers_[config_type];
    obs.erase(std::remove(obs.begin(), obs.end(), observer), obs.end());
}

void ConfigManager::NotifyObservers(const std::string& config_type, std::shared_ptr<ConfigBase> config) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = observers_.find(config_type);
    if (it != observers_.end()) {
        for (auto observer : it->second) {
            observer->OnConfigChanged(config_type, config);
        }
    }
}

bool ConfigManager::SaveConfig(const std::string& config_type) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = configs_.find(config_type);
    if (it == configs_.end()) {
        return false;
    }
    
    std::string file_path = config_dir_ + "/" + config_type + ".json";
    std::ofstream file(file_path);
    if (!file.is_open()) {
        ESP_LOGE(TAG, "Failed to open config file: %s", file_path.c_str());
        return false;
    }
    
    nlohmann::json j = it->second->ToJson();
    file << j.dump(4);
    ESP_LOGI(TAG, "Saved config to file: %s", file_path.c_str());
    return true;
}

bool ConfigManager::LoadConfig(const std::string& config_type) {
    std::string file_path = config_dir_ + "/" + config_type + ".json";
    std::ifstream file(file_path);
    if (!file.is_open()) {
        ESP_LOGE(TAG, "Failed to open config file: %s", file_path.c_str());
        return false;
    }
    
    nlohmann::json j;
    file >> j;
    
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = configs_.find(config_type);
    if (it != configs_.end()) {
        it->second->FromJson(j);
        NotifyObservers(config_type, it->second);
        ESP_LOGI(TAG, "Loaded config from file: %s", file_path.c_str());
        return true;
    }
    return false;
}

bool ConfigManager::SaveAllConfigs() {
    bool success = true;
    for (const auto& [config_type, _] : configs_) {
        if (!SaveConfig(config_type)) {
            success = false;
        }
    }
    return success;
}

bool ConfigManager::LoadAllConfigs() {
    bool success = true;
    for (const auto& [config_type, _] : configs_) {
        if (!LoadConfig(config_type)) {
            success = false;
        }
    }
    return success;
}

void ConfigManager::StartHotReload() {
    if (hot_reload_running_) {
        return;
    }
    
    hot_reload_running_ = true;
    hot_reload_thread_ = std::thread([this]() {
        while (hot_reload_running_) {
            CheckFileChanges();
            std::this_thread::sleep_for(std::chrono::milliseconds(check_interval_ms_));
        }
    });
}

void ConfigManager::StopHotReload() {
    if (!hot_reload_running_) {
        return;
    }
    
    hot_reload_running_ = false;
    if (hot_reload_thread_.joinable()) {
        hot_reload_thread_.join();
    }
}

void ConfigManager::CheckFileChanges() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& [config_type, config] : configs_) {
        std::string file_path = config_dir_ + "/" + config_type + ".json";
        auto current_time = GetFileLastModified(file_path);
        
        auto it = file_mod_times_.find(file_path);
        if (it == file_mod_times_.end()) {
            file_mod_times_[file_path] = current_time;
        } else if (current_time > it->second) {
            ESP_LOGI(TAG, "Config file changed: %s", file_path.c_str());
            LoadConfig(config_type);
            it->second = current_time;
        }
    }
}

std::chrono::system_clock::time_point ConfigManager::GetFileLastModified(const std::string& file_path) {
    try {
        auto ftime = std::filesystem::last_write_time(file_path);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
        return sctp;
    } catch (const std::exception& e) {
        ESP_LOGE(TAG, "Failed to get file last modified time: %s", e.what());
        return std::chrono::system_clock::time_point();
    }
}

} // namespace xiaozhi 