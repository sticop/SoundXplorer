#include "FileListComponent.h"
#include "LookAndFeel.h"

//==============================================================================
SampleFileListComponent::SampleFileListComponent (SampleLibrary& lib)
    : library (lib)
{
    // Setup table header
    auto& header = table.getHeader();
    
    header.addColumn ("",     FavoriteColumn, 30,  30,  30,  juce::TableHeaderComponent::notSortable);
    header.addColumn ("Name", NameColumn,     300, 100, 600, juce::TableHeaderComponent::defaultFlags);
    header.addColumn ("Type", TypeColumn,     70,  50,  100, juce::TableHeaderComponent::defaultFlags);
    header.addColumn ("BPM",  BpmColumn,      60,  40,  80,  juce::TableHeaderComponent::defaultFlags);
    header.addColumn ("Key",  KeyColumn,      70,  40,  100, juce::TableHeaderComponent::defaultFlags);
    header.addColumn ("Tags", TagsColumn,     250, 100, 500, juce::TableHeaderComponent::defaultFlags);
    
    header.setStretchToFitActive (true);
    header.setSortColumnId (NameColumn, true);
    
    table.setModel (this);
    table.setMultipleSelectionEnabled (false);
    table.setRowHeight (32);
    table.setColour (juce::ListBox::backgroundColourId, juce::Colour (SoundXplorerLookAndFeel::bgDark));
    table.setHeaderHeight (30);
    addAndMakeVisible (table);
    
    // File count label
    fileCountLabel.setFont (SoundXplorerLookAndFeel::getDefaultFont (11.0f));
    fileCountLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textTertiary));
    addAndMakeVisible (fileCountLabel);
}

void SampleFileListComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (SoundXplorerLookAndFeel::bgDark));
}

void SampleFileListComponent::resized()
{
    auto bounds = getLocalBounds();
    fileCountLabel.setBounds (bounds.removeFromTop (20).reduced (8, 0));
    table.setBounds (bounds);
}

void SampleFileListComponent::updateContent (const juce::Array<SampleItem>& items)
{
    displayedItems = items;
    sortData();
    
    // Update file count
    auto count = displayedItems.size();
    juce::String countText;
    if (count >= 1000)
        countText = juce::String (count / 1000.0, 1) + "k files shown";
    else
        countText = juce::String (count) + " files shown";
    
    fileCountLabel.setText ("(" + countText + ")", juce::dontSendNotification);
    
    table.updateContent();
    table.repaint();
}

//==============================================================================
int SampleFileListComponent::getNumRows()
{
    return displayedItems.size();
}

void SampleFileListComponent::paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll (juce::Colour (SoundXplorerLookAndFeel::bgHighlight));
    else if (rowNumber % 2 == 0)
        g.fillAll (juce::Colour (SoundXplorerLookAndFeel::bgDark));
    else
        g.fillAll (juce::Colour (SoundXplorerLookAndFeel::bgCard));
        
    // Bottom divider — Airbnb hairline
    g.setColour (juce::Colour (SoundXplorerLookAndFeel::bgLight).withAlpha (0.4f));
    g.drawLine (0.0f, (float) height - 0.5f, (float) width, (float) height - 0.5f);
}

void SampleFileListComponent::paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/)
{
    if (rowNumber >= displayedItems.size())
        return;
    
    auto& item = displayedItems.getReference (rowNumber);
    g.setFont (SoundXplorerLookAndFeel::getDefaultFont (13.0f));
    
    switch (columnId)
    {
        case FavoriteColumn:
        {
            auto heartArea = juce::Rectangle<float> ((width - 16.0f) * 0.5f, (height - 16.0f) * 0.5f, 16.0f, 16.0f);
            SoundXplorerLookAndFeel::drawHeartIcon (g, heartArea,
                item.isFavorite ? juce::Colour (SoundXplorerLookAndFeel::heartColor)
                                : juce::Colour (SoundXplorerLookAndFeel::textTertiary),
                item.isFavorite);
            break;
        }
        case NameColumn:
        {
            g.setColour (juce::Colour (SoundXplorerLookAndFeel::rausch));
            g.setFont (SoundXplorerLookAndFeel::getDefaultFont (13.0f));
            g.drawText (item.name, 10, 0, width - 20, height, juce::Justification::centredLeft, true);
            break;
        }
        case TypeColumn:
        {
            auto iconArea = juce::Rectangle<float> ((width - 16.0f) * 0.5f, (height - 16.0f) * 0.5f, 16.0f, 16.0f);
            if (item.type == "Loop")
                SoundXplorerLookAndFeel::drawLoopIcon (g, iconArea, juce::Colour (SoundXplorerLookAndFeel::textSecondary));
            else
                SoundXplorerLookAndFeel::drawArrowRightIcon (g, iconArea, juce::Colour (SoundXplorerLookAndFeel::textSecondary));
            break;
        }
        case BpmColumn:
        {
            g.setColour (juce::Colour (SoundXplorerLookAndFeel::textPrimary));
            g.setFont (SoundXplorerLookAndFeel::getBookFont (12.0f));
            if (item.bpm > 0.0)
                g.drawText (juce::String ((int) item.bpm), 0, 0, width, height, juce::Justification::centred);
            else
                g.drawText (juce::CharPointer_UTF8("\xe2\x80\x93"), 0, 0, width, height, juce::Justification::centred);
            break;
        }
        case KeyColumn:
        {
            g.setColour (juce::Colour (SoundXplorerLookAndFeel::textPrimary));
            g.setFont (SoundXplorerLookAndFeel::getBookFont (12.0f));
            if (item.key.isNotEmpty())
                g.drawText (item.key, 0, 0, width, height, juce::Justification::centred);
            else
                g.drawText (juce::CharPointer_UTF8("\xe2\x80\x93"), 0, 0, width, height, juce::Justification::centred);
            break;
        }
        case TagsColumn:
        {
            auto area = juce::Rectangle<int> (8, 2, width - 16, height - 4);
            int tagIndex = 0;
            for (auto& tag : item.tags)
            {
                auto colour = (tagIndex % 2 == 0) ? juce::Colour (SoundXplorerLookAndFeel::tagColor1) : juce::Colour (SoundXplorerLookAndFeel::tagColor2);
                drawTag (g, tag, area, colour);
                tagIndex++;
            }
            break;
        }
    }
}

void SampleFileListComponent::drawTag (juce::Graphics& g, const juce::String& tag, juce::Rectangle<int>& area, juce::Colour colour)
{
    auto font = SoundXplorerLookAndFeel::getBoldFont (10.0f);
    g.setFont (font);
    
    juce::GlyphArrangement glyphs;
    glyphs.addLineOfText (font, tag, 0.0f, 0.0f);
    int tagWidth = (int) std::ceil (glyphs.getBoundingBox (0, -1, false).getWidth()) + 14;
    
    if (area.getWidth() < tagWidth + 4)
        return;
    
    auto tagBounds = area.removeFromLeft (tagWidth).reduced (0, 3);
    area.removeFromLeft (5); // spacing
    
    // Airbnb-style pill tag: tinted fill, no border
    g.setColour (colour.withAlpha (0.15f));
    g.fillRoundedRectangle (tagBounds.toFloat(), SoundXplorerLookAndFeel::radiusSmall * 0.6f);
    
    g.setColour (colour);
    g.drawText (tag, tagBounds, juce::Justification::centred);
}

void SampleFileListComponent::cellClicked (int rowNumber, int columnId, const juce::MouseEvent&)
{
    if (rowNumber >= displayedItems.size())
        return;
    
    if (columnId == FavoriteColumn)
    {
        auto& item = displayedItems.getReference (rowNumber);
        if (onFavoriteToggled)
            onFavoriteToggled (item.file);
    }
    else
    {
        if (onSampleSelected)
            onSampleSelected (displayedItems[rowNumber]);
    }
}

void SampleFileListComponent::cellDoubleClicked (int rowNumber, int /*columnId*/, const juce::MouseEvent&)
{
    if (rowNumber < displayedItems.size() && onSampleDoubleClicked)
        onSampleDoubleClicked (displayedItems[rowNumber]);
}

void SampleFileListComponent::sortOrderChanged (int newSortColumnId, bool isForwards)
{
    currentSortColumn = newSortColumnId;
    sortForward = isForwards;
    sortData();
    table.updateContent();
    table.repaint();
}

void SampleFileListComponent::sortData()
{
    auto col = currentSortColumn;
    auto forward = sortForward;
    
    std::sort (displayedItems.begin(), displayedItems.end(),
               [col, forward] (const SampleItem& a, const SampleItem& b)
    {
        int result = 0;
        switch (col)
        {
            case NameColumn: result = a.name.compareIgnoreCase (b.name); break;
            case TypeColumn: result = a.type.compareIgnoreCase (b.type); break;
            case BpmColumn:  result = (a.bpm < b.bpm) ? -1 : (a.bpm > b.bpm) ? 1 : 0; break;
            case KeyColumn:  result = a.key.compareIgnoreCase (b.key); break;
            default: result = a.name.compareIgnoreCase (b.name); break;
        }
        return forward ? result < 0 : result > 0;
    });
}

juce::Component* SampleFileListComponent::refreshComponentForCell (int /*rowNumber*/, int /*columnId*/, bool /*isRowSelected*/, juce::Component* existingComponentToUpdate)
{
    // We handle everything in paintCell, no custom components needed
    jassert (existingComponentToUpdate == nullptr);
    return nullptr;
}
