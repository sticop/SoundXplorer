#pragma once
#include <JuceHeader.h>
#include "SampleLibrary.h"

//==============================================================================
// Main file list table showing sample files with columns
//==============================================================================
class SampleFileListComponent : public juce::Component,
                           public juce::TableListBoxModel
{
public:
    SampleFileListComponent (SampleLibrary& library);
    ~SampleFileListComponent() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;
    
    // Update displayed content
    void updateContent (const juce::Array<SampleItem>& items);
    
    // TableListBoxModel
    int getNumRows() override;
    void paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    void cellClicked (int rowNumber, int columnId, const juce::MouseEvent& e) override;
    void cellDoubleClicked (int rowNumber, int columnId, const juce::MouseEvent& e) override;
    void sortOrderChanged (int newSortColumnId, bool isForwards) override;
    juce::Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override;
    
    // Callbacks
    std::function<void (const SampleItem&)> onSampleSelected;
    std::function<void (const SampleItem&)> onSampleDoubleClicked;
    std::function<void (const juce::File&)> onFavoriteToggled;
    
    // Column IDs
    enum ColumnIds
    {
        FavoriteColumn = 1,
        NameColumn,
        TypeColumn,
        BpmColumn,
        KeyColumn,
        TagsColumn
    };

private:
    void drawTag (juce::Graphics& g, const juce::String& tag, juce::Rectangle<int>& area, juce::Colour colour);
    void sortData();
    
    SampleLibrary& library;
    juce::TableListBox table;
    juce::Array<SampleItem> displayedItems;
    juce::Label fileCountLabel;
    
    int currentSortColumn = NameColumn;
    bool sortForward = true;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleFileListComponent)
};
