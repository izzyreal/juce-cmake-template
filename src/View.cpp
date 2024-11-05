#include "View.hpp"

#include "SvgComponent.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include <nlohmann/json.hpp>

#include <fstream>

using json = nlohmann::json;

static void from_json(const json& j, node& n)
{
    if (j.contains("name"))     j.at("name").get_to(n.name);
    if (j.contains("svg"))      j.at("svg").get_to(n.svg);
    if (j.contains("children")) j.at("children").get_to(n.children);
    if (j.contains("margin"))   j.at("margin").get_to(n.margin); else n.margin = 0.f;
    if (j.contains("label"))         j.at("label").get_to(n.label);

    printf("=== Deserialized node ===\n");
    if (!n.name.empty())     printf("-     name: %s\n", n.name.c_str());
    if (!n.svg.empty())      printf("-      svg: %s\n", n.svg.c_str());
    if (!n.children.empty()) printf("- children: %zu\n", n.children.size());
    if (n.margin != 0.f)       printf("-   margin: %f\n", n.margin);
    if (!n.label.empty())    printf("-    label: %s\n", n.label.c_str());
    printf("=========================\n");
}

void View::addViewNodesAsJuceComponents(node& n)
{
    if (!n.svg.empty())
    {
        components.emplace_back(new SvgComponent(n.svg));
        addAndMakeVisible(components.back());
        n.component = components.back();
    }

    for (auto& c : n.children)
    {
        addViewNodesAsJuceComponents(c);
        n.component = nullptr;
    }
}

View::View()
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

void View::createFlexBoxes(juce::FlexBox& parent, node& n, std::vector<juce::FlexBox> &flexBoxes)
{
    flexBoxes.emplace_back(juce::FlexBox());
    flexBoxes.back().flexDirection = juce::FlexBox::Direction::column;
    flexBoxes.back().flexWrap = juce::FlexBox::Wrap::wrap;
    flexBoxes.back().alignContent = juce::FlexBox::AlignContent::flexStart;
    flexBoxes.back().alignItems = juce::FlexBox::AlignItems::flexStart;

    for (auto& c : n.children)
    {
        if (c.component != nullptr)
        {
            parent.items.add(juce::FlexItem(*c.component).withFlex(1.f).withMinWidth(1.f).withMinHeight(getHeight() / 2.f).withMargin(n.margin));
        }
        else
        {
            createFlexBoxes(flexBoxes.back(), c, flexBoxes);
        }
    }

    parent.items.add(juce::FlexItem(flexBoxes.back()).withFlex(1.f).withMinWidth(getWidth()).withMaxHeight(getHeight()));
}

void View::resized()
{
    std::vector<juce::FlexBox> flexBoxes;

    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.alignContent = juce::FlexBox::AlignContent::flexStart;
    fb.alignItems = juce::FlexBox::AlignItems::flexStart;

    for (auto &c : view_root.children)
    {
        createFlexBoxes(fb, c, flexBoxes);
    }

    fb.performLayout(getLocalBounds());
}

