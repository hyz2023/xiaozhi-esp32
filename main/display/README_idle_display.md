# 待机显示功能说明

## 概述

待机显示功能为ESP32设备提供了一个美观的待机界面，显示时钟、天气信息和动画等内容。该功能采用模块化设计，支持自定义配置和主题。

## 功能特性

### 1. 时钟显示
- 支持12/24小时制
- 显示公历日期
- 显示农历日期
- 支持时区设置

### 2. 天气信息
- 当前天气状况（温度、天气、湿度、风速）
- 空气质量指数（AQI）
- 紫外线指数
- 3天天气预报

### 3. 动画区域
- 支持多帧动画播放
- 可配置播放间隔
- 支持循环播放
- 支持暂停/恢复

### 4. 配置管理
- JSON格式配置文件
- 支持主题自定义
- 支持布局调整
- 支持语言切换

## 架构设计

```
idle_display/
├── idle_display.h/cc          # 核心显示类
├── widgets/                   # UI组件
│   ├── base_widget.h/cc      # 组件基类
│   ├── clock_widget.h/cc     # 时钟组件
│   ├── weather_widget.h/cc   # 天气组件
│   └── animation_widget.h/cc # 动画组件
└── config/                   # 配置管理
    ├── idle_config.h/cc     # 配置定义和管理
```

## 使用方法

### 1. 基本使用

```cpp
#include "idle_display.h"
#include "config/idle_config.h"

// 初始化配置
auto& config_manager = ConfigManager::GetInstance();
config_manager.LoadConfig("/spiffs/idle_config.json");

// 初始化待机显示
auto& idle_display = IdleDisplay::GetInstance();
idle_display.Init();

// 启动显示
idle_display.Start();

// 显示数据
ClockData clock_data;
clock_data.time_str = "12:30";
clock_data.date_str = "2024-03-20";
clock_data.lunar_date = "农历二月十一";
idle_display.ShowClock(clock_data);

// 停止显示
idle_display.Stop();
```

### 2. 配置主题

```cpp
IdleTheme theme;
theme.background_color = 0x000000;  // 黑色背景
theme.text_color = 0xFFFFFF;        // 白色文字
theme.border_color = 0xCCCCCC;      // 灰色边框
theme.widget_bg_color = 0x1A1A1A;   // 深灰色组件背景
theme.border_width = 1;
theme.margin = 20;
theme.padding = 15;
theme.widget_spacing = 20;

idle_display.SetTheme(theme);
```

### 3. 配置布局

```cpp
LayoutConfig layout;
layout.widget_width = 240;
layout.widget_height = 200;
layout.forecast_height = 120;
layout.clock_height = 150;
layout.show_lunar = true;
layout.show_air_quality = true;
layout.show_uv_index = true;

idle_display.SetLayout(layout);
```

### 4. 显示天气信息

```cpp
WeatherData weather_data;
weather_data.current.temperature = 25.5f;
weather_data.current.condition = "晴";
weather_data.current.humidity = 65;
weather_data.current.wind_speed = 3.5f;
weather_data.current.city = "上海";
weather_data.current.air_quality.aqi = 75;
weather_data.current.air_quality.level = "良";
weather_data.current.uv_index.value = 6;
weather_data.current.uv_index.level = "中等";

// 添加预报
WeatherData::Forecast forecast;
forecast.date = "3/21";
forecast.max_temp = 26.0f;
forecast.min_temp = 18.0f;
forecast.condition = "多云";
weather_data.forecast.push_back(forecast);

idle_display.ShowWeather(weather_data);
```

### 5. 显示动画

```cpp
AnimationData animation_data;
animation_data.frame_paths = {"frame1.png", "frame2.png", "frame3.png"};
animation_data.current_frame = 0;
animation_data.frame_interval_ms = 1000;
animation_data.is_playing = true;

idle_display.ShowAnimation(animation_data);
```

## 配置文件格式

配置文件使用JSON格式，存储在`/spiffs/idle_config.json`：

```json
{
  "clock": {
    "format": "24h",
    "timezone": "Asia/Shanghai",
    "date_format": "YYYY-MM-DD",
    "show_lunar": true,
    "language": "zh_CN"
  },
  "weather": {
    "city": "Shanghai",
    "update_interval": 3600,
    "units": "metric",
    "show_air_quality": true,
    "show_uv_index": true,
    "api_key": "",
    "api_url": ""
  },
  "animation": {
    "frame_interval": 1000,
    "frames": ["frame1.png", "frame2.png", "frame3.png"],
    "auto_play": true,
    "loop": true
  },
  "display": {
    "width": 800,
    "height": 480,
    "refresh_rate": 30,
    "power_save": {
      "enabled": true,
      "timeout": 300,
      "brightness": 50
    }
  },
  "layout": {
    "margin": 20,
    "padding": 15,
    "widget_spacing": 20,
    "widget_width": 240,
    "widget_height": 200,
    "forecast_height": 120,
    "border_width": 1,
    "border_color": "#CCCCCC",
    "current_layout": 0
  },
  "language": "zh_CN"
}
```

## 与主系统集成

在Application类中集成待机显示：

```cpp
// 在Application.h中添加
#include "display/idle_display.h"

// 在Application类中添加方法
void EnterIdleMode();
void ExitIdleMode();

// 在Application.cc中实现
void Application::EnterIdleMode() {
    // 停止主系统显示
    display_->Stop();
    
    // 启动待机显示
    IdleDisplay::GetInstance().Start();
    
    // 更新系统状态
    SetDeviceState(kDeviceStateIdle);
}

void Application::ExitIdleMode() {
    // 停止待机显示
    IdleDisplay::GetInstance().Stop();
    
    // 恢复主系统显示
    display_->Start();
    
    // 更新系统状态
    SetDeviceState(kDeviceStateNormal);
}
```

## 性能优化

### 1. 内存管理
- 使用智能指针管理组件生命周期
- 及时释放不需要的资源
- 使用LVGL内存池

### 2. 更新频率
- 时钟：每秒更新
- 天气：每小时更新
- 动画：可配置帧间隔

### 3. 电源管理
- 支持自动降低亮度
- 支持定时关闭显示
- 支持运动检测唤醒

## 调试支持

### 1. 日志输出
所有组件都支持ESP_LOG输出，可以通过日志级别控制输出：

```cpp
// 设置日志级别
esp_log_level_set("IdleDisplay", ESP_LOG_INFO);
esp_log_level_set("ClockWidget", ESP_LOG_DEBUG);
esp_log_level_set("WeatherWidget", ESP_LOG_DEBUG);
esp_log_level_set("AnimationWidget", ESP_LOG_DEBUG);
```

### 2. 性能监控
可以通过日志监控组件创建、更新和销毁的性能：

```
I (1234) IdleDisplay: Starting IdleDisplay
I (1235) ClockWidget: Creating ClockWidget
I (1236) WeatherWidget: Creating WeatherWidget
I (1237) AnimationWidget: Creating AnimationWidget
I (1238) IdleDisplay: All widgets created successfully
```

## 扩展开发

### 1. 添加新组件
继承BaseWidget类创建新组件：

```cpp
class CustomWidget : public BaseWidget {
public:
    CustomWidget(lv_obj_t* parent) : BaseWidget(parent, "CustomWidget") {}
    
    void Update() override {
        // 实现更新逻辑
    }
    
    int GetPreferredWidth() const override { return 200; }
    int GetPreferredHeight() const override { return 150; }
};
```

### 2. 添加新配置项
在IdleConfig结构中添加新配置：

```cpp
struct IdleConfig {
    // 现有配置...
    
    struct CustomConfig {
        std::string custom_setting = "default";
        int custom_value = 0;
    } custom;
};
```

## 注意事项

1. **内存使用**：待机显示会占用一定内存，建议在内存受限的设备上谨慎使用
2. **显示兼容性**：确保LVGL配置支持所需的显示功能
3. **文件系统**：配置文件需要存储在SPIFFS文件系统中
4. **线程安全**：所有显示操作应在主线程中进行
5. **错误处理**：组件创建失败时会有日志输出，注意检查错误信息

## 故障排除

### 1. 组件创建失败
- 检查LVGL是否正确初始化
- 检查内存是否充足
- 查看日志输出

### 2. 显示异常
- 检查屏幕分辨率配置
- 检查主题颜色设置
- 检查布局参数

### 3. 配置加载失败
- 检查配置文件路径
- 检查JSON格式是否正确
- 检查SPIFFS是否挂载

### 4. 动画不播放
- 检查图片资源是否存在
- 检查定时器是否正确创建
- 检查帧间隔设置 