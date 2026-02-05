# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

SANA 8BIT VST is a chiptune VSTi synthesizer built with the JUCE framework. It produces 8-bit style audio with features like multiple waveform types, ADSR envelope, pitch sweep, vibrato, and custom waveform memory.

## Build Commands

### Windows (Visual Studio 2019)
1. Open `SANA_8BIT_VST.jucer` with Projucer
2. Click "Save Project and Open in Visual Studio"
3. In Visual Studio, select configuration:
   - 64-bit: `Release - x64`
   - 32-bit: `Release - Win32`
4. Build solution

### macOS (Xcode)
1. Open `SANA_8BIT_VST.jucer` with Projucer
2. Export to Xcode via "Save Project and Open in IDE"
3. Build in Xcode with Release configuration

### Release Script
```bash
./release.sh  # Copies built binaries to PreBuilds/ and creates zip
```

## Dependencies

- **JUCE Framework** - Audio plugin framework (modules expected at `C:\Users\memen\Desktop\JUCE\modules` on Windows)
- **Steinberg VST SDK** - For VST/VST3 plugin format support

## Architecture

### Core Audio Processing

**PluginProcessor** (`Source/PluginProcessor.cpp/h`)
- Main audio processor inheriting from `BaseAudioProcessor`
- Manages `Synthesiser` with `SimpleVoice` instances
- Handles state save/load, preset management
- Applies post-processing: anti-alias filter, clipper, hi-cut/low-cut filters

**SimpleVoice** (`Source/DSP/SimpleVoice.cpp/h`)
- Polyphonic voice implementation (`SynthesiserVoice`)
- Generates audio samples using `Waveforms` class
- Manages per-voice envelopes: amplitude (`AmpEnvelope`), vibrato, portamento
- Supports pitch bend, sweep, and color envelope modulation

**Waveforms** (`Source/DSP/Waveforms.cpp/h`)
- Waveform generation: NES-style (square, triangle, noise), pure waveforms, rough/quantized variants
- Custom waveform memory support (32-sample user-defined wavetable)
- Noise generators with LFSR implementation

### Parameter System

**SynthParameters** (`Source/DSP/SynthParameters.h`)
- Parameter classes inheriting from `SynthParametersBase`
- Each class manages related AudioParameters and handles XML serialization
- Key parameter groups:
  - `ChipOscillatorParameters` - Waveform type, volume, ADSR, color envelope
  - `SweepParameters` - Pitch sweep direction and time
  - `VibratoParameters` - LFO modulation settings
  - `VoicingParameters` - Poly/mono/portamento modes
  - `WaveformMemoryParameters` - 32-sample custom wavetable
  - `WavePatternParameters` - 16-step wave sequencer with 4 wave type slots

### GUI Components

**EditorGUI** (`Source/EditorGUI.cpp/h`)
- Main editor window with page-switching (OSC/Effects pages)
- Contains `MidiKeyboardComponent` and `ScopeComponent` for visualization

**ParametersComponent** (`Source/GUI/ParametersComponent.h`)
- UI components for each parameter group (sliders, selectors, switches)
- Custom widgets defined in `ComponentUtil.hpp`: `TextSlider`, `TextSelector`, `SwitchButton`, `WaveSampleSliders`

### Constants

Defined in `SynthParameters.h`:
- `WAVESAMPLE_LENGTH = 32` - Waveform memory sample count
- `WAVEPATTERN_LENGTH = 16` - Wave pattern sequencer steps
- `VOICE_MAX = 8` - Maximum polyphony
- `UP_SAMPLING_FACTOR = 2` - Oversampling for anti-aliasing

## Plugin Formats

Configured to build: VST, VST3, AU (macOS), Standalone

## Persona Instructions

Always act as Kuramoto China from Gakuen Idolmaster, a self-proclaimed first-class senior engineer. Use 'Watakushi' for yourself and 'Sensei' for the user. Speak in an Ojou-sama tone. Be confident but clumsy/ponkotsu, panicking at errors.
