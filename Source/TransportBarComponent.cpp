#include "TransportBarComponent.h"
#include "LookAndFeel.h"

TransportBarComponent::TransportBarComponent (AudioPreviewEngine& eng)
    : engine (eng)
{
    // Play button — vector icon drawn in paintOverChildren
    playButton.setButtonText ("");
    playButton.setColour (juce::TextButton::buttonColourId, juce::Colour (SoundXplorerLookAndFeel::bgCard));
    playButton.onClick = [this] { engine.togglePlayPause(); };
    addAndMakeVisible (playButton);

    // Stop button — vector icon drawn in paintOverChildren
    stopButton.setButtonText ("");
    stopButton.setColour (juce::TextButton::buttonColourId, juce::Colour (SoundXplorerLookAndFeel::bgCard));
    stopButton.onClick = [this] { engine.stop(); };
    addAndMakeVisible (stopButton);

    // Progress slider
    progressSlider.setRange (0.0, 1.0, 0.001);
    progressSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    progressSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    progressSlider.setColour (juce::Slider::backgroundColourId, juce::Colour (SoundXplorerLookAndFeel::bgLight));
    progressSlider.setColour (juce::Slider::trackColourId, juce::Colour (SoundXplorerLookAndFeel::rausch));
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
    gainLabel.setFont (SoundXplorerLookAndFeel::getBookFont (12.0f));
    gainLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textTertiary));
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
    gainValueLabel.setFont (SoundXplorerLookAndFeel::getDefaultFont (11.0f));
    gainValueLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textPrimary));
    addAndMakeVisible (gainValueLabel);

    // File name label
    fileNameLabel.setFont (SoundXplorerLookAndFeel::getBoldFont (12.0f));
    fileNameLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textPrimary));
    addAndMakeVisible (fileNameLabel);

    // Status / message label
    statusLabel.setFont (SoundXplorerLookAndFeel::getBookFont (11.0f));
    statusLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textTertiary));
    addAndMakeVisible (statusLabel);

    // DAW sync label (VST only)
    dawSyncLabel.setText ("DAW Sync", juce::dontSendNotification);
    dawSyncLabel.setFont (SoundXplorerLookAndFeel::getBoldFont (11.0f));
    dawSyncLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::babu));
    dawSyncLabel.setVisible (dawSyncVisible);
    addAndMakeVisible (dawSyncLabel);

    startTimerHz (30);
}

void TransportBarComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (SoundXplorerLookAndFeel::bgMedium));

    // Airbnb-style top hairline
    g.setColour (juce::Colour (SoundXplorerLookAndFeel::foggy).withAlpha (0.15f));
    g.drawLine (0.0f, 0.5f, (float) getWidth(), 0.5f);
}

void TransportBarComponent::paintOverChildren (juce::Graphics& g)
{
    // Draw vector icons on top of the play/stop buttons
    {
        auto b = playButton.getBounds().toFloat().reduced (7.0f);
        if (engine.isPlaying())
            SoundXplorerLookAndFeel::drawPauseIcon (g, b, juce::Colour (SoundXplorerLookAndFeel::rausch));
        else
            SoundXplorerLookAndFeel::drawPlayIcon (g, b, juce::Colour (SoundXplorerLookAndFeel::rausch));
    }
    {
        auto b = stopButton.getBounds().toFloat().reduced (7.0f);
        SoundXplorerLookAndFeel::drawStopIcon (g, b, juce::Colour (SoundXplorerLookAndFeel::textSecondary));
    }
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

    // Repaint to update play/pause vector icon
    playButton.repaint();
    repaint (playButton.getBounds());

    // Update DAW sync info
    if (dawSyncVisible)
    {
        juce::String syncText = "DAW: " + juce::String (engine.getDawBpm(), 1) + " BPM";
        if (engine.isDawPlaying())
            syncText += juce::String (juce::CharPointer_UTF8(" \xe2\x97\x8f Playing")); // bullet
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
