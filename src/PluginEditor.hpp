#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "PluginProcessor.hpp"
#include "View.hpp"

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);

    bool keyPressed(const juce::KeyPress &keyPress) override;
    void resized() override;

private:
    PluginProcessor &pluginProcessor;
    View view;
};
