/*
  ==============================================================================

    Filter.cpp
    Created: 12 Jan 2021 11:53:29pm
    Author:  Viet-

  ==============================================================================
*/


#include <JuceHeader.h>
#include "Filter.h"
#include <include_juce_gui_basics.cpp>

//==============================================================================
Filter::Filter(SynthAudioProcessor& p) :
    processor(p)
{
    setSize(200, 200);

    filterMenu.addItem("Low Pass", 1);
    filterMenu.addItem("High Pass", 2);
    filterMenu.addItem("Band Pass", 3);
    filterMenu.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&filterMenu);
    filterTypeVal = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(processor.tree, "filterType", filterMenu);

    filterCutoff.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterCutoff.setRange(20.0, 10000.0);
    filterCutoff.setValue(400.0);
    filterCutoff.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&filterCutoff);
    filterVal = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.tree, "filterCutoff", filterCutoff);
    filterCutoff.setSkewFactorFromMidPoint(1000.0);

    filterRes.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterRes.setRange(1, 5);
    filterRes.setValue(1);
    filterRes.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(&filterRes);
    resVal = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.tree, "filterRes", filterRes);
}

Filter::~Filter()
{
}

void Filter::paint(juce::Graphics& g)
{
    //background ui stuff
    juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawText("Filter", titleArea, juce::Justification::centredTop);

    juce::Rectangle <float> area(25, 25, 150, 150);

    g.setColour(juce::Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void Filter::resized()
{
    //need to come back and dynamically set these...ok for now
    juce::Rectangle<int> area = getLocalBounds().reduced(40);

    filterMenu.setBounds(area.removeFromTop(20));
    filterCutoff.setBounds(30, 100, 70, 70);
    filterRes.setBounds(100, 100, 70, 70);
}