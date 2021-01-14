/*
  ==============================================================================

    Oscillator2.h
    Created: 12 Jan 2021 11:54:01pm
    Author:  Viet-

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
 */
class Oscillator2 : public juce::Component
{
public:
    Oscillator2(SynthAudioProcessor&);
    ~Oscillator2();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox osc2Menu;
    juce::Slider Blendslider;

    juce::ScopedPointer<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveSelection2;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> blendVal;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthAudioProcessor& processor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator2)
};
