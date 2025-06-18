#pragma once

#include "idle_screen.h"

class OledIdleScreen : public IdleScreen {
public:
    explicit OledIdleScreen(Display* parent);
    ~OledIdleScreen() override = default;

protected:
    void SetupUI() override;
}; 