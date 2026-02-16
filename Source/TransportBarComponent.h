#pragma once
#include <JuceHeader.h>
#include "AudioPreviewEngine.h"

//==============================================================================
// Bottom transport bar with playback controls, gain, and status
//==============================================================================
class TransportBarComponent : public juce::Component,
                               public juce::Timer
{
public:
    TransportBarComponent (AudioPreviewEngine& engine);
    ~TransportBarComponent() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    
    void setCurrentFileName (const juce::String& name);
    void setStatusMessage (const juce::String& msg);
    
    // DAW sync display (VST only)
    void setDawSyncVisible (bool visible);

private:
    AudioPreviewEngine& engine;
    
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::Slider gainSlider;
    juce::Label gainLabel;
    juce::Label gainValueLabel;
    juce::Label fileNameLabel;
    juce::Label statusLabel;
    juce::Label dawSyncLabel;
    
    // Playback progress
    juce::Slider progressSlider;
    
    bool dawSyncVisible = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportBarComponent)
};
