#include "TagFilterComponent.h"
#include "LookAndFeel.h"

TagFilterComponent::TagFilterComponent()
{
    modeLabel.setText ("OR", juce::dontSendNotification);
    modeLabel.setFont (SoundXplorerLookAndFeel::getBoldFont (11.0f));
    modeLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textTertiary));
    addAndMakeVisible (modeLabel);

    viewport.setViewedComponent (&tagContainer, false);
    viewport.setScrollBarsShown (false, true);
    addAndMakeVisible (viewport);
}

void TagFilterComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (SoundXplorerLookAndFeel::bgDark));

    // Airbnb-style hairline
    g.setColour (juce::Colour (SoundXplorerLookAndFeel::foggy).withAlpha (0.15f));
    g.drawLine (0.0f, 0.5f, (float) getWidth(), 0.5f);
}

void TagFilterComponent::resized()
{
    auto bounds = getLocalBounds().reduced (8, 4);

    // Navigation arrows and OR label on the right
    auto modeArea = bounds.removeFromRight (40);
    modeLabel.setBounds (modeArea);

    viewport.setBounds (bounds);
    rebuildTagButtons();
}

void TagFilterComponent::setAvailableTags (const juce::StringArray& tags)
{
    allTags = tags;
    rebuildTagButtons();
}

juce::StringArray TagFilterComponent::getActiveTags() const
{
    return activeTags;
}

void TagFilterComponent::rebuildTagButtons()
{
    tagButtons.clear();
    tagContainer.removeAllChildren();

    int x = 0;
    int y = 0;
    int buttonHeight = 22;
    int spacing = 4;

    for (auto& tag : allTags)
    {
        auto* button = new juce::TextButton (tag);
        button->setClickingTogglesState (true);
        button->setToggleState (activeTags.contains (tag), juce::dontSendNotification);

        auto font = SoundXplorerLookAndFeel::getBoldFont (10.0f);
        juce::GlyphArrangement glyphs;
        glyphs.addLineOfText (font, tag, 0.0f, 0.0f);
        int btnWidth = (int) std::ceil (glyphs.getBoundingBox (0, -1, false).getWidth()) + 20;

        button->setColour (juce::TextButton::buttonColourId, juce::Colour (SoundXplorerLookAndFeel::bgCard));
        button->setColour (juce::TextButton::buttonOnColourId, juce::Colour (SoundXplorerLookAndFeel::rausch));

        button->setBounds (x, y, btnWidth, buttonHeight);

        button->onClick = [this, tag, button]
        {
            if (button->getToggleState())
                activeTags.addIfNotAlreadyThere (tag);
            else
                activeTags.removeString (tag);

            if (onTagFilterChanged)
                onTagFilterChanged (activeTags);
        };

        tagContainer.addAndMakeVisible (button);
        tagButtons.add (button);

        x += btnWidth + spacing;
    }

    tagContainer.setSize (x, juce::jmax (buttonHeight, getHeight() - 8));
}
