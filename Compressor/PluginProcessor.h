/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginParameter.h"


//==============================================================================
/**
*/
class Mu45effectAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Mu45effectAudioProcessor();
    ~Mu45effectAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //Mu45Param: Add a pointer for each user parameter
    PluginParameter* volumeUParam;
	PluginParameter* thresholdUParam;
	PluginParameter* ratioUParam;
	PluginParameter* attackUParam;
	PluginParameter* releaseUParam;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mu45effectAudioProcessor)
    
    void calcAlgorithmParams();
    
    float thresholdAParam, ratioAParam, attackAParam, releaseAParam;
	float lSq, rSq, lMean, rMean, b, bDyn, tau, tauA, tauR, mono, monodB, cdB, cdBdyn, A, R, cLin;
	float fs;
};


#endif  // PLUGINPROCESSOR_H_INCLUDED