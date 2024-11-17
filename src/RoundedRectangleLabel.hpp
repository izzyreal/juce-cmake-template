#pragma once

#include "LabelComponent.hpp"

#include "SimpleLabel.hpp"
#include "Constants.hpp"

class RoundedRectangleLabel : public LabelComponent {
    public:
        RoundedRectangleLabel(const std::function<float()>& getScaleToUse, std::string textToUse)
        {
            simpleLabel = new SimpleLabel(getScaleToUse, textToUse, Constants::chassisColour);
            addAndMakeVisible(simpleLabel);
        }

        float getRequiredWidth() override
        {
            return simpleLabel->getRequiredWidth();
        }

        float getRequiredHeight() override
        {
            return simpleLabel->getRequiredHeight();
        }

        void resized() override
        {
            juce::FlexBox fb;
            fb.items.add(juce::FlexItem(*simpleLabel).withFlex(1.f));
            fb.performLayout(getLocalBounds()); 
        }

        void paint(juce::Graphics &g) override
        {
            g.setColour(Constants::darkLabelColour);
            g.drawRoundedRectangle(getLocalBounds().toFloat(), 5.f, 1.f);
            g.fillRoundedRectangle(getLocalBounds().toFloat(), 5.f);

            LabelComponent::paint(g);
        }

        ~RoundedRectangleLabel() override
        {
            delete simpleLabel;
        }

    private:
        SimpleLabel* simpleLabel = nullptr;
};
