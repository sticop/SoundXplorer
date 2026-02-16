#include "SearchBarComponent.h"
#include "LookAndFeel.h"

SearchBarComponent::SearchBarComponent()
{
    // No emoji — icon drawn in paint()
    searchIcon.setText ("", juce::dontSendNotification);
    addAndMakeVisible (searchIcon);

    searchEditor.setTextToShowWhenEmpty ("What sounds are you looking for?",
                                         juce::Colour (SoundXplorerLookAndFeel::textTertiary));
    searchEditor.setFont (SoundXplorerLookAndFeel::getDefaultFont (15.0f));
    searchEditor.setColour (juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
    searchEditor.setColour (juce::TextEditor::textColourId, juce::Colour (SoundXplorerLookAndFeel::textPrimary));
    searchEditor.setColour (juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
    searchEditor.setColour (juce::TextEditor::focusedOutlineColourId, juce::Colours::transparentBlack);
    searchEditor.addListener (this);
    addAndMakeVisible (searchEditor);
}

void SearchBarComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Airbnb-style search pill: dark surface + subtle border, generous rounding
    g.setColour (juce::Colour (SoundXplorerLookAndFeel::bgCard));
    g.fillRoundedRectangle (bounds, SoundXplorerLookAndFeel::radiusMedium);

    g.setColour (juce::Colour (SoundXplorerLookAndFeel::bgLight));
    g.drawRoundedRectangle (bounds.reduced (0.5f), SoundXplorerLookAndFeel::radiusMedium, 1.0f);

    // Draw search icon (vector)
    auto iconArea = juce::Rectangle<float> (8.0f, (bounds.getHeight() - 20.0f) * 0.5f, 20.0f, 20.0f);
    SoundXplorerLookAndFeel::drawSearchIcon (g, iconArea, juce::Colour (SoundXplorerLookAndFeel::textTertiary));
}

void SearchBarComponent::resized()
{
    auto bounds = getLocalBounds().reduced (4);
    bounds.removeFromLeft (28); // space for icon
    searchEditor.setBounds (bounds);
}

void SearchBarComponent::textEditorTextChanged (juce::TextEditor&)
{
    if (onSearchChanged)
        onSearchChanged (searchEditor.getText());
}
