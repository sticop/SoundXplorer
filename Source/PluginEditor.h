#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeel.h"
#include "SearchBarComponent.h"
#include "LibraryBrowserComponent.h"
#include "FileListComponent.h"
#include "TransportBarComponent.h"
#include "TagFilterComponent.h"

//==============================================================================
// Main editor — assembles all UI components
//==============================================================================
class SoundXplorerEditor : public juce::AudioProcessorEditor,
                            public juce::ChangeListener
{
public:
    explicit SoundXplorerEditor (SoundXplorerProcessor& processor);
    ~SoundXplorerEditor() override;

    void paint (juce::Graphics& g) override;
    void paintOverChildren (juce::Graphics& g) override;
    void resized() override;
    
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;

private:
    void refreshFileList();
    void onSearchChanged (const juce::String& query);
    void onTagFilterChanged (const juce::StringArray& tags);
    void onSampleSelected (const SampleItem& item);
    void onSampleDoubleClicked (const SampleItem& item);
    void onFavoriteToggled (const juce::File& file);
    
    SoundXplorerProcessor& processor;
    SoundXplorerLookAndFeel lookAndFeel;
    
    // UI Components
    SearchBarComponent searchBar;
    LibraryBrowserComponent libraryBrowser;
    SampleFileListComponent fileList;
    TransportBarComponent transportBar;
    TagFilterComponent tagFilter;
    
    // Tab bar
    juce::TabbedButtonBar tabBar { juce::TabbedButtonBar::TabsAtTop };
    
    // Sidebar icons (as toggle buttons)
    juce::OwnedArray<juce::TextButton> sidebarButtons;
    
    // Favorites filter button
    juce::TextButton favoritesButton;
    bool showFavoritesOnly = false;
    
    // Current state
    juce::String currentSearchQuery;
    juce::StringArray currentActiveTags;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundXplorerEditor)
};
