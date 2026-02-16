#pragma once
#include <JuceHeader.h>
#include "SampleLibrary.h"

//==============================================================================
// Left panel: Library folder browser with tree view
//==============================================================================
class LibraryBrowserComponent : public juce::Component,
                                 public juce::ChangeListener,
                                 public juce::FileBrowserListener
{
public:
    LibraryBrowserComponent (SampleLibrary& library);
    ~LibraryBrowserComponent() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    
    // FileBrowserListener
    void selectionChanged() override {}
    void fileClicked (const juce::File&, const juce::MouseEvent&) override {}
    void fileDoubleClicked (const juce::File&) override {}
    void browserRootChanged (const juce::File&) override {}
    
    std::function<void()> onLibraryChanged;

private:
    void addLibraryClicked();
    void refreshClicked();
    void updateFolderList();
    
    SampleLibrary& library;
    
    juce::TextButton addLibraryButton { "+ ADD LIBRARY" };
    juce::TextButton refreshButton { "Refresh" };
    juce::Label titleLabel;
    juce::Label analysisLabel;
    juce::Label sortLabel;
    
    juce::ListBox folderListBox;
    
    class FolderListModel : public juce::ListBoxModel
    {
    public:
        FolderListModel (SampleLibrary& lib) : library (lib) {}
        
        int getNumRows() override { return library.getLibraryFolders().size(); }
        
        void paintListBoxItem (int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
        
        void listBoxItemClicked (int row, const juce::MouseEvent&) override;
        void deleteKeyPressed (int lastRowSelected) override;
        
        SampleLibrary& library;
        std::function<void()> onChanged;
    };
    
    FolderListModel folderModel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryBrowserComponent)
};
