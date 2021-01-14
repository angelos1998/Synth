/*
  ==============================================================================

    Enveloppe.h
    Created: 12 Jan 2021 11:53:23pm
    Author:  Viet-

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Enveloppe : public juce::Component
{
public:
    Enveloppe(SynthAudioProcessor&);
    ~Enveloppe();

    void paint(juce::Graphics&) override;
    void resized() override;


private:
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> attackVal;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> decayVal;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> sustainVal;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> releaseVal;


    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Enveloppe)
};