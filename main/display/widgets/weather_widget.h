#ifndef WEATHER_WIDGET_H
#define WEATHER_WIDGET_H

#include "base_widget.h"
#include "idle_display.h"
#include <string>
#include <vector>

// 前向声明
struct WeatherData;

// 天气组件
class WeatherWidget : public BaseWidget {
public:
    WeatherWidget(lv_obj_t* parent);
    ~WeatherWidget() override;
    
    // 核心接口实现
    void Update() override;
    void SetData(const WeatherData& data);
    
    // 布局相关
    int GetPreferredWidth() const override { return 240; }
    int GetPreferredHeight() const override { return 200; }
    void UpdateLayout(const LayoutConfig& layout) override;
    
    // 显示控制
    void ShowAirQuality(bool show);
    void ShowUVIndex(bool show);
    void ShowForecast(bool show);
    void SetUnits(const std::string& units);  // "metric" or "imperial"
    
private:
    // 私有方法
    void CreateWidgets();
    void DestroyWidgets();
    void CreateCurrentWeatherWidgets();
    void CreateAirQualityWidgets();
    void CreateUVIndexWidgets();
    void CreateForecastWidgets();
    
    void UpdateCurrentWeather();
    void UpdateAirQuality();
    void UpdateUVIndex();
    void UpdateForecast();
    
    void ApplyTextStyles();
    std::string FormatTemperature(float temp) const;
    std::string GetAQILevel(int aqi) const;
    std::string GetUVLevel(int uv) const;
    
    // 当前天气UI元素
    lv_obj_t* city_label_ = nullptr;
    lv_obj_t* temp_label_ = nullptr;
    lv_obj_t* condition_label_ = nullptr;
    lv_obj_t* humidity_label_ = nullptr;
    lv_obj_t* wind_label_ = nullptr;
    lv_obj_t* weather_icon_ = nullptr;
    
    // 空气质量UI元素
    lv_obj_t* aqi_label_ = nullptr;
    lv_obj_t* aqi_level_label_ = nullptr;
    lv_obj_t* aqi_container_ = nullptr;
    
    // 紫外线UI元素
    lv_obj_t* uv_label_ = nullptr;
    lv_obj_t* uv_level_label_ = nullptr;
    lv_obj_t* uv_container_ = nullptr;
    
    // 预报UI元素
    lv_obj_t* forecast_container_ = nullptr;
    std::vector<lv_obj_t*> forecast_labels_;
    
    // 数据
    WeatherData current_data_;
    std::string units_ = "metric";
    bool show_air_quality_ = true;
    bool show_uv_index_ = true;
    bool show_forecast_ = true;
    
    // 样式
    lv_style_t text_style_;
    lv_style_t title_style_;
    lv_style_t value_style_;
    bool styles_initialized_ = false;
};

#endif // WEATHER_WIDGET_H 