#include "LibraryBrowserComponent.h"
#include "LookAndFeel.h"

//==============================================================================
LibraryBrowserComponent::LibraryBrowserComponent (SampleLibrary& lib)
    : library (lib), folderModel (lib)
{
    library.addChangeListener (this);

    // Title
    titleLabel.setText ("LIBRARIES", juce::dontSendNotification);
    titleLabel.setFont (SoundXplorerLookAndFeel::getBoldFont (14.0f));
    titleLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textPrimary));
    addAndMakeVisible (titleLabel);

    // Analysis progress label
    analysisLabel.setFont (SoundXplorerLookAndFeel::getDefaultFont (11.0f));
    analysisLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::rausch));
    addAndMakeVisible (analysisLabel);
    updateFolderList();

    // Sort label
    sortLabel.setText ("Name (ASC)", juce::dontSendNotification);
    sortLabel.setFont (SoundXplorerLookAndFeel::getDefaultFont (11.0f));
    sortLabel.setColour (juce::Label::textColourId, juce::Colour (SoundXplorerLookAndFeel::textTertiary));
    addAndMakeVisible (sortLabel);

    // Buttons
    addLibraryButton.setColour (juce::TextButton::buttonColourId, juce::Colour (SoundXplorerLookAndFeel::bgLight));
    addLibraryButton.onClick = [this] { addLibraryClicked(); };
    addAndMakeVisible (addLibraryButton);

    refreshButton.setColour (juce::TextButton::buttonColourId, juce::Colour (SoundXplorerLookAndFeel::bgLight));
    refreshButton.onClick = [this] { refreshClicked(); };
    addAndMakeVisible (refreshButton);

    // Folder list
    folderListBox.setModel (&folderModel);
    folderListBox.setColour (juce::ListBox::backgroundColourId, juce::Colours::transparentBlack);
    folderListBox.setRowHeight (28);
    addAndMakeVisible (folderListBox);

    folderModel.onChanged = [this]
    {
        updateFolderList();
        if (onLibraryChanged)
            onLibraryChanged();
    };
}

LibraryBrowserComponent::~LibraryBrowserComponent()
{
    library.removeChangeListener (this);
}

void LibraryBrowserComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (SoundXplorerLookAndFeel::bgDark));

    // Right border
    g.setColour (juce::Colour (SoundXplorerLookAndFeel::bgLight));
    g.drawLine ((float) getWidth() - 0.5f, 0.0f, (float) getWidth() - 0.5f, (float) getHeight());
}

void LibraryBrowserComponent::resized()
{
    auto bounds = getLocalBounds().reduced (8);

    auto headerArea = bounds.removeFromTop (24);
    titleLabel.setBounds (headerArea.removeFromLeft (100));
    analysisLabel.setBounds (headerArea);

    bounds.removeFromTop (8);

    auto buttonRow = bounds.removeFromTop (28);
    addLibraryButton.setBounds (buttonRow.removeFromLeft (buttonRow.getWidth() / 2 - 4));
    buttonRow.removeFromLeft (8);
    refreshButton.setBounds (buttonRow);

    bounds.removeFromTop (12);

    sortLabel.setBounds (bounds.removeFromTop (20));

    bounds.removeFromTop (4);
    folderListBox.setBounds (bounds);
}

void LibraryBrowserComponent::changeListenerCallback (juce::ChangeBroadcaster*)
{
    updateFolderList();
}

void LibraryBrowserComponent::addLibraryClicked()
{
    auto chooser = std::make_shared<juce::FileChooser> ("Select Audio Library Folder");

    chooser->launchAsync (juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories,
                          [this, chooser] (const juce::FileChooser& fc)
    {
        auto result = fc.getResult();
        if (result.isDirectory())
        {
            library.addLibraryFolder (result);
            updateFolderList();
            if (onLibraryChanged)
                onLibraryChanged();
        }
    });
}

void LibraryBrowserComponent::refreshClicked()
{
    library.refreshLibraries();
    updateFolderList();
    if (onLibraryChanged)
        onLibraryChanged();
}

void LibraryBrowserComponent::updateFolderList()
{
    auto progress = library.getAnalysisProgress();
    auto total = library.getTotalFileCount();

    juce::String analysisText;
    if (progress < 1.0f && progress > 0.0f)
        analysisText = "All Libraries " + juce::String (progress * 100.0f, 1) + "% Analyzed";
    else if (total > 0)
        analysisText = "All Libraries " + juce::String (total) + " files";

    analysisLabel.setText (analysisText, juce::dontSendNotification);
    folderListBox.updateContent();
    repaint();
}

//==============================================================================
void LibraryBrowserComponent::FolderListModel::paintListBoxItem (int rowNumber, juce::Graphics& g,
                                                                  int width, int height, bool rowIsSelected)
{
    auto& folders = library.getLibraryFolders();
    if (rowNumber >= folders.size())
        return;

    if (rowIsSelected)
    {
        g.setColour (juce::Colour (SoundXplorerLookAndFeel::bgHighlight));
        g.fillRoundedRectangle (0.0f, 0.0f, (float) width, (float) height, 6.0f);
    }

    // Folder icon (vector)
    auto iconArea = juce::Rectangle<float> (6.0f, (height - 16.0f) * 0.5f, 16.0f, 16.0f);
    SoundXplorerLookAndFeel::drawFolderIcon (g, iconArea, juce::Colour (SoundXplorerLookAndFeel::babu));

    // Folder name
    g.setColour (juce::Colour (SoundXplorerLookAndFeel::textPrimary));
    g.setFont (SoundXplorerLookAndFeel::getDefaultFont (13.0f));
    g.drawText (folders[rowNumber].getFileName(), 28, 0, width - 32, height, juce::Justification::centredLeft);
}

void LibraryBrowserComponent::FolderListModel::listBoxItemClicked (int /*row*/, const juce::MouseEvent&)
{
    // Could expand to show sub-folders
}

void LibraryBrowserComponent::FolderListModel::deleteKeyPressed (int lastRowSelected)
{
    auto& folders = library.getLibraryFolders();
    if (lastRowSelected >= 0 && lastRowSelected < folders.size())
    {
        library.removeLibraryFolder (folders[lastRowSelected]);
        if (onChanged)
            onChanged();
    }
}
