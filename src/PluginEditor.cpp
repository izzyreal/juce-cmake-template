#include "PluginEditor.hpp"

PluginEditor::PluginEditor(PluginProcessor& p)
        : AudioProcessorEditor(&p), pluginProcessor(p)
{
    setSize(320, 200);
    setWantsKeyboardFocus(true);

    addAndMakeVisible(debugComponent1);
    addAndMakeVisible(debugComponent2);
    addAndMakeVisible(debugComponent3);
    addAndMakeVisible(debugComponent4);
    addAndMakeVisible(debugComponent5);

    debugComponent1.text = "(1, 1, 1, 1)";
    debugComponent2.text = "(1, 2, 1, 2)";
    debugComponent3.text = "(2, 1, 2, 2)";
    debugComponent4.text = "(2, 2, 2, 3)";
    debugComponent5.text = "(3, 1, 3, 3)";
}

void PluginEditor::resized()
{
    juce::Grid grid;

    grid.templateRows = { juce::Grid::Fr(1), juce::Grid::Fr(1), juce::Grid::Fr(1) };
    grid.templateColumns = { juce::Grid::Fr(1), juce::Grid::Fr(1) };

    grid.items.add(juce::GridItem(debugComponent1).withArea(1, 1, 1, 1));
    grid.items.add(juce::GridItem(debugComponent2).withArea(1, 2, 1, 2));
    grid.items.add(juce::GridItem(debugComponent3).withArea(2, 1, 2, 2));
    grid.items.add(juce::GridItem(debugComponent4).withArea(2, 2, 2, 3));
    grid.items.add(juce::GridItem(debugComponent5).withArea(3, 1, 3, 3));

    grid.performLayout(getLocalBounds());
}

bool PluginEditor::keyPressed(const juce::KeyPress &)
{
    return true;
}
