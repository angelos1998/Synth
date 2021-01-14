/*
  ==============================================================================

    SynthSound.h
    Created: 12 Jan 2021 4:07:00pm
    Author:  Viet-

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) { return true; }
    bool appliesToChannel(int midiChannel) { return true; }
};