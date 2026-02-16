#include "TransportBarComponent.h"
#include "LookAndFeel.h"

TransportBarComponent::TransportBarComponent (AudioPreviewEngine& eng)
    : engine (eng)
{
    // Play button
    playButton.setButtonText (juce::CharPointer_UTF8("\xe2\x96\xb6")); // play triangle
    playButton.setColour (juce::TextButton::buttonColourId, juce::Colour (SoundXplorerLookAndFeel::bgLight));
    playButton.onClick = [this] { engine.togglePlayPause(); };
    addAndMakeVisible (playButton);
    
    // Stop button
    stopButton.setButtonText (juce::CharPointer_UTF8("\xe2\x96\xa0")); // stop square
    stopButton.setColour (juce::TextButton::buttonColourId, juce::Colour (SoundXplorerLookAndFeel::bgLight));
    stopButton.onClick = [this] { engine.stop(); };
    addAndMakeVisible (stopButton);
    
    // Progress slider
    progressSlider.setRange (0.0, 1.0, 0.001);
    progressSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    progressSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    progressSlider.setColour (juce::Slider::backgroundColourId, juce::Colour (SoundXplorerLookAndFeel::bgLight));
    progressSlider.setColour (juce::Slider::trackColourId, juce::Colour (SoundXplorerLookAndFeel::accentBlue));
    progressSlider.onValueChange = [this]
    {
        if (progressSlider.isMouseButtonDown())
        {
            auto& ts = engine.getTransportSource();
            auto len = ts.getLengthInSeconds();
            if (len > 0.0)
                ts.setPosition (progressSlider.getValue() * len);
        }
    };
    addAndMakeVisible (progressSlider);
    
    // Gain label
    gainLabel.setText ("Gain", juce::dontSendNotification);
    gainLabel.setFont (juce::Font (12.0f));
    gainLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textSecondary));
    addAndMakeVisible (gainLabel);
    
    // Gain slider  
    gainSlider.setRange (0.0, 2.0, 0.01);
    gainSlider.setValue (1.0);
    gainSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    gainSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    gainSlider.onValueChange = [this] { engine.setGain ((float) gainSlider.getValue()); };
    addAndMakeVisible (gainSlider);
    
    // Gain value label
    gainValueLabel.setText ("0.0dB", juce::dontSendNotification);
    gainValueLabel.setFont (juce::Font (11.0f));
    gainValueLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textPrimary));
    addAndMakeVisible (gainValueLabel);
    
    // File name label
    fileNameLabel.setFont (juce::Font (12.0f));
    fileNameLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textPrimary));
    addAndMakeVisible (fileNameLabel);
    
    // Status / message label
    statusLabel.setFont (juce::Font (11.0f));
    statusLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textSecondary));
    addAndMakeVisible (statusLabel);
    
    // DAW sync label (VST only)
    dawSyncLabel.setText ("DAW Sync", juce::dontSendNotification);
    dawSyncLabel.setFont (juce::Font (11.0f, juce::Font::bold));
    dawSyncLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::accentBlue));
    dawSyncLabel.setVisible (dawSyncVisible);
    addAndMakeVisible (dawSyncLabel);
    
    startTimerHz (30);
}

void TransportBarComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (SoundXplorerLookAndFeel::bgMedium));
    
    // Top border
    g.setColour (juce::Colour (SoundXplorerLookAndFeel::bgLight));
    g.drawLine (0.0f, 0.0f, (float) getWidth(), 0.0f);
}

void TransportBarComponent::resized()
{
    auto bounds = getLocalBounds().reduced (8, 4);
    
    // Left: transport controls
    playButton.setBounds (bounds.removeFromLeft (32).reduced (0, 4));
    bounds.removeFromLeft (4);
    stopButton.setBounds (bounds.removeFromLeft (32).reduced (0, 4));
    bounds.removeFromLeft (8);
    
    // Progress bar
    progressSlider.setBounds (bounds.removeFromLeft (150).reduced (0, 6));
    bounds.removeFromLeft (12);
    
    // File name
    fileNameLabel.setBounds (bounds.removeFromLeft (200));
    bounds.removeFromLeft (8);
    
    // Right side: gain controls
    auto rightArea = bounds;
    
    gainValueLabel.setBounds (rightArea.removeFromRight (50));
    gainSlider.setBounds (rightArea.removeFromRight (120).reduced (0, 6));
    gainLabel.setBounds (rightArea.removeFromRight (40));
    
    // DAW sync label
    if (dawSyncVisible)
        dawSyncLabel.setBounds (rightArea.removeFromRight (80));
    
    statusLabel.setBounds (rightArea);
}

void TransportBarComponent::timerCallback()
{
    // Update progress slider
    if (! progressSlider.isMouseButtonDown())
        progressSlider.setValue (engine.getPlaybackPosition(), juce::dontSendNotification);
    
    // Update gain display
    float gainDb = juce::Decibels::gainToDecibels (engine.getGain(), -60.0f);
    gainValueLabel.setText (juce::String (gainDb, 1) + "dB", juce::dontSendNotification);
    
    // Update play button state
    playButton.setButtonText (engine.isPlaying() ? juce::CharPointer_UTF8("\xe2\x8f\xb8") : juce::CharPointer_UTF8("\xe2\x96\xb6"));
    
    // Update DAW sync info
    if (dawSyncVisible)
    {
        juce::String syncText = "DAW: " + juce::String (engine.getDawBpm(), 1) + " BPM";
        if (engine.isDawPlaying())
            syncText += " [Playing]";
        dawSyncLabel.setText (syncText, juce::dontSendNotification);
    }
}

void TransportBarComponent::setCurrentFileName (const juce::String& name)
{
    fileNameLabel.setText (name, juce::dontSendNotification);
}

void TransportBarComponent::setStatusMessage (const juce::String& msg)
{
    statusLabel.setText (msg, juce::dontSendNotification);
}

void TransportBarComponent::setDawSyncVisible (bool visible)
{
    dawSyncVisible = visible;
    dawSyncLabel.setVisible (visible);
    resized();
}
