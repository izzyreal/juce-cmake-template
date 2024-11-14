#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

class DebugComponent : public juce::Component {
    public:
        std::string text;
        void paint(juce::Graphics& g) override
        {
            g.fillAll(randomColour);
            g.setColour(randomColour.darker(1.0f));
            g.drawRect(getLocalBounds());
            g.drawText(text, 0, 0, getWidth(), getHeight(), juce::Justification::centred);
        }
    
    private:
        const juce::Colour randomColour = juce::Colour::fromHSV(juce::Random::getSystemRandom().nextFloat(), 0.5f, 0.8f, 0.9f);
};
