#pragma once

#include "display.h"
#include "screens/oled_idle_screen.h"

#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>

class OledDisplay : public Display {
public:
    OledDisplay(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_handle_t panel,
                int width, int height, bool mirror_x, bool mirror_y, DisplayFonts fonts);
    ~OledDisplay() override;

    // Display接口实现
    void Show() override;
    void Hide() override;
    void Update() override;
    bool Lock(int timeout_ms = 0) override;
    void Unlock() override;
    void SetTheme(const std::string& theme_name) override;
    std::string GetTheme() const override { return current_theme_name_; }

    // 待机屏幕接口实现
    void EnterIdleMode() override;
    void ExitIdleMode() override;

protected:
    void SetupUI() override;
    void SetupIdleScreen() override;
    void SetupUI_128x64();
    void SetupUI_128x32();

private:
    esp_lcd_panel_io_handle_t panel_io_;
    esp_lcd_panel_handle_t panel_;
    DisplayFonts fonts_;
    int width_;
    int height_;
    lv_display_t* display_ = nullptr;
    lv_obj_t* container_ = nullptr;
    lv_obj_t* content_ = nullptr;
    lv_obj_t* content_left_ = nullptr;
    lv_obj_t* content_right_ = nullptr;
    lv_obj_t* status_bar_ = nullptr;
    lv_obj_t* side_bar_ = nullptr;
    lv_obj_t* emotion_label_ = nullptr;
    lv_obj_t* notification_label_ = nullptr;
    lv_obj_t* status_label_ = nullptr;
    lv_obj_t* mute_label_ = nullptr;
    lv_obj_t* network_label_ = nullptr;
    lv_obj_t* battery_label_ = nullptr;
    lv_obj_t* low_battery_popup_ = nullptr;
    lv_obj_t* low_battery_label_ = nullptr;
    lv_obj_t* chat_message_label_ = nullptr;
    std::string current_theme_name_;
};
