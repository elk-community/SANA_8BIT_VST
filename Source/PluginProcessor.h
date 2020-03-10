/*
==============================================================================

        PluginProcessor.h
        Created: 16 May 2018 1:55:55am
        Modified: 11 September 2018
        Author:  MasakiMori, COx2
        ChangeLog:
                Modified some parameters
                Modified some components

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DSP/DspUtils.h"
#include "DSP/SimpleSynthParameters.h"
#include "GUI/ScopeComponent.hpp"

#include <set>

//==============================================================================
class SimpleSynthAudioProcessor : public AudioProcessor {
 public:
  //==============================================================================
  SimpleSynthAudioProcessor();
  ~SimpleSynthAudioProcessor();

  //==============================================================================
  void prepareToPlay(double sampleRate, std::int32_t samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

  void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const String getProgramName(int index) override;
  void changeProgramName(int index, const String& newName) override;
  void initProgram();

  //==============================================================================
  void getStateInformation(MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  //==============================================================================
  MidiKeyboardState& getKeyboardState() { return keyboardState; }

  AudioBufferQueue<float>& getAudioBufferQueue() { return scopeDataQueue; }

  void changeVoiceSize();

  // Parameterの用意[1]
  const StringArray OSC_WAVE_TYPES{
      "NES_Square50%",    "NES_Square25%",   "NES_Square12.5%",
      "NES_Triangle",     "Pure_Square50%",  "Pure_Square25%",
      "Pure_Square12.5%", "Pure_Triangle",   "Pure_Sine",
      "Pure_Saw",         "NES_LongNoise",   "NES_ShortNoise",
      "Pure_Lo-bitNoise", "Waveform Memory",
  };
  const StringArray SWEEP_SWITCH{"OFF", "Positive", "Negative"};
  const StringArray VOICING_SWITCH{"POLY", "MONO", "PORTAMENTO", "ARPEGGIO"};

  PresetsParameters presetsParameters;
  ChipOscillatorParameters chipOscParameters;
  SweepParameters sweepParameters;
  VibratoParameters vibratoParameters;
  VoicingParameters voicingParameters;
  OptionsParameters optionsParameters;
  WaveformMemoryParameters waveformMemoryParameters;
  MidiEchoParameters midiEchoParameters;
  FilterParameters filterParameters;

  // HOST先APPの情報を保持する
  AudioPlayHead* playHead;
  AudioPlayHead::CurrentPositionInfo currentPositionInfo;

 private:
  Synthesiser synth;

  // preset index
  std::int32_t currentProgIndex;

  //[3]のオブジェクトの初期化に必要な情報を保持する構造体[4]
  dsp::ProcessSpec spec;

  //アンチエイリアスフィルタ用
  antiAliasFilter m_antiAliasFilter;

  const std::int32_t upSamplingFactor = 2;

  // DSPエフェクト，クリッパー，ドライブ，フィルタ
  dsp::WaveShaper<float> clipper;
  dsp::Gain<float> drive;
  dsp::ProcessorDuplicator<dsp::IIR::Filter<float>,
                           dsp::IIR::Coefficients<float>>
      hicutFilter;
  dsp::ProcessorDuplicator<dsp::IIR::Filter<float>,
                           dsp::IIR::Coefficients<float>>
      lowcutFilter;

  // GUI上のキーボードコンポーネントで生成されたMIDI情報を保持しておくオブジェクト.
  // MIDIキーボードの状態を同期するためのステートオブジェクト
  MidiKeyboardState keyboardState;

  // スコープパネルに波形を表示するためのデータバッファ
  AudioBufferQueue<float> scopeDataQueue;
  ScopeDataCollector<float> scopeDataCollector;

  std::set<int> midiList;
  MidiBuffer eventsToAdd;

  // 関数
  static float clippingFunction(float inputValue);
  std::int32_t getNumVoices();
  void addVoice();

  // midiKeyのコピペ
  template <typename Type>
  Type jlimit(Type lowerLimit, Type upperLimit,
              Type valueToConstrain) noexcept {
    jassert(lowerLimit <= upperLimit);  // if these are in the wrong order,
                                        // results are unpredictable..

    return valueToConstrain < lowerLimit
               ? lowerLimit
               : (upperLimit < valueToConstrain ? upperLimit
                                                : valueToConstrain);
  }

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleSynthAudioProcessor)
};
