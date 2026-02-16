#pragma once
#include <JuceHeader.h>

//==============================================================================
// Represents a single audio sample file with metadata
//==============================================================================
struct SampleItem
{
    juce::File file;
    juce::String name;
    juce::String type;       // "One-Shot", "Loop", etc.
    double bpm = 0.0;
    juce::String key;        // e.g. "F maj", "C min"
    juce::StringArray tags;
    bool isFavorite = false;
    int64_t fileSize = 0;
    double lengthSeconds = 0.0;
};

//==============================================================================
// Manages the library of audio samples
//==============================================================================
class SampleLibrary : public juce::ChangeBroadcaster
{
public:
    SampleLibrary();
    ~SampleLibrary() override;

    // Library management
    void addLibraryFolder (const juce::File& folder);
    void removeLibraryFolder (const juce::File& folder);
    void refreshLibraries();

    const juce::Array<juce::File>& getLibraryFolders() const { return libraryFolders; }

    // Sample access
    const juce::Array<SampleItem>& getAllSamples() const { return allSamples; }
    juce::Array<SampleItem> getFilteredSamples (const juce::String& searchQuery,
                                                 const juce::StringArray& activeTags,
                                                 bool favoritesOnly) const;

    // Favorites
    void toggleFavorite (const juce::File& file);
    bool isFavorite (const juce::File& file) const;

    // Tags
    juce::StringArray getAllTags() const;

    // Persistence
    void saveState();
    void loadState();

    int getTotalFileCount() const { return allSamples.size(); }
    float getAnalysisProgress() const { return analysisProgress.load(); }

private:
    void scanFolder (const juce::File& folder);
    SampleItem analyzeFile (const juce::File& file);
    juce::String detectType (const juce::File& file, double lengthSec);
    juce::String guessKeyFromFilename (const juce::String& name);
    double guessBpmFromFilename (const juce::String& name);
    juce::StringArray guessTagsFromPath (const juce::File& file);

    juce::Array<juce::File> libraryFolders;
    juce::Array<SampleItem> allSamples;
    juce::StringArray favoriteFiles;

    std::atomic<float> analysisProgress { 0.0f };

    juce::File getSettingsFile() const;

    juce::AudioFormatManager formatManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleLibrary)
};
