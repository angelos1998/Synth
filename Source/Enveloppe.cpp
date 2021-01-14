/*
  ==============================================================================

    Enveloppe.cpp
    Created: 12 Jan 2021 11:53:23pm
    Author:  Viet-

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Enveloppe.h"

//==============================================================================
Enveloppe::Enveloppe(SynthAudioProcessor& p) :
    processor(p)
{
    setSize(200, 200);

    //slider initialization values
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    attackSlider.setRange(0.1f, 5000.0f);
    attackSlider.setValue(0.1f);
    attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&attackSlider);

    decaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    decaySlider.setRange(1.0f, 2000.0f);
    decaySlider.setValue(1.0f);
    decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&decaySlider);

    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sustainSlider.setRange(0.0f, 1.0f);
    sustainSlider.setValue(0.8f);
    sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&sustainSlider);

    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    releaseSlider.setRange(0.1f, 5000.0f);
    releaseSlider.setValue(0.8f);
    releaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&releaseSlider);

    //sends value of the sliders to the tree state in the processor
    attackVal = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.tree, "attack", attackSlider);
    decayVal = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.tree, "decay", decaySlider);
    sustainVal = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.tree, "sustain", sustainSlider);
    releaseVal = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.tree, "release", releaseSlider);
}

Enveloppe::~Enveloppe()
{
}

void Enveloppe::paint(juce::Graphics& g)
{
    //fancy stuff for the UI background etc
    juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawText("Envelope", titleArea, juce::Justification::centredTop);

    //static positioning for now due to time, make dynamic later
    g.drawText("A", 53, 150, 20, 20, juce::Justification::centredTop);
    g.drawText("D", 77, 150, 20, 20, juce::Justification::centredTop);
    g.drawText("S", 103, 150, 20, 20, juce::Justification::centredTop);
    g.drawText("R", 128, 150, 20, 20, juce::Justification::centredTop);

    juce::Rectangle <float> area(25, 25, 150, 150);

    g.setColour(juce::Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void Enveloppe::resized()
{
    //draws the sliders...we use a rectangle object to dynamically size the UI (if we want to resize for IPad etc without needing to change ALL settings
    juce::Rectangle<int> area = getLocalBounds().reduced(50);

    int sliderWidth = 25;
    int sliderHeight = 175;

    //draw sliders by reducing area from rectangle above
    attackSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    decaySlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    sustainSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    releaseSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
}

