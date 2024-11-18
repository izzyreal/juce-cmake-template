#include "ViewUtil.hpp"

#include "GridWrapper.hpp"
#include "FlexBoxWrapper.hpp"
#include "SvgComponent.hpp"
#include "SvgWithLabelGrid.hpp"
#include "LineFlankedLabel.hpp"
#include "FunctionKeyLabel.hpp"
#include "RoundedRectangleLabel.hpp"
#include "JOrLShape.hpp"
#include "Rectangle.hpp"
#include "Constants.hpp"

#include <fstream>

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
    n.line_flanked_label_component = nullptr;
    n.j_or_l_shape_component = nullptr;
    n.rectangle_component = nullptr;

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
    else if (n.node_type == "j_shape" || n.node_type == "l_shape")
    {
        const auto jOrLShape = new JOrLShape(n.node_type == "j_shape" ? JOrLShape::Shape::J : JOrLShape::Shape::L, getScale);
        components.emplace_back(jOrLShape);
        parent->addAndMakeVisible(components.back());
        n.j_or_l_shape_component = components.back();
        return;
    }
    else if (n.node_type == "face_paint_grey_rectangle" || n.node_type == "chassis_rectangle")
    {
        const auto rectangle = new Rectangle(n.node_type == "chassis_rectangle" ? Constants::chassisColour : Constants::greyFacePaintColour);
        components.emplace_back(rectangle);
        parent->addAndMakeVisible(components.back());
        n.rectangle_component = rectangle;
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

        if (n.label_style == "chassis_background")
        {
            labelComponent = new RoundedRectangleLabel(getScale, n.label, Constants::chassisColour, Constants::darkLabelColour);
        }
        else if (n.label_style == "rounded")
        {
            labelComponent = new RoundedRectangleLabel(getScale, n.label, Constants::darkLabelColour, Constants::chassisColour);
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

juce::Font& ViewUtil::getFont(const float scale)
{
    static juce::Font font;

    if (!font.getTypefaceName().contains("Nimbus"))
    {
        std::ifstream file{"/Users/izmar/Downloads/nimbus-sans-novus-semibold-rounded.otf", std::ios::binary};
        std::vector<char> fontData(std::istreambuf_iterator<char>(file), {});
        font = juce::Font(juce::Typeface::createSystemTypefaceFor(fontData.data(), fontData.size()));
    }

    font.setHeight(Constants::BASE_FONT_SIZE * scale);
    
    return font;
}

