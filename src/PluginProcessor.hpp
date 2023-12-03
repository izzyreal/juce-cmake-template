#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

class PluginProcessor : public juce::AudioProcessor
{
public:
    PluginProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void getStateInformation(juce::MemoryBlock &destData) override;

    void setStateInformation(const void *data, int sizeInBytes) override;

    juce::AudioProcessorEditor *createEditor() override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;

    bool producesMidi() const override;

    bool isMidiEffect() const override;

    double getTailLengthSeconds() const override;

    int getNumPrograms() override;

    int getCurrentProgram() override;

    void setCurrentProgram(int index) override;

    const juce::String getProgramName(int index) override;

    void changeProgramName(int index, const juce::String &newName) override;

    void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

    bool hasEditor() const override { return true; }
};
