#include "Node.hpp"
#include <juce_gui_basics/juce_gui_basics.h>

#include <vector>
#include <functional>

class ViewUtil
{
    public:
        static void createComponents(
                node &n,
                std::vector<juce::Component*>& components,
                juce::Component* parent,
                const std::function<float()>& getScale);
};

