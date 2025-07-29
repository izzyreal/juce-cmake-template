#include "PluginProcessor.hpp"
#include "PluginEditor.hpp"

PluginProcessor::PluginProcessor()
        : AudioProcessor(BusesProperties()
                                 .withInput("Stereo In", juce::AudioChannelSet::stereo(), true)
                                 .withOutput("Stereo Out", juce::AudioChannelSet::stereo(), true)),
      currentFrequency(0.0f),
      phase(0.0f),
      phaseDelta(0.0f),
      sampleRate(44100.0f),
      isNoteOn(false)
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
    this->sampleRate = sampleRate;
    phase = 0.0f;
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

void PluginProcessor::processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &midiMessages)
{
    buffer.clear();
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear output buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Process MIDI messages
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        if (message.isNoteOn())
        {
            isNoteOn = true;
            currentFrequency = juce::MidiMessage::getMidiNoteInHertz(message.getNoteNumber());
            phaseDelta = (float)(currentFrequency * 2.0 * juce::MathConstants<double>::pi / sampleRate);
        }
        else if (message.isNoteOff())
        {
            isNoteOn = false;
        }
    }

    // Generate sine wave
    auto* channelData = buffer.getWritePointer(0);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float output = isNoteOn ? std::sin(phase) * 0.5f : 0.0f;

        // Write to all output channels
        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            buffer.getWritePointer(channel)[sample] = output;
        }

        // Update phase
        phase += phaseDelta;
        if (phase > 2.0f * juce::MathConstants<float>::pi)
            phase -= 2.0f * juce::MathConstants<float>::pi;
    }

    midiMessages.clear();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
