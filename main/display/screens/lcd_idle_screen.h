#pragma once

#include "idle_screen.h"

class LcdIdleScreen : public IdleScreen {
public:
    explicit LcdIdleScreen(Display* parent);
    ~LcdIdleScreen() override = default;

protected:
    void SetupUI() override;
}; 