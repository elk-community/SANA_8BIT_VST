cmake_minimum_required(VERSION 3.22)

project(SANA_8bit_VST VERSION 2.0.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS Universal Binary support
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build architectures for macOS")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum macOS deployment version")
endif()

# JUCE path - can be overridden via -DJUCE_DIR=<path>
if(NOT DEFINED JUCE_DIR)
    set(JUCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/JUCE")
endif()

# Add JUCE
add_subdirectory(${JUCE_DIR} JUCE)

# Plugin sources
set(PLUGIN_SOURCES
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/EditorGUI.cpp
    Source/EditorGUI.h
    Source/BaseAudioProcessor.cpp
    Source/BaseAudioProcessor.h
    Source/DSP/AmpEnvelope.cpp
    Source/DSP/AmpEnvelope.h
    Source/DSP/ColorEnvelope.cpp
    Source/DSP/ColorEnvelope.h
    Source/DSP/DspUtils.h
    Source/DSP/MIDIEcho.h
    Source/DSP/SimpleSound.cpp
    Source/DSP/SimpleSound.h
    Source/DSP/SimpleVoice.cpp
    Source/DSP/SimpleVoice.h
    Source/DSP/SynthParameters.cpp
    Source/DSP/SynthParameters.h
    Source/DSP/Timer.cpp
    Source/DSP/Timer.h
    Source/DSP/Waveforms.cpp
    Source/DSP/Waveforms.h
    Source/GUI/ComponentUtil.hpp
    Source/GUI/ParametersComponent.cpp
    Source/GUI/ParametersComponent.h
    Source/GUI/ScopeComponent.hpp
)

# Create the audio plugin target
juce_add_plugin(SANA_8bit_VST
    COMPANY_NAME "MasakiMori"
    BUNDLE_ID "com.MasakiMori.SANA"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Msmr
    PLUGIN_CODE Sana
    FORMATS VST3 AU Standalone
    VST3_CATEGORIES Instrument Synth
    AU_MAIN_TYPE kAudioUnitType_MusicDevice
    PRODUCT_NAME "SANA 8bit VST"
)

# Generate JuceHeader.h for legacy compatibility
juce_generate_juce_header(SANA_8bit_VST)

# Disable Unity Build to avoid include order issues
set_target_properties(SANA_8bit_VST PROPERTIES UNITY_BUILD OFF)

# Add source files to target
target_sources(SANA_8bit_VST PRIVATE ${PLUGIN_SOURCES})

# Include directories
target_include_directories(SANA_8bit_VST PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/DSP
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/GUI
    ${CMAKE_BINARY_DIR}/SANA_8bit_VST_artefacts/JuceLibraryCode
)

# Preprocessor definitions
target_compile_definitions(SANA_8bit_VST PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=1
)

# Force UTF-8 encoding for MSVC (fixes Japanese comments in source files)
if(MSVC)
    target_compile_options(SANA_8bit_VST PRIVATE /utf-8)
endif()

# Link JUCE modules
target_link_libraries(SANA_8bit_VST
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_cryptography
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_opengl
        juce::juce_video
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
