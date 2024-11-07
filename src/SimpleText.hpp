#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include <string>

class SimpleText : public juce::Component {
    public:
        SimpleText(const std::string _text, const std::string _label_style) : text(_text), label_style(_label_style) {}

       void paint(juce::Graphics& g) override
        {
            g.setFont(font);

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
            return (float) font.getStringWidth(text) + (label_margin * 2);
        }

    private:
        std::string text;
        std::string label_style;
        juce::Font font = juce::Font("Helvetica Neue", 7.f, juce::Font::plain);
        const float label_margin = font.getHeight() * 0.6f;
};

