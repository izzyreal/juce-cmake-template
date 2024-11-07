#include "PluginEditor.hpp"

PluginEditor::PluginEditor(PluginProcessor& p)
        : AudioProcessorEditor(&p), pluginProcessor(p)
{
    view = new View([&] { return (float) getHeight() / initial_height; });

    setSize(initial_width, initial_height);
    setWantsKeyboardFocus(true);

    setResizable(true, true);
    addAndMakeVisible(view);
}

PluginEditor::~PluginEditor()
{
    delete view;
}

void PluginEditor::resized()
{
    view->setBounds(0, 0, getWidth(), getHeight());
}

bool PluginEditor::keyPressed(const juce::KeyPress &)
{
    return true;
}
