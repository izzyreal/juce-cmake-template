#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "PluginProcessor.hpp"

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);

    bool keyPressed(const juce::KeyPress &keyPress) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void resized() override;
    void paint(juce::Graphics &g) override;

private:
    PluginProcessor &pluginProcessor;

    struct Footswitch {
        juce::String label;
        int cc;
        bool isPressed = false;
        juce::Rectangle<float> bounds;
    };

    std::vector<Footswitch> footswitches;
    juce::File configFile;

    void saveConfig();
    void loadConfig();
    void drawFootswitches(juce::Graphics &g);
    void editFootswitch(Footswitch& fs);
};

