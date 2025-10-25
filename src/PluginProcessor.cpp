#include "PluginProcessor.hpp"
#include "PluginEditor.hpp"

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Stereo In", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Stereo Out", juce::AudioChannelSet::stereo(), true))
{
}

const juce::String PluginProcessor::getName() const { return JucePlugin_Name; }

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

double PluginProcessor::getTailLengthSeconds() const { return 0.0; }
int PluginProcessor::getNumPrograms() { return 1; }
int PluginProcessor::getCurrentProgram() { return 0; }
void PluginProcessor::setCurrentProgram(int) {}
const juce::String PluginProcessor::getProgramName(int) { return {}; }
void PluginProcessor::changeProgramName(int, const juce::String&) {}

void PluginProcessor::prepareToPlay(double, int) {}
void PluginProcessor::releaseResources() {}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout &) const { return true; }

void PluginProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    juce::ignoreUnused(destData);
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
}

void PluginProcessor::pushMidiMessage(const juce::MidiMessage& msg)
{
    const juce::ScopedLock lock(midiLock);
    outgoingMidi.addEvent(msg, 0);
}

void PluginProcessor::processBlock(juce::AudioSampleBuffer &buf, juce::MidiBuffer &midiBuf)
{
    buf.clear();

    // Append outgoing MIDI messages queued by the editor
    {
        const juce::ScopedLock lock(midiLock);
        midiBuf.addEvents(outgoingMidi, 0, -1, 0);
        outgoingMidi.clear();
    }
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

