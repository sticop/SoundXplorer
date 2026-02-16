#pragma once
#include <JuceHeader.h>

//==============================================================================
// Airbnb-inspired LookAndFeel — clean, warm, generous spacing
//==============================================================================
class SoundXplorerLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SoundXplorerLookAndFeel();
    ~SoundXplorerLookAndFeel() override = default;

    // ─── Airbnb colour palette (dark mode adaptation) ───
    static constexpr uint32_t bgDark        = 0xFF191919;  // Hazy Black — deepest layer
    static constexpr uint32_t bgMedium      = 0xFF222222;  // Card/panel surface
    static constexpr uint32_t bgLight       = 0xFF2E2E2E;  // Elevated surface / borders
    static constexpr uint32_t bgHighlight   = 0xFF383838;  // Hover / selection
    static constexpr uint32_t bgCard        = 0xFF252525;  // Slightly raised card
    static constexpr uint32_t textPrimary   = 0xFFF7F7F7;  // Primary text (near-white)
    static constexpr uint32_t textSecondary = 0xFFB0B0B0;  // Secondary / muted text
    static constexpr uint32_t textTertiary  = 0xFF717171;  // Airbnb gray caption
    static constexpr uint32_t rausch        = 0xFFFF385C;  // Airbnb Rausch — primary accent (coral-pink)
    static constexpr uint32_t rauschHover   = 0xFFE0294D;  // Rausch pressed / hover
    static constexpr uint32_t babu          = 0xFF00A699;  // Airbnb Babu teal — secondary accent
    static constexpr uint32_t arches        = 0xFFFC642D;  // Airbnb Arches orange — warm highlight
    static constexpr uint32_t hackberry     = 0xFF914669;  // Airbnb Hackberry — soft berry/purple
    static constexpr uint32_t hof           = 0xFF484848;  // Airbnb Hof — dark gray
    static constexpr uint32_t foggy         = 0xFFDDDDDD;  // Airbnb Foggy — light border/divider
    static constexpr uint32_t tagColor1     = 0xFFFF385C;  // Rausch tags
    static constexpr uint32_t tagColor2     = 0xFF00A699;  // Babu tags
    static constexpr uint32_t heartColor    = 0xFFFF385C;  // Rausch heart
    static constexpr uint32_t sliderThumb   = 0xFFFF385C;  // Rausch slider
    static constexpr uint32_t sliderTrack   = 0xFF484848;  // Hof track

    // ─── Airbnb-style corner radii ───
    static constexpr float radiusSmall  = 8.0f;
    static constexpr float radiusMedium = 12.0f;
    static constexpr float radiusLarge  = 16.0f;

    // ─── Font helpers (Cereal-like system sans-serif) ───
    static juce::Font getDefaultFont (float height);
    static juce::Font getBoldFont (float height);
    static juce::Font getBookFont (float height);

    // ─── Custom icon drawing (no emoji) ───
    static void drawSearchIcon      (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawHeartIcon       (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour, bool filled);
    static void drawPlayIcon        (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawPauseIcon       (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawStopIcon        (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawFolderIcon      (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawHomeIcon        (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawUserIcon        (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawTagIcon         (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawWaveformIcon    (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawBellIcon        (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawGlobeIcon       (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawHelpIcon        (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawGearIcon        (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawLoopIcon        (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawArrowRightIcon  (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);
    static void drawRefreshIcon     (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour);

    // ─── LookAndFeel overrides ───
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
