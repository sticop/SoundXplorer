#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoundXplorerProcessor::SoundXplorerProcessor()
    : AudioProcessor (BusesProperties()
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
}

SoundXplorerProcessor::~SoundXplorerProcessor()
{
}

//==============================================================================
void SoundXplorerProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    previewEngine.getTransportSource().prepareToPlay (samplesPerBlock, sampleRate);
}

void SoundXplorerProcessor::releaseResources()
{
    previewEngine.getTransportSource().releaseResources();
}

void SoundXplorerProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't have input
    for (auto i = getTotalNumInputChannels(); i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

#if SOUNDXPLORER_IS_VST
    // Get DAW transport info
    if (auto* playHead = getPlayHead())
    {
        auto position = playHead->getPosition();
        if (position.hasValue())
        {
            if (auto bpm = position->getBpm())
                previewEngine.setDawBpm (*bpm);

            previewEngine.setDawPlaying (position->getIsPlaying());

            if (auto timeInSamples = position->getTimeInSamples())
                previewEngine.setDawPositionSamples (*timeInSamples);
        }
    }
#endif

    // Get audio from preview engine
    juce::AudioSourceChannelInfo info (&buffer, 0, buffer.getNumSamples());
    previewEngine.getTransportSource().getNextAudioBlock (info);
}

//==============================================================================
juce::AudioProcessorEditor* SoundXplorerProcessor::createEditor()
{
    return new SoundXplorerEditor (*this);
}

//==============================================================================
void SoundXplorerProcessor::getStateInformation (juce::MemoryBlock& /*destData*/)
{
    // Save state if needed
}

void SoundXplorerProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
    // Restore state if needed
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoundXplorerProcessor();
}
