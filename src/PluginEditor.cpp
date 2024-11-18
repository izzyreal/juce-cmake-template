#define ENABLE_GUI_INSPECTOR 0

#include "PluginEditor.hpp"

#include "ViewUtil.hpp"

PluginEditor::PluginEditor(PluginProcessor& p)
        : AudioProcessorEditor(&p), pluginProcessor(p)
{
    view = new View([&] { return (float) getHeight() / (float) initial_height; });

    setSize((int) (initial_width * initial_scale), (int) (initial_height * initial_scale));
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
    // We assure the custom font is not present in the statically allocated font
    ViewUtil::getFont(0.f).setTypefaceName("foo");
    delete view;
    delete inspector;
}

void PluginEditor::resized()
{
    printf("Resized to %i, %i\n", getWidth(), getHeight());
    view->setBounds(0, 0, getWidth(), getHeight());
}

bool PluginEditor::keyPressed(const juce::KeyPress &)
{
    return true;
}
