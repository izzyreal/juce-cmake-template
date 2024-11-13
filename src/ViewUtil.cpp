#include "ViewUtil.hpp"

#include "GridWrapper.hpp"
#include "FlexBoxWrapper.hpp"
#include "SvgComponent.hpp"
#include "SimpleText.hpp"
#include "SvgWithLabelGrid.hpp"

float ViewUtil::getLabelHeight(const std::string& text, const std::function<float()>& getScale)
{
    const auto newlineCount = (float) std::count(text.begin(), text.end(), '\n');

    return ((BASE_FONT_SIZE * (newlineCount + 1)) + (LINE_SIZE * newlineCount)) * getScale();
}

void ViewUtil::createComponent(
        node &n,
        std::vector<juce::Component*>& components,
        juce::Component* parent,
        const std::function<float()>& getScale)
{
    n.svg_component = nullptr;
    n.svg_with_label_grid_component = nullptr;
    n.label_component = nullptr;
    n.flex_box_wrapper_component = nullptr;
    n.grid_wrapper_component = nullptr;

    if (n.node_type == "grid")
    {
        auto gridWrapper = new GridWrapper(n);
        createComponents(n, gridWrapper->components, gridWrapper, getScale);
        components.emplace_back(gridWrapper);
        parent->addAndMakeVisible(components.back());
        n.grid_wrapper_component = components.back();
        return;
    }
    else if (n.node_type == "flex_box")
    {
        auto flexBoxWrapper = new FlexBoxWrapper(n, getScale);
        createComponents(n, flexBoxWrapper->components, flexBoxWrapper, getScale);
        components.emplace_back(flexBoxWrapper);
        parent->addAndMakeVisible(components.back());
        n.flex_box_wrapper_component = components.back();
        return;
    }

    if (n.svg.empty())
    {
        return;
    }

    if (n.label.empty())
    {
        components.emplace_back(new SvgComponent(n.svg));
        parent->addAndMakeVisible(components.back());
        n.svg_component = components.back();
        return;
    }

    auto simpleText = new SimpleText(getScale, n.label, n.label_style);
    auto svgComponent = new SvgComponent(n.svg);

    n.svg_component = svgComponent;
    n.label_component = simpleText;

    if (dynamic_cast<GridWrapper*>(parent))
    {
        auto svgWithLabelGrid = new SvgWithLabelGrid(n, getScale);
        svgWithLabelGrid->components.push_back(simpleText);
        svgWithLabelGrid->addAndMakeVisible(simpleText);

        svgWithLabelGrid->components.push_back(svgComponent);

        components.push_back(svgWithLabelGrid);

        parent->addAndMakeVisible(svgWithLabelGrid);
        n.svg_with_label_grid_component = svgWithLabelGrid;
    }
    else /* if parent is FlexBoxWrapper */
    {
        components.push_back(svgComponent);
        parent->addAndMakeVisible(svgComponent);

        components.push_back(simpleText);
        parent->addAndMakeVisible(simpleText);
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

