#pragma once

#include "Node.hpp"
#include <juce_gui_basics/juce_gui_basics.h>

#include <vector>
#include <functional>

class ViewUtil
{
    public:
        static float getLabelHeight(const std::string& text,
                const std::function<float()>& getScale);

        static void createComponents(
                node &n,
                std::vector<juce::Component*>& components,
                juce::Component* parent,
                const std::function<float()>& getScale);

        static void createComponent(
                node &n,
                std::vector<juce::Component*>& components,
                juce::Component* parent,
                const std::function<float()>& getScale);

        static juce::Font& getFont(const float scale);
};

