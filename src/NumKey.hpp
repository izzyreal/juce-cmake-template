#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "SimpleLabel.hpp"
#include "Constants.hpp"
#include "SvgComponent.hpp"
#include "FunctionKeyLabel.hpp"

class NumKey : public juce::Component {
    public:
        NumKey(const std::function<float()> &getScaleToUse, const std::string topLabelToUse, const std::string bottomLabelToUse, std::string svgPath)
        {
            topLabel = new SimpleLabel(getScaleToUse, topLabelToUse, Constants::darkLabelColour);
            bottomLabel = new FunctionKeyLabel(getScaleToUse, bottomLabelToUse);
            svgComponent = new SvgComponent(svgPath);

            addAndMakeVisible(topLabel);
            addAndMakeVisible(bottomLabel);
            addAndMakeVisible(svgComponent);
        }

        ~NumKey() override
        {
            delete topLabel;
            delete bottomLabel;
            delete svgComponent;
        }

        void resized() override
        {
            juce::Grid grid;
            grid.templateRows = { juce::Grid::Fr(1), juce::Grid::Fr(1), juce::Grid::Fr(1) };
            grid.templateColumns = { juce::Grid::Fr(1) };

            grid.items.add(juce::GridItem(topLabel).withArea(1, 1, 1, 1));
            grid.items.add(juce::GridItem(svgComponent).withArea(2, 1, 2, 1));
            grid.items.add(juce::GridItem(bottomLabel).withArea(3, 1, 3, 1));

            grid.performLayout(getLocalBounds());
        }

    private:
        SimpleLabel* topLabel = nullptr;
        FunctionKeyLabel* bottomLabel = nullptr;
        SvgComponent* svgComponent = nullptr;
};
