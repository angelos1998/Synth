/*
  ==============================================================================

    Filter.h
    Created: 12 Jan 2021 11:53:29pm
    Author:  Viet-

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Filter : public juce::Component
{
public:
    Filter(SynthAudioProcessor&);
    ~Filter();

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider filterCutoff;
    juce::Slider filterRes;

    juce::ComboBox filterMenu;

    juce::ScopedPointer<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeVal;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> filterVal;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> resVal;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Filter)
};