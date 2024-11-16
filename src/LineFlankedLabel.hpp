#include <juce_gui_basics/juce_gui_basics.h>

#include "SimpleText.hpp"

class LineFlankedLabel : public juce::Component {
    public:
        LineFlankedLabel(const std::string &textToUse, const std::function<float()> &getScaleToUse)
            : text(textToUse), getScale(getScaleToUse)
        {
            simpleText = new SimpleText(getScaleToUse, textToUse, "");
            addAndMakeVisible(simpleText);
        }

        ~LineFlankedLabel() override
        {
            delete simpleText;
        }

        void resized() override
        {
            simpleText->setBounds(getLocalBounds());
        }

        void paint(juce::Graphics& g) override
        {
            const auto textWidth = simpleText->getTotalWidth();
            const auto lineInterruptionStartX = (getWidth() - textWidth) / 2;
            const auto lineInterruptionEndX = getWidth() - lineInterruptionStartX;

            g.setColour(juce::Colours::black);
            const auto line_y_base = getHeight() / 2;
            const auto line_thickness = 2;
            g.drawLine(0, line_y_base, lineInterruptionStartX, line_y_base, line_thickness);
            g.drawLine(lineInterruptionEndX, line_y_base, getWidth(), line_y_base, line_thickness);
        }

    private:
        const std::string text;
        const std::function<float()>& getScale;
        SimpleText* simpleText = nullptr;

};
