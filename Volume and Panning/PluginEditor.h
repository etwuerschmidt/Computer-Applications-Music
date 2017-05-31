/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PluginParameter.h"

//==============================================================================
/**
*/
class Mu45effectAudioProcessorEditor  : public AudioProcessorEditor,
                                        public Slider::Listener,
                                        public Timer
{
public:
    Mu45effectAudioProcessorEditor (Mu45effectAudioProcessor&);
    ~Mu45effectAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider*) override;
    void timerCallback() override;

    static const char* banksy3_jpg;
    static const int banksy3_jpgSize;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Mu45effectAudioProcessor& processor;
    
    //Mu45Param: Add pointers to sliders and other UI objects here:
    ScopedPointer<Slider> volumeSlider;     // like a Slider* that we don't have to delete
	ScopedPointer<Slider> balanceSlider;

    //Mu45Param: If you like you can also create labels
    ScopedPointer<Label> volumeLabel;
	ScopedPointer<Label> balanceLabel;
	
	ScopedPointer<HyperlinkButton> hyperlinkButton;
    Image cachedImage_banksy3_jpg_1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mu45effectAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED