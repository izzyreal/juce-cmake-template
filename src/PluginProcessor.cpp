#include "PluginProcessor.hpp"
#include "PluginEditor.hpp"

PluginProcessor::PluginProcessor()
        : AudioProcessor(BusesProperties()
                                 .withInput("Stereo In", juce::AudioChannelSet::stereo(), true)
                                 .withOutput("Stereo Out", juce::AudioChannelSet::stereo(), true))
{
}

const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String PluginProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void PluginProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);
}

void PluginProcessor::releaseResources()
{
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

bool PluginProcessor::isBusesLayoutSupported(const juce::AudioProcessor::BusesLayout &layouts) const
{
    return true;
}

void PluginProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    juce::ignoreUnused(destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
}

void PluginProcessor::processBlock(juce::AudioSampleBuffer &buf, juce::MidiBuffer &midiBuf)
{
    buf.clear();
    midiBuf.clear();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
