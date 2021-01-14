/*
  ==============================================================================

    Frontgui.h
    Created: 12 Jan 2021 11:53:40pm
    Author:  Viet-

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/*
 */
class Frontgui : public juce::Component
{
public:
    Frontgui(SynthAudioProcessor&);
    ~Frontgui();

    void paint(juce::Graphics&) override;
    void resized() override;


private:
    juce::Slider mastergainSlider;
    juce::Slider pbupSlider;
    juce::Slider pbdownSlider;


    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> mastergainVal;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> pbupVal;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> pbdownVal;



    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Frontgui)
};