#pragma once
#include <JuceHeader.h>
#include "SampleLibrary.h"
#include "AudioPreviewEngine.h"

//==============================================================================
// Audio processor for Sound Xplorer (works for both VST3 and Standalone)
//==============================================================================
class SoundXplorerProcessor : public juce::AudioProcessor
{
public:
    SoundXplorerProcessor();
    ~SoundXplorerProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Sound Xplorer"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Shared components
    SampleLibrary& getSampleLibrary() { return sampleLibrary; }
    AudioPreviewEngine& getPreviewEngine() { return previewEngine; }

private:
    SampleLibrary sampleLibrary;
    AudioPreviewEngine previewEngine;

    double currentSampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundXplorerProcessor)
};
