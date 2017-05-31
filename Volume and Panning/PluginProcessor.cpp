/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Mu45effectAudioProcessor::Mu45effectAudioProcessor()
{
    //Mu45Param: Create and initialize a PluginParameter for each user parameter
	//From pointer in PluginProcessor.h
    addParameter( volumeUParam = new PluginParameter("volume",     // parameterId
                                        0.0,         // actualDefaultValue
                                        -96.0,       // actualMinimum
                                        10.0,        // actualMaximum
                                        "Volume",    // parameterName
                                        "dB"));      // parameterLabel (can be String::empty)

	addParameter(balanceUParam = new PluginParameter("balance",
		0.0,
		-50.0,
		50.0,
		"Balance",
		""));
    
}

Mu45effectAudioProcessor::~Mu45effectAudioProcessor()
{
}

//==============================================================================
const String Mu45effectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Mu45effectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Mu45effectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Mu45effectAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double Mu45effectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Mu45effectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Mu45effectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Mu45effectAudioProcessor::setCurrentProgram (int index)
{
}

const String Mu45effectAudioProcessor::getProgramName (int index)
{
    return String();
}

void Mu45effectAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
//Mu45: The following methods are where we will do most of our work
//==============================================================================

void Mu45effectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //Mu45: Do any initial calculations here that require the sample rate
    
    //Mu45: Initialize any member variables
    gainAParam = 0.0;
	balParam = 0.0;
}

void Mu45effectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    //Mu45: You may need to release memory here.
}

//Mu45: This function is a good place to calculate algorithm parameter values from user parameter values.
void Mu45effectAudioProcessor::calcAlgorithmParams()
{
	//dB = 20log(x)
    gainAParam = pow(10, volumeUParam->getActualValue()/20.0);
	balParam = (50 + balanceUParam->getActualValue()) / 100.0;
}


// Mu45: This is where audio actually gets changed!
void Mu45effectAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //Mu45: Before we process a buffer, update the algorithm params!
    calcAlgorithmParams();
    
    // This version assumes we always have 2 channels
    float* left = buffer.getWritePointer (0);
    float* right = buffer.getWritePointer (1);
    const int numSamps = buffer.getNumSamples();
    
    //Mu45: This is the "inner loop" where we calculate each output sample
    for (int samp = 0; samp < numSamps; samp++)
    {
        left[samp] = gainAParam * left[samp];
        right[samp] = gainAParam * right[samp];

		//Modified equation found from http://folk.ntnu.no/oyvinbra/delete/Lesson1Panning.html
		right[samp] = sin(balParam * (M_PI/2)) * right[samp];
		left[samp] = cos(balParam * (M_PI/2)) * left[samp];
    }
}

//==============================================================================
bool Mu45effectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Mu45effectAudioProcessor::createEditor()
{
    return new Mu45effectAudioProcessorEditor (*this);
}

//==============================================================================
void Mu45effectAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Mu45effectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Mu45effectAudioProcessor();
}
