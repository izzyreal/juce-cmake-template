#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include <string>
#include <functional>

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
            g.drawText(text, 0, 0, getWidth(), getHeight(), juce::Justification::centred);
        }

        float getTotalWidth()
        {
            return (float) getFont().getStringWidth(text) + (getLabelMargin() * 2);
        }

    private:
        std::string text;
        std::string label_style;
        const std::function<float()> getScale;

        juce::Font getFont()
        {
            return juce::Font("Helvetica Neue", 5.f * getScale(), juce::Font::plain);
        }

        float getLabelMargin()
        {
            return getFont().getHeight() * 0.6f;
        }

};

