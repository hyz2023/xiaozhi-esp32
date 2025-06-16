#include "wifi_board.h"
#include "audio_codecs/no_audio_codec.h"
#include "display/lcd_display.h"
#include "application.h"
#include <wifi_station.h>
#include "button.h"
#include "pin_config.h"
#include "config.h"
#include "led/single_led.h"
#include "iot/thing_manager.h"
#include <esp_log.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_rgb.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_st7262.h"


LV_FONT_DECLARE(font_puhui_30_4);
LV_FONT_DECLARE(font_awesome_30_4);

#define TAG "JC8048W550"

class Jc8048w550 : public WifiBoard {
private:
    // Button boot_button_;  // 暂时注释掉按钮定义
    LcdDisplay* display_;

    void InitializeST7262Display() {
        ESP_LOGI(TAG, "Init ST7262 RGB LCD");
        
        esp_lcd_panel_st7262_panel_t panel;
        esp_lcd_panel_st7262_conf_t panel_config = ESP_LCD_PANEL_ST7262_8048S043;
        
        ESP_LOGI(TAG, "Creating ST7262 LCD panel...");
        esp_err_t error = esp_lcd_panel_st7262_new(&panel_config, &panel);
        if (error != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to create ST7262 LCD panel: %s", esp_err_to_name(error));
            return;
        }

        ESP_LOGI(TAG, "Resetting ST7262 LCD panel...");
        error = esp_lcd_panel_st7262_reset(&panel);
        if (error != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to reset ST7262 LCD panel: %s", esp_err_to_name(error));
            return;
        }

        ESP_LOGI(TAG, "Initializing ST7262 LCD panel...");
        error = esp_lcd_panel_st7262_init(&panel);
        if (error != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to initialize ST7262 LCD panel: %s", esp_err_to_name(error));
            return;
        }

        ESP_LOGI(TAG, "Turning on backlight...");
        error = esp_lcd_panel_st7262_backlight_on_ff(&panel_config, true);
        if (error != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to turn on backlight: %s", esp_err_to_name(error));
            return;
        }
        
        ESP_LOGI(TAG, "Creating display...");
        display_ = new RgbLcdDisplay(nullptr, panel.handle,
            panel_config.width, panel_config.height, 
            0, 0, false, false, false,
            {
                .text_font = &font_puhui_30_4,
                .icon_font = &font_awesome_30_4,
                .emoji_font = font_emoji_64_init(),
            });
        
        ESP_LOGI(TAG, "LCD initialization completed");
    }

    // void InitializeButtons() {  // 暂时注释掉按钮初始化
    //     boot_button_.OnClick([this]() {
    //         auto& app = Application::GetInstance();
    //         if (app.GetDeviceState() == kDeviceStateStarting && !WifiStation::GetInstance().IsConnected()) {
    //             ResetWifiConfiguration();
    //         }
    //     });
    //     boot_button_.OnPressDown([this]() {
    //         Application::GetInstance().StartListening();
    //     });
    //     boot_button_.OnPressUp([this]() {
    //         Application::GetInstance().StopListening();
    //     });
    // }

    void InitializeLeds() {
        // 可扩展LED初始化
    }

    // 物联网初始化，添加对 AI 可见设备
    void InitializeIot() {
        auto& thing_manager = iot::ThingManager::GetInstance();
        thing_manager.AddThing(iot::CreateThing("Speaker"));
        thing_manager.AddThing(iot::CreateThing("Screen"));
    }

public:
    Jc8048w550() {  // 移除按钮初始化
        // InitializeButtons();  // 暂时注释掉按钮初始化
        InitializeLeds();
        InitializeST7262Display();
        // 移除RestoreBrightness调用，因为我们已经用GPIO直接控制了背光
        // GetBacklight()->RestoreBrightness();
    }

    virtual Led* GetLed() override {
        static SingleLed led(JC8048W550_STATUS_LED);
        return &led;
    }

    virtual AudioCodec* GetAudioCodec() override {
        static NoAudioCodecSimplex audio_codec(AUDIO_INPUT_SAMPLE_RATE, AUDIO_OUTPUT_SAMPLE_RATE,
            AUDIO_I2S_SPK_GPIO_BCLK, AUDIO_I2S_SPK_GPIO_LRCK, AUDIO_I2S_SPK_GPIO_DOUT, AUDIO_I2S_MIC_GPIO_SCK, AUDIO_I2S_MIC_GPIO_WS, AUDIO_I2S_MIC_GPIO_DIN);

        return &audio_codec;
    }

    virtual Display* GetDisplay() override {
        return display_;
    }

};

DECLARE_BOARD(Jc8048w550); 