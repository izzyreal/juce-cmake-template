#pragma once

#include "SvgComponent.hpp"

class Led : public SvgComponent, juce::Timer {
    public:
        enum LedColor { RED, GREEN };

        Led(const LedColor ledColorToUse, const std::function<float()> &getScaleToUse) : SvgComponent("led_on_green.svg", nullptr, 0, getScaleToUse), ledColor(ledColorToUse)
        {
            startTimer(500);
        }

        void timerCallback() override
        {
            setLedOnEnabled(!ledOnEnabled);
        }

        void setLedOnEnabled(const bool b)
        {
            if (ledOnEnabled == b) return;
            
            ledOnEnabled = b;
            
            if (!ledOnEnabled)
            {
                setSvgPath("led_off.svg");
                return;
            }

            if (ledColor == LedColor::RED)
            {
                setSvgPath("led_on_red.svg");
                return;
            }

            setSvgPath("led_on_green.svg");
        }

    private:
        const LedColor ledColor;
        bool ledOnEnabled = false;

};
