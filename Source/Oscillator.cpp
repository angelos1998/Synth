/*
  ==============================================================================

    Oscillator.cpp
    Created: 12 Jan 2021 11:53:48pm
    Author:  Viet-

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Oscillator.h"
#include "PluginProcessor.h"

//==============================================================================
Oscillator::Oscillator(SynthAudioProcessor& p) :
    processor(p)
{
    setSize(200, 200);

    oscMenu.addItem("Square", 1);
    oscMenu.addItem("Saw", 2);
    oscMenu.addItem("Sine", 3);
    oscMenu.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&oscMenu);

    waveSelection = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(processor.tree, "wavetype", oscMenu);
}

Oscillator::~Oscillator()
{
}

void Oscillator::paint(juce::Graphics& g)
{
    //background stuff
    juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawText("Oscillator One", titleArea, juce::Justification::centredTop);


    juce::Rectangle <float> area(25, 25, 150, 150);

    g.setColour(juce::Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void Oscillator::resized()
{
    juce::Rectangle<int> area = getLocalBounds().reduced(40);
    oscMenu.setBounds(area.removeFromTop(20));
}