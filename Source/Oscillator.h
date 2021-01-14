/*
  ==============================================================================

    Oscillator.h
    Created: 12 Jan 2021 11:53:48pm
    Author:  Viet-

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Oscillator : public juce::Component
{
public:
    Oscillator(SynthAudioProcessor&);
    ~Oscillator();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox oscMenu;

    juce::ScopedPointer<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveSelection;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthAudioProcessor& processor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator)
};