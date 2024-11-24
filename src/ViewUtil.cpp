#include "ViewUtil.hpp"

#include "GridWrapper.hpp"
#include "FlexBoxWrapper.hpp"
#include "SvgComponent.hpp"
#include "SvgWithLabelGrid.hpp"
#include "LineFlankedLabel.hpp"
#include "RectangleLabel.hpp"
#include "JOrLShape.hpp"
#include "Rectangle.hpp"
#include "Constants.hpp"
#include "NumKey.hpp"

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
    n.num_key_component = nullptr;

    if (n.node_type == "grid")
    {
        auto gridWrapper = new GridWrapper(n, getScale);
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
    else if (n.node_type == "num_key")
    {
        std::string topLabel;
        std::string bottomLabel;
        
        bool doingTop = true;

        for (auto c : n.label)
        {
            if (c == '\n' && !doingTop)
            {
                break;
            }
            if (c == '\n' && doingTop)
            {
                doingTop = false;
                continue;
            }
            if (doingTop) topLabel += c;
            else bottomLabel += c;
        }

        const auto numKey = new NumKey(getScale, topLabel, bottomLabel, n.svg);
        components.emplace_back(numKey);
        parent->addAndMakeVisible(components.back());
        n.num_key_component = numKey;
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
            labelComponent = new RectangleLabel(getScale, n.label, n.label, Constants::greyFacePaintColour, Constants::darkLabelColour, 0.5f, 10.f);
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
            labelComponent = new RectangleLabel(getScale, n.label, n.label_text_to_calculate_width, Constants::chassisColour, Constants::darkLabelColour, 0.f, 2.f);
        }
        else if (n.label_style == "rounded")
        {
            labelComponent = new RectangleLabel(getScale, n.label, n.label_text_to_calculate_width, Constants::darkLabelColour, Constants::chassisColour, 1.5f, 6.f);
        }
        else if (n.label_style == "pad_letters")
        {
            labelComponent = new SimpleLabel(getScale, n.label, Constants::betweenChassisAndLabelColour); 
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
        std::ifstream file{"/Users/izmar/Downloads/nimbus-sans-novus-semibold-rounded-50.otf", std::ios::binary};
        std::vector<char> fontData(std::istreambuf_iterator<char>(file), {});
        font = juce::Font(juce::Typeface::createSystemTypefaceFor(fontData.data(), fontData.size()));
    }

    font.setHeight(Constants::BASE_FONT_SIZE * scale);
    
    return font;
}

