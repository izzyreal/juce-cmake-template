#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "PluginProcessor.hpp"
#include "DebugComponent.hpp"

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);

    void resized() override;

    bool keyPressed(const juce::KeyPress &keyPress) override;

private:
    PluginProcessor &pluginProcessor;
    DebugComponent debugComponent1;
    DebugComponent debugComponent2;
    DebugComponent debugComponent3;
    DebugComponent debugComponent4;
    DebugComponent debugComponent5;
};
