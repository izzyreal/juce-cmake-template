#include "View.hpp"

#include "GridWrapper.hpp"
#include "FlexBoxWrapper.hpp"
#include "ViewUtil.hpp"

#include <nlohmann/json.hpp>

#include <fstream>

using json = nlohmann::json;

static void from_json(const json& j, node& n)
{
    if (j.contains("include"))
    {
        std::ifstream jsonFile("/Users/izmar/projects/VMPC2000XL/vector UI/views/" + j.at("include").get<std::string>() + ".json");
        json data = json::parse(jsonFile);
        n = data.template get<node>();
        if (j.contains("area")) j.at("area").get_to(n.area);
    }
    else
    {
        if (j.contains("name"))             j.at("name").get_to(n.name);
        if (j.contains("type"))             j.at("type").get_to(n.node_type);
        if (j.contains("svg"))              j.at("svg").get_to(n.svg);
        if (j.contains("children"))         j.at("children").get_to(n.children);
        if (j.contains("margin"))           j.at("margin").get_to(n.margin); else n.margin = 0.f;
        if (j.contains("label"))            j.at("label").get_to(n.label);
        if (j.contains("label_style"))      j.at("label_style").get_to(n.label_style);
        if (j.contains("direction"))        j.at("direction").get_to(n.direction);
        if (j.contains("flex_grow"))        j.at("flex_grow").get_to(n.flex_grow); else n.flex_grow = 0.f;
        if (j.contains("align_items"))      j.at("align_items").get_to(n.align_items);
        if (j.contains("row_fractions"))    j.at("row_fractions").get_to(n.row_fractions);
        if (j.contains("column_fractions")) j.at("column_fractions").get_to(n.column_fractions);
        if (j.contains("area"))             j.at("area").get_to(n.area);
    }


    if (!n.area.empty())
    {
        // JUCE's withArea(rowStart, columnStart, rowEnd, columnEnd) API expects exclusive
        // rowEnd and columnEnd arguments. I find that unintuitive, so in the layouts I
        // specify inclusive end arguments, and do a fix-up here.
        n.area[2] += 1;
        n.area[3] += 1;
    }

    printf("=== Deserialized node ===\n");
    if (!n.name.empty())        printf("-        name: %s\n", n.name.c_str());
    if (!n.svg.empty())         printf("-         svg: %s\n", n.svg.c_str());
    if (!n.children.empty())    printf("-    children: %zu\n", n.children.size());
    if (n.margin != 0.f)        printf("-      margin: %f\n", n.margin);
    if (!n.label.empty())       printf("-       label: %s\n", n.label.c_str());
    if (!n.label_style.empty()) printf("- label_style: %s\n", n.label_style.c_str());
    if (n.flex_grow != 0.f)     printf("-   flex_grow: %f\n", n.flex_grow);

    if (!n.row_fractions.empty())
    {
        printf("- row_fractions:");
        for (auto& f : n.row_fractions)
            printf(" %i ", f);
        printf("\n");
    }

    if (!n.area.empty())
    {
        printf("-          area:");
        for (auto& f : n.area)
            printf(" %i ", f);
        printf("\n");
    }

    printf("=========================\n");
}

View::View(const std::function<float()>& getScaleToUse) : getScale(getScaleToUse)
{
    std::ifstream jsonFile("/Users/izmar/projects/VMPC2000XL/vector UI/views/default.json");
    json data = json::parse(jsonFile);

    view_root = data.template get<node>();

    ViewUtil::createComponent(view_root, components, this, getScale);
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

void View::resized()
{
    assert(!components.empty());

    auto rootComponent = components.front();

    assert(dynamic_cast<GridWrapper*>(rootComponent) != nullptr ||
            dynamic_cast<FlexBoxWrapper*>(rootComponent) != nullptr);

    rootComponent->setSize(getWidth(), getHeight());
}

