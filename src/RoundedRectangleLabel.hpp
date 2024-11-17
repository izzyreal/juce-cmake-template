#pragma once

#include "LabelComponent.hpp"

#include "SimpleLabel.hpp"
#include "Constants.hpp"

class RoundedRectangleLabel : public LabelComponent {
    public:
        RoundedRectangleLabel(const std::function<float()>& getScaleToUse, std::string textToUse)
            : getScale(getScaleToUse)
        {
            simpleLabel = new SimpleLabel(getScaleToUse, textToUse, Constants::chassisColour);
            addAndMakeVisible(simpleLabel);
        }

        float getRequiredWidth() override
        {
            return simpleLabel->getRequiredWidth() + 10.f;
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
            //g.fillAll(juce::Colours::yellow);
            g.setColour(Constants::darkLabelColour);
            const auto radius = 1.5f * getScale();
            const auto margin = 5.f;
            const auto requiredHeight = simpleLabel->getRequiredHeight() + margin;
            auto backgroundRect = getLocalBounds();
            backgroundRect.reduce(0.f, (float) (getHeight()) - requiredHeight);
            g.drawRoundedRectangle(backgroundRect.toFloat(), radius, 1.f);
            g.fillRoundedRectangle(backgroundRect.toFloat(), radius);

            LabelComponent::paint(g);
        }

        ~RoundedRectangleLabel() override
        {
            delete simpleLabel;
        }

    private:
        SimpleLabel* simpleLabel = nullptr;
        const std::function<float()> &getScale;
};
