#pragma once

#include "layout_base.h"
#include "../components/clock_widget.h"
#include "../components/weather_widget.h"
#include "../components/animation_widget.h"

class AlternateLayout : public LayoutBase {
public:
    explicit AlternateLayout(lv_obj_t* parent);
    ~AlternateLayout() override = default;

protected:
    void SetupLayout() override;
    void ArrangeWidgets() override;

private:
    std::unique_ptr<ClockWidget> clock_widget_;
    std::unique_ptr<WeatherWidget> weather_widget_;
    std::unique_ptr<AnimationWidget> animation_widget_;
}; 