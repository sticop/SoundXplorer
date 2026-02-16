cmake_minimum_required(VERSION 3.22)
project(SoundXplorer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(JUCE)

# Shared source files (used by both VST and Standalone)
set(SHARED_SOURCES
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/SampleLibrary.cpp
    Source/AudioPreviewEngine.cpp
    Source/FileListComponent.cpp
    Source/LibraryBrowserComponent.cpp
    Source/SearchBarComponent.cpp
    Source/TransportBarComponent.cpp
    Source/TagFilterComponent.cpp
    Source/LookAndFeel.cpp
)

# ─────────────────────── VST3 Plugin ───────────────────────
juce_add_plugin(SoundXplorerVST
    PRODUCT_NAME "Sound Xplorer"
    COMPANY_NAME "SoundXplorer"
    PLUGIN_MANUFACTURER_CODE SxPl
    PLUGIN_CODE SxVs
    FORMATS VST3
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CATEGORIES Tools Analyzer
)

target_sources(SoundXplorerVST PRIVATE ${SHARED_SOURCES})

juce_generate_juce_header(SoundXplorerVST)

target_compile_definitions(SoundXplorerVST PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    SOUNDXPLORER_IS_VST=1
    SOUNDXPLORER_IS_STANDALONE=0
)

target_link_libraries(SoundXplorerVST
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# ─────────────────────── Standalone App ───────────────────────
juce_add_plugin(SoundXplorerApp
    PRODUCT_NAME "Sound Xplorer"
    COMPANY_NAME "SoundXplorer"
    PLUGIN_MANUFACTURER_CODE SxPl
    PLUGIN_CODE SxAp
    FORMATS Standalone
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
)

target_sources(SoundXplorerApp PRIVATE ${SHARED_SOURCES})

juce_generate_juce_header(SoundXplorerApp)

target_compile_definitions(SoundXplorerApp PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    SOUNDXPLORER_IS_VST=0
    SOUNDXPLORER_IS_STANDALONE=1
)

target_link_libraries(SoundXplorerApp
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
