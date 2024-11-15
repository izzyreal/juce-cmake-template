#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include <string>
#include <functional>

const float BASE_FONT_SIZE = 4.f;
const float LINE_SIZE = BASE_FONT_SIZE * 0.3f;

class SimpleText : public juce::Component {
    public:
        SimpleText(
                const std::function<float()>& getScaleToUse,
                const std::string _text,
                const std::string _label_style)
            : text(_text), label_style(_label_style), getScale(getScaleToUse)
        {}

        void paint(juce::Graphics& g) override
        {
            g.setFont(getFont());

            if (label_style == "top_with_background")
            {
                auto background_rect = juce::Rectangle((getWidth() - getTotalWidth()) / 2.f, 0.f, getTotalWidth(), (float) getHeight());
                juce::Colour label_background = juce::Colour::fromRGB(139, 151, 163);
                g.setColour(label_background);
                g.fillRect(background_rect);
            }

            g.setColour(juce::Colours::black);

            const auto newlineCount = (float) std::count(text.begin(), text.end(), '\n');

            const auto labelHeight = ((BASE_FONT_SIZE * (newlineCount + 1)) + (LINE_SIZE * newlineCount)) * getScale();

            const auto yOffset = (getHeight() - labelHeight) / 2;

            int row = 0;
            std::string buf;
            bool should_draw = false;

            for (auto c : text) 
            {
                if (c == '\n')
                {
                    g.drawText(buf, 0, yOffset + row * (BASE_FONT_SIZE + LINE_SIZE) * getScale(), getWidth(), getHeight(), juce::Justification::centredTop);
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
                g.drawText(buf, 0, yOffset + row * (BASE_FONT_SIZE + LINE_SIZE) * getScale(), getWidth(), getHeight(), juce::Justification::centredTop);
            }
        }

        float getTotalWidth()
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

            return ((float) upper + (getLabelMargin() * 2));
        }

    private:
        std::string text;
        std::string label_style;
        const std::function<float()> getScale;

        juce::Font getFont()
        {
            return juce::Font("Helvetica Neue", BASE_FONT_SIZE * getScale(), juce::Font::bold);
        }

        float getLabelMargin()
        {
            return getFont().getHeight() * 0.6f;
        }

};

