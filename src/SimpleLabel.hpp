#pragma once

#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include "LabelComponent.hpp"
#include "Constants.hpp"

#include <string>
#include <functional>
#include <fstream>

class SimpleLabel : public LabelComponent {
    public:
        SimpleLabel(
                const std::function<float()>& getScaleToUse,
                const std::string textToUse,
                const juce::Colour colourToUse)
            : text(textToUse), getScale(getScaleToUse), colour(colourToUse)
        {}

        void paint(juce::Graphics& g) override
        {
            //g.fillAll(juce::Colours::yellowgreen);
            g.setFont(getFont());

            int row = 0;
            std::string buf;
            bool should_draw = false;

            g.setColour(colour);
            const auto yOffset = (getHeight() - getRequiredHeight()) / 2;

            for (auto c : text) 
            {
                if (c == '\n')
                {
                    g.drawText(buf, 0, yOffset + row * (Constants::BASE_FONT_SIZE + Constants::LINE_SIZE) * getScale(), getWidth(), getHeight(), juce::Justification::centredTop);
                    row++;
                    should_draw = false;
                    buf.clear();
                    continue;
                }
                buf += c;
                should_draw = true;
            }

            if (should_draw && !buf.empty())
            {
                g.drawText(buf, 0, yOffset + row * (Constants::BASE_FONT_SIZE + Constants::LINE_SIZE) * getScale(), getWidth(), getHeight(), juce::Justification::centredTop);
            }
        }

        float getRequiredWidth() override
        {
            int upper = 0;

            std::string buf;
            bool should_check = true;

            for (auto c : text)
            {
                if (c == '\n')  
                {
                    upper = std::max<int>(getFont().getStringWidth(buf), upper);
                    buf.clear();
                    should_check = false;
                    continue;
                }

                buf += c;
                should_check = true;
            }

            if (should_check && !buf.empty())
            {
                upper = std::max<int>(getFont().getStringWidth(buf), upper);
            }

            return ((float) upper );
        }

        float getRequiredHeight() override
        {
            const auto newlineCount = (float) std::count(text.begin(), text.end(), '\n');
            const auto labelHeight = ((Constants::BASE_FONT_SIZE * (newlineCount + 1)) + (Constants::LINE_SIZE * newlineCount)) * getScale();
            return labelHeight;
        }

        juce::Font getFont()
        {

            if (!font.getTypefaceName().contains("Nimbus"))
            {
                std::ifstream file{"/Users/izmar/Downloads/nimbus-sans-novus-semibold-rounded.otf", std::ios::binary};
                std::vector<char> fontData(std::istreambuf_iterator<char>(file), {});
                font = juce::Font(juce::Typeface::createSystemTypefaceFor(fontData.data(), fontData.size()));
            }

            font.setHeight(Constants::BASE_FONT_SIZE * getScale());

            return font;
        }

    private:
        std::string text;
        const std::function<float()> getScale;
        juce::Font font;
        juce::Colour colour;
};

