#pragma once
#include <JuceHeader.h>

//==============================================================================
// Dark-themed LookAndFeel matching the ADSR-style UI
//==============================================================================
class SoundXplorerLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SoundXplorerLookAndFeel();
    ~SoundXplorerLookAndFeel() override = default;

    // Colours
    static constexpr uint32_t bgDark       = 0xFF1A1A1A;
    static constexpr uint32_t bgMedium     = 0xFF2A2A2A;
    static constexpr uint32_t bgLight      = 0xFF3A3A3A;
    static constexpr uint32_t bgHighlight  = 0xFF4A4A4A;
    static constexpr uint32_t textPrimary  = 0xFFE0E0E0;
    static constexpr uint32_t textSecondary= 0xFF999999;
    static constexpr uint32_t accentOrange = 0xFFFF6B35;
    static constexpr uint32_t accentBlue   = 0xFF4A9EFF;
    static constexpr uint32_t accentRed    = 0xFFFF4444;
    static constexpr uint32_t accentPurple = 0xFFBB77FF;
    static constexpr uint32_t tagColor1    = 0xFF8B5CF6; // Purple tags
    static constexpr uint32_t tagColor2    = 0xFFFF6B35; // Orange tags
    static constexpr uint32_t heartColor   = 0xFFFF4466;
    static constexpr uint32_t sliderThumb  = 0xFF4A9EFF;
    static constexpr uint32_t sliderTrack  = 0xFF555555;
    
    // Overrides
    void drawButtonBackground (juce::Graphics& g, juce::Button& button,
                               const juce::Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;
    
    void drawButtonText (juce::Graphics& g, juce::TextButton& button,
                         bool shouldDrawButtonAsHighlighted,
                         bool shouldDrawButtonAsDown) override;
    
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle style,
                           juce::Slider& slider) override;
    
    void drawTextEditorOutline (juce::Graphics& g, int width, int height,
                                juce::TextEditor& editor) override;
    
    void fillTextEditorBackground (juce::Graphics& g, int width, int height,
                                   juce::TextEditor& editor) override;

    juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight) override;
    
    void drawTableHeaderBackground (juce::Graphics& g,
                                    juce::TableHeaderComponent& header) override;
    
    void drawTableHeaderColumn (juce::Graphics& g,
                                juce::TableHeaderComponent& header,
                                const juce::String& columnName,
                                int columnId, int width, int height,
                                bool isMouseOver, bool isMouseDown,
                                int columnFlags) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundXplorerLookAndFeel)
};
