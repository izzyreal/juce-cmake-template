#include "GridWrapper.hpp"

#include "FlexBoxWrapper.hpp"

#include "LabelComponent.hpp"

#include <cassert>

GridWrapper::GridWrapper(struct node &nodeToUse) : node(nodeToUse)
{
    printf("GridWrapper for %s\n", node.name.c_str());
}

GridWrapper::~GridWrapper()
{
    for (auto c : components)
        delete c;
}

static void processChildren(
        juce::Grid& parent,
        const std::vector<node>& children)
{
    for (auto& c : children)
    {
        if (c.node_type == "grid")
        {
            parent.items.add(juce::GridItem(c.grid_wrapper_component).withArea(c.area[0], c.area[1], c.area[2], c.area[3]));
            continue;
        }
        else if (c.node_type == "flex_box")
        {
            parent.items.add(juce::GridItem(c.flex_box_wrapper_component).withArea(c.area[0], c.area[1], c.area[2], c.area[3]));
            continue;
        }
        else if (c.node_type == "line_flanked_label")
        {
            parent.items.add(juce::GridItem(c.line_flanked_label_component).withArea(c.area[0], c.area[1], c.area[2], c.area[3]));
            continue;
        }

        if (c.svg_with_label_grid_component != nullptr)
        {
            parent.items.add(juce::GridItem(c.svg_with_label_grid_component).withArea(c.area[0], c.area[1], c.area[2], c.area[3]));
            continue;
        }

        if (c.svg_component != nullptr)
        {
           // The case where there's both an SVG, as well as a label, should be handled by svg_with_label_grid_component.
           // Hence we make sure there's no label Component associated with this node.
            assert(c.label_component == nullptr);
            parent.items.add(juce::GridItem(c.svg_component).withArea(c.area[0], c.area[1], c.area[2], c.area[3]));
            continue;
        }

        if (c.label_component != nullptr)
        {
            const auto width = dynamic_cast<LabelComponent*>(c.label_component)->getRequiredWidth();
            const auto item = juce::GridItem(c.label_component).withArea(c.area[0], c.area[1], c.area[2], c.area[3]);
            parent.items.add(item.withWidth(width));
        }
    }
}

void GridWrapper::resized()
{
    printf("GridWrapper for %s resized to %i, %i\n", node.name.c_str(), getWidth(), getHeight());
    juce::Grid grid;
    grid.justifyItems = juce::Grid::JustifyItems::center;

    juce::Array<juce::Grid::TrackInfo> rowTrackInfos;
    juce::Array<juce::Grid::TrackInfo> columnTrackInfos;

    for (auto& f : node.row_fractions)
        rowTrackInfos.add(juce::Grid::TrackInfo(juce::Grid::Fr(f)));

    for (auto& f : node.column_fractions)
        columnTrackInfos.add(juce::Grid::TrackInfo(juce::Grid::Fr(f)));

    grid.templateRows = rowTrackInfos;
    grid.templateColumns = columnTrackInfos;

    processChildren(grid, node.children);

    grid.performLayout(getLocalBounds());
}

