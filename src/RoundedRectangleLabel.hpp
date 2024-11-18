#pragma once

#include "LabelComponent.hpp"

#include "SimpleLabel.hpp"

class RoundedRectangleLabel : public LabelComponent {
    public:
        RoundedRectangleLabel(const std::function<float()>& getScaleToUse, std::string textToUse, juce::Colour bgColourToUse, juce::Colour fgColourToUse)
            : getScale(getScaleToUse), bgColour(bgColourToUse)
        {
            simpleLabel = new SimpleLabel(getScaleToUse, textToUse, fgColourToUse);
            addAndMakeVisible(simpleLabel);
        }

        float getRequiredWidth() override
        {
            const auto widthOfBiggestRoundedRectangleLabelInTheLocateSection =
                ViewUtil::getFont(getScale()).getStringWidth("START");
            return widthOfBiggestRoundedRectangleLabelInTheLocateSection + 6.f;
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
            g.setColour(bgColour);
            const auto radius = 1.5f * getScale();
            const auto margin = 3.f;
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
        const juce::Colour bgColour;
};
