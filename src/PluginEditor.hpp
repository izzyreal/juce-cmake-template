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
    const float initial_width =  491.f;
    const float initial_height = 330.f;
    const float initial_scale = 2.4f;
};
