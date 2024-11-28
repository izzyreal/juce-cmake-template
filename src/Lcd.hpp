#pragma once
#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include "Constants.hpp"

class Lcd : public juce::Component {
    public:
        void paint(juce::Graphics &g) override
        {
            juce::Image img(juce::Image::PixelFormat::RGB, 248*2, 60*2, false);

            for (uint8_t y = 0; y < 60; y++)
            {
                for (uint8_t x = 0; x < 248; x++)
                {
                    const bool on = y == 0 || y == 59 || x == 0 || x == 247;
                    drawLcdPixel(img, x, y, on);
                }
            }

            g.drawImage(img, getLocalBounds().toFloat());
        }

    private:
        void drawLcdPixel(juce::Image &img, const uint8_t lcdX, const uint8_t lcdY, const bool on)
        {
            const juce::Colour c1 = on ? Constants::lcdOn : Constants::lcdOff;
            const juce::Colour c2 = on ? Constants::lcdOnLight : Constants::lcdOff;
            img.setPixelAt(lcdX*2, lcdY*2, c1);
            img.setPixelAt(lcdX*2 + 1, lcdY*2, c2);
            img.setPixelAt(lcdX*2 + 1, lcdY*2 + 1, c2);
            img.setPixelAt(lcdX*2, lcdY*2 + 1, c2);
        }
};

