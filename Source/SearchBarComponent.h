#pragma once
#include <JuceHeader.h>

//==============================================================================
// Search bar at the top of the application
//==============================================================================
class SearchBarComponent : public juce::Component,
                           public juce::TextEditor::Listener
{
public:
    SearchBarComponent();
    ~SearchBarComponent() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;

    juce::String getSearchText() const { return searchEditor.getText(); }
    void clear() { searchEditor.clear(); }

    std::function<void (const juce::String&)> onSearchChanged;

    void textEditorTextChanged (juce::TextEditor& editor) override;

private:
    juce::TextEditor searchEditor;
    juce::Label searchIcon;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SearchBarComponent)
};
