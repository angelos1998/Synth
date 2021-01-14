/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessor::SynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),

    tree(*this, nullptr, "PARAMETERS",
        { std::make_unique<juce::AudioParameterFloat>("attack", "Attack", juce::NormalisableRange<float>(0.1f, 5000.0f), 0.1f),
            std::make_unique<juce::AudioParameterFloat>("decay", "Decay", juce::NormalisableRange<float>(1.0f, 2000.0f), 1.0f),
            std::make_unique<juce::AudioParameterFloat>("sustain", "Sustain", juce::NormalisableRange<float>(0.0f, 1.0f), 0.8f),
            std::make_unique<juce::AudioParameterFloat>("release", "Release", juce::NormalisableRange<float>(0.1f, 5000.0f), 0.1f),
            std::make_unique<juce::AudioParameterFloat>("wavetype", "WaveType", juce::NormalisableRange<float>(0.0f, 2.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("wavetype2", "WaveType2", juce::NormalisableRange<float>(0.0f, 2.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("filterType", "FilterType", juce::NormalisableRange<float>(0.0f, 2.0f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("filterCutoff", "FilterCutoff", juce::NormalisableRange<float>(20.0f, 10000.0f), 400.0f),
            std::make_unique<juce::AudioParameterFloat>("filterRes", "FilterRes", juce::NormalisableRange<float>(1.0f, 5.0f), 1.0f),
            std::make_unique<juce::AudioParameterFloat>("blend", "Osc2Blend", juce::NormalisableRange<float>(0.0f, 1.0f), 0.6f),
            std::make_unique<juce::AudioParameterFloat>("mastergain", "MasterGain", juce::NormalisableRange<float>(0.0f, 1.0f), 0.7f),
            std::make_unique<juce::AudioParameterFloat>("pbup", "PBup", juce::NormalisableRange<float>(1.0f, 12.0f), 2.0f),
            std::make_unique<juce::AudioParameterFloat>("pbdown", "PBdown", juce::NormalisableRange<float>(1.0f, 12.0f), 2.0f),
        })
#endif
{



    synth.clearVoices();

    for (int i = 0; i < 5; i++)
    {
        synth.addVoice(new SynthVoice());
    }
    synth.clearSounds();
    synth.addSound(new SynthSound());
}

SynthAudioProcessor::~SynthAudioProcessor()
{
}

//==============================================================================
const juce::String SynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //ignoreUnused(samplesPerBlock);
    lastSampleRate = sampleRate;
    synth.setCurrentPlaybackSampleRate(lastSampleRate);

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    stateVariableFilter.reset();
    stateVariableFilter.prepare(spec);
    updateFilter();
}

void SynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
void SynthAudioProcessor::processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&)
{
}
#endif

void SynthAudioProcessor::updateFilter()
{
    int menuChoice = *tree.getRawParameterValue("filterType");
    int freq = *tree.getRawParameterValue("filterCutoff");
    int res = *tree.getRawParameterValue("filterRes");

    if (menuChoice == 0)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, freq, res);
    }

    if (menuChoice == 1)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, freq, res);
    }

    if (menuChoice == 2)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, freq, res);
    }
}

void SynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) 
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

  
    //get the voice and get the params needed to make the voice
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        //if myVoice sucessfully casts as a SynthVoice*, get the voice and set the params
        if ((myVoice = dynamic_cast<SynthVoice*>(synth.getVoice(i))))
        {

            myVoice->getEnvelopeParams(
                (float*)tree.getRawParameterValue("attack"),
                (float*) tree.getRawParameterValue("decay"),
                (float*) tree.getRawParameterValue("sustain"),
                (float*) tree.getRawParameterValue("release")
            );

            myVoice->getOscType(
                (float*) tree.getRawParameterValue("wavetype")
            );

            myVoice->getOsc2Type(
                (float*) tree.getRawParameterValue("wavetype2")
            );

            myVoice->getFilterParams(
                (float*) tree.getRawParameterValue("filterType"),
                (float*) tree.getRawParameterValue("filterCutoff"),
                (float*) tree.getRawParameterValue("filterRes")
            );

            myVoice->getWillsParams(
                (float*) tree.getRawParameterValue("mastergain"),
                (float*) tree.getRawParameterValue("blend"),
                (float*) tree.getRawParameterValue("pbup"),
                (float*) tree.getRawParameterValue("pbdown")
            );
        }
        

        }
}

//==============================================================================
bool SynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthAudioProcessor::createEditor()
{
    return new SynthAudioProcessorEditor (*this);
}

//==============================================================================
void SynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthAudioProcessor();
}
