#include "View.hpp"

#include "SvgComponent.h"
#include "SimpleText.hpp"
#include "juce_gui_basics/juce_gui_basics.h"

#include <nlohmann/json.hpp>

#include <fstream>

using json = nlohmann::json;

static void from_json(const json& j, node& n)
{
    if (j.contains("name"))        j.at("name").get_to(n.name);
    if (j.contains("svg"))         j.at("svg").get_to(n.svg);
    if (j.contains("children"))    j.at("children").get_to(n.children);
    if (j.contains("margin"))      j.at("margin").get_to(n.margin); else n.margin = 0.f;
    if (j.contains("label"))       j.at("label").get_to(n.label);
    if (j.contains("label_style")) j.at("label_style").get_to(n.label_style);
    if (j.contains("direction"))   j.at("direction").get_to(n.direction);

    if (j.contains("include"))
    {
        std::ifstream jsonFile("/Users/izmar/projects/VMPC2000XL/vector UI/views/" + j.at("include").get<std::string>() + ".json");
        json data = json::parse(jsonFile);
        n = data.template get<node>();
    }

    printf("=== Deserialized node ===\n");
    if (!n.name.empty())        printf("-        name: %s\n", n.name.c_str());
    if (!n.svg.empty())         printf("-         svg: %s\n", n.svg.c_str());
    if (!n.children.empty())    printf("-    children: %zu\n", n.children.size());
    if (n.margin != 0.f)        printf("-      margin: %f\n", n.margin);
    if (!n.label.empty())       printf("-       label: %s\n", n.label.c_str());
    if (!n.label_style.empty()) printf("- label_style: %s\n", n.label_style.c_str());
    printf("=========================\n");
}

void View::addViewNodesAsJuceComponents(node& n)
{
    if (n.svg.empty())
    {
        n.svg_component = nullptr;
    }
    else
    {
        components.emplace_back(new SvgComponent(n.svg));
        addAndMakeVisible(components.back());
        n.svg_component = components.back();
    }

    if (n.label.empty())
    {
        n.label_component = nullptr;
    }
    else
    {
        components.emplace_back(new SimpleText(getScale, n.label, n.label_style));
        addAndMakeVisible(components.back());
        n.label_component = components.back();
    }

    for (auto& c : n.children)
    {
        addViewNodesAsJuceComponents(c);
    }
}

View::View(const std::function<float()>& getScaleToUse) : getScale(getScaleToUse)
{
    std::ifstream jsonFile("/Users/izmar/projects/VMPC2000XL/vector UI/views/default.json");
    json data = json::parse(jsonFile);
    view_root = data.template get<node>();

    addViewNodesAsJuceComponents(view_root);
}

View::~View()
{
    for (auto& c : components)
    {
        delete c;
    }
}

void View::paint(juce::Graphics& g)
{
    juce::Colour mpc2000xl_chassis = juce::Colour::fromRGB(230, 238, 233);
    g.fillAll(mpc2000xl_chassis);
}

float View::getLabelHeight()
{
    return 5.f * getScale();
}

void View::createFlexBoxes(juce::FlexBox& parent, node& n, std::vector<std::unique_ptr<juce::FlexBox>> &flexBoxes)
{
    for (auto& c : n.children)
    {
        const bool child_is_leaf = c.children.empty();

        if (!child_is_leaf)
        {
            auto childFlexBox = std::make_unique<juce::FlexBox>();
            childFlexBox->justifyContent = juce::FlexBox::JustifyContent::center;
            if (c.direction == "column")
            {
                childFlexBox->flexDirection = juce::FlexBox::Direction::column;
            }
            else
            {
                childFlexBox->flexDirection = juce::FlexBox::Direction::row;
            }


            parent.items.add(juce::FlexItem(*childFlexBox).withMinWidth(1.f).withFlex(1.f));

            flexBoxes.push_back(std::move(childFlexBox));
            createFlexBoxes(*flexBoxes.back(), c, flexBoxes);
        }
        else if (c.svg_component != nullptr)
        {
            const auto drawable_bounds = dynamic_cast<SvgComponent*>(c.svg_component)->getDrawableBounds();
            const auto minWidth = drawable_bounds.getWidth() * getScale();
            const auto minHeight = drawable_bounds.getHeight() * getScale();

            if (c.label.empty())
            {
                parent.items.add(juce::FlexItem(*c.svg_component).withMinWidth(minWidth).withMinHeight(minHeight));
            }
            else
            {
                auto childFlexBox = std::make_unique<juce::FlexBox>();
                childFlexBox->flexDirection = juce::FlexBox::Direction::column;
                const auto min_width_label = dynamic_cast<SimpleText*>(c.label_component)->getTotalWidth();

                const auto childFlexBoxMinWidth = std::max<float>((float) min_width_label, minWidth);

                parent.items.add(juce::FlexItem(*childFlexBox).withMinWidth(childFlexBoxMinWidth).withMinHeight(minHeight + (getLabelHeight() * 2)));
                flexBoxes.push_back(std::move(childFlexBox));

                auto label_item = juce::FlexItem(*c.label_component)
                    .withMinWidth(childFlexBoxMinWidth)
                    .withMinHeight(getLabelHeight())
                    .withMargin(juce::FlexItem::Margin(0.f, 0.f, getLabelHeight(), 0.f));
                
                flexBoxes.back()->items.add(label_item);
                flexBoxes.back()->items.add(juce::FlexItem(*c.svg_component).withMinWidth(minWidth).withMinHeight(minHeight));
            }
        }
    }
}

void View::resized()
{
    std::vector<std::unique_ptr<juce::FlexBox>> flexBoxes;

    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;

    createFlexBoxes(fb, view_root, flexBoxes);

    fb.performLayout(getLocalBounds());
}

