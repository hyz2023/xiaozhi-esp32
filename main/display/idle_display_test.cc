#include "idle_display.h"
#include "config/idle_config.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char* TAG = "IdleDisplayTest";

// 测试任务
void idle_display_test_task(void* arg) {
    ESP_LOGI(TAG, "Starting idle display test");
    
    // 初始化配置管理器
    auto& config_manager = ConfigManager::GetInstance();
    config_manager.LoadConfig("/spiffs/idle_config.json");
    
    // 初始化待机显示
    auto& idle_display = IdleDisplay::GetInstance();
    idle_display.Init();
    
    // 设置配置
    const auto& config = config_manager.GetConfig();
    idle_display.SetTheme({
        .background_color = 0x000000,
        .text_color = 0xFFFFFF,
        .border_color = 0xCCCCCC,
        .widget_bg_color = 0x1A1A1A,
        .border_width = 1,
        .margin = 20,
        .padding = 15,
        .widget_spacing = 20
    });
    
    idle_display.SetLayout({
        .widget_width = 240,
        .widget_height = 200,
        .forecast_height = 120,
        .clock_height = 150,
        .show_lunar = true,
        .show_air_quality = true,
        .show_uv_index = true
    });
    
    // 启动待机显示
    idle_display.Start();
    
    // 创建测试数据
    ClockData clock_data;
    clock_data.time_str = "12:30";
    clock_data.date_str = "2024-03-20";
    clock_data.lunar_date = "农历二月十一";
    clock_data.is_24h_format = true;
    clock_data.timezone = "Asia/Shanghai";
    
    WeatherData weather_data;
    weather_data.current.temperature = 25.5f;
    weather_data.current.condition = "晴";
    weather_data.current.icon = "sunny";
    weather_data.current.humidity = 65;
    weather_data.current.wind_speed = 3.5f;
    weather_data.current.city = "上海";
    weather_data.current.air_quality.aqi = 75;
    weather_data.current.air_quality.level = "良";
    weather_data.current.air_quality.pm2_5 = 35;
    weather_data.current.air_quality.pm10 = 45;
    weather_data.current.uv_index.value = 6;
    weather_data.current.uv_index.level = "中等";
    weather_data.current.uv_index.protection = "建议涂抹防晒霜";
    
    // 添加预报数据
    WeatherData::Forecast forecast1;
    forecast1.date = "3/21";
    forecast1.lunar_date = "二月十二";
    forecast1.max_temp = 26.0f;
    forecast1.min_temp = 18.0f;
    forecast1.condition = "多云";
    forecast1.icon = "cloudy";
    forecast1.aqi = 80;
    forecast1.aqi_level = "良";
    weather_data.forecast.push_back(forecast1);
    
    WeatherData::Forecast forecast2;
    forecast2.date = "3/22";
    forecast2.lunar_date = "二月十三";
    forecast2.max_temp = 24.0f;
    forecast2.min_temp = 17.0f;
    forecast2.condition = "雨";
    forecast2.icon = "rainy";
    forecast2.aqi = 60;
    forecast2.aqi_level = "良";
    weather_data.forecast.push_back(forecast2);
    
    WeatherData::Forecast forecast3;
    forecast3.date = "3/23";
    forecast3.lunar_date = "二月十四";
    forecast3.max_temp = 25.0f;
    forecast3.min_temp = 19.0f;
    forecast3.condition = "晴";
    forecast3.icon = "sunny";
    forecast3.aqi = 70;
    forecast3.aqi_level = "良";
    weather_data.forecast.push_back(forecast3);
    
    AnimationData animation_data;
    animation_data.frame_paths = {"frame1.png", "frame2.png", "frame3.png"};
    animation_data.current_frame = 0;
    animation_data.frame_interval_ms = 1000;
    animation_data.is_playing = true;
    
    // 显示数据
    idle_display.ShowClock(clock_data);
    idle_display.ShowWeather(weather_data);
    idle_display.ShowAnimation(animation_data);
    
    ESP_LOGI(TAG, "Test data displayed");
    
    // 保持运行一段时间
    vTaskDelay(pdMS_TO_TICKS(30000));  // 30秒
    
    // 停止待机显示
    idle_display.Stop();
    
    ESP_LOGI(TAG, "Idle display test completed");
    
    // 删除任务
    vTaskDelete(nullptr);
}

// 启动测试
void start_idle_display_test() {
    xTaskCreate(idle_display_test_task, "idle_test", 8192, nullptr, 5, nullptr);
} 