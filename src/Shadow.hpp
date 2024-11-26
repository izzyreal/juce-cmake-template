#pragma once

#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <melatonin_blur/melatonin_blur.h>

class Shadow : public juce::Component {

    public:
        Shadow(const std::function<float()> &getScaleToUse, const std::function<juce::Path()> getPathToUse, const float shadowSizeToUse) :
            getPath(getPathToUse), getScale(getScaleToUse), shadowSize(shadowSizeToUse) {}

        void paint(juce::Graphics &g) override
        {
            auto scale = getScale();
            auto radius = 1.f * scale * shadowSize;
            juce::Point<float> offset = { 1.f * scale * shadowSize, 0.1f * scale * shadowSize };
            auto spread = 0.f;
            auto color = juce::Colours::black.withAlpha(0.6f);
            melatonin::DropShadow shadow = { color, radius, offset, spread };
            auto path = getPath();
            juce::AffineTransform transform;
            transform = transform.translated(25.f, 25.f);
            path.applyTransform(transform);
            shadow.render(g, path);
        }

    private:
        const std::function<juce::Path()> getPath;
        const std::function<float()> &getScale;
        const float shadowSize;
};
