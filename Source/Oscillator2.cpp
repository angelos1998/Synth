/*
  ==============================================================================

    Oscillator2.cpp
    Created: 12 Jan 2021 11:54:01pm
    Author:  Viet-

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Oscillator2.h"

//==============================================================================
Oscillator2::Oscillator2(SynthAudioProcessor& p) :
    processor(p)
{
    setSize(200, 200);

    osc2Menu.addItem("Saw", 1);
    osc2Menu.addItem("Square", 2);
    osc2Menu.addItem("Sine", 3);
    osc2Menu.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&osc2Menu);

    waveSelection2 = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(processor.tree, "wavetype2", osc2Menu);

    //slider initialization values
    Blendslider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    Blendslider.setRange(0.0f, 1.0f);
    Blendslider.setValue(1.0f);
    Blendslider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(&Blendslider);

    //sends value of the sliders to the tree state in the processor
    blendVal = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.tree, "blend", Blendslider);
}

Oscillator2::~Oscillator2()
{
}

void Oscillator2::paint(juce::Graphics& g)
{
    //background stuff
    juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawText("Oscillator Two", titleArea, juce::Justification::centredTop);


    juce::Rectangle <float> area(25, 25, 150, 150);

    g.setColour(juce::Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void Oscillator2::resized()
{
    juce::Rectangle<int> area = getLocalBounds().reduced(40);
    osc2Menu.setBounds(area.removeFromTop(20));

    //draws the sliders...we use a rectangle object to dynamically size the UI (if we want to resize for IPad etc without needing to change ALL settings


    int sliderWidth = 25;
    int sliderHeight = 175;

    //draw sliders by reducing area from rectangle above
    Blendslider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
}