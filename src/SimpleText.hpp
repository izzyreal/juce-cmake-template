#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include <string>

class SimpleText : public juce::Component {
    public:
        SimpleText(const std::string _text) : text(_text) {}

        void paint(juce::Graphics& g) override
        {
            g.setFont(juce::Font("Helvetica Neue", getHeight(), juce::Font::plain));
            auto text_width = g.getCurrentFont().getStringWidth(text);
            const auto label_margin = getHeight() * 0.6f;
            auto background_rect = juce::Rectangle((getWidth() - (text_width + (label_margin * 2))) / 2.f, 0.f, text_width + (label_margin * 2), (float) getHeight());
            g.setColour(juce::Colours::darkorange);
            g.fillRect(background_rect);
            g.setColour(juce::Colours::black);
            g.drawText(text, 0, 0, getWidth(), getHeight(), juce::Justification::centred);
        }

    private:
        std::string text;
};

