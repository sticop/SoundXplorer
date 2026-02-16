#pragma once
#include <JuceHeader.h>

//==============================================================================
// Audio engine for previewing sound files
//==============================================================================
class AudioPreviewEngine : public juce::ChangeListener,
                           public juce::Timer
{
public:
    AudioPreviewEngine();
    ~AudioPreviewEngine() override;

    // Playback control  
    void loadAndPlay (const juce::File& file);
    void stop();
    void pause();
    void togglePlayPause();
    
    bool isPlaying() const;
    double getPlaybackPosition() const; // 0.0 to 1.0
    double getPlaybackLengthSeconds() const;
    
    // Gain
    void setGain (float gainLinear);
    float getGain() const { return currentGain.load(); }
    
    // DAW Sync (VST only)
    void setDawBpm (double bpm);
    void setDawPlaying (bool playing);
    void setDawPositionSamples (int64_t position);
    double getDawBpm() const { return dawBpm.load(); }
    bool isDawPlaying() const { return dawPlaying.load(); }
    
    // Transport source access
    juce::AudioTransportSource& getTransportSource() { return transportSource; }
    juce::AudioFormatManager& getFormatManager() { return formatManager; }

    // Current file info
    juce::File getCurrentFile() const { return currentFile; }
    
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void timerCallback() override;

    std::function<void()> onPlaybackStopped;
    std::function<void()> onPlaybackStarted;
    std::function<void (double)> onPositionChanged;

private:
    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    
    juce::File currentFile;
    std::atomic<float> currentGain { 1.0f };
    
    // DAW sync state
    std::atomic<double> dawBpm { 120.0 };
    std::atomic<bool> dawPlaying { false };
    std::atomic<int64_t> dawPositionSamples { 0 };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPreviewEngine)
};
