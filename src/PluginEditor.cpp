#include "PluginEditor.hpp"
#include <juce_data_structures/juce_data_structures.h>

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), pluginProcessor(p)
{
    setResizable(true, true);
    setResizeLimits(200, 150, 2000, 1500);
    setWantsKeyboardFocus(true);

    configFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                     .getChildFile("VirtualFootswitchConfig.json");

    loadConfig();

    if (footswitches.empty()) {
        for (int i = 0; i < 40; ++i)
            footswitches.push_back({ "FS" + juce::String(i + 1), 20 + i, false, {} });
    }

    setSize(640, 400);
}

void PluginEditor::resized()
{
    auto area = getLocalBounds().toFloat();
    int cols = 10, rows = 4;
    float w = area.getWidth() / cols;
    float h = area.getHeight() / rows;

    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) {
            int idx = r * cols + c;
            if (idx < (int)footswitches.size())
                footswitches[idx].bounds = { c * w + 2, r * h + 2, w - 4, h - 4 };
        }
}

void PluginEditor::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::darkgrey);
    drawFootswitches(g);
}

void PluginEditor::drawFootswitches(juce::Graphics &g)
{
    for (auto &fs : footswitches) {
        g.setColour(fs.isPressed ? juce::Colours::orange : juce::Colours::lightgrey);
        g.fillRoundedRectangle(fs.bounds, 6.0f);

        g.setColour(juce::Colours::black);
        g.drawRoundedRectangle(fs.bounds, 6.0f, 1.0f);

        g.drawText(fs.label + " (CC " + juce::String(fs.cc) + ")",
                   fs.bounds, juce::Justification::centred);
    }
}

void PluginEditor::mouseDown(const juce::MouseEvent& e)
{
    for (auto &fs : footswitches) {
        if (fs.bounds.contains(e.position)) {

            if (e.mods.isRightButtonDown()) {
                editFootswitch(fs);
                return;
            }

            fs.isPressed = true;
            pluginProcessor.pushMidiMessage(
                juce::MidiMessage::controllerEvent(1, fs.cc, 64));
            repaint();
            break;
        }
    }
}

void PluginEditor::mouseUp(const juce::MouseEvent& e)
{
    for (auto &fs : footswitches) {
        if (fs.isPressed && fs.bounds.contains(e.position)) {
            fs.isPressed = false;
            pluginProcessor.pushMidiMessage(
                juce::MidiMessage::controllerEvent(1, fs.cc, 0));
            repaint();
            break;
        }
    }
}

bool PluginEditor::keyPressed(const juce::KeyPress &) { return true; }

void PluginEditor::saveConfig()
{
    juce::DynamicObject::Ptr root = new juce::DynamicObject();
    juce::Array<juce::var> arr;

    for (auto &fs : footswitches) {
        juce::DynamicObject::Ptr obj = new juce::DynamicObject();
        obj->setProperty("label", fs.label);
        obj->setProperty("cc", fs.cc);
        arr.add(juce::var(obj.get()));
    }
    root->setProperty("footswitches", arr);

    juce::FileOutputStream out(configFile);
    if (out.openedOk()) {
        out.setPosition(0);
        out.truncate();
        out.writeText(juce::JSON::toString(juce::var(root.get()), true, true), false, false, "\n");
    }
}

void PluginEditor::loadConfig()
{
    if (configFile.existsAsFile()) {
        juce::var data = juce::JSON::parse(configFile);
        if (auto* root = data.getDynamicObject()) {
            auto arr = root->getProperty("footswitches");
            if (arr.isArray()) {
                footswitches.clear();
                for (auto &v : *arr.getArray()) {
                    if (auto* obj = v.getDynamicObject())
                        footswitches.push_back({
                            obj->getProperty("label").toString(),
                            (int)obj->getProperty("cc"), false, {}
                        });
                }
                return;
            }
        }
    }

    footswitches.clear();

    const juce::StringArray defaultLabels = {
        "PLAY STRT",
        "PLAY",
        "STOP",
        "REC+PLAY",
        "ODUB+PLAY",
        "REC/PUNCH",
        "ODUB/PNCH",
        "TAP",
        "PAD BNK A",
        "PAD BNK B",
        "PAD BNK C",
        "PAD BNK D",
        "PAD 1", "PAD 2", "PAD 3", "PAD 4",
        "PAD 5", "PAD 6", "PAD 7", "PAD 8",
        "PAD 9", "PAD 10", "PAD 11", "PAD 12",
        "PAD 13", "PAD 14", "PAD 15", "PAD 16",
        "F1", "F2", "F3", "F4", "F5", "F6"
    };

    for (int i = 0; i < defaultLabels.size(); ++i)
    {
        Footswitch fs;
        fs.label = defaultLabels[i];
        fs.cc = i; // ascending CC from 0 upwards
        fs.isPressed = false;
        footswitches.push_back(fs);
    }

    saveConfig(); // persist the default config
}

void PluginEditor::editFootswitch(Footswitch& fs)
{
    auto* window = new juce::AlertWindow("Edit Footswitch",
                                         "Modify the label and CC number.",
                                         juce::AlertWindow::NoIcon);

    window->addTextEditor("label", fs.label, "Label:");
    window->addTextEditor("cc", juce::String(fs.cc), "MIDI CC:");
    window->getTextEditor("cc")->setInputRestrictions(3, "0123456789");

    window->addButton("OK", 1);
    window->addButton("Cancel", 0);

    // Use async to avoid blocking the message thread
    window->enterModalState(true, juce::ModalCallbackFunction::create(
        [this, window, &fs](int result)
        {
            if (result == 1) // OK pressed
            {
                fs.label = window->getTextEditor("label")->getText();
                fs.cc = window->getTextEditor("cc")->getText().getIntValue();
                saveConfig();
                repaint();
            }
            delete window;
        }
    ));
}
