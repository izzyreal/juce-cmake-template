#include "ViewUtil.hpp"

#include "GridWrapper.hpp"
#include "FlexBoxWrapper.hpp"
#include "SvgComponent.hpp"
#include "SimpleText.hpp"

void ViewUtil::createComponent(
        node &c,
        std::vector<juce::Component*>& components,
        juce::Component* parent,
        const std::function<float()>& getScale)
{
    if (c.node_type == "grid")
    {
        auto gridWrapper = new GridWrapper(c);
        createComponents(c, gridWrapper->components, gridWrapper, getScale);
        components.emplace_back(gridWrapper);
        parent->addAndMakeVisible(components.back());
        c.grid_wrapper_component = components.back();
        return;
    }
    else if (c.node_type == "flex_box")
    {
        auto flexBoxWrapper = new FlexBoxWrapper(c, getScale);
        createComponents(c, flexBoxWrapper->components, flexBoxWrapper, getScale);
        components.emplace_back(flexBoxWrapper);
        parent->addAndMakeVisible(components.back());
        c.flex_box_wrapper_component = components.back();
        return;
    }

    if (c.svg.empty())
    {
        c.svg_component = nullptr;
    }
    else if (!c.svg.empty())
    {
        components.emplace_back(new SvgComponent(c.svg));
        parent->addAndMakeVisible(components.back());
        c.svg_component = components.back();
    }

    if (c.label.empty())
    {
        c.label_component = nullptr;
    }
    else
    {
        components.emplace_back(new SimpleText(getScale, c.label, c.label_style));
        parent->addAndMakeVisible(components.back());
        c.label_component = components.back();
    }
}

void ViewUtil::createComponents(
        node &n,
        std::vector<juce::Component*>& components,
        juce::Component* parent,
        const std::function<float()>& getScale)
{
    for (auto& c : n.children)
    {
        createComponent(c, components, parent, getScale);
    }
}

