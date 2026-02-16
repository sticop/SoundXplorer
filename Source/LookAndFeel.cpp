#include "LookAndFeel.h"

//==============================================================================
// Font helpers — Cereal-like clean sans-serif
//==============================================================================
juce::Font SoundXplorerLookAndFeel::getDefaultFont (float height)
{
    return juce::Font (juce::FontOptions().withName ("Helvetica Neue").withHeight (height));
}

juce::Font SoundXplorerLookAndFeel::getBoldFont (float height)
{
    return juce::Font (juce::FontOptions().withName ("Helvetica Neue").withStyle ("Bold").withHeight (height));
}

juce::Font SoundXplorerLookAndFeel::getBookFont (float height)
{
    return juce::Font (juce::FontOptions().withName ("Helvetica Neue").withStyle ("Light").withHeight (height));
}

//==============================================================================
// Constructor — set the palette
//==============================================================================
SoundXplorerLookAndFeel::SoundXplorerLookAndFeel()
{
    setColour (juce::ResizableWindow::backgroundColourId,             juce::Colour (bgDark));
    setColour (juce::TextEditor::backgroundColourId,                  juce::Colour (bgMedium));
    setColour (juce::TextEditor::textColourId,                        juce::Colour (textPrimary));
    setColour (juce::TextEditor::outlineColourId,                     juce::Colour (bgLight));
    setColour (juce::TextEditor::focusedOutlineColourId,              juce::Colour (rausch));
    setColour (juce::Label::textColourId,                             juce::Colour (textPrimary));
    setColour (juce::ListBox::backgroundColourId,                     juce::Colour (bgDark));
    setColour (juce::ListBox::textColourId,                           juce::Colour (textPrimary));
    setColour (juce::ScrollBar::thumbColourId,                        juce::Colour (bgHighlight));
    setColour (juce::ScrollBar::trackColourId,                        juce::Colour (bgMedium));
    setColour (juce::ComboBox::backgroundColourId,                    juce::Colour (bgMedium));
    setColour (juce::ComboBox::textColourId,                          juce::Colour (textPrimary));
    setColour (juce::ComboBox::outlineColourId,                       juce::Colour (bgLight));
    setColour (juce::PopupMenu::backgroundColourId,                   juce::Colour (bgCard));
    setColour (juce::PopupMenu::textColourId,                         juce::Colour (textPrimary));
    setColour (juce::PopupMenu::highlightedBackgroundColourId,        juce::Colour (bgHighlight));
    setColour (juce::TextButton::buttonColourId,                      juce::Colour (bgMedium));
    setColour (juce::TextButton::textColourOffId,                     juce::Colour (textPrimary));
    setColour (juce::TableHeaderComponent::backgroundColourId,        juce::Colour (bgMedium));
    setColour (juce::TableHeaderComponent::textColourId,              juce::Colour (textTertiary));
    setColour (juce::TableHeaderComponent::outlineColourId,           juce::Colour (bgLight));
}

//==============================================================================
// Vector icon drawing — clean Airbnb-style line icons (1.5–2px stroke)
//==============================================================================
void SoundXplorerLookAndFeel::drawSearchIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.22f);
    auto cx = r.getCentreX() - r.getWidth() * 0.08f;
    auto cy = r.getCentreY() - r.getHeight() * 0.08f;
    auto rad = r.getWidth() * 0.32f;

    g.setColour (colour);
    g.drawEllipse (cx - rad, cy - rad, rad * 2.0f, rad * 2.0f, 1.8f);

    // Handle
    auto angle = juce::MathConstants<float>::pi * 0.25f;
    auto hx1 = cx + rad * std::cos (angle);
    auto hy1 = cy + rad * std::sin (angle);
    auto hx2 = hx1 + r.getWidth() * 0.28f;
    auto hy2 = hy1 + r.getHeight() * 0.28f;
    g.drawLine (hx1, hy1, hx2, hy2, 1.8f);
}

void SoundXplorerLookAndFeel::drawHeartIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour, bool filled)
{
    auto r = area.reduced (area.getWidth() * 0.2f);
    juce::Path heart;

    auto w = r.getWidth();
    auto h = r.getHeight();
    auto x = r.getX();
    auto y = r.getY();

    heart.startNewSubPath (x + w * 0.5f, y + h * 0.9f);
    heart.cubicTo (x, y + h * 0.55f, x, y + h * 0.1f, x + w * 0.25f, y + h * 0.1f);
    heart.cubicTo (x + w * 0.4f, y + h * 0.1f, x + w * 0.5f, y + h * 0.27f, x + w * 0.5f, y + h * 0.27f);
    heart.cubicTo (x + w * 0.5f, y + h * 0.27f, x + w * 0.6f, y + h * 0.1f, x + w * 0.75f, y + h * 0.1f);
    heart.cubicTo (x + w, y + h * 0.1f, x + w, y + h * 0.55f, x + w * 0.5f, y + h * 0.9f);
    heart.closeSubPath();

    g.setColour (colour);
    if (filled)
        g.fillPath (heart);
    else
        g.strokePath (heart, juce::PathStrokeType (1.5f));
}

void SoundXplorerLookAndFeel::drawPlayIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.28f);
    juce::Path tri;
    tri.addTriangle (r.getX(), r.getY(), r.getX(), r.getBottom(), r.getRight(), r.getCentreY());
    g.setColour (colour);
    g.fillPath (tri);
}

void SoundXplorerLookAndFeel::drawPauseIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.28f);
    auto barW = r.getWidth() * 0.3f;
    g.setColour (colour);
    g.fillRoundedRectangle (r.getX(), r.getY(), barW, r.getHeight(), 2.0f);
    g.fillRoundedRectangle (r.getRight() - barW, r.getY(), barW, r.getHeight(), 2.0f);
}

void SoundXplorerLookAndFeel::drawStopIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.3f);
    g.setColour (colour);
    g.fillRoundedRectangle (r, 2.0f);
}

void SoundXplorerLookAndFeel::drawFolderIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.2f);
    juce::Path p;
    auto y = r.getY() + r.getHeight() * 0.15f;
    auto tabW = r.getWidth() * 0.35f;
    auto tabH = r.getHeight() * 0.15f;

    p.startNewSubPath (r.getX(), y + tabH);
    p.lineTo (r.getX(), y);
    p.lineTo (r.getX() + tabW, y);
    p.lineTo (r.getX() + tabW + tabH, y + tabH);
    p.lineTo (r.getRight(), y + tabH);
    p.lineTo (r.getRight(), r.getBottom());
    p.lineTo (r.getX(), r.getBottom());
    p.closeSubPath();

    g.setColour (colour);
    g.strokePath (p, juce::PathStrokeType (1.5f));
}

void SoundXplorerLookAndFeel::drawHomeIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.22f);
    juce::Path p;
    // Roof
    p.startNewSubPath (r.getCentreX(), r.getY());
    p.lineTo (r.getX(), r.getCentreY());
    p.lineTo (r.getRight(), r.getCentreY());
    p.closeSubPath();
    // Body
    p.addRectangle (r.getX() + r.getWidth() * 0.15f, r.getCentreY(),
                    r.getWidth() * 0.7f, r.getHeight() * 0.45f);
    g.setColour (colour);
    g.strokePath (p, juce::PathStrokeType (1.5f));
}

void SoundXplorerLookAndFeel::drawUserIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.24f);
    g.setColour (colour);
    // Head
    auto headR = r.getWidth() * 0.22f;
    g.drawEllipse (r.getCentreX() - headR, r.getY(), headR * 2.0f, headR * 2.0f, 1.5f);
    // Body arc
    juce::Path body;
    auto bodyY = r.getY() + headR * 2.0f + r.getHeight() * 0.1f;
    body.addArc (r.getX(), bodyY, r.getWidth(), r.getHeight() * 0.6f,
                 juce::MathConstants<float>::pi, juce::MathConstants<float>::twoPi, true);
    g.strokePath (body, juce::PathStrokeType (1.5f));
}

void SoundXplorerLookAndFeel::drawTagIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.24f);
    juce::Path p;
    p.startNewSubPath (r.getX(), r.getCentreY());
    p.lineTo (r.getX(), r.getY());
    p.lineTo (r.getCentreX(), r.getY());
    p.lineTo (r.getRight(), r.getCentreY());
    p.lineTo (r.getCentreX(), r.getBottom());
    p.lineTo (r.getX(), r.getBottom());
    p.closeSubPath();
    g.setColour (colour);
    g.strokePath (p, juce::PathStrokeType (1.5f));
    // Dot
    g.fillEllipse (r.getX() + r.getWidth() * 0.15f, r.getCentreY() - 1.5f, 3.0f, 3.0f);
}

void SoundXplorerLookAndFeel::drawWaveformIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.22f);
    g.setColour (colour);
    float bars[] = { 0.3f, 0.7f, 1.0f, 0.5f, 0.8f };
    int n = 5;
    float barW = r.getWidth() / (float)(n * 2 - 1);
    for (int i = 0; i < n; ++i)
    {
        float bh = r.getHeight() * bars[i];
        float bx = r.getX() + i * barW * 2.0f;
        float by = r.getCentreY() - bh * 0.5f;
        g.fillRoundedRectangle (bx, by, barW, bh, 1.0f);
    }
}

void SoundXplorerLookAndFeel::drawBellIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.24f);
    g.setColour (colour);
    juce::Path p;
    p.startNewSubPath (r.getX(), r.getBottom() - r.getHeight() * 0.15f);
    p.lineTo (r.getX() + r.getWidth() * 0.15f, r.getCentreY());
    p.quadraticTo (r.getX() + r.getWidth() * 0.15f, r.getY(), r.getCentreX(), r.getY());
    p.quadraticTo (r.getRight() - r.getWidth() * 0.15f, r.getY(),
                   r.getRight() - r.getWidth() * 0.15f, r.getCentreY());
    p.lineTo (r.getRight(), r.getBottom() - r.getHeight() * 0.15f);
    p.closeSubPath();
    g.strokePath (p, juce::PathStrokeType (1.5f));
    // Clapper
    g.fillEllipse (r.getCentreX() - 2.0f, r.getBottom() - 4.0f, 4.0f, 4.0f);
}

void SoundXplorerLookAndFeel::drawGlobeIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.24f);
    g.setColour (colour);
    auto rad = juce::jmin (r.getWidth(), r.getHeight()) * 0.5f;
    g.drawEllipse (r.getCentreX() - rad, r.getCentreY() - rad, rad * 2.0f, rad * 2.0f, 1.5f);
    // Meridians
    g.drawEllipse (r.getCentreX() - rad * 0.5f, r.getCentreY() - rad, rad, rad * 2.0f, 1.0f);
    g.drawLine (r.getCentreX() - rad, r.getCentreY(), r.getCentreX() + rad, r.getCentreY(), 1.0f);
}

void SoundXplorerLookAndFeel::drawHelpIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.24f);
    g.setColour (colour);
    auto rad = juce::jmin (r.getWidth(), r.getHeight()) * 0.5f;
    g.drawEllipse (r.getCentreX() - rad, r.getCentreY() - rad, rad * 2.0f, rad * 2.0f, 1.5f);
    g.setFont (getBoldFont (r.getHeight() * 0.55f));
    g.drawText ("?", r, juce::Justification::centred);
}

void SoundXplorerLookAndFeel::drawGearIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.24f);
    auto cx = r.getCentreX();
    auto cy = r.getCentreY();
    auto outerR = juce::jmin (r.getWidth(), r.getHeight()) * 0.5f;
    auto innerR = outerR * 0.6f;

    g.setColour (colour);
    g.drawEllipse (cx - innerR * 0.6f, cy - innerR * 0.6f, innerR * 1.2f, innerR * 1.2f, 1.5f);

    int teeth = 8;
    for (int i = 0; i < teeth; ++i)
    {
        float angle = (float)i / (float)teeth * juce::MathConstants<float>::twoPi;
        auto x1 = cx + innerR * std::cos (angle);
        auto y1 = cy + innerR * std::sin (angle);
        auto x2 = cx + outerR * std::cos (angle);
        auto y2 = cy + outerR * std::sin (angle);
        g.drawLine (x1, y1, x2, y2, 1.5f);
    }
}

void SoundXplorerLookAndFeel::drawLoopIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.22f);
    g.setColour (colour);
    // Two arcs forming a loop
    juce::Path p;
    p.addArc (r.getX(), r.getY(), r.getWidth() * 0.55f, r.getHeight(),
              juce::MathConstants<float>::pi * 0.5f, juce::MathConstants<float>::pi * 1.5f, true);
    p.addArc (r.getX() + r.getWidth() * 0.45f, r.getY(), r.getWidth() * 0.55f, r.getHeight(),
              -juce::MathConstants<float>::pi * 0.5f, juce::MathConstants<float>::pi * 0.5f, true);
    g.strokePath (p, juce::PathStrokeType (1.5f));
    // Arrow tips
    auto aw = 3.0f;
    g.drawLine (r.getRight() * 0.72f - aw, r.getY() - aw, r.getRight() * 0.72f, r.getY(), 1.5f);
    g.drawLine (r.getRight() * 0.72f + aw, r.getY() - aw, r.getRight() * 0.72f, r.getY(), 1.5f);
}

void SoundXplorerLookAndFeel::drawArrowRightIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.3f);
    g.setColour (colour);
    g.drawLine (r.getX(), r.getCentreY(), r.getRight(), r.getCentreY(), 1.5f);
    g.drawLine (r.getRight() - r.getWidth() * 0.35f, r.getY() + r.getHeight() * 0.2f,
                r.getRight(), r.getCentreY(), 1.5f);
    g.drawLine (r.getRight() - r.getWidth() * 0.35f, r.getBottom() - r.getHeight() * 0.2f,
                r.getRight(), r.getCentreY(), 1.5f);
}

void SoundXplorerLookAndFeel::drawRefreshIcon (juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    auto r = area.reduced (area.getWidth() * 0.26f);
    g.setColour (colour);
    auto rad = juce::jmin (r.getWidth(), r.getHeight()) * 0.42f;
    juce::Path arc;
    arc.addArc (r.getCentreX() - rad, r.getCentreY() - rad, rad * 2.0f, rad * 2.0f,
                0.0f, juce::MathConstants<float>::pi * 1.5f, true);
    g.strokePath (arc, juce::PathStrokeType (1.5f));
    // Arrow head
    auto ex = r.getCentreX();
    auto ey = r.getCentreY() - rad;
    g.drawLine (ex - 3.0f, ey, ex, ey - 3.0f, 1.5f);
    g.drawLine (ex + 3.0f, ey, ex, ey - 3.0f, 1.5f);
}

//==============================================================================
// Button drawing
//==============================================================================
void SoundXplorerLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button,
                                                     const juce::Colour& backgroundColour,
                                                     bool shouldDrawButtonAsHighlighted,
                                                     bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f);
    auto baseColour = backgroundColour;

    if (shouldDrawButtonAsDown)
        baseColour = baseColour.brighter (0.15f);
    else if (shouldDrawButtonAsHighlighted)
        baseColour = baseColour.brighter (0.08f);

    g.setColour (baseColour);
    g.fillRoundedRectangle (bounds, radiusSmall);

    if (button.getToggleState())
    {
        g.setColour (juce::Colour (rausch));
        g.drawRoundedRectangle (bounds, radiusSmall, 1.8f);
    }
}

void SoundXplorerLookAndFeel::drawButtonText (juce::Graphics& g, juce::TextButton& button,
                                               bool /*shouldDrawButtonAsHighlighted*/,
                                               bool /*shouldDrawButtonAsDown*/)
{
    auto colour = button.getToggleState() ? juce::Colour (rausch) : button.findColour (juce::TextButton::textColourOffId);
    g.setColour (colour);
    g.setFont (getDefaultFont (13.0f));
    g.drawFittedText (button.getButtonText(), button.getLocalBounds().reduced (6, 2),
                      juce::Justification::centred, 1);
}

//==============================================================================
// Slider drawing — Airbnb Rausch gradient slider
//==============================================================================
void SoundXplorerLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                                 float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/,
                                                 const juce::Slider::SliderStyle /*style*/,
                                                 juce::Slider& /*slider*/)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height);
    auto trackY = bounds.getCentreY();
    auto trackHeight = 4.0f;

    // Track background
    g.setColour (juce::Colour (sliderTrack));
    g.fillRoundedRectangle (bounds.getX(), trackY - trackHeight / 2.0f, bounds.getWidth(), trackHeight, trackHeight / 2.0f);

    // Filled portion — Rausch gradient
    auto filledWidth = sliderPos - bounds.getX();
    if (filledWidth > 0.0f)
    {
        juce::ColourGradient grad (juce::Colour (rausch), bounds.getX(), trackY,
                                    juce::Colour (arches), sliderPos, trackY, false);
        g.setGradientFill (grad);
        g.fillRoundedRectangle (bounds.getX(), trackY - trackHeight / 2.0f, filledWidth, trackHeight, trackHeight / 2.0f);
    }

    // Thumb — clean white circle with shadow
    auto thumbRadius = 7.0f;
    // Shadow
    g.setColour (juce::Colours::black.withAlpha (0.25f));
    g.fillEllipse (sliderPos - thumbRadius, trackY - thumbRadius + 1.0f, thumbRadius * 2.0f, thumbRadius * 2.0f);
    // White circle
    g.setColour (juce::Colours::white);
    g.fillEllipse (sliderPos - thumbRadius, trackY - thumbRadius, thumbRadius * 2.0f, thumbRadius * 2.0f);
}

//==============================================================================
// Text editor — search-bar-like rounded pill
//==============================================================================
void SoundXplorerLookAndFeel::drawTextEditorOutline (juce::Graphics& g, int width, int height,
                                                      juce::TextEditor& editor)
{
    auto bounds = juce::Rectangle<float> (0.0f, 0.0f, (float) width, (float) height);
    auto focused = editor.hasKeyboardFocus (true);
    g.setColour (focused ? juce::Colour (rausch) : juce::Colour (bgLight));
    g.drawRoundedRectangle (bounds.reduced (0.5f), radiusMedium, focused ? 2.0f : 1.0f);
}

void SoundXplorerLookAndFeel::fillTextEditorBackground (juce::Graphics& g, int width, int height,
                                                         juce::TextEditor& /*editor*/)
{
    g.setColour (juce::Colour (bgMedium));
    g.fillRoundedRectangle (0.0f, 0.0f, (float) width, (float) height, radiusMedium);
}

juce::Font SoundXplorerLookAndFeel::getTextButtonFont (juce::TextButton&, int /*buttonHeight*/)
{
    return getDefaultFont (13.0f);
}

//==============================================================================
// Table header — Airbnb-style minimal header
//==============================================================================
void SoundXplorerLookAndFeel::drawTableHeaderBackground (juce::Graphics& g,
                                                          juce::TableHeaderComponent& header)
{
    g.fillAll (juce::Colour (bgMedium));
    g.setColour (juce::Colour (bgLight));
    g.drawLine (0.0f, (float) header.getHeight() - 0.5f, (float) header.getWidth(), (float) header.getHeight() - 0.5f);
}

void SoundXplorerLookAndFeel::drawTableHeaderColumn (juce::Graphics& g,
                                                      juce::TableHeaderComponent& /*header*/,
                                                      const juce::String& columnName,
                                                      int /*columnId*/, int width, int height,
                                                      bool isMouseOver, bool /*isMouseDown*/,
                                                      int /*columnFlags*/)
{
    if (isMouseOver)
    {
        g.setColour (juce::Colour (bgHighlight));
        g.fillRect (0, 0, width, height);
    }

    g.setColour (juce::Colour (textTertiary));
    g.setFont (getBoldFont (11.0f));
    g.drawText (columnName, 10, 0, width - 20, height, juce::Justification::centredLeft);
}
