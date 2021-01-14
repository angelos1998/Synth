/*
  ==============================================================================

    SynthVoice.h
    Created: 12 Jan 2021 4:06:39pm
    Author:  Viet-

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "C:\Users\Viet-\Documents\JUCE\Synth\Source\Maximilian\src\maximilian.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound)
    {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override
    {
        noteNumber = midiNoteNumber;
        env1.trigger = 1;
        setPitchBend(currentPitchWheelPosition);
        frequency = noteHz(noteNumber, pitchBendCents());
        level = velocity;
    }

    void stopNote(float velocity, bool allowTailOff) override
    {
        env1.trigger = 0;
        allowTailOff = true;

        if (velocity == 0)
            clearCurrentNote();
    }

    void setPitchBend(int pitchWheelPos)
    {
        if (pitchWheelPos > 8192)
        {
            // shifting up
            pitchBend = float(pitchWheelPos - 8192) / (16383 - 8192);
        }
        else
        {
            // shifting down
            pitchBend = float(8192 - pitchWheelPos) / -8192;    // negative number
        }
    }

    float pitchBendCents()
    {
        if (pitchBend >= 0.0f)
        {
            // shifting up
            return pitchBend * pitchBendUpSemitones * 100;
        }
        else
        {
            // shifting down
            return pitchBend * pitchBendDownSemitones * 100;
        }
    }

    static double noteHz(int midiNoteNumber, double centsOffset)
    {
        double hertz = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        hertz *= std::pow(2.0, centsOffset / 1200);
        return hertz;
    }

    //=======================================================

    void getOscType(float* selection)
    {
        theWave = *selection;

    }

    void getOsc2Type(float* selection)
    {

        theWave2 = *selection;
    }
    //=======================================================

    double setOscType()

    {
        double sample1, sample2;

        switch (theWave)
        {
        case 0:
            sample1 = osc1.square(frequency);
            break;
        case 1:
            sample1 = osc1.saw(frequency);
            break;
        default:
            sample1 = osc1.sinewave(frequency);
            break;
        }

        switch (theWave2)
        {
        case 0:
            sample2 = osc2.saw(frequency / 2.0);
            break;
        case 1:
            sample2 = osc2.square(frequency / 2.0);
            break;
        default:
            sample2 = osc2.sinewave(frequency / 2.0);
            break;
        }

        return sample1 + osc2blend * sample2;
    }

    //=======================================================

    void getEnvelopeParams(float* attack, float* decay, float* sustain, float* release)
    {
        env1.setAttack(*attack);
        env1.setDecay(*decay);
        env1.setSustain(*sustain);
        env1.setRelease(*release);
    }

    //=======================================================

    double setEnvelope()
    {
        return env1.adsr(setOscType(), env1.trigger);
    }

    //=======================================================

    void getWillsParams(float* mGain, float* blend, float* pbup, float* pbdn)
    {
        masterGain = *mGain;
        osc2blend = *blend;
        pitchBendUpSemitones = *pbup;
        pitchBendDownSemitones = *pbdn;
    }

    void getFilterParams(float* filterType, float* filterCutoff, float* filterRes)
    {
        filterChoice = *filterType;
        cutoff = *filterCutoff;
        resonance = *filterRes;
    }
    
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    
    void pitchWheelMoved(int newPitchWheelValue) override
    {
        setPitchBend(newPitchWheelValue);
        frequency = noteHz(noteNumber, pitchBendCents());
    }
    
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    
    void renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, startSample, setEnvelope() * masterGain);
            }
            ++startSample;
        }
    }


private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); } };
    juce::dsp::Gain<float> gain;
    bool isPrepared{ false };

    double level;
    double frequency;
    int theWave, theWave2;

    float masterGain;
    float osc2blend;

    int noteNumber;
    float pitchBend = 0.0f;
    float pitchBendUpSemitones = 2.0f;
    float pitchBendDownSemitones = 2.0f;

    int filterChoice;
    float cutoff;
    float resonance;

    maxiOsc osc1, osc2;
    maxiEnv env1;
};
