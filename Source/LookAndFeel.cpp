#include "LookAndFeel.h"

SoundXplorerLookAndFeel::SoundXplorerLookAndFeel()
{
    // Set base colour scheme
    setColour (juce::ResizableWindow::backgroundColourId, juce::Colour (bgDark));
    setColour (juce::TextEditor::backgroundColourId, juce::Colour (bgMedium));
    setColour (juce::TextEditor::textColourId, juce::Colour (textPrimary));
    setColour (juce::TextEditor::outlineColourId, juce::Colour (bgLight));
    setColour (juce::TextEditor::focusedOutlineColourId, juce::Colour (accentOrange));
    setColour (juce::Label::textColourId, juce::Colour (textPrimary));
    setColour (juce::ListBox::backgroundColourId, juce::Colour (bgDark));
    setColour (juce::ListBox::textColourId, juce::Colour (textPrimary));
    setColour (juce::ScrollBar::thumbColourId, juce::Colour (bgHighlight));
    setColour (juce::ScrollBar::trackColourId, juce::Colour (bgMedium));
    setColour (juce::ComboBox::backgroundColourId, juce::Colour (bgMedium));
    setColour (juce::ComboBox::textColourId, juce::Colour (textPrimary));
    setColour (juce::ComboBox::outlineColourId, juce::Colour (bgLight));
    setColour (juce::PopupMenu::backgroundColourId, juce::Colour (bgMedium));
    setColour (juce::PopupMenu::textColourId, juce::Colour (textPrimary));
    setColour (juce::PopupMenu::highlightedBackgroundColourId, juce::Colour (bgHighlight));
    setColour (juce::TextButton::buttonColourId, juce::Colour (bgMedium));
    setColour (juce::TextButton::textColourOffId, juce::Colour (textPrimary));
    setColour (juce::TableHeaderComponent::backgroundColourId, juce::Colour (bgMedium));
    setColour (juce::TableHeaderComponent::textColourId, juce::Colour (textSecondary));
    setColour (juce::TableHeaderComponent::outlineColourId, juce::Colour (bgLight));
}

void SoundXplorerLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button,
                                                     const juce::Colour& backgroundColour,
                                                     bool shouldDrawButtonAsHighlighted,
                                                     bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f);
    auto baseColour = backgroundColour;
    
    if (shouldDrawButtonAsDown)
        baseColour = baseColour.brighter (0.2f);
    else if (shouldDrawButtonAsHighlighted)
        baseColour = baseColour.brighter (0.1f);
    
    g.setColour (baseColour);
    g.fillRoundedRectangle (bounds, 4.0f);
    
    if (button.getToggleState())
    {
        g.setColour (juce::Colour (accentOrange));
        g.drawRoundedRectangle (bounds, 4.0f, 1.5f);
    }
}

void SoundXplorerLookAndFeel::drawButtonText (juce::Graphics& g, juce::TextButton& button,
                                               bool /*shouldDrawButtonAsHighlighted*/,
                                               bool /*shouldDrawButtonAsDown*/)
{
    auto colour = button.getToggleState() ? juce::Colour (accentOrange) : button.findColour (juce::TextButton::textColourOffId);
    g.setColour (colour);
    g.setFont (juce::Font (13.0f));
    g.drawFittedText (button.getButtonText(), button.getLocalBounds().reduced (4, 2),
                      juce::Justification::centred, 1);
}

void SoundXplorerLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                                 float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/,
                                                 const juce::Slider::SliderStyle /*style*/,
                                                 juce::Slider& /*slider*/)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height);
    auto trackY = bounds.getCentreY();
    auto trackHeight = 4.0f;
    
    // Track background
    g.setColour (juce::Colour (sliderTrack));
    g.fillRoundedRectangle (bounds.getX(), trackY - trackHeight / 2.0f, bounds.getWidth(), trackHeight, 2.0f);
    
    // Filled portion
    g.setColour (juce::Colour (sliderThumb));
    g.fillRoundedRectangle (bounds.getX(), trackY - trackHeight / 2.0f, sliderPos - bounds.getX(), trackHeight, 2.0f);
    
    // Thumb
    auto thumbWidth = 8.0f;
    auto thumbHeight = (float) height * 0.7f;
    g.setColour (juce::Colours::white);
    g.fillRoundedRectangle (sliderPos - thumbWidth / 2.0f, trackY - thumbHeight / 2.0f,
                            thumbWidth, thumbHeight, 3.0f);
}

void SoundXplorerLookAndFeel::drawTextEditorOutline (juce::Graphics& g, int width, int height,
                                                      juce::TextEditor& editor)
{
    auto bounds = juce::Rectangle<float> (0.0f, 0.0f, (float) width, (float) height);
    g.setColour (editor.hasKeyboardFocus (true) ? juce::Colour (accentOrange) : juce::Colour (bgLight));
    g.drawRoundedRectangle (bounds.reduced (0.5f), 6.0f, 1.0f);
}

void SoundXplorerLookAndFeel::fillTextEditorBackground (juce::Graphics& g, int width, int height,
                                                         juce::TextEditor& /*editor*/)
{
    g.setColour (juce::Colour (bgMedium));
    g.fillRoundedRectangle (0.0f, 0.0f, (float) width, (float) height, 6.0f);
}

juce::Font SoundXplorerLookAndFeel::getTextButtonFont (juce::TextButton&, int /*buttonHeight*/)
{
    return juce::Font (13.0f);
}

void SoundXplorerLookAndFeel::drawTableHeaderBackground (juce::Graphics& g,
                                                          juce::TableHeaderComponent& header)
{
    g.fillAll (juce::Colour (bgMedium));
    g.setColour (juce::Colour (bgLight));
    g.drawLine (0.0f, (float) header.getHeight() - 0.5f, (float) header.getWidth(), (float) header.getHeight() - 0.5f);
}

void SoundXplorerLookAndFeel::drawTableHeaderColumn (juce::Graphics& g,
                                                      juce::TableHeaderComponent& /*header*/,
                                                      const juce::String& columnName,
                                                      int /*columnId*/, int width, int height,
                                                      bool isMouseOver, bool /*isMouseDown*/,
                                                      int /*columnFlags*/)
{
    if (isMouseOver)
    {
        g.setColour (juce::Colour (bgLight));
        g.fillRect (0, 0, width, height);
    }
    
    g.setColour (juce::Colour (textSecondary));
    g.setFont (juce::Font (12.0f, juce::Font::bold));
    g.drawText (columnName, 8, 0, width - 16, height, juce::Justification::centredLeft);
    
    // Column separator
    g.setColour (juce::Colour (bgLight));
    g.drawLine ((float) width - 0.5f, 4.0f, (float) width - 0.5f, (float) height - 4.0f);
}
