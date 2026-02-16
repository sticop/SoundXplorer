#include "SearchBarComponent.h"
#include "LookAndFeel.h"

SearchBarComponent::SearchBarComponent()
{
    searchIcon.setText (juce::CharPointer_UTF8("\xf0\x9f\x94\x8d"), juce::dontSendNotification); // magnifying glass emoji
    searchIcon.setFont (juce::Font (16.0f));
    searchIcon.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textSecondary));
    addAndMakeVisible (searchIcon);
    
    searchEditor.setTextToShowWhenEmpty ("What sounds are you looking for?",
                                         juce::Colour (SoundXplorerLookAndFeel::textSecondary));
    searchEditor.setFont (juce::Font (15.0f));
    searchEditor.setColour (juce::TextEditor::backgroundColourId, juce::Colour (SoundXplorerLookAndFeel::bgMedium));
    searchEditor.setColour (juce::TextEditor::textColourId, juce::Colour (SoundXplorerLookAndFeel::textPrimary));
    searchEditor.setColour (juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
    searchEditor.addListener (this);
    addAndMakeVisible (searchEditor);
}

void SearchBarComponent::paint (juce::Graphics& g)
{
    g.setColour (juce::Colour (SoundXplorerLookAndFeel::bgMedium));
    g.fillRoundedRectangle (getLocalBounds().toFloat(), 6.0f);
    
    g.setColour (juce::Colour (SoundXplorerLookAndFeel::bgLight));
    g.drawRoundedRectangle (getLocalBounds().toFloat().reduced (0.5f), 6.0f, 1.0f);
}

void SearchBarComponent::resized()
{
    auto bounds = getLocalBounds().reduced (4);
    searchIcon.setBounds (bounds.removeFromLeft (30));
    searchEditor.setBounds (bounds);
}

void SearchBarComponent::textEditorTextChanged (juce::TextEditor&)
{
    if (onSearchChanged)
        onSearchChanged (searchEditor.getText());
}
