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
    addParameter(wetDryUParam = new PluginParameter("wet / dry",
		50.0,
		0.0,
		100.0,
		"Wet / Dry",
		"%"));

	addParameter(feedbackUParam = new PluginParameter("feedback",
		50.0,
		0.0,
		95.0,
		"Feedback",
		"%"));

	addParameter(delayUParam = new PluginParameter("delay",
		5.0,
		0.0,
		10.0,
		"Delay",
		"ms"));

	addParameter(depthUParam = new PluginParameter("depth",
		0.0,
		0.0,
		(delayUParam->getActualValue() > 5.0) ? 10.0 - delayUParam->getActualValue() : delayUParam->getActualValue(),
		"Delay",
		"ms"));

	addParameter(rateUParam = new PluginParameter("rate",
		5.0,
		0.0,
		10.0,
		"Rate",
		"Hz"));

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

float Mu45effectAudioProcessor::calcDelaySampsFromMsec(float time)
{
	float samps = fs * time / 1000;
	return samps;
}

void Mu45effectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	fs = sampleRate;

	float samps = calcDelaySampsFromMsec(10);
	delayL.setMaximumDelay(samps);
	delayR.setMaximumDelay(samps);

	wetAParam = 0.0;
	dryAParam = 0.0;
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
	feedbackAParam = feedbackUParam->getActualValue() / 100;
	wetAParam = wetDryUParam->getActualValue() / 100;
	dryAParam = 1 - (wetDryUParam->getActualValue() / 100);
	depthAParam = depthUParam->getActualValue();

	float lfoFreq = rateUParam->getActualValue();

	lfoL.setFreq(lfoFreq, fs);
	lfoR.setFreq(lfoFreq, fs);

	float delaymsec = delayUParam->getActualValue();
	delayAParam = calcDelaySampsFromMsec(delaymsec);

	float depthmsec = depthUParam->getActualValue();
	depthAParam = calcDelaySampsFromMsec(depthmsec);

	
	//delayL.setDelay(samps);
	//delayR.setDelay(samps);


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
		float tempDelayL = delayAParam + (depthAParam * lfoL.tick());
		float tempDelayR = delayAParam + (depthAParam * lfoR.tick());

		//tempDelayL = (tempDelayL > 10.0) ? 10.0 : tempDelayL;
		//tempDelayR = (tempDelayR > 10.0) ? 10.0 : tempDelayR;

		delayL.setDelay(tempDelayL);
		delayR.setDelay(tempDelayR);

		left[samp] = dryAParam*left[samp] + wetAParam*delayL.tick(left[samp] + feedbackAParam*delayL.nextOut());
		right[samp] = dryAParam*right[samp] + wetAParam*delayR.tick(right[samp] + feedbackAParam*delayR.nextOut());
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
