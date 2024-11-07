#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "PluginProcessor.hpp"
#include "View.hpp"
#include "melatonin_inspector/melatonin_inspector.h"

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    bool keyPressed(const juce::KeyPress &keyPress) override;
    void resized() override;

private:
    melatonin::Inspector* inspector = nullptr;
    PluginProcessor &pluginProcessor;
    View* view = nullptr;
    int initial_width = 200;
    int initial_height = 500;
};
