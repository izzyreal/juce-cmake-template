#define ENABLE_GUI_INSPECTOR 0

#include "PluginEditor.hpp"

PluginEditor::PluginEditor(PluginProcessor& p)
        : AudioProcessorEditor(&p), pluginProcessor(p)
{
    view = new View([&] { return (float) getHeight() / initial_height; });

    setSize(initial_width, initial_height);
    setWantsKeyboardFocus(true);

    setResizable(true, true);
    addAndMakeVisible(view);
#if ENABLE_GUI_INSPECTOR == 1
    inspector = new melatonin::Inspector(*this);
    inspector->setVisible(true);
    inspector->toggle(true);
#endif
}

PluginEditor::~PluginEditor()
{
    delete view;
    delete inspector;
}

void PluginEditor::resized()
{
    view->setBounds(0, 0, getWidth(), getHeight());
}

bool PluginEditor::keyPressed(const juce::KeyPress &)
{
    return true;
}
