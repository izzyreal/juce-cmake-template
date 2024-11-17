#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class LabelComponent : public juce::Component {
    public:
        virtual float getRequiredWidth() { return 0.f; }
        virtual float getRequiredHeight() { return 0.f; }
};
