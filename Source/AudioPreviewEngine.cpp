#include "AudioPreviewEngine.h"

AudioPreviewEngine::AudioPreviewEngine()
{
    formatManager.registerBasicFormats();
    transportSource.addChangeListener (this);
    startTimerHz (30);
}

AudioPreviewEngine::~AudioPreviewEngine()
{
    stopTimer();
    transportSource.removeChangeListener (this);
    transportSource.setSource (nullptr);
    readerSource.reset();
}

void AudioPreviewEngine::loadAndPlay (const juce::File& file)
{
    // Stop current playback
    transportSource.stop();
    transportSource.setSource (nullptr);
    readerSource.reset();
    
    auto* reader = formatManager.createReaderFor (file);
    if (reader != nullptr)
    {
        currentFile = file;
        readerSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
        transportSource.setSource (readerSource.get(), 0, nullptr, reader->sampleRate);
        transportSource.setGain (currentGain.load());
        transportSource.start();
        
        if (onPlaybackStarted)
            onPlaybackStarted();
    }
}

void AudioPreviewEngine::stop()
{
    transportSource.stop();
    transportSource.setPosition (0.0);
}

void AudioPreviewEngine::pause()
{
    if (transportSource.isPlaying())
        transportSource.stop();
}

void AudioPreviewEngine::togglePlayPause()
{
    if (transportSource.isPlaying())
        transportSource.stop();
    else
        transportSource.start();
}

bool AudioPreviewEngine::isPlaying() const
{
    return transportSource.isPlaying();
}

double AudioPreviewEngine::getPlaybackPosition() const
{
    auto length = transportSource.getLengthInSeconds();
    if (length <= 0.0)
        return 0.0;
    return transportSource.getCurrentPosition() / length;
}

double AudioPreviewEngine::getPlaybackLengthSeconds() const
{
    return transportSource.getLengthInSeconds();
}

void AudioPreviewEngine::setGain (float gainLinear)
{
    currentGain = juce::jlimit (0.0f, 2.0f, gainLinear);
    transportSource.setGain (currentGain.load());
}

void AudioPreviewEngine::setDawBpm (double bpm)
{
    dawBpm = bpm;
}

void AudioPreviewEngine::setDawPlaying (bool playing)
{
    dawPlaying = playing;
}

void AudioPreviewEngine::setDawPositionSamples (int64_t position)
{
    dawPositionSamples = position;
}

void AudioPreviewEngine::changeListenerCallback (juce::ChangeBroadcaster*)
{
    if (! transportSource.isPlaying() && transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds() - 0.01)
    {
        // Playback finished
        if (onPlaybackStopped)
            onPlaybackStopped();
    }
}

void AudioPreviewEngine::timerCallback()
{
    if (onPositionChanged && transportSource.isPlaying())
        onPositionChanged (getPlaybackPosition());
}
