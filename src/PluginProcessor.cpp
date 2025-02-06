#include "PluginProcessor.hpp"
#include "PluginEditor.hpp"

PluginProcessor::PluginProcessor()
        : AudioProcessor(getBusesProperties())
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

juce::AudioProcessor::BusesProperties PluginProcessor::getBusesProperties()
{
    // Hosttypes don't seem to work at this point, but we may need to verify that.
    // First, let's make all decisions based on wrapper type, and then we can
    // further refine if necessary.
    typedef juce::AudioProcessor::WrapperType W;
    typedef juce::AudioChannelSet C;

    const auto wrapper = juce::PluginHostType::jucePlugInClientCurrentWrapperType;
    const bool isStandalone = juce::JUCEApplication::isStandaloneApp();
    const bool isAUv2 = wrapper == W::wrapperType_AudioUnit;
    const bool isAUv3 = wrapper == W::wrapperType_AudioUnitv3;

    int monoInCount;
    int stereoInCount;
    int monoOutCount;
    int stereoOutCount;

    if (isAUv2 || isAUv3)
    {
        monoInCount = 0;
        stereoInCount = 0;
        monoOutCount = 8;
        stereoOutCount = 1;
    }
    else if (isStandalone)
    {
        monoInCount = 0;
        stereoInCount = 1;
        monoOutCount = 0;
        stereoOutCount = 5;
    }
    else /* if LV2 or VST3 */
    {
        monoInCount = 2;
        stereoInCount = 1;
        monoOutCount = 8;
        stereoOutCount = 5;
    }

    juce::AudioProcessor::BusesProperties result;

    for (int i = 0; i < stereoInCount; i++)
        result = result.withInput("RECORD IN L/R", C::stereo(), true);

    for (int i = 0; i < stereoOutCount; i++)
    {
        const auto name = i == 0 ? "STEREO OUT L/R" : ("MIX OUT " + std::to_string((i * 2) - 1) + "/" + std::to_string(i*2));
        const bool enabledByDefault = i == 0 || isStandalone || isAUv2 || isAUv3;
        result = result.withOutput(name, C::stereo(), enabledByDefault);
    }

    for (int i = 0; i < monoInCount; i++)
        result = result.withInput("RECORD IN " + std::string((i%2 == 0) ? "L" : "R"), C::mono(), true);

    for (int i = 0; i < monoOutCount; i++)
    {
        const auto name = "MIX OUT " + std::to_string(i + 1);
        const bool enabledByDefault = isStandalone || isAUv2 || isAUv3;
        result = result.withOutput("", C::mono(), false);
    }

    return result;
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
    const std::function<int(const bool isInput)> getTotalChannelCountForBus = [&] (const bool isInput) {
        int result = 0;
        for (int i = 0; i < layouts.getBuses(isInput).size(); i++) result += layouts.getNumChannels(isInput, i);
        return result;
    };

    const std::function<std::vector<int>(const bool isInput)> getChannelCounts = [&] (const bool isInput) {
        std::vector<int> result;
        for (int i = 0; i < layouts.getBuses(isInput).size(); i++) result.push_back(layouts.getNumChannels(isInput, i));
        return result;
    };

    const std::function<int(const bool isInput, const int numChannels)> getBusCountForNumChannels = [&] (const bool isInput, const int numChannels) {
        int result = 0;
        for (int i = 0; i < layouts.getBuses(isInput).size(); i++) if (layouts.getNumChannels(isInput, i) == numChannels) result++;
        return result;
    };

    typedef juce::AudioProcessor::WrapperType W;

    const auto wrapper = juce::PluginHostType::jucePlugInClientCurrentWrapperType;
    const bool isStandalone = juce::JUCEApplication::isStandaloneApp();
    const bool isAUv2 = wrapper == W::wrapperType_AudioUnit;
    const bool isAUv3 = wrapper == W::wrapperType_AudioUnitv3;
    const bool isVST3 = wrapper == W::wrapperType_VST3;
    const bool isLV2  = wrapper == W::wrapperType_LV2;

    const int monoInputCount = getBusCountForNumChannels(true, 1);
    const int monoOutputCount = getBusCountForNumChannels(false, 1);
    const int stereoInputCount = getBusCountForNumChannels(true, 2);
    const int stereoOutputCount = getBusCountForNumChannels(false, 2);
    const int totalNumInputChannels = getTotalChannelCountForBus(true);
    const int totalNumOutputChannels = getTotalChannelCountForBus(false);

    bool result = false;

    if (isAUv2 || isAUv3)
    {
//        result = result || (stereoOutputCount == 1 && monoOutputCount == 0 && stereoInputCount == 1 && monoInputCount == 0);
        result = result || (stereoOutputCount == 1 && monoOutputCount == 8 && stereoInputCount == 0 && monoInputCount == 0);
    }
    else if (isStandalone)
    {
        result = result || (totalNumInputChannels <= 2 && totalNumOutputChannels <= 10);
    }
    else if (isVST3 || isLV2)
    {
        result = result || (totalNumInputChannels <= 4 && totalNumOutputChannels <= 18);
    }

    return result;
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
