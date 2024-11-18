#include <juce_gui_basics/juce_gui_basics.h>

#include "LabelComponent.hpp"

#include "SimpleLabel.hpp"
#include "Constants.hpp"

class FunctionKeyLabel : public LabelComponent {

    public:
        FunctionKeyLabel(const std::function<float()>& getScaleToUse, std::string textToUse)
            : getScale(getScaleToUse)
        {
            simpleLabel = new SimpleLabel(getScaleToUse, textToUse, Constants::darkLabelColour);
            addAndMakeVisible(simpleLabel);
        }

        ~FunctionKeyLabel() override
        {
            delete simpleLabel;
        }

        void resized() override
        {
            juce::FlexBox fb;
            fb.items.add(juce::FlexItem(*simpleLabel).withFlex(1.f).withHeight(simpleLabel->getRequiredHeight()).withAlignSelf(juce::FlexItem::AlignSelf::center));
            fb.performLayout(getLocalBounds());
        }

        void paint(juce::Graphics &g) override
        {
            auto requiredWidth = getRequiredWidth();

            auto background_rect = juce::Rectangle((getWidth() - requiredWidth) / 2.f, 0.f, requiredWidth, (float) getHeight());

            g.setColour(Constants::greyFacePaintColour);
            g.fillRect(background_rect);
            Component::paint(g);
        }

        float getRequiredWidth() override
        {
            return simpleLabel->getRequiredWidth() + (getLabelMargin() * 2);
        }

        float getRequiredHeight() override
        {
            return simpleLabel->getRequiredHeight();
        }

    private:
        SimpleLabel* simpleLabel = nullptr;
        const std::function<float()> &getScale;

        float getLabelMargin()
        {
            return ViewUtil::getFont(getScale()).getHeight() * 0.6f;
        }
};
