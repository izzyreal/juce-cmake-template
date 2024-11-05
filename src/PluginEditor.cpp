#include "PluginEditor.hpp"

PluginEditor::PluginEditor(PluginProcessor& p)
        : AudioProcessorEditor(&p), pluginProcessor(p)
{
    setSize(1000, 800);
    setWantsKeyboardFocus(true);
    view.setBounds(0, 0, 1000, 800);
    addAndMakeVisible(view);
}

bool PluginEditor::keyPressed(const juce::KeyPress &)
{
    return true;
}
