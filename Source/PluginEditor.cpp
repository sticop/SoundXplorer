#include "PluginEditor.h"

//==============================================================================
SoundXplorerEditor::SoundXplorerEditor (SoundXplorerProcessor& p)
    : AudioProcessorEditor (&p),
      processor (p),
      libraryBrowser (p.getSampleLibrary()),
      fileList (p.getSampleLibrary()),
      transportBar (p.getPreviewEngine())
{
    setLookAndFeel (&lookAndFeel);

    // Listen for library changes
    processor.getSampleLibrary().addChangeListener (this);

    // ─── Search bar ───
    searchBar.onSearchChanged = [this] (const juce::String& q) { onSearchChanged (q); };
    addAndMakeVisible (searchBar);

    // ─── Tab bar ───
    tabBar.addTab ("New Search", juce::Colour (SoundXplorerLookAndFeel::bgMedium), -1);
    tabBar.setColour (juce::TabbedButtonBar::tabOutlineColourId, juce::Colour (SoundXplorerLookAndFeel::bgLight));
    tabBar.setColour (juce::TabbedButtonBar::tabTextColourId, juce::Colour (SoundXplorerLookAndFeel::textPrimary));
    addAndMakeVisible (tabBar);

    // ─── Library browser ───
    libraryBrowser.onLibraryChanged = [this] { refreshFileList(); };
    addAndMakeVisible (libraryBrowser);

    // ─── File list ───
    fileList.onSampleSelected = [this] (const SampleItem& item) { onSampleSelected (item); };
    fileList.onSampleDoubleClicked = [this] (const SampleItem& item) { onSampleDoubleClicked (item); };
    fileList.onFavoriteToggled = [this] (const juce::File& file) { onFavoriteToggled (file); };
    addAndMakeVisible (fileList);

    // ─── Tag filter ───
    tagFilter.onTagFilterChanged = [this] (const juce::StringArray& tags) { onTagFilterChanged (tags); };
    addAndMakeVisible (tagFilter);

    // ─── Transport bar ───
#if SOUNDXPLORER_IS_VST
    transportBar.setDawSyncVisible (true);
#else
    transportBar.setDawSyncVisible (false);
#endif
    addAndMakeVisible (transportBar);

    // ─── Favorites button (heart icon drawn in paintOverChildren) ───
    favoritesButton.setButtonText ("");
    favoritesButton.setClickingTogglesState (true);
    favoritesButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    favoritesButton.setColour (juce::TextButton::textColourOffId, juce::Colour (SoundXplorerLookAndFeel::textTertiary));
    favoritesButton.onClick = [this]
    {
        showFavoritesOnly = favoritesButton.getToggleState();
        refreshFileList();
    };
    addAndMakeVisible (favoritesButton);

    // ─── Sidebar buttons (vector icons drawn in paintOverChildren) ───
    for (int i = 0; i < 9; ++i)
    {
        auto* btn = new juce::TextButton ("");
        btn->setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
        btn->setColour (juce::TextButton::textColourOffId, juce::Colour (SoundXplorerLookAndFeel::textTertiary));
        addAndMakeVisible (btn);
        sidebarButtons.add (btn);
    }

    // Initial refresh
    refreshFileList();

    // Set size
    setSize (1200, 700);
    setResizable (true, true);
    setResizeLimits (800, 500, 2400, 1400);
}

SoundXplorerEditor::~SoundXplorerEditor()
{
    processor.getSampleLibrary().removeChangeListener (this);
    setLookAndFeel (nullptr);
}

//==============================================================================
void SoundXplorerEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (SoundXplorerLookAndFeel::bgDark));
}

void SoundXplorerEditor::paintOverChildren (juce::Graphics& g)
{
    // Draw vector icons on sidebar buttons
    using LF = SoundXplorerLookAndFeel;
    auto iconColour = juce::Colour (LF::textTertiary);
    auto activeColour = juce::Colour (LF::rausch);
    
    using DrawFn = void (*)(juce::Graphics&, juce::Rectangle<float>, juce::Colour);
    DrawFn drawFns[] = {
        LF::drawHomeIcon, LF::drawUserIcon, LF::drawFolderIcon,
        LF::drawTagIcon,  LF::drawWaveformIcon, LF::drawBellIcon,
        LF::drawGlobeIcon, LF::drawHelpIcon, LF::drawGearIcon
    };
    
    for (int i = 0; i < sidebarButtons.size() && i < 9; ++i)
    {
        auto b = sidebarButtons[i]->getBounds().toFloat().reduced (7.0f);
        // First icon (Home) highlighted by default
        drawFns[i] (g, b, (i == 0) ? activeColour : iconColour);
    }
    
    // Draw heart icon on favorites button
    {
        auto b = favoritesButton.getBounds().toFloat().reduced (5.0f);
        LF::drawHeartIcon (g, b,
            showFavoritesOnly ? juce::Colour (LF::heartColor) : juce::Colour (LF::textTertiary),
            showFavoritesOnly);
    }
}

void SoundXplorerEditor::resized()
{
    auto bounds = getLocalBounds();

    // ─── Left sidebar (icon strip) ───
    auto sidebarWidth = 36;
    auto sidebarArea = bounds.removeFromLeft (sidebarWidth);

    // Distribute sidebar buttons
    auto sidebarTop = sidebarArea.reduced (2, 8);
    for (auto* btn : sidebarButtons)
    {
        btn->setBounds (sidebarTop.removeFromTop (36).reduced (2));
        sidebarTop.removeFromTop (2);
    }

    // ─── Bottom transport bar ───
    transportBar.setBounds (bounds.removeFromBottom (44));

    // ─── Top area: tab bar + search ───
    auto topArea = bounds.removeFromTop (70);
    tabBar.setBounds (topArea.removeFromTop (30));
    auto searchArea = topArea.reduced (8, 4);
    favoritesButton.setBounds (searchArea.removeFromRight (30));
    searchBar.setBounds (searchArea);

    // ─── Tag filter bar ───
    tagFilter.setBounds (bounds.removeFromBottom (32));

    // ─── Library browser (left panel) ───
    auto libraryWidth = 220;
    libraryBrowser.setBounds (bounds.removeFromLeft (libraryWidth));

    // ─── File list (main area) ───
    fileList.setBounds (bounds);
}

//==============================================================================
void SoundXplorerEditor::changeListenerCallback (juce::ChangeBroadcaster*)
{
    refreshFileList();
}

void SoundXplorerEditor::refreshFileList()
{
    auto& library = processor.getSampleLibrary();
    auto filtered = library.getFilteredSamples (currentSearchQuery, currentActiveTags, showFavoritesOnly);
    fileList.updateContent (filtered);

    // Update available tags
    tagFilter.setAvailableTags (library.getAllTags());
}

void SoundXplorerEditor::onSearchChanged (const juce::String& query)
{
    currentSearchQuery = query;
    refreshFileList();
}

void SoundXplorerEditor::onTagFilterChanged (const juce::StringArray& tags)
{
    currentActiveTags = tags;
    refreshFileList();
}

void SoundXplorerEditor::onSampleSelected (const SampleItem& item)
{
    transportBar.setCurrentFileName (item.name);
    transportBar.setStatusMessage (item.file.getFullPathName());
}

void SoundXplorerEditor::onSampleDoubleClicked (const SampleItem& item)
{
    // Double-click plays the sample
    processor.getPreviewEngine().loadAndPlay (item.file);
    transportBar.setCurrentFileName (item.name);
}

void SoundXplorerEditor::onFavoriteToggled (const juce::File& file)
{
    processor.getSampleLibrary().toggleFavorite (file);
    refreshFileList();
}
