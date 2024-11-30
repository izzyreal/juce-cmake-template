#define ENABLE_GUI_INSPECTOR 0

#include "PluginEditor.hpp"
#include "Constants.hpp"
#include <fstream>

PluginEditor::PluginEditor(PluginProcessor& p)
        : AudioProcessorEditor(&p), pluginProcessor(p)
{
    std::ifstream file{"/Users/izmar/Downloads/nimbus-sans-novus-semibold-rounded-50.otf", std::ios::binary};
    std::vector<char> fontData(std::istreambuf_iterator<char>(file), {});

    nimbusSans = juce::Font(juce::Typeface::createSystemTypefaceFor(fontData.data(), fontData.size()));

    const auto getScale = [&] { return (float) getHeight() / (float) initial_height; };
    const auto getNimbusSansScaled = [&, getScale]() -> juce::Font& {
        nimbusSans.setHeight(Constants::BASE_FONT_SIZE * getScale());
        return nimbusSans;
    };

    view = new View(getScale, getNimbusSansScaled);

    setSize((int) (initial_width * initial_scale), (int) (initial_height * initial_scale));
    setWantsKeyboardFocus(true);

    setResizable(true, true);
    getConstrainer()->setFixedAspectRatio(initial_width / initial_height);
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
