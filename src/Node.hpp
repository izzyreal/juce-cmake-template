#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

#include <vector>
#include <string>

struct node {         
    std::string name;
    std::string node_type;
    std::string svg;
    std::vector<node> children;
    float margin;
    std::string label;
    std::string label_style;
    std::string direction;
    float flex_grow;
    std::string align_items;
    std::vector<float> margins;

    // grid
    std::vector<uint8_t> row_fractions;
    std::vector<uint8_t> column_fractions;

    // grid leaf item
    std::vector<uint8_t> area;

    juce::Component *svg_component;
    juce::Component *label_component;
    juce::Component *grid_wrapper_component;
    juce::Component *flex_box_wrapper_component;
    juce::Component *svg_with_label_grid_component;
    juce::Component *line_flanked_label_component;
    juce::Component *j_or_l_shape_component;
    juce::Component *rectangle_component;
};
