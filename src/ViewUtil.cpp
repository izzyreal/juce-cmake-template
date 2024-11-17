#include "ViewUtil.hpp"

#include "GridWrapper.hpp"
#include "FlexBoxWrapper.hpp"
#include "SvgComponent.hpp"
#include "SvgWithLabelGrid.hpp"
#include "LineFlankedLabel.hpp"
#include "FunctionKeyLabel.hpp"
#include "RoundedRectangleLabel.hpp"
#include "Constants.hpp"

float ViewUtil::getLabelHeight(const std::string& text, const std::function<float()>& getScale)
{
    const auto newlineCount = (float) std::count(text.begin(), text.end(), '\n');

    return ((Constants::BASE_FONT_SIZE * (newlineCount + 1)) + (Constants::LINE_SIZE * newlineCount)) * getScale();
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
    else if (n.node_type == "line_flanked_label")
    {
        components.emplace_back(new LineFlankedLabel(n.label, getScale));
        parent->addAndMakeVisible(components.back());
        n.line_flanked_label_component = components.back();
        return;
    }

    if (!n.svg.empty() && n.label.empty())
    {
        components.emplace_back(new SvgComponent(n.svg));
        parent->addAndMakeVisible(components.back());
        n.svg_component = components.back();
        return;
    }

    if (!n.svg.empty() && !n.label.empty())
    {
        LabelComponent* labelComponent;
        if (n.label_style == "function_key")
        {
            labelComponent = new FunctionKeyLabel(getScale, n.label);
        }
        else
        {
            labelComponent = new SimpleLabel(getScale, n.label, Constants::labelColour);
        }
        auto svgComponent = new SvgComponent(n.svg);

        n.svg_component = svgComponent;
        n.label_component = labelComponent;

        if (dynamic_cast<GridWrapper*>(parent))
        {
            auto svgWithLabelGrid = new SvgWithLabelGrid(n, getScale);
            svgWithLabelGrid->components.push_back(labelComponent);
            svgWithLabelGrid->addAndMakeVisible(labelComponent);

            svgWithLabelGrid->components.push_back(svgComponent);

            components.push_back(svgWithLabelGrid);

            parent->addAndMakeVisible(svgWithLabelGrid);
            n.svg_with_label_grid_component = svgWithLabelGrid;
        }
        else /* if parent is FlexBoxWrapper */
        {
            components.push_back(svgComponent);
            parent->addAndMakeVisible(svgComponent);

            components.push_back(labelComponent);
            parent->addAndMakeVisible(labelComponent);
        }
        return;
    }

    if (!n.label.empty())
    {
        LabelComponent* labelComponent = nullptr;

        if (n.label_style == "rounded")
        {
            labelComponent = new RoundedRectangleLabel(getScale, n.label);
        }
        else
        {
            labelComponent = new SimpleLabel(getScale, n.label, Constants::labelColour); 
        }

        n.label_component = labelComponent;
        components.push_back(labelComponent);
        parent->addAndMakeVisible(labelComponent);
        return;
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

