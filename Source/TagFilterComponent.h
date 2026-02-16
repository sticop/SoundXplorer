#pragma once
#include <JuceHeader.h>

//==============================================================================
// Tag filter bar with clickable tag buttons (OR logic)
//==============================================================================
class TagFilterComponent : public juce::Component
{
public:
    TagFilterComponent();
    ~TagFilterComponent() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;
    
    void setAvailableTags (const juce::StringArray& tags);
    juce::StringArray getActiveTags() const;
    
    std::function<void (const juce::StringArray&)> onTagFilterChanged;

private:
    void rebuildTagButtons();
    
    juce::StringArray allTags;
    juce::StringArray activeTags;
    
    juce::OwnedArray<juce::TextButton> tagButtons;
    juce::Label modeLabel;
    juce::Viewport viewport;
    juce::Component tagContainer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TagFilterComponent)
};
