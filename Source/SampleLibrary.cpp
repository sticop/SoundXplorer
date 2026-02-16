#include "SampleLibrary.h"

//==============================================================================
SampleLibrary::SampleLibrary()
{
    formatManager.registerBasicFormats();
    loadState();
}

SampleLibrary::~SampleLibrary()
{
    saveState();
}

//==============================================================================
void SampleLibrary::addLibraryFolder (const juce::File& folder)
{
    if (folder.isDirectory() && ! libraryFolders.contains (folder))
    {
        libraryFolders.add (folder);
        scanFolder (folder);
        saveState();
        sendChangeMessage();
    }
}

void SampleLibrary::removeLibraryFolder (const juce::File& folder)
{
    libraryFolders.removeAllInstancesOf (folder);
    // Remove samples from that folder
    for (int i = allSamples.size(); --i >= 0;)
        if (allSamples[i].file.getFullPathName().startsWith (folder.getFullPathName()))
            allSamples.remove (i);
    saveState();
    sendChangeMessage();
}

void SampleLibrary::refreshLibraries()
{
    allSamples.clear();
    analysisProgress = 0.0f;
    
    for (auto& folder : libraryFolders)
        scanFolder (folder);
    
    sendChangeMessage();
}

//==============================================================================
void SampleLibrary::scanFolder (const juce::File& folder)
{
    auto audioExtensions = juce::StringArray { "wav", "aif", "aiff", "mp3", "flac", "ogg", "m4a" };
    
    juce::Array<juce::File> files;
    for (auto& ext : audioExtensions)
    {
        auto found = folder.findChildFiles (juce::File::findFiles, true, "*." + ext);
        files.addArray (found);
    }
    
    int total = files.size();
    int processed = 0;
    
    for (auto& f : files)
    {
        auto item = analyzeFile (f);
        item.isFavorite = favoriteFiles.contains (f.getFullPathName());
        allSamples.add (item);
        
        processed++;
        analysisProgress = (float) processed / (float) juce::jmax (1, total);
    }
}

SampleItem SampleLibrary::analyzeFile (const juce::File& file)
{
    SampleItem item;
    item.file = file;
    item.name = file.getFileNameWithoutExtension();
    item.fileSize = file.getSize();
    
    // Try to read audio properties
    std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor (file));
    if (reader != nullptr)
    {
        item.lengthSeconds = (double) reader->lengthInSamples / reader->sampleRate;
    }
    
    item.type = detectType (file, item.lengthSeconds);
    item.bpm = guessBpmFromFilename (item.name);
    item.key = guessKeyFromFilename (item.name);
    item.tags = guessTagsFromPath (file);
    
    return item;
}

juce::String SampleLibrary::detectType (const juce::File& file, double lengthSec)
{
    auto name = file.getFileNameWithoutExtension().toLowerCase();
    
    if (name.contains ("loop") || lengthSec > 2.0)
        return "Loop";
    
    return "One-Shot";
}

juce::String SampleLibrary::guessKeyFromFilename (const juce::String& name)
{
    // Common key patterns: "C maj", "Am", "F#m", etc.
    juce::StringArray notes = { "C", "C#", "Db", "D", "D#", "Eb", "E", "F", "F#", "Gb", "G", "G#", "Ab", "A", "A#", "Bb", "B" };
    juce::StringArray qualities = { "maj", "min", "major", "minor", "m" };
    
    auto lower = name.toLowerCase();
    
    for (auto& note : notes)
    {
        for (auto& qual : qualities)
        {
            auto pattern = note.toLowerCase() + " " + qual.toLowerCase();
            if (lower.contains (pattern))
            {
                juce::String result = note;
                if (qual == "m" || qual == "min" || qual == "minor")
                    result += " min";
                else
                    result += " maj";
                return result;
            }
        }
    }
    
    return {};
}

double SampleLibrary::guessBpmFromFilename (const juce::String& name)
{
    // Look for patterns like "120bpm", "120 bpm", "bpm120"
    auto lower = name.toLowerCase();
    
    // Pattern: digits followed by "bpm"
    for (int i = 0; i < lower.length() - 2; ++i)
    {
        if (juce::CharacterFunctions::isDigit (lower[i]))
        {
            int start = i;
            while (i < lower.length() && juce::CharacterFunctions::isDigit (lower[i]))
                ++i;
            
            auto numStr = lower.substring (start, i);
            
            // Check if followed by "bpm"
            auto remaining = lower.substring (i).trimStart();
            if (remaining.startsWith ("bpm"))
            {
                double bpm = numStr.getDoubleValue();
                if (bpm >= 40.0 && bpm <= 300.0)
                    return bpm;
            }
        }
    }
    
    // Pattern: "bpm" followed by digits
    int bpmIdx = lower.indexOf ("bpm");
    if (bpmIdx >= 0)
    {
        auto after = lower.substring (bpmIdx + 3).trimStart();
        auto numStr = after.initialSectionContainingOnly ("0123456789.");
        if (numStr.isNotEmpty())
        {
            double bpm = numStr.getDoubleValue();
            if (bpm >= 40.0 && bpm <= 300.0)
                return bpm;
        }
    }
    
    return 0.0;
}

juce::StringArray SampleLibrary::guessTagsFromPath (const juce::File& file)
{
    juce::StringArray tags;
    auto path = file.getFullPathName().toLowerCase();
    auto name = file.getFileNameWithoutExtension().toLowerCase();
    
    // Category tags from path/filename
    struct TagMapping { const char* keyword; const char* tag; };
    TagMapping mappings[] = {
        { "kick",      "DRUMS" },
        { "snare",     "DRUMS" },
        { "hihat",     "DRUMS" },
        { "hi-hat",    "DRUMS" },
        { "hat",       "DRUMS" },
        { "clap",      "DRUMS" },
        { "tom",       "DRUMS" },
        { "cymbal",    "DRUMS" },
        { "percussion","DRUMS" },
        { "drum",      "DRUMS" },
        { "bass",      "BASS" },
        { "808",       "BASS" },
        { "sub",       "BASS" },
        { "synth",     "SYNTHS" },
        { "lead",      "SYNTHS" },
        { "pad",       "SYNTHS" },
        { "keys",      "KEYS" },
        { "piano",     "KEYS" },
        { "organ",     "KEYS" },
        { "guitar",    "GUITAR" },
        { "vocal",     "VOCALS" },
        { "vox",       "VOCALS" },
        { "voice",     "VOCALS" },
        { "fx",        "FX" },
        { "effect",    "FX" },
        { "riser",     "FX" },
        { "sweep",     "FX" },
        { "impact",    "FX" },
        { "foley",     "FOLEY" },
        { "ambient",   "AMBIENT" },
        { "atmosphere","AMBIENT" },
        { "texture",   "TEXTURE" },
        { "loop",      "LOOPS" },
        { "one shot",  "ONE-SHOTS" },
        { "oneshot",   "ONE-SHOTS" },
        { "one-shot",  "ONE-SHOTS" },
        { "phone",     "TELEPHONES" },
        { "telephone", "TELEPHONES" },
        { "radio",     "RADIOS & STATIC" },
        { "static",    "RADIOS & STATIC" },
        { "sound effect", "SOUND EFFECTS" },
        { "sfx",       "SOUND EFFECTS" },
        { "string",    "STRINGS" },
        { "brass",     "BRASS" },
        { "horn",      "BRASS" },
        { "flute",     "WOODWINDS" },
        { "wind",      "WOODWINDS" },
    };
    
    for (auto& m : mappings)
    {
        if (path.contains (m.keyword) || name.contains (m.keyword))
        {
            if (! tags.contains (m.tag))
                tags.add (m.tag);
        }
    }
    
    // If no tags found, add generic
    if (tags.isEmpty())
        tags.add ("UNCATEGORIZED");
    
    return tags;
}

//==============================================================================
juce::Array<SampleItem> SampleLibrary::getFilteredSamples (const juce::String& searchQuery,
                                                           const juce::StringArray& activeTags,
                                                           bool favoritesOnly) const
{
    juce::Array<SampleItem> results;
    auto query = searchQuery.toLowerCase();
    
    for (auto& item : allSamples)
    {
        // Favorites filter
        if (favoritesOnly && ! item.isFavorite)
            continue;
        
        // Search filter
        if (query.isNotEmpty())
        {
            bool matchesSearch = item.name.toLowerCase().contains (query);
            
            if (! matchesSearch)
            {
                for (auto& tag : item.tags)
                {
                    if (tag.toLowerCase().contains (query))
                    {
                        matchesSearch = true;
                        break;
                    }
                }
            }
            
            if (! matchesSearch)
                continue;
        }
        
        // Tag filter (OR mode)
        if (! activeTags.isEmpty())
        {
            bool matchesTag = false;
            for (auto& tag : activeTags)
            {
                if (item.tags.contains (tag))
                {
                    matchesTag = true;
                    break;
                }
            }
            if (! matchesTag)
                continue;
        }
        
        results.add (item);
    }
    
    return results;
}

//==============================================================================
void SampleLibrary::toggleFavorite (const juce::File& file)
{
    auto path = file.getFullPathName();
    
    if (favoriteFiles.contains (path))
        favoriteFiles.removeString (path);
    else
        favoriteFiles.add (path);
    
    // Update in allSamples
    for (auto& item : allSamples)
    {
        if (item.file == file)
        {
            item.isFavorite = favoriteFiles.contains (path);
            break;
        }
    }
    
    saveState();
    sendChangeMessage();
}

bool SampleLibrary::isFavorite (const juce::File& file) const
{
    return favoriteFiles.contains (file.getFullPathName());
}

juce::StringArray SampleLibrary::getAllTags() const
{
    juce::StringArray tags;
    for (auto& item : allSamples)
        for (auto& tag : item.tags)
            if (! tags.contains (tag))
                tags.add (tag);
    
    tags.sort (true);
    return tags;
}

//==============================================================================
juce::File SampleLibrary::getSettingsFile() const
{
    auto appData = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                       .getChildFile ("SoundXplorer");
    appData.createDirectory();
    return appData.getChildFile ("library_settings.xml");
}

void SampleLibrary::saveState()
{
    auto xml = std::make_unique<juce::XmlElement> ("SoundXplorerLibrary");
    
    auto* foldersXml = xml->createNewChildElement ("Folders");
    for (auto& folder : libraryFolders)
        foldersXml->createNewChildElement ("Folder")->setAttribute ("path", folder.getFullPathName());
    
    auto* favoritesXml = xml->createNewChildElement ("Favorites");
    for (auto& fav : favoriteFiles)
        favoritesXml->createNewChildElement ("File")->setAttribute ("path", fav);
    
    xml->writeTo (getSettingsFile());
}

void SampleLibrary::loadState()
{
    auto settingsFile = getSettingsFile();
    if (! settingsFile.existsAsFile())
        return;
    
    auto xml = juce::XmlDocument::parse (settingsFile);
    if (xml == nullptr)
        return;
    
    // Load folders
    if (auto* foldersXml = xml->getChildByName ("Folders"))
    {
        for (auto* folderXml : foldersXml->getChildIterator())
        {
            juce::File folder (folderXml->getStringAttribute ("path"));
            if (folder.isDirectory())
                libraryFolders.add (folder);
        }
    }
    
    // Load favorites
    if (auto* favoritesXml = xml->getChildByName ("Favorites"))
    {
        for (auto* favXml : favoritesXml->getChildIterator())
            favoriteFiles.add (favXml->getStringAttribute ("path"));
    }
    
    // Scan all folders
    for (auto& folder : libraryFolders)
        scanFolder (folder);
}
