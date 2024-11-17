#include <juce_gui_basics/juce_gui_basics.h>

#include "LabelComponent.hpp"

#include "SimpleLabel.hpp"
#include "Constants.hpp"

class FunctionKeyLabel : public LabelComponent {

    public:
        FunctionKeyLabel(const std::function<float()>& getScaleToUse, std::string textToUse)
        {
            simpleLabel = new SimpleLabel(getScaleToUse, textToUse, Constants::labelColour);
            addAndMakeVisible(simpleLabel);
        }

        ~FunctionKeyLabel() override
        {
            delete simpleLabel;
        }

        void resized() override
        {
            juce::FlexBox fb;
            fb.justifyContent = juce::FlexBox::JustifyContent::center;
            fb.alignContent = juce::FlexBox::AlignContent::center;

            fb.items.add(juce::FlexItem(*simpleLabel).withFlex(1.f).withHeight(simpleLabel->getRequiredHeight()).withAlignSelf(juce::FlexItem::AlignSelf::center));
            fb.performLayout(getLocalBounds());
        }

        void paint(juce::Graphics &g) override
        {
            auto requiredWidth = getRequiredWidth();

            auto background_rect = juce::Rectangle((getWidth() - requiredWidth) / 2.f, 0.f, requiredWidth, (float) getHeight());

            juce::Colour label_background = juce::Colour::fromRGB(139, 151, 163);
            g.setColour(label_background);
            g.fillRect(background_rect);
            Component::paint(g);
        }

        float getRequiredWidth() override
        {
            return simpleLabel->getRequiredWidth() + (getLabelMargin() * 2);
        }

    private:
        SimpleLabel* simpleLabel = nullptr;

        float getLabelMargin()
        {
            return simpleLabel->getFont().getHeight() * 0.6f;
        }
};
