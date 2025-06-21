#include "weather_widget.h"
#include <esp_log.h>
#include <sstream>
#include <iomanip>

static const char* TAG = "WeatherWidget";

WeatherWidget::WeatherWidget(lv_obj_t* parent)
    : BaseWidget(parent, "WeatherWidget") {
    ESP_LOGI(TAG, "Creating WeatherWidget");
    CreateWidgets();
}

WeatherWidget::~WeatherWidget() {
    ESP_LOGI(TAG, "Destroying WeatherWidget");
    DestroyWidgets();
}

void WeatherWidget::CreateWidgets() {
    if (!container_) {
        ESP_LOGE(TAG, "Container is null, cannot create widgets");
        return;
    }
    
    CreateCurrentWeatherWidgets();
    CreateAirQualityWidgets();
    CreateUVIndexWidgets();
    CreateForecastWidgets();
    ApplyTextStyles();
    
    ESP_LOGI(TAG, "Weather widgets created successfully");
}

void WeatherWidget::CreateCurrentWeatherWidgets() {
    // 城市名称
    city_label_ = lv_label_create(container_);
    if (city_label_) {
        lv_label_set_text(city_label_, "上海");
        lv_obj_set_style_text_align(city_label_, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_set_pos(city_label_, 10, 10);
    }
    
    // 温度
    temp_label_ = lv_label_create(container_);
    if (temp_label_) {
        lv_label_set_text(temp_label_, "25°C");
        lv_obj_set_style_text_align(temp_label_, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_set_pos(temp_label_, 10, 35);
    }
    
    // 天气状况
    condition_label_ = lv_label_create(container_);
    if (condition_label_) {
        lv_label_set_text(condition_label_, "晴");
        lv_obj_set_style_text_align(condition_label_, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_set_pos(condition_label_, 10, 60);
    }
    
    // 湿度
    humidity_label_ = lv_label_create(container_);
    if (humidity_label_) {
        lv_label_set_text(humidity_label_, "湿度: 65%");
        lv_obj_set_style_text_align(humidity_label_, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_set_pos(humidity_label_, 10, 85);
    }
    
    // 风速
    wind_label_ = lv_label_create(container_);
    if (wind_label_) {
        lv_label_set_text(wind_label_, "风速: 3.5m/s");
        lv_obj_set_style_text_align(wind_label_, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_set_pos(wind_label_, 10, 110);
    }
}

void WeatherWidget::CreateAirQualityWidgets() {
    aqi_container_ = lv_obj_create(container_);
    if (aqi_container_) {
        lv_obj_set_size(aqi_container_, 100, 60);
        lv_obj_set_pos(aqi_container_, 130, 10);
        lv_obj_set_style_bg_opa(aqi_container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(aqi_container_, 0, 0);
        lv_obj_set_style_pad_all(aqi_container_, 5, 0);
    }
    
    aqi_label_ = lv_label_create(aqi_container_);
    if (aqi_label_) {
        lv_label_set_text(aqi_label_, "AQI: 75");
        lv_obj_set_style_text_align(aqi_label_, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_pos(aqi_label_, 0, 0);
    }
    
    aqi_level_label_ = lv_label_create(aqi_container_);
    if (aqi_level_label_) {
        lv_label_set_text(aqi_level_label_, "良");
        lv_obj_set_style_text_align(aqi_level_label_, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_pos(aqi_level_label_, 0, 25);
    }
}

void WeatherWidget::CreateUVIndexWidgets() {
    uv_container_ = lv_obj_create(container_);
    if (uv_container_) {
        lv_obj_set_size(uv_container_, 100, 60);
        lv_obj_set_pos(uv_container_, 130, 80);
        lv_obj_set_style_bg_opa(uv_container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(uv_container_, 0, 0);
        lv_obj_set_style_pad_all(uv_container_, 5, 0);
    }
    
    uv_label_ = lv_label_create(uv_container_);
    if (uv_label_) {
        lv_label_set_text(uv_label_, "UV: 6");
        lv_obj_set_style_text_align(uv_label_, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_pos(uv_label_, 0, 0);
    }
    
    uv_level_label_ = lv_label_create(uv_container_);
    if (uv_level_label_) {
        lv_label_set_text(uv_level_label_, "中等");
        lv_obj_set_style_text_align(uv_level_label_, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_pos(uv_level_label_, 0, 25);
    }
}

void WeatherWidget::CreateForecastWidgets() {
    forecast_container_ = lv_obj_create(container_);
    if (forecast_container_) {
        lv_obj_set_size(forecast_container_, 220, 80);
        lv_obj_set_pos(forecast_container_, 10, 150);
        lv_obj_set_style_bg_opa(forecast_container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(forecast_container_, 0, 0);
        lv_obj_set_style_pad_all(forecast_container_, 5, 0);
    }
    
    // 创建3个预报标签
    for (int i = 0; i < 3; ++i) {
        lv_obj_t* label = lv_label_create(forecast_container_);
        if (label) {
            lv_label_set_text(label, "3/21 多云 26°/18°");
            lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_pos(label, i * 70, 0);
            lv_obj_set_width(label, 65);
            forecast_labels_.push_back(label);
        }
    }
}

void WeatherWidget::DestroyWidgets() {
    // 销毁当前天气组件
    if (city_label_) { lv_obj_del(city_label_); city_label_ = nullptr; }
    if (temp_label_) { lv_obj_del(temp_label_); temp_label_ = nullptr; }
    if (condition_label_) { lv_obj_del(condition_label_); condition_label_ = nullptr; }
    if (humidity_label_) { lv_obj_del(humidity_label_); humidity_label_ = nullptr; }
    if (wind_label_) { lv_obj_del(wind_label_); wind_label_ = nullptr; }
    if (weather_icon_) { lv_obj_del(weather_icon_); weather_icon_ = nullptr; }
    
    // 销毁空气质量组件
    if (aqi_label_) { lv_obj_del(aqi_label_); aqi_label_ = nullptr; }
    if (aqi_level_label_) { lv_obj_del(aqi_level_label_); aqi_level_label_ = nullptr; }
    if (aqi_container_) { lv_obj_del(aqi_container_); aqi_container_ = nullptr; }
    
    // 销毁紫外线组件
    if (uv_label_) { lv_obj_del(uv_label_); uv_label_ = nullptr; }
    if (uv_level_label_) { lv_obj_del(uv_level_label_); uv_level_label_ = nullptr; }
    if (uv_container_) { lv_obj_del(uv_container_); uv_container_ = nullptr; }
    
    // 销毁预报组件
    for (auto label : forecast_labels_) {
        if (label) lv_obj_del(label);
    }
    forecast_labels_.clear();
    if (forecast_container_) { lv_obj_del(forecast_container_); forecast_container_ = nullptr; }
    
    ESP_LOGI(TAG, "Weather widgets destroyed");
}

void WeatherWidget::Update() {
    UpdateCurrentWeather();
    UpdateAirQuality();
    UpdateUVIndex();
    UpdateForecast();
}

void WeatherWidget::SetData(const WeatherData& data) {
    current_data_ = data;
    Update();
}

void WeatherWidget::UpdateLayout(const LayoutConfig& layout) {
    BaseWidget::UpdateLayout(layout);
    
    // 调整组件大小和位置
    SetSize(layout.widget_width, layout.widget_height);
    
    // 重新定位组件
    if (aqi_container_) {
        lv_obj_set_pos(aqi_container_, 130, 10);
    }
    if (uv_container_) {
        lv_obj_set_pos(uv_container_, 130, 80);
    }
    if (forecast_container_) {
        lv_obj_set_pos(forecast_container_, 10, layout.widget_height - 90);
    }
    
    ESP_LOGI(TAG, "Weather layout updated");
}

void WeatherWidget::ShowAirQuality(bool show) {
    show_air_quality_ = show;
    if (aqi_container_) {
        lv_obj_set_style_opa(aqi_container_, show ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
    }
}

void WeatherWidget::ShowUVIndex(bool show) {
    show_uv_index_ = show;
    if (uv_container_) {
        lv_obj_set_style_opa(uv_container_, show ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
    }
}

void WeatherWidget::ShowForecast(bool show) {
    show_forecast_ = show;
    if (forecast_container_) {
        lv_obj_set_style_opa(forecast_container_, show ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
    }
}

void WeatherWidget::SetUnits(const std::string& units) {
    units_ = units;
    UpdateCurrentWeather();
}

void WeatherWidget::UpdateCurrentWeather() {
    if (city_label_) {
        std::string city = current_data_.current.city;
        if (city.empty()) city = "上海";
        lv_label_set_text(city_label_, city.c_str());
    }
    
    if (temp_label_) {
        std::string temp = FormatTemperature(current_data_.current.temperature);
        lv_label_set_text(temp_label_, temp.c_str());
    }
    
    if (condition_label_) {
        std::string condition = current_data_.current.condition;
        if (condition.empty()) condition = "晴";
        lv_label_set_text(condition_label_, condition.c_str());
    }
    
    if (humidity_label_) {
        std::string humidity = "湿度: " + std::to_string(current_data_.current.humidity) + "%";
        lv_label_set_text(humidity_label_, humidity.c_str());
    }
    
    if (wind_label_) {
        std::string wind = "风速: " + std::to_string(current_data_.current.wind_speed) + "m/s";
        lv_label_set_text(wind_label_, wind.c_str());
    }
}

void WeatherWidget::UpdateAirQuality() {
    if (!show_air_quality_ || !aqi_label_ || !aqi_level_label_) return;
    
    int aqi = current_data_.current.air_quality.aqi;
    std::string aqi_text = "AQI: " + std::to_string(aqi);
    lv_label_set_text(aqi_label_, aqi_text.c_str());
    
    std::string level = current_data_.current.air_quality.level;
    if (level.empty()) {
        level = GetAQILevel(aqi);
    }
    lv_label_set_text(aqi_level_label_, level.c_str());
}

void WeatherWidget::UpdateUVIndex() {
    if (!show_uv_index_ || !uv_label_ || !uv_level_label_) return;
    
    int uv = current_data_.current.uv_index.value;
    std::string uv_text = "UV: " + std::to_string(uv);
    lv_label_set_text(uv_label_, uv_text.c_str());
    
    std::string level = current_data_.current.uv_index.level;
    if (level.empty()) {
        level = GetUVLevel(uv);
    }
    lv_label_set_text(uv_level_label_, level.c_str());
}

void WeatherWidget::UpdateForecast() {
    if (!show_forecast_ || forecast_labels_.empty()) return;
    
    for (size_t i = 0; i < forecast_labels_.size() && i < current_data_.forecast.size(); ++i) {
        const auto& forecast = current_data_.forecast[i];
        std::string forecast_text = forecast.date + " " + forecast.condition + " " + 
                                   FormatTemperature(forecast.max_temp) + "/" + 
                                   FormatTemperature(forecast.min_temp);
        lv_label_set_text(forecast_labels_[i], forecast_text.c_str());
    }
}

void WeatherWidget::ApplyTextStyles() {
    if (styles_initialized_) return;
    
    // 初始化样式
    lv_style_init(&text_style_);
    lv_style_init(&title_style_);
    lv_style_init(&value_style_);
    
    // 文本样式
    lv_style_set_text_color(&text_style_, lv_color_hex(0xFFFFFF));
    lv_style_set_text_align(&text_style_, LV_TEXT_ALIGN_LEFT);
    
    // 标题样式
    lv_style_set_text_color(&title_style_, lv_color_hex(0xCCCCCC));
    lv_style_set_text_align(&title_style_, LV_TEXT_ALIGN_LEFT);
    
    // 数值样式
    lv_style_set_text_color(&value_style_, lv_color_hex(0xFFFFFF));
    lv_style_set_text_align(&value_style_, LV_TEXT_ALIGN_CENTER);
    
    // 应用样式
    if (city_label_) lv_obj_add_style(city_label_, &title_style_, 0);
    if (temp_label_) lv_obj_add_style(temp_label_, &value_style_, 0);
    if (condition_label_) lv_obj_add_style(condition_label_, &text_style_, 0);
    if (humidity_label_) lv_obj_add_style(humidity_label_, &text_style_, 0);
    if (wind_label_) lv_obj_add_style(wind_label_, &text_style_, 0);
    
    if (aqi_label_) lv_obj_add_style(aqi_label_, &value_style_, 0);
    if (aqi_level_label_) lv_obj_add_style(aqi_level_label_, &text_style_, 0);
    if (uv_label_) lv_obj_add_style(uv_label_, &value_style_, 0);
    if (uv_level_label_) lv_obj_add_style(uv_level_label_, &text_style_, 0);
    
    for (auto label : forecast_labels_) {
        if (label) lv_obj_add_style(label, &text_style_, 0);
    }
    
    styles_initialized_ = true;
    ESP_LOGI(TAG, "Text styles applied");
}

std::string WeatherWidget::FormatTemperature(float temp) const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << temp;
    return oss.str() + "°C";
}

std::string WeatherWidget::GetAQILevel(int aqi) const {
    if (aqi <= 50) return "优";
    if (aqi <= 100) return "良";
    if (aqi <= 150) return "轻度污染";
    if (aqi <= 200) return "中度污染";
    if (aqi <= 300) return "重度污染";
    return "严重污染";
}

std::string WeatherWidget::GetUVLevel(int uv) const {
    if (uv <= 2) return "低";
    if (uv <= 5) return "中等";
    if (uv <= 7) return "高";
    if (uv <= 10) return "很高";
    return "极高";
} 