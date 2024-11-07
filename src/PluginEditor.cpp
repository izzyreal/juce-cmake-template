#include "PluginEditor.hpp"

PluginEditor::PluginEditor(PluginProcessor& p)
        : AudioProcessorEditor(&p), pluginProcessor(p)
{
    setSize(500, 800);
    setWantsKeyboardFocus(true);
    view.setBounds(0, 0, 500, 800);
    setResizable(true, true);
    addAndMakeVisible(view);
}

void PluginEditor::resized()
{
    view.setBounds(0, 0, getWidth(), getHeight());
}

bool PluginEditor::keyPressed(const juce::KeyPress &)
{
    return true;
}
